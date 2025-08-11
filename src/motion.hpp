#pragma once

#include <opencv2/opencv.hpp>

#include "replay.hpp"
#include "time.hpp"

class MotionDetector
{
public:
    MotionDetector(unsigned int frame_width, unsigned int frame_height) : m_replayBuf(frame_width, frame_height, 15, 120, 5)
    {
        m_bgSubtractor = cv::createBackgroundSubtractorMOG2(2048, 16.0, true);
    }
    ~MotionDetector() {}

    void poll(const cv::Mat &frame)
    {
        m_bgSubtractor->apply(frame, m_fgMask);

        cv::erode(m_fgMask, m_fgMask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(m_fgMask, m_fgMask, cv::Mat(), cv::Point(-1, -1), 2);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(m_fgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto &contour : contours)
        {
            double contourArea = cv::contourArea(contour);
            if (contourArea > 500)
            {
                cv::Rect boundRect = cv::boundingRect(contour);
                cv::rectangle(frame, boundRect, cv::Scalar(255.0, 255.0, 0), 1);

                m_motion_timer = 100.0f;
                m_motion_happening = true;
                m_replayBuf.enable();
            }
        }

        m_replayBuf.push(frame);

        if (m_motion_timer < 1.0f)
        {
            m_motion_timer = 0.0f;
        }
        else
        {
            std::cout << m_motion_timer << std::endl;
            m_motion_timer /= 1.1f;
        }

        if (m_motion_timer < 1.0f && m_motion_happening)
        {
            std::cout << "motion stopped" << std::endl;
            m_replayBuf.save("replays/" + getFormattedTime() + ".mp4");
            m_replayBuf.disable();
        }

        if (m_motion_timer < 1.0f)
        {
            m_motion_happening = false;
        }

        cv::imshow("Camera Motion", m_fgMask);
    }

private:
    cv::Ptr<cv::BackgroundSubtractor> m_bgSubtractor;
    cv::Mat m_fgMask;

    float m_motion_timer = 0.0f;
    bool m_motion_happening = false;

    ReplayBuf m_replayBuf;
};