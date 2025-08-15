#pragma once

#include <opencv2/opencv.hpp>

#include "../replay.hpp"
#include "detector.hpp"

class MotionDetector : public Detector
{
public:
    MotionDetector();
    ~MotionDetector() {}

    void poll(const cv::Mat &frame, float steady_fps) override;
    const cv::Mat &getViewFrame() const override { return m_fgMask; }

private:
    cv::Ptr<cv::BackgroundSubtractor> m_bgSubtractor;
    cv::Mat m_fgMask;
};