#pragma once

#include <string>
#include <chrono>
#include <vector>

#include "camera_module.hpp"
#include "motion.hpp"

class Camera
{
private:
    inline static constexpr int FPS_SMOOTH_ITERATIONS = 10;
    inline static constexpr int MAX_MISSED_FRAMES = 1000;

public:
    Camera(const std::string &capture_device);
    ~Camera();

    void update();

private:
    void reset()
    {
        m_cap.release();
        m_cap = cv::VideoCapture(m_cap_id);

        m_motion_module.init(m_cap);
    }

    void update_steady_fps(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);

private:
    cv::VideoCapture m_cap;
    std::string m_cap_id;

    cv::Mat m_frame;

    MotionDetector m_motion_detector;
    CameraModule m_motion_module;

    float m_steady_fps = 1.0f;

    float m_fps_history[FPS_SMOOTH_ITERATIONS] = {};
    int m_fps_history_index = 0;

    int m_missed_frames = 0;
};