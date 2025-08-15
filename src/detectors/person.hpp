#pragma once

#include <opencv2/opencv.hpp>

#include "../time.hpp"

class PersonDetector
{
public:
    PersonDetector()
    {
        m_hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    }
    ~PersonDetector() {}

    void poll(const cv::Mat &frame)
    {
        m_frame = frame.clone();

        std::vector<cv::Rect> detections;
        m_hog.detectMultiScale(m_frame, detections);

        for (auto &rect : detections)
        {
            cv::rectangle(m_frame, rect, cv::Scalar(0, 255, 0), 2);
        }
    }
    const cv::Mat &getFrame() { return m_frame; }

private:
    cv::HOGDescriptor m_hog;
    cv::Mat m_frame;

    float m_motion_timer = 0.0f;
    bool m_motion_happening = false;
};