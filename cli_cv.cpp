#include "socket.hpp"
#include "sockmat.hpp"
#include <cv.hpp>
using namespace cv;

int main()
{
    Socket cli;
    SockMat recvmat(640,480);

    cli.CreateAddr("127.0.0.1",8888);
    cli.Connect();
    int recvfd = cli.Getfd();
    int buf[1] = {0};

    while(1)
    {
        if(recv(recvfd,buf,sizeof(buf),0) <= 0)
        {
            perror("recv failed:");
            exit(1);
        }
        if(buf[0] != 3)
        {
            cout << "channels error" <<endl;
            exit(1);
        }
        cout<< "vedio channels = " << buf[0] << std::endl;
        send(recvfd,"OK",2,0);

        Mat image;
        while(1)
        {
            image = recvmat.Receive(recvfd);
            imshow("test",image);
            waitKey(30);
        }
    }
    return 0;
}