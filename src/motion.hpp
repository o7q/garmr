#pragma once

#include <opencv2/opencv.hpp>

#include "replay.hpp"
#include "time.hpp"

class MotionDetector
{
public:
    MotionDetector(unsigned int frame_width, unsigned int frame_height);
    ~MotionDetector() {}

    void poll(const cv::Mat &frame);
    const cv::Mat &getFrame() { return m_fgMask; }

private:
    cv::Ptr<cv::BackgroundSubtractor> m_bgSubtractor;
    cv::Mat m_fgMask;

    float m_motion_timer = 0.0f;
    bool m_motion_happening = false;

    ReplayBuf m_replayBuf;
};