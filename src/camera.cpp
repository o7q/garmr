#include "camera.hpp"

Camera::~Camera()
{
    if (m_motion_detector != nullptr)
    {
        delete m_motion_detector;
    }
    if (m_fps_history)
    {
        delete[] m_fps_history;
    }
    m_cap.release();
}

void Camera::poll()
{
    // test_capture_device();

    std::chrono::steady_clock::time_point clock_start = std::chrono::steady_clock::now();

    m_cap >> m_frame;

    if (m_frame.empty())
    {
        return;
    }

    m_motion_detector->poll(m_frame, m_steady_fps);

    std::chrono::steady_clock::time_point clock_end = std::chrono::steady_clock::now();
    update_steady_fps(clock_start, clock_end);

    cv::imshow("Camera", m_frame);
    cv::imshow("Camera Motion", m_motion_detector->getFrame());
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