#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


int main(int argc, char **argv) {
    bool capturing = true;
    // Question for you
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }
    std::cout << "Video size: " << cap.get(cv::CAP_PROP_FRAME_WIDTH)
              << "x" << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    do {
        cv::Mat frame;
        if (cap.read(frame)) {
            //red area only
            cv::Mat redOnly;
            cv::inRange(frame, cv::Scalar(0, 0, 100), cv::Scalar(100, 100, 255), redOnly);
            //Blur
            cv::blur(redOnly, redOnly, cv::Size(9, 9));
            cv::dilate(redOnly, redOnly, cv::Mat(), cv::Point(-1, -1), 2);
            cv::erode(redOnly, redOnly, cv::Mat(), cv::Point(-1, -1), 2);
            //dilate erode
            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;
            cv::findContours(redOnly, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
            std::vector<std::vector<cv::Point>> largest_contours;
            std::vector<cv::Vec4i> largest_hierarchy;
            double epsilon = 5;
            std::vector<double> areas;
            for (int i = 0; i < contours.size(); i++) {
                areas.push_back(cv::contourArea(contours[i]));
            }
            std::sort(areas.begin(), areas.end());
            for (int i = 0; i < contours.size(); i++) {
                if (cv::contourArea(contours[i]) == areas[areas.size() - 1]) {
                    largest_contours.push_back(contours[i]);
                    largest_hierarchy.push_back(hierarchy[i]);
                }
                if (cv::contourArea(contours[i]) == areas[areas.size() - 2]) {
                    largest_contours.push_back(contours[i]);
                    largest_hierarchy.push_back(hierarchy[i]);
                }
            }
            cv::Mat drawing = cv::Mat::zeros(redOnly.size(), CV_8UC3);
            for (int i = 0; i < largest_contours.size(); i++) {
                cv::Scalar color = cv::Scalar(255, 255, 255);
                cv::drawContours(drawing, largest_contours, i, color, 2, 8, largest_hierarchy, 0, cv::Point());
            }
            //find the center of the two biggest contours
            cv::Moments m1 = cv::moments(largest_contours[0]);
            cv::Moments m2 = cv::moments(largest_contours[1]);
            cv::Point p1 = cv::Point(m1.m10 / m1.m00, (m1.m01 / m1.m00) - 10);
            cv::Point p2 = cv::Point(m2.m10 / m2.m00, (m2.m01 / m2.m00) - 10);
            cv::Point p3 = cv::Point(p1.x, p1.y + 20);
            cv::Point p4 = cv::Point(p2.x, p2.y + 20);


            if (p1.y > p2.y - 20 && p1.y < p2.y + 20) {
                //check circles size
                double dist = cv::norm(p1 - p2);
                int MinThickness = 3;
                int MaxThickness = 10;
                if (dist > 250) {
                    for (int i = MinThickness; i < MaxThickness; i++) {
                        MaxThickness = i;
                        cv::line(frame, p1, p2, cv::Scalar(0, 255, 0), i);
                        cv::line(frame, p3, p4, cv::Scalar(255, 0, 0), i);
                    }
                } else {
                    for (int i = MaxThickness; i > MinThickness; i--) {
                        MaxThickness = i;
                        cv::line(frame, p1, p2, cv::Scalar(0, 255, 0), i);
                        cv::line(frame, p3, p4, cv::Scalar(255, 0, 0), i);
                    }
                }
                std::cout << dist << std::endl;
            }
            cv::imshow("redOnly", redOnly);
            cv::imshow("frame", frame);

            //czekaj na klawisz, sprawdz czy to jest 'esc'
            if ((cv::waitKey(1000.0 / 60.0) & 0x0ff) == 27) capturing = false;
        }
    } while (capturing);
    return 0;
}
