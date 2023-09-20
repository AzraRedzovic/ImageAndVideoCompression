#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include<iostream>
using namespace cv;

int main(){
    namedWindow("Original image");
    Mat image = imread("LenaImage.jpg", cv::IMREAD_COLOR);
    imshow("Original image", image);

    //Lossless compression - PNG
    namedWindow("Lossless compressed image");
    imwrite("LenaImageLossless.png", image);
    Mat image2 = imread("LenaImageLossless.png", cv::IMREAD_COLOR);
    imshow("Lossless compressed image", image2);

    //Lossy compression - JPEG, faktor kvalitete 90
    std::vector<int>parametri;
    parametri.push_back(IMWRITE_JPEG_QUALITY);
    parametri.push_back(90);
    namedWindow("Lossy compressed image - factor quality 90");
    imwrite("LenaImageLossy90.jpg", image, parametri);
    Mat image3 = imread("LenaImageLossy90.jpg", cv::IMREAD_COLOR);
    imshow("Lossy compressed image - factor quality 90", image3);
    //Lossy compression - JPEG, faktor kvalitete 10
    std::vector<int>parametri2;
    parametri2.push_back(IMWRITE_JPEG_QUALITY);
    parametri2.push_back(10);
    namedWindow("Lossy compressed image - factor quality 10");
    imwrite("LenaImageLossy10.jpg", image, parametri2);
    Mat image4 = imread("LenaImageLossy10.jpg", cv::IMREAD_COLOR);
    imshow("Lossy compressed image - factor quality 10", image4);
    waitKey(0);
    return 0;
}
