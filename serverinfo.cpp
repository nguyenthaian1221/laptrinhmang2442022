// Tao serverinfo TCP
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
    // Khoi tao winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Tao socket cho ket noi
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Tao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int port = strtol(argv[1], NULL, 10);
    addr.sin_port = htons(port);

    // Gan socket voi cau truc dia chi
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));

    // Chuyen socket sang trang thai cho ket noi
    listen(listener, 5);

    // Chap nhan ket noi
    printf("\nDang cho ket noi...\n");

    SOCKADDR_IN clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    SOCKET client = accept(listener, (SOCKADDR*)&clientAddr, &clientAddrLen);
    printf("\nKet noi moi: %d, dia chi IP: %s\n", client, inet_ntoa(clientAddr.sin_addr));



    printf("\nDang cho du lieu...\n");


    char buf[1000];

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);

        if (ret <= 0)
        {
            if (ret == -1)
                ret = WSAGetLastError();
            break;
        }


        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s", buf);
    }

    printf("Ket thuc nhan du lieu");

    // Dong ket noi
    closesocket(client);
}