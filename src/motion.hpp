#pragma once

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
    cv::Mat m_fgMask;
};