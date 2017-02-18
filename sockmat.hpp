#pragma once
#include <cv.hpp>

typedef unsigned char uchar

class SockMat
{
public:
    SockMat(int width,int height,const int resolution);
    bool Transmit(cv::Mat& image);
private:
    void ScanImage(cv::Mat image)
private:
    uchar _buf[BUFFER_SIZE];
    int _width;
    int _height;
    const int BUFFER_SIZE;
}