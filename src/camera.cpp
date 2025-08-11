#include "camera.hpp"

Camera::Camera(int index)
{
    m_cap = cv::VideoCapture(index);

    m_motion_detector = new MotionDetector(m_cap.get(cv::CAP_PROP_FRAME_WIDTH), m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    m_person_detector = new PersonDetector();
}
Camera::~Camera()
{
    m_cap.release();
    delete m_motion_detector;
    delete m_person_detector;
}

void Camera::poll()
{
    m_cap >> m_frame;

    if (m_frame.empty())
    {
        return;
    }

    m_motion_detector->poll(m_frame);
    // m_person_detector->poll(m_frame);

    // cv::imshow("Camera", m_frame);
    // cv::imshow("Camera Motion", m_motion_detector->getFrame());
    // cv::imshow("Camera Motion", m_person_detector->getFrame());
}