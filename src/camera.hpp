#pragma once

#include <string>
#include <chrono>
#include <vector>

#include "camera_module.hpp"

class Camera
{
public:
    Camera(std::string capture_device);
    ~Camera();

    void update();

private:
    void update_steady_fps(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);

private:
    cv::VideoCapture m_cap;
    cv::Mat m_frame;

    std::vector<CameraModule *> m_modules;

    float m_steady_fps = 1.0f;
    float *m_fps_history = nullptr;
    int m_fps_history_index = 0;
    int m_fps_smooth_iterations = 10;
};