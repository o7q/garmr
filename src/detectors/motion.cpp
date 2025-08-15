#include "motion.hpp"

MotionDetector::MotionDetector()
{
    m_bgSubtractor = cv::createBackgroundSubtractorMOG2(500, 16.0, false);
}

void MotionDetector::poll(const cv::Mat &frame, float steady_fps)
{
    m_frame_marked = frame.clone();

    m_bgSubtractor->apply(frame, m_fgMask);

    cv::erode(m_fgMask, m_fgMask, cv::Mat(), cv::Point(-1, -1), 4);
    cv::dilate(m_fgMask, m_fgMask, cv::Mat(), cv::Point(-1, -1), 4);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(m_fgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    m_detection = false;

    for (const auto &contour : contours)
    {
        double contourArea = cv::contourArea(contour);
        if (contourArea > 1000)
        {
            cv::Rect boundRect = cv::boundingRect(contour);
            cv::rectangle(m_frame_marked, boundRect, cv::Scalar(255.0, 255.0, 0), 1);

            m_detection = true;
        }
    }
}