#include "replay.hpp"

#include "time.hpp"

ReplayBuf::ReplayBuf(unsigned int width,
                     unsigned int height,
                     unsigned int start_padding_time)
{
    m_frame_size = cv::Size(width, height);
    m_start_padding_time = start_padding_time;
}

ReplayBuf::~ReplayBuf()
{
}

void ReplayBuf::update(const cv::Mat &frame)
{
    m_start_padding_buffer.push_back(frame.clone());

    while (m_start_padding_buffer.size() > m_start_padding_time * m_framerate)
    {
        m_start_padding_buffer.pop_front();
    }

    if (m_do_padding_write)
    {
        for (const cv::Mat &start_padding_frame : m_start_padding_buffer)
        {
            m_video_writer.write(start_padding_frame);
        }

        m_do_padding_write = false;
    }

    if (m_write_active)
    {
        m_video_writer.write(frame);
    }
}

void ReplayBuf::setEnabled(const std::string &path)
{
    if (m_write_active)
    {
        return;
    }

    m_video_writer.open(path + "/motion_" + getFormattedTime() + ".mp4", cv::VideoWriter::fourcc('a', 'v', 'c', '1'), m_framerate, m_frame_size);

    if (!m_video_writer.isOpened())
    {
        std::cerr << "[Error] ReplayBuf: Failed to create VideoWriter" << std::endl;
        return;
    }

    m_write_active = true;
    m_do_padding_write = true;
}

void ReplayBuf::disable()
{
    m_write_active = false;
    m_do_padding_write = false;

    if (m_video_writer.isOpened())
    {
        m_video_writer.release();
    }
}