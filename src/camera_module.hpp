#pragma once

#include <opencv2/opencv.hpp>

#include "detectors/detector.hpp"
#include "replay.hpp"

class CameraModule
{
public:
    CameraModule(const cv::VideoCapture &cap, Detector *detector) : m_replay(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT))
    {
        m_detector = detector;
    }

    void update(const cv::Mat &frame, float steady_fps)
    {
        m_replay.setFramerate(steady_fps);

        m_detector->poll(frame, steady_fps);

        if (m_detector->isDetecting())
        {
            m_detection_timer = 100.0f;
            m_detection_active = true;
            m_replay.setEnabled("replays");
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
    }

    const Detector *getDetector() const { return m_detector; }

    ~CameraModule()
    {
        if (m_detector)
        {
            delete m_detector;
        }
    }

private:
    Detector *m_detector = nullptr;
    ReplayBuf m_replay;

    float m_detection_timer = 0.0f;
    float m_detection_active = false;
};