#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

static const int    kernel_size = 7;
static const double std_dev     = 1.5;
static const int    block_size  = 47;
static const int    fc          = -5; 

int main() {
    cv::VideoCapture stream1(0);
     
    if (!stream1.isOpened()) {
        std::cout << "cannot open camera";
    }

    cv::SimpleBlobDetector::Params params;
    params.minThreshold = 40;
    params.maxThreshold = 60;
    params.thresholdStep = 5;

    params.minArea = 20; 
    params.minConvexity = 0.3;
    params.minInertiaRatio = 0.01;

    params.maxArea = 8000;
    params.maxConvexity = 10;

    params.filterByColor = false;
    params.filterByCircularity = false;

    cv::SimpleBlobDetector blobDetector(params);
    blobDetector.create("SimpleBlob");

    std::vector<cv::KeyPoint> keyPoints;
    std::vector<std::vector <cv::Point> > contours;

    while (true) {
        cv::Mat in, filtered, blurred, balanced, thresholded, out;
        stream1.read(in);

        // Filter    
        cv::cvtColor(in, filtered, CV_BGR2GRAY);
        cv::GaussianBlur(filtered, blurred, cv::Size(kernel_size, kernel_size), std_dev, std_dev);
//        cv::bsdiff(blurred, balanced, gaussian_frame);
        cv::adaptiveThreshold(blurred, thresholded, 255, cv::ADAPTIVE_THRESH_MEAN_C,
                                cv::THRESH_BINARY, block_size, fc);

        // Detect
        blobDetector.detect(thresholded, keyPoints);

        // Visualize
        cv::drawKeypoints(thresholded, keyPoints, out, CV_RGB(0,255,0), cv::DrawMatchesFlags::DEFAULT);

        cv::imshow("cam", out);
        if (cv::waitKey(30) >= 0)
            break;
    }
    return 0;
}
