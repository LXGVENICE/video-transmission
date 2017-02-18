#include "socket.hpp"
#include "sockmat.hpp"
#include <cv.hpp>
#include <iostream>
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
            perror("recv failed cli1:");
            exit(1);
        }
        if(buf[0] != 3)
        {
            std::cout << "channels error" <<std::endl;
            exit(1);
        }
        std::cout<< "vedio channels = " << buf[0] << std::endl;
        send(recvfd,"OK",2,0);

        Mat image;
        int i = 0;
        while(1)
        {
            image = recvmat.Receive(recvfd);
            //if(i == 23)
            //{
            //    for(int j = 0;j < i;++j)
            //    {    
                    imshow("test",image);
                    waitKey(30);
                //}
            //    i == -1;
           // }
           // i++;
        }
    }
    return 0;
}