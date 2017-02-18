#include "socket.hpp"
#include "sockmat.hpp"
#include <cv.hpp>
using namespace cv;

int main()
{
    Socket ser;
    SockMat transmat(640,480,640*480);
    ser.CreateAddr("127.0.0.1",8888);
    ser.Bind();
    ser.Listen();
    VideoCapture capture(0);
    if(!capture.isOpened()) exit(1);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);  
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480); 

    Mat frame;
    while(1)
    {
        int clifd = ser.Accept();
        while(1)
        {
            if (!capture.isOpened()) return 0;
            capture >> frame;
            transmat.Transmit(frame);
        }
    }
}
