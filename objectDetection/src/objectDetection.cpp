// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv4/opencv2/opencv.hpp>   // Include OpenCV API

#define WIDTH 1280
#define HEIGHT 720


int main(int argc, char * argv[]) {
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;

    
    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration

    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, WIDTH, HEIGHT, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, WIDTH, HEIGHT, RS2_FORMAT_Z16, 30);

    // Start the pipeline with the specified configuration
    pipe.start(cfg);

    cv::Size size = cv::Size(WIDTH, HEIGHT);

    // Initialize variables
    rs2::frameset data;
    rs2::frame color, depth;

    cv::Mat frameResize, gray, thresh, frameReference;
    int widthResize = WIDTH;
    int heightResize = HEIGHT;
    int coefGaussianBlur = 21;
    int frameCount = 0;
    int framesBetweenReference = 1000;
    bool resetFrameReference = 0;

    int cX = 0, oldCX = 0;
    int cY = 0, oldCY = 0;
    double coefAverageMovingFilter = 0.3;
    cv::Moments m;
    std::string objectDetected ;

    int lastFrameWithMovement = 0;
    int deadZone = 100;

    while (true)
    {
        // Wait for new frames
        data = pipe.wait_for_frames(); // Wait for new frames

        // Get the color image
        color = data.get_color_frame();
        depth = data.get_depth_frame().apply_filter(color_map);

        // Check that the frames are valid
        if (!color || !depth) continue;

        // Convert the color image to cv::Mat (BGR format for OpenCV)
        cv::Mat colorFrame(size, CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
        cvtColor(colorFrame, colorFrame, cv::COLOR_RGB2BGR); // Convert from RGB to BGR

        // Convert the depth image to cv::Mat
        cv::Mat depthFrame(size, CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);

        cv::resize(colorFrame, frameResize, cv::Size(widthResize, heightResize));
        cv::cvtColor(frameResize, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);
        cv::GaussianBlur(gray, gray, cv::Size(coefGaussianBlur, coefGaussianBlur), 0);

        // Reset the reference image every x frames or during camera movement
        if (frameCount == 0 || resetFrameReference)
        {
            frameReference = gray.clone();
            resetFrameReference = false;
        }

        // cv::imshow("Frame Reference", frameReference);

        cv::absdiff(frameReference, gray, thresh);
        cv::threshold(thresh, thresh, 25, 255, cv::THRESH_BINARY);
        cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);
        // cv::erode(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2); // The cv::erode function in OpenCV is used to perform erosion on an image. Erosion is a morphological operation that reduces white objects on a black background. It works by sliding a kernel (or structuring element) over the image and replacing each pixel with the minimum value of the pixels covered by the kernel.

        if (cv::countNonZero(thresh) > 2000){

            m = cv::moments(thresh, true);

            cX = coefAverageMovingFilter * ((m.m00 > 1) ? m.m10 / m.m00 : cX) + (1 - coefAverageMovingFilter) * cX;
            
            cY = coefAverageMovingFilter * ((m.m00 > 1) ? m.m01 / m.m00 : cY) + (1 - coefAverageMovingFilter) * cY;

            if (abs(cX - oldCX) < deadZone || abs(cY - oldCY) < deadZone)
            {
                
                if (frameCount - lastFrameWithMovement > 100) objectDetected = "object detected at " + std::to_string(cX) + ", " + std::to_string(cY);
                else objectDetected = "";
            } else {
                lastFrameWithMovement = frameCount;
            }

            oldCX = cX;
            oldCY = cY;
        }

        // Display
        // Display the number of white points in the upper half of the image on the image
        cv::putText(thresh, (std::string) "nb px blancs Area : " + std::to_string(cv::countNonZero(thresh)), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255), 2);

        cv::putText(colorFrame, objectDetected, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255), 2);
        
        cv::putText(colorFrame, std::to_string(frameCount - lastFrameWithMovement), cv::Point(10, HEIGHT-10), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255), 2);

        // Display the centroid on the image
        cv::circle(colorFrame, cv::Point(cX, cY), 5, cv::Scalar(0, 255, 0), -1);

        // Display the images
        imshow("Normal Image", colorFrame);
        // imshow("Depth Image", depthFrame);
        imshow("Thresh", thresh);

        ++frameCount;

        if (cv::waitKey(1) == 'q') break;
    }

    return EXIT_SUCCESS;
}
