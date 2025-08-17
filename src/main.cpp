#include <iostream>

#include <opencv2/opencv.hpp>

#include "camera_station.hpp"

int main(int argc, char **argv)
{
    CameraStation cameraStation;

    cameraStation.add("/dev/v4l/by-id/usb-046d_HD_Pro_Webcam_C920_D99D197F-video-index0");

    getchar();
    cameraStation.stop();
    cv::destroyAllWindows();
    
    return 0;
}
