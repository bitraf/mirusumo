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
static const char *modeNames[VisualizationModes] = {
    "real",
    "IR",
    "filtered IR",
    "B/W & IR",
    "real & trackpoints"
};


void visualize(cv::Mat &real, cv::Mat &ir, cv::Mat &filteredIr,
               const std::vector<cv::KeyPoint> &keyPoints, VisualizationMode mode)
{
    const float robotSize = 20;
    cv::Mat bwReal, temp;
    cv::Mat &out = real;

    switch (mode) {
    case RealVideoOnly:
        out = real;
        break;
    case IrVideoOnly:
        out = ir;
        break;
    case FilteredIrOnly:
        out = filteredIr;
        break;
    case CombinedRealAndIr:
        cv::cvtColor(real, bwReal, CV_BGR2GRAY);
        cv::addWeighted(bwReal, 0.8, filteredIr, 0.7, 0.0, temp);
        cv::imshow("cam", temp);
        out = temp;
        break;
    case CombinedRealAndTrackpoints:
        cv::drawKeypoints(real, keyPoints, temp, CV_RGB(0,255,0), cv::DrawMatchesFlags::DEFAULT);
        for (int i=0; i<keyPoints.size(); i++) {
            const cv::KeyPoint &kp = keyPoints[i];
            cv::Point topRight(kp.pt.x-(robotSize/2), kp.pt.y-robotSize/2);
            cv::Point bottomLeft(kp.pt.x+(robotSize/2), kp.pt.y+(robotSize/2));
            cv::rectangle(temp, topRight, bottomLeft, 0xFF00FF);
        }
        out = temp;
        break;
    default:
        throw std::runtime_error("Invalid visualization mode");
    }

    // Show which mode is active
    std::string text(modeNames[mode]);
    const int fontFace = cv::FONT_HERSHEY_PLAIN;
    const double fontScale = 2.5;
    const int thickness = 2;
    int baseline=0;
    cv::Size textSize = cv::getTextSize(text, fontFace,
                          fontScale, thickness, &baseline);
    cv::Point textOrg((out.cols - textSize.width)/2,
                  (out.rows - textSize.height));
    cv::putText(out, text, textOrg, fontFace, fontScale,
        cv::Scalar::all(255), thickness, 8);

    cv::imshow("cam", out);
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

