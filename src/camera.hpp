#pragma once

#include <string>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "motion.hpp"
#include "person.hpp"

class Camera
{
public:
    Camera(std::string capture_device)
    {
        m_capture_device = capture_device;
        test_capture_device();

        m_motion_detector = new MotionDetector(m_cap.get(cv::CAP_PROP_FRAME_WIDTH), m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));

        m_fps_history = new float[m_fps_smooth_iterations];

        for (int i = 0; i < m_fps_smooth_iterations; ++i)
        {
            m_fps_history[i] = 1.0f;
        }
    }
    ~Camera();

    void poll();

private:
    void test_capture_device()
    {
        if (!m_cap.isOpened())
        {
            m_cap = cv::VideoCapture(m_capture_device);
        }
    }

    void update_steady_fps(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);

private:
    cv::VideoCapture m_cap;
    cv::Mat m_frame;

    MotionDetector *m_motion_detector = nullptr;

    float m_steady_fps = 1.0f;
    float *m_fps_history = nullptr;
    int m_fps_history_index = 0;
    int m_fps_smooth_iterations = 10;

    std::string m_capture_device = "";
};