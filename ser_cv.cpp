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
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);  
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480); 
    Mat frame;
    capture >> frame;
    int buf[1] = {frame.channels()};
    std::cout << "video channels = " << buf[0] << std::endl;
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
            printf("sdf\n");
            if (!capture.isOpened()) return 0;
            capture >> frame;
            transmat.Transmit(frame,clifd);
        }
    }
    return 0;
}
