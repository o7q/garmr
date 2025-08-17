#pragma once

#include <deque>
#include <opencv2/opencv.hpp>

class ReplayBuf
{
public:
    ReplayBuf(const ReplayBuf &other) = delete;
    ReplayBuf &operator=(const ReplayBuf &other) = delete;
    ReplayBuf() {}
    ~ReplayBuf() { disable(); }

    void setWriteDirectory(const std::string &directory)
    {
        m_write_directory = directory;
    }

    void setSize(int width, int height)
    {
        m_frame_size.width = width;
        m_frame_size.height = height;
    }

    void setFramerate(float framerate)
    {
        if (!m_write_active)
        {
            m_framerate = framerate;
        }
    }

    void setStartPaddingTime(int padding)
    {
        m_start_padding_time = padding;
    }

    void update(const cv::Mat &frame);

    void enable();
    void disable();

private:
    cv::VideoWriter m_video_writer;
    std::deque<cv::Mat> m_start_padding_buffer;

    bool m_write_active = false;
    bool m_do_padding_write = false;

private:
    cv::Size m_frame_size;
    float m_framerate = 1.0f;

    int m_start_padding_time = 3;
    std::string m_write_directory = "./replays";
};
