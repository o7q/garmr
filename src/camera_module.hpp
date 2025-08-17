#pragma once

#include <opencv2/opencv.hpp>

#include "replay.hpp"
#include "detector.hpp"

class CameraModule
{
public:
    CameraModule(Detector *detector) : m_detector(detector) {}

    void init(const cv::VideoCapture &capture)
    {
        if (!capture.isOpened())
        {
            return;
        }

        m_replay.setSize(static_cast<unsigned int>(capture.get(cv::CAP_PROP_FRAME_WIDTH)),
                         static_cast<unsigned int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
    }

    Detector::Result update(const cv::Mat &frame, float steady_fps)
    {
        m_replay.setFramerate(steady_fps);

        Detector::Result result = m_detector->check(frame);

        if (result.detected)
        {
            m_detection_timer = 100.0f;
            m_detection_active = true;
            m_replay.setEnabled();
        }

        m_replay.update(frame);

        if (m_detection_timer < 1.0f)
        {
            m_detection_timer = 0.0f;
        }
        else
        {
            std::cout << m_detection_timer << std::endl;
            m_detection_timer /= 1.1f;
        }

        if (m_detection_timer < 1.0f && m_detection_active)
        {
            std::cout << "Motion Stopped" << std::endl;
            m_replay.disable();
        }

        if (m_detection_timer < 1.0f)
        {
            m_detection_active = false;
        }

        return result;
    }

private:
    Detector *m_detector = nullptr;
    ReplayBuf m_replay;

    float m_detection_timer = 0.0f;
    float m_detection_active = false;
};