#pragma once

#include <opencv2/opencv.hpp>

#include "motion.hpp"

class Camera
{
public:
    Camera(int index = 0)
    {
        m_cap = cv::VideoCapture(index);

        m_motion_detector = new MotionDetector(m_cap.get(cv::CAP_PROP_FRAME_WIDTH), m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    }
    ~Camera()
    {
        m_cap.release();
        delete m_motion_detector;
    }

    void poll()
    {
        m_cap >> m_frame;

        if (m_frame.empty())
        {
            return;
        }

        m_motion_detector->poll(m_frame);

        cv::imshow("Camera", m_frame);
    }

private:
    cv::VideoCapture m_cap;
    cv::Mat m_frame;

    MotionDetector* m_motion_detector = nullptr;
};