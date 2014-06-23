#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

static const int    kernel_size = 7;
static const double std_dev     = 1.5;
static const int    block_size  = 47;
static const int    fc          = -5; 

enum VisualizationMode {
    RealVideoOnly = 0,
    IrVideoOnly,
    FilteredIrOnly,
    CombinedRealAndIr,
    CombinedRealAndTrackpoints,
    VisualizationModes
};

void visualize(const cv::Mat &real, const cv::Mat &ir, const cv::Mat &filteredIr,
               const std::vector<cv::KeyPoint> &keyPoints, VisualizationMode mode)
{
    const float robotSize = 20;
    cv::Mat bwReal, out;

    switch (mode) {
    case RealVideoOnly:
        cv::imshow("cam", real);
        break;
    case IrVideoOnly:
        cv::imshow("cam", ir);
        break;
    case FilteredIrOnly:
        cv::imshow("cam", filteredIr);
        break;
    case CombinedRealAndIr:
        cv::cvtColor(real, bwReal, CV_BGR2GRAY);
        cv::addWeighted(bwReal, 0.8, filteredIr, 0.7, 0.0, out);
        cv::imshow("cam", out);
        break;
    case CombinedRealAndTrackpoints:
        cv::drawKeypoints(real, keyPoints, out, CV_RGB(0,255,0), cv::DrawMatchesFlags::DEFAULT);
        for (int i=0; i<keyPoints.size(); i++) {
            const cv::KeyPoint &kp = keyPoints[i];
            cv::Point topRight(kp.pt.x-(robotSize/2), kp.pt.y-robotSize/2);
            cv::Point bottomLeft(kp.pt.x+(robotSize/2), kp.pt.y+(robotSize/2));
            cv::rectangle(out, topRight, bottomLeft, 0xFF00FF);
        }
        cv::imshow("cam", out);
        break;
    default:
        throw std::runtime_error("Invalid visualization mode");
    }
}

// FIXME: implement camera offset calibration
// TODO: allow to configure which stream is what
int main() {
    cv::VideoCapture irStream(0);
    cv::VideoCapture realStream(1);
     
    if (!irStream.isOpened() || !realStream.isOpened()) {
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

    VisualizationMode mode = RealVideoOnly;
    while (true) {
        cv::Mat in, filtered, blurred, balanced, thresholded, real;

        irStream.read(in);
        realStream.read(real);

        // Process/filter    
        cv::cvtColor(in, filtered, CV_BGR2GRAY);
        cv::GaussianBlur(filtered, blurred, cv::Size(kernel_size, kernel_size), std_dev, std_dev);
//        cv::bsdiff(blurred, balanced, gaussian_frame);
        cv::adaptiveThreshold(blurred, thresholded, 255, cv::ADAPTIVE_THRESH_MEAN_C,
                                cv::THRESH_BINARY, block_size, fc);

        // Detect
        blobDetector.detect(thresholded, keyPoints);

        // Visualize
        visualize(real, in, thresholded, keyPoints, mode);

        const char key = cv::waitKey(30);
        switch (key) {
        case 's':
            mode = (VisualizationMode)(mode+1);
            if (mode >= VisualizationModes) {
                mode = (VisualizationMode)0;
            }
        default:
            break;
        };
    }
    return 0;
}

