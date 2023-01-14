#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
const double MARKER_MM = 30;

std::vector<cv::Point2f> fromPixelsToMm(std::vector<cv::Point2f> original)
{
    std::vector<cv::Point2f> result;
    double distances = 0;
    for (int i = 0; i < 4; i++) {
        cv::Point2f v = (original[i] - original[(i + 1) % 4]);

        distances += std::sqrt(v.x * v.x + v.y * v.y);
    }
    distances = distances / 4;

    for (int i = 0; i < 4; i++) {
        result.push_back({static_cast<float>(MARKER_MM * (original[i].x / distances)), static_cast<float>(MARKER_MM * (original[i].y / distances))});
    }
    return result;
}

int main(int argc, char **argv) {
    bool capturing = true;
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }
    std::cout << "Video size: " << cap.get(cv::CAP_PROP_FRAME_WIDTH)
              << "x" << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();
    detectorParams->cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX; // Dokładne wyznaczenie rogu markerów

    do {

        cv::Mat frame;
        if (cap.read(frame)) {

            std::vector<int> ids;
            std::vector<std::vector<cv::Point2f>> corners;
            cv::aruco::detectMarkers(frame, dictionary, corners, ids, detectorParams);

            if (ids.size() > 1) {
                std::vector<cv::Point2f> marker1 = fromPixelsToMm(corners[0]);
                std::cout << "Marker 1: " << marker1[0] << ", " << marker1[1] << ", " << marker1[2] << ", " << marker1[3] << std::endl;
                std::vector<cv::Point2f> marker2 = fromPixelsToMm(corners[1]);
                std::cout << "Marker 2: " << marker2[0] << ", " << marker2[1] << ", " << marker2[2] << ", " << marker2[3] << std::endl;
                std::cout << "Distance between markers: " << cv::norm(marker1[0] - marker2[0]) << " mm" << std::endl;
                cv::line(frame, corners[0][0], corners[1][0], cv::Scalar(0, 0, 255), 2);
                //draw distance on the line
                cv::putText(frame, std::to_string(cv::norm(marker1[0] - marker2[0])) + " mm", cv::Point((corners[0][0].x + corners[1][0].x) / 2, (corners[0][0].y + corners[1][0].y) / 2), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
                //draw box around markers
                for (int i = 0; i < ids.size(); i++) {
                    cv::aruco::drawDetectedMarkers(frame, corners, ids);
                }
            }

            cv::imshow("Output", frame);
            int key = cv::waitKey(1);
            if (key == 27) {
                capturing = false;
            }
        } else {
            std::cerr << "error reading frame" << std::endl;
            capturing = false;
        }
    } while (capturing);
    return 0;
}