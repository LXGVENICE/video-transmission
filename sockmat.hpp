#pragma once
#include <cv.hpp>

//typedef unsigned char uchar;
#define BUFFER_SIZE 640*480

class SockMat
{
public:
    SockMat(int width,int height);
    bool Transmit(cv::Mat& image,int sendfd);
    cv::Mat Receive(int recvfd);
private:
    bool ScanImage_R(cv::Mat& image);
    bool ScanImage_W(cv::Mat& image);
private:
    uchar _buf[BUFFER_SIZE];
    int _width;
    int _height;
};