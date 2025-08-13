#pragma once

#include <deque>
#include <opencv2/opencv.hpp>

class ReplayBuf
{
public:
    ReplayBuf(unsigned int width,
              unsigned int height,
              unsigned int start_padding_time);

    ~ReplayBuf();

    void setFramerate(float framerate)
    {
        if (!m_write_active)
        {
            m_framerate = framerate;
        }
    }

    void update(const cv::Mat &frame);

    void setEnabled(const std::string &path);
    void disable();

private:
    cv::VideoWriter m_video_writer;
    std::deque<cv::Mat> m_start_padding_buffer;

    bool m_write_active = false;
    bool m_do_padding_write = false;

    float m_framerate = 1.0f;

    cv::Size m_frame_size;
    unsigned int m_start_padding_time = 0;
};