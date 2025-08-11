#pragma once

#include <opencv2/opencv.hpp>

#include "motion.hpp"
#include "person.hpp"

class Camera
{
public:
    Camera(int index = 0);
    ~Camera();

    void poll();

private:
    cv::VideoCapture m_cap;
    cv::Mat m_frame;

    MotionDetector* m_motion_detector = nullptr;
    PersonDetector* m_person_detector = nullptr;
};