#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

#include "detector.hpp"

class MotionDetector : public Detector
{
public:
    MotionDetector();
    ~MotionDetector() {}

    Result check(const cv::Mat &frame) override;

private:
    cv::Ptr<cv::BackgroundSubtractor> m_bgSubtractor;
    std::vector<std::vector<cv::Point>> m_contours_prealloc;
    cv::Mat m_kernel;
    cv::Mat m_fgMask;
};
