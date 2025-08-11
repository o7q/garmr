#pragma once

#include <deque>
#include <opencv2/opencv.hpp>

class ReplayBuf
{
public:
    ReplayBuf(int width,
              int height,
              int framerate,
              int max_time,
              int time_padding_start)
    {
        m_frame_size = cv::Size(width, height);
        m_framerate = framerate;
        m_max_frames = max_time * framerate;
        m_time_padding_start = time_padding_start;

        disable();
    }

    ~ReplayBuf()
    {
    }

    void push(const cv::Mat &frame)
    {
        m_buffer.push_back(frame.clone());

        while (m_buffer.size() > m_keep_buffer && m_keep_buffer > 0)
        {
            m_buffer.pop_front();
        }
    }

    void save(const std::string &path)
    {
        cv::VideoWriter writer(path, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), m_framerate, m_frame_size);

        if (!writer.isOpened())
        {
            std::cerr << "[Error] ReplayBuf: Failed to create VideoWriter" << std::endl;
            return;
        }

        for (const cv::Mat &frame : m_buffer)
        {
            writer.write(frame);
        }

        writer.release();
    }

    void enable()
    {
        m_keep_buffer = m_max_frames;
    }

    void disable()
    {
        m_keep_buffer = m_time_padding_start * m_framerate;
    }

private:
    std::deque<cv::Mat> m_buffer;

    cv::Size m_frame_size;
    int m_framerate = 0;
    int m_max_frames = 0;
    int m_time_padding_start = 0;

    unsigned int m_keep_buffer = 0;
};