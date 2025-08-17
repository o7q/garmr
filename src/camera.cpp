#include "camera.hpp"

#include "camera_module.hpp"

Camera::Camera(const std::string &capture_device) : m_motion_module(&m_motion_detector)
{
    m_cap_id = capture_device;
    reset();
}

Camera::~Camera()
{
    m_cap.release();
}

void Camera::update()
{
    std::chrono::steady_clock::time_point clock_start = std::chrono::steady_clock::now();

    m_cap >> m_frame;

    if (m_missed_frames >= MAX_MISSED_FRAMES)
    {
        reset();
        std::cout << "Attempting Camera Reset" << std::endl;

        m_missed_frames = 0;
    }

    if (m_frame.empty())
    {
        ++m_missed_frames;
        return;
    }

    cv::imshow("Camera", m_frame);
    cv::imshow("Motion Camera", m_motion_module.update(m_frame, m_steady_fps).marked_frame);

    std::chrono::steady_clock::time_point clock_end = std::chrono::steady_clock::now();
    update_steady_fps(clock_start, clock_end);
}

void Camera::update_steady_fps(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end)
{
    const float frame_seconds = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();

    m_fps_history[m_fps_history_index] = 1.0f / frame_seconds;

    float fps_sum = 0.0f;

    for (int i = 0; i < FPS_SMOOTH_ITERATIONS; ++i)
    {
        fps_sum += m_fps_history[i];
    }

    m_fps_history_index = (m_fps_history_index + 1) % FPS_SMOOTH_ITERATIONS;

    m_steady_fps = fps_sum / FPS_SMOOTH_ITERATIONS;
}
