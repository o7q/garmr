#pragma once

#include <opencv2/opencv.hpp>

#include "replay.hpp"
#include "detector.hpp"

class CameraModule
{
public:
    CameraModule(const ReplayBuf &other) = delete;
    CameraModule &operator=(const ReplayBuf &other) = delete;
    CameraModule(Detector *detector) : m_detector(detector) {}
    ~CameraModule() {}

    void init(const cv::VideoCapture &capture);
    Detector::Result update(const cv::Mat &frame, float steady_fps);

private:
    Detector *m_detector = nullptr;
    ReplayBuf m_replay;

    float m_detection_timer = 0.0f;
    float m_detection_active = false;
};
