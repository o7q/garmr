#pragma once

#include <opencv2/opencv.hpp>

#include "replay.hpp"
#include "time.hpp"

class PersonDetector
{
public:
    PersonDetector()
    {
        hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    }
    ~PersonDetector() {}

    void poll(const cv::Mat &frame)
    {
        m_frame = frame.clone();

        std::vector<cv::Rect> detections;
        hog.detectMultiScale(m_frame, detections);

        for (auto &rect : detections)
        {
            cv::rectangle(m_frame, rect, cv::Scalar(0, 255, 0), 2);
        }
    }
    const cv::Mat &getFrame() { return m_frame; }

private:
    cv::HOGDescriptor hog;
    cv::Mat m_frame;

    float m_motion_timer = 0.0f;
    bool m_motion_happening = false;

    // ReplayBuf m_replayBuf;
};