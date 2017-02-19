#include "socket.hpp"
#include "sockmat.hpp"
#include <cv.hpp>
#include <iostream>
using namespace cv;

int main()
{
    Socket ser;
    SockMat transmat(640,480);
    ser.CreateAddr("127.0.0.1",8888);
    ser.Bind();
    ser.Listen();
    VideoCapture capture(0);
    if(!capture.isOpened()) exit(1);
    int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);  
    int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT); 
    Mat frame;
    capture >> frame;
    int buf[3] = {frame.channels(),width,height};
    printf("channel = %d,width = %d,height = %d\n",buf[0],buf[1],buf[2]);
    char str[3] = {0};

    while(1)
    {
        int clifd = ser.Accept();
        send(clifd,buf,sizeof(buf),0);
        if(recv(clifd,str,2,0) <= 0)
        {
            perror("recv failed:");
            exit(1);
        }
        std::cout << "one client connected : " << str << std::endl;
        
        while(1)
        {
            if (!capture.isOpened()) return 0;
            capture >> frame;
            transmat.Transmit(frame,clifd);
        }
    }
    return 0;
}
