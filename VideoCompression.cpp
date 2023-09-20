#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include<iostream>
using namespace cv;

int main()
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return 1;
    }
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    cv::VideoWriter video_mp4("snimak_kamere.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 30, cv::Size(frame_width, frame_height)); //MPEG-4
    cv::VideoWriter video_avi("snimak_kamere.avi", cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, cv::Size(frame_width, frame_height));
    cv::VideoWriter video_mkv("snimak_kamere.mkv", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(frame_width, frame_height)); //MJPEG

    for(;;) {
        Mat frame;
        cap >> frame;
        if(frame.empty()) break;
        resize(frame,frame,cv::Size(640,480));
        video_avi.write(frame);
        video_mkv.write(frame);
        video_mp4.write(frame);
        cv::imshow("Frame", frame);
        char c = (char)cv::waitKey(25);
        if(c==27) break;
    }
    video_avi.release();
    video_mkv.release();
    video_mp4.release();
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
