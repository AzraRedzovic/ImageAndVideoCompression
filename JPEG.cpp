#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include<iostream>
using namespace cv;

float Luminance[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};
float Chrominance[8][8] = {
    {17, 18, 24, 27, 99, 99, 99, 99},
    {18, 21, 26, 66, 99, 99, 99, 99},
    {24, 26, 56, 99, 99, 99, 99, 99},
    {47, 66, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99}
};

int main()
{
    namedWindow("Original image");
    Mat image, compressedImage, decompressedImage;
    int height = 480; int width = 640;
    image = imread("slika.jpg", cv::IMREAD_COLOR); //CV_8U
    resize(image,image,Size(640,480));
    imshow("Original image", image);

    //                KOMPRESIJA              //
    //RGB to YCbCr
    Mat image_y;
    namedWindow("YCrCb image");
    cvtColor(image,image_y,COLOR_BGR2YCrCb);
    imshow("YCrCb image", image_y);

    //Razdvajanje Y, Cr i Cb
    std::vector<Mat>YCrCb(3);
    split(image_y,YCrCb);
    Mat Y = YCrCb[0]; Mat Cr = YCrCb[1]; Mat Cb = YCrCb[2];
    namedWindow("Y"); namedWindow("Cr"); namedWindow("Cb");
    imshow("Y", YCrCb[0]); imshow("Cr", YCrCb[1]); imshow("Cb", YCrCb[2]);

    //Downsampling
    resize(Cr, Cr, Size(width/2,height/2));
    resize(Cb, Cb, Size(width/2,height/2));

    for(int i=0; i<Y.cols; i+=8){
        for(int j=0; j<Y.rows; j+=8){
            Mat blockY = Y(Rect(i,j,8,8));
            blockY.convertTo(blockY,CV_64FC1);

            //oduzima se srednja vrijednost(128)
            for(int x=0; x<blockY.cols; x++){
                for(int y=0; y<blockY.rows; y++){
                    blockY.at<float>(x,y) = blockY.at<float>(x,y) - 128.0;
                }
            }

            //DCT
            dct(blockY,blockY);
            //Kvantizacija
            for(int x=0; x<blockY.cols; x++){
                for(int y=0; y<blockY.rows; y++){
                    blockY.at<float>(x,y) = blockY.at<float>(x,y) / Luminance[x][y];
                }
            }
            blockY.convertTo(blockY, CV_8UC1);
            blockY.copyTo(YCrCb[0](Rect(i,j,8,8)));
        }
    }
    for(int i=0; i<Cr.cols; i+=8){
        for(int j=0; j<Cr.rows; j+=8){
            Mat blockCr = Cr(Rect(i,j,8,8));
            Mat blockCb = Cb(Rect(i,j,8,8));
            blockCr.convertTo(blockCr,CV_64FC1);
            blockCb.convertTo(blockCb,CV_64FC1);

            //oduzima se srednja vrijednost(128)
            for(int x=0; x<blockCr.cols; x++){
                for(int y=0; y<blockCr.rows; y++){
                    blockCr.at<float>(x,y) = blockCr.at<float>(x,y) - 128;
                    blockCb.at<float>(x,y) = blockCb.at<float>(x,y) - 128;
                }
            }

            //DCT
            dct(blockCr,blockCr);
            dct(blockCb,blockCb);
            //Kvantizacija
            for(int x=0; x<blockCr.cols; x++){
                for(int y=0; y<blockCr.rows; y++){
                    blockCr.at<float>(x,y) = blockCr.at<float>(x,y) / Chrominance[x][y];
                    blockCb.at<float>(x,y) = blockCb.at<float>(x,y) / Chrominance[x][y];
                }
            }
            blockCr.convertTo(blockCr, CV_8UC1);
            blockCb.convertTo(blockCb, CV_8UC1);
            blockCr.copyTo(YCrCb[1](Rect(i,j,8,8)));
            blockCb.copyTo(YCrCb[2](Rect(i,j,8,8)));
        }
    }
    merge(YCrCb,compressedImage);
    namedWindow("Compressed image");
    imshow("Compressed image", compressedImage);
    imwrite("compressed_image.jpg", compressedImage);

    //               DEKOMPRESIJA             //
    std::vector<Mat>YCrCb2(3);
    split(compressedImage,YCrCb2);
    Mat Y2 = YCrCb2[0]; Mat Cr2 = YCrCb2[1]; Mat Cb2 = YCrCb2[2];
    for(int i=0; i<Y2.cols; i+=8){
        for(int j=0; j<Y2.rows; j+=8){
            Mat blockY = Y2(Rect(i,j,8,8));
            blockY.convertTo(blockY,CV_64FC1);

            //dodaje se srednja vrijednost(128)
            for(int x=0; x<blockY.cols; x++){
                for(int y=0; y<blockY.rows; y++){
                    blockY.at<float>(x,y) = blockY.at<float>(x,y) + 128.0;
                }
            }

            //inverzna DCT
            idct(blockY,blockY);
            //Dekvantizacija
            for(int x=0; x<blockY.cols; x++){
                for(int y=0; y<blockY.rows; y++){
                    blockY.at<float>(x,y) = blockY.at<float>(x,y) * Luminance[x][y];
                }
            }
            blockY.convertTo(blockY, CV_8UC1);
            blockY.copyTo(YCrCb2[0](Rect(i,j,8,8)));
        }
    }
    for(int i=0; i<Cr2.cols; i+=8){
        for(int j=0; j<Cr2.rows; j+=8){
            Mat blockCr = Cr2(Rect(i,j,8,8));
            Mat blockCb = Cb2(Rect(i,j,8,8));
            blockCr.convertTo(blockCr,CV_64FC1);
            blockCb.convertTo(blockCb,CV_64FC1);

            //dodaje se srednja vrijednost(128)
            for(int x=0; x<blockCr.cols; x++){
                for(int y=0; y<blockCr.rows; y++){
                    blockCr.at<float>(x,y) = blockCr.at<float>(x,y) + 128;
                    blockCb.at<float>(x,y) = blockCb.at<float>(x,y) + 128;
                }
            }

            //inverzna DCT
            idct(blockCr,blockCr);
            idct(blockCb,blockCb);
            //Dekvantizacija
            for(int x=0; x<blockCr.cols; x++){
                for(int y=0; y<blockCr.rows; y++){
                    blockCr.at<float>(x,y) = blockCr.at<float>(x,y) * Chrominance[x][y];
                    blockCb.at<float>(x,y) = blockCb.at<float>(x,y) * Chrominance[x][y];
                }
            }
            blockCr.convertTo(blockCr, CV_8UC1);
            blockCb.convertTo(blockCb, CV_8UC1);
            blockCr.copyTo(YCrCb2[1](Rect(i,j,8,8)));
            blockCb.copyTo(YCrCb2[2](Rect(i,j,8,8)));
        }
    }
    merge(YCrCb2,decompressedImage);
    //YCbCr to RGB
    cvtColor(decompressedImage,decompressedImage,COLOR_YCrCb2BGR);
    namedWindow("Decompressed image");
    imshow("Decompressed image", decompressedImage);
    imwrite("decompressed_image.jpg", decompressedImage);
    waitKey(0);
    return 0;
}





