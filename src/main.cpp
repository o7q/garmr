#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "camera.hpp"
#include "replay.hpp"

#include "time.hpp"

int main()
{
    Camera camera(0);

    bool active = true;

    while (active)
    {
        camera.poll();

        if (cv::waitKey(1) == 27)
        {
            active = false;
        }
    }

    cv::destroyAllWindows();

    return 0;
}