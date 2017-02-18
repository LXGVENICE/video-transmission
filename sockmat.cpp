#include "sockmat.hpp"
#include <stdio.h>
#include <strings.h>
#include <cv.hpp>
#include <sys/types.h>
#include <sys/socket.h>

//typedef unsigned char uchar;

SockMat::SockMat(int width,int height):_width(width),_height(height)
{
    bzero(_buf,0);
}

bool SockMat::Transmit(cv::Mat& image,int sendfd)
{
    if(image.empty()) return false;
    ScanImage_R(image);
    if(send(sendfd,_buf,sizeof(_buf),0) == -1)
    {
        perror("send error ser1:");
        return false;
    }
    bzero(_buf,0);
    return true;
}

cv::Mat SockMat::Receive(int recvfd)
{
    int len = 1;
    if(recv(recvfd,_buf,BUFFER_SIZE,0) <= 0)
    {
        perror("recv failed cil2:");
        exit(1);
    }
    cv::Mat image(_height,_width,CV_8UC3,cv::Scalar(0,0,255)); 
    ScanImage_W(image);
    return image;
}

bool SockMat::ScanImage_R(cv::Mat& image)
{
    if(image.empty()) return false;

    int channels = image.channels();
    int rows = _height;
    int cols = _width * channels;
    if(image.isContinuous())
    {
        cols *= rows;
        rows = 1;
    }
    uchar* p = NULL; 
    for(int i = 0;i < rows; ++i)
    {
        p = image.ptr<uchar>(i);
        for(int j = 0;j < cols; ++j)
        {
            _buf[i*rows + j] = p[j];
        } 
    }
    return true;
}

bool SockMat::ScanImage_W(cv::Mat& image)
{
    if(image.empty()) return false;

    int channels = 3;
    int rows = _height;
    int cols = _width * channels;

    uchar* p = NULL;
    for(int i = 0;i < rows; ++i)
    {
        p = image.ptr<uchar>(i);
        for(int j = 0;j < cols; ++j)
        {
            p[j] = _buf[i*rows + j];
        } 
    }
    return true;
}

