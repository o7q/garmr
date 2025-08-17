#include "motion.hpp"

MotionDetector::MotionDetector()
{
    m_bgSubtractor = cv::createBackgroundSubtractorMOG2(500, 16.0, false);
    m_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    m_contours_prealloc.reserve(100);
}

Detector::Result MotionDetector::check(const cv::Mat &frame)
{
    Result motionResult{frame.clone(), false};

    m_bgSubtractor->apply(frame, m_fgMask);

    cv::erode(m_fgMask, m_fgMask, m_kernel, cv::Point(-1, -1), 4);
    cv::dilate(m_fgMask, m_fgMask, m_kernel, cv::Point(-1, -1), 4);

    cv::findContours(m_fgMask, m_contours_prealloc, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const std::vector<cv::Point> &contour : m_contours_prealloc)
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
