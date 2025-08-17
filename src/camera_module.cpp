#include "camera_module.hpp"

#include <opencv2/opencv.hpp>

#include "replay.hpp"
#include "detector.hpp"

void CameraModule::init(const cv::VideoCapture &capture)
{
    m_replay.disable();

    if (!capture.isOpened())
    {
        return;
    }

    m_replay.setSize(static_cast<unsigned int>(capture.get(cv::CAP_PROP_FRAME_WIDTH)),
                     static_cast<unsigned int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
}

Detector::Result CameraModule::update(const cv::Mat &frame, float steady_fps)
{
    m_replay.setFramerate(steady_fps);

    Detector::Result result = m_detector->check(frame);

    if (result.detected)
    {
        m_detection_timer = 100.0f;
        m_detection_active = true;
        m_replay.enable();
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
