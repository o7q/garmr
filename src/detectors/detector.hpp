#pragma once

#include <opencv2/opencv.hpp>

class Detector
{
public:
    Detector(const Detector &other) = delete;
    Detector &operator=(const Detector &other) = delete;
    Detector() {}
    virtual ~Detector() {}

    virtual void poll(const cv::Mat &frame, float steady_fps) {}

    const cv::Mat &getMarkedFrame() const { return m_frame_marked; }
    virtual const cv::Mat &getViewFrame() const
    {
        static const cv::Mat empty;
        return empty;
    }

    bool isDetecting() const { return m_detection; }

protected:
    bool m_detection = false;

    cv::Mat m_frame_marked;
};