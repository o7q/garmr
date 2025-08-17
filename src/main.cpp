#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "camera.hpp"
#include "replay.hpp"

#include "time.hpp"

int main(int argc, char **argv)
{
    {
        Camera camera("/dev/v4l/by-id/usb-046d_HD_Pro_Webcam_C920_D99D197F-video-index0");

        bool active = true;

        while (active)
        {
            camera.update();

            if (cv::waitKey(1) == 27)
            {
                active = false;
            }
        }
    }

    cv::destroyAllWindows();

    return 0;
}
