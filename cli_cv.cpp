#include "socket.hpp"
#include "sockmat.hpp"
#include <cv.hpp>
#include <iostream>
using namespace cv;

int main()
{
    Socket cli;
    cli.CreateAddr("127.0.0.1",8888);
    cli.Connect();
    int recvfd = cli.Getfd();
    int buf[3] = {0};

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
        printf("channel = %d,width = %d,height = %d\n",buf[0],buf[1],buf[2]);
        send(recvfd,"OK",2,0);

        SockMat recvmat(buf[1],buf[2]);
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