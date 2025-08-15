#include "camera.hpp"

#include "detectors/motion.hpp"
#include "camera_module.hpp"

Camera::Camera(std::string capture_device)
{
    m_cap = cv::VideoCapture(capture_device);

    m_modules.push_back(new CameraModule(m_cap, new MotionDetector()));

    m_fps_history = new float[m_fps_smooth_iterations];

    for (int i = 0; i < m_fps_smooth_iterations; ++i)
    {
        m_fps_history[i] = 1.0f;
    }
}

Camera::~Camera()
{
    if (m_fps_history)
    {
        delete[] m_fps_history;
    }

    for (CameraModule *module : m_modules)
    {
        if (module)
        {
            delete module;
        }
    }
    m_modules.clear();

    m_cap.release();
}

void Camera::update()
{
    std::chrono::steady_clock::time_point clock_start = std::chrono::steady_clock::now();

    m_cap >> m_frame;

    if (m_frame.empty())
    {
        return;
    }

    cv::imshow("Camera", m_frame);

    for (CameraModule *module : m_modules)
    {
        module->update(m_frame, m_steady_fps);
        cv::imshow("Camera Motion", module->getDetector()->getMarkedFrame());
    }

    std::chrono::steady_clock::time_point clock_end = std::chrono::steady_clock::now();
    update_steady_fps(clock_start, clock_end);
}

void Camera::update_steady_fps(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end)
{
    const float frame_seconds = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();

    m_fps_history[m_fps_history_index] = 1.0f / frame_seconds;

    float fps_sum = 0.0f;

    for (int i = 0; i < m_fps_smooth_iterations; ++i)
    {
        fps_sum += m_fps_history[i];
    }

    m_fps_history_index = (m_fps_history_index + 1) % m_fps_smooth_iterations;

    m_steady_fps = fps_sum / m_fps_smooth_iterations;
}