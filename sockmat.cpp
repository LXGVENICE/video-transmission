#include "sockmat.hpp"
#include <stdio.h>
#include <iostream>
#include <strings.h>
#include <cv.hpp>
#include <sys/types.h>
#include <sys/socket.h>

//typedef unsigned char uchar;

SockMat::SockMat(int width,int height):_width(width),_height(height),_size(height*width*3)
{
    _buf = new uchar[_size];
    bzero(_buf,_size);
}

bool SockMat::Transmit(cv::Mat& image,int sendfd)
{
    if(image.empty()) return false;
    ScanImage_R(image);
    //printf("%d\n",image.total());
    if(send(sendfd,_buf,_size,0) == -1)
    {
        perror("send error ser1:");
        return false;
    }
    bzero(_buf,_size);
    return true;
}

cv::Mat SockMat::Receive(int recvfd)
{
    int len = 1;
    if(recv(recvfd,_buf,_size,0) <= 0)
    {
        perror("recv failed cil2:");
        exit(1);
    }
    cv::Mat image(_height,_width,CV_8UC3,cv::Scalar(0,0,255)); 
    //printf("%d\n",image.total());
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

