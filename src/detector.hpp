#pragma once

#include <opencv2/opencv.hpp>

class Detector
{
public:
    struct Result
    {
        cv::Mat marked_frame;
        bool detected = false;
    };

public:
    Detector(const Detector &other) = delete;
    Detector &operator=(const Detector &other) = delete;
    Detector() = default;
    virtual ~Detector() = default;

    virtual Result check(const cv::Mat &frame) { return Result(); }

protected:
    cv::Mat m_frame_marked;
};
