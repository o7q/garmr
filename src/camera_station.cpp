#include "camera_station.hpp"

void CameraStation::add(const std::string &camera_id)
{
    m_camera_threads.emplace_back(&CameraStation::camera_thread, this, camera_id);
}

void CameraStation::stop()
{
    m_active = false;
    for (std::thread &thread : m_camera_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    m_camera_threads.clear();
}

void CameraStation::camera_thread(std::string camera_id)
{
    Camera camera(camera_id);
    while (m_active)
    {
        camera.update();
    }
}