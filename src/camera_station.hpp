#pragma once

#include <thread>
#include <atomic>
#include <vector>

#include "camera.hpp"

class CameraStation
{
public:
    ~CameraStation() { stop(); }

    void add(const std::string &camera_id);
    void stop();

private:
    void camera_thread(std::string camera_id);

private:
    std::vector<std::thread> m_camera_threads;
    std::atomic<bool> m_active{true};
};