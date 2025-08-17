#include "motion.hpp"

MotionDetector::MotionDetector()
{
    m_bgSubtractor = cv::createBackgroundSubtractorMOG2(500, 16.0, false);
}

Detector::Result MotionDetector::check(const cv::Mat &frame)
{
    Result motionResult{frame.clone(), false};

    m_bgSubtractor->apply(frame, m_fgMask);

    cv::erode(m_fgMask, m_fgMask, cv::Mat(), cv::Point(-1, -1), 4);
    cv::dilate(m_fgMask, m_fgMask, cv::Mat(), cv::Point(-1, -1), 4);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(m_fgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    motionResult.detected = false;

    for (const auto &contour : contours)
    {
        double contourArea = cv::contourArea(contour);
        if (contourArea > 1000)
        {
            cv::Rect boundRect = cv::boundingRect(contour);
            cv::rectangle(motionResult.marked_frame, boundRect, cv::Scalar(255.0, 255.0, 0), 1);

            motionResult.detected = true;
        }
    }

    return motionResult;
}