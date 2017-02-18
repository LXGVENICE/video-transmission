#include "sockmat.hpp"
#include <strings.h>
#include <cv.hpp>
#include <sys/types.h>
#include <sys/socket.h>

typedef unsigned char uchar

SockMat::SockMat():_width(width),_height(height),BUFFER_SIZE(resolution)
{
    bzero(_buf,0);
}

bool SockMat::Transmit(cv::Mat& image,int sendfd)
{
    if(image.empty()) return false;
    if(send(sendfd,_buf,sizeof(_buf),0) == -1)
    {
        perror("send error:");
        return false;
    }
}

bool SockMat::ScanImage(cv::Mat image)
{
    if(image.empty()) return -1;

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
            _buf[i*rows + j] = p[i*rows + j];
        } 
    }
    return true;
}