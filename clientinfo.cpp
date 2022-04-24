#include <WinSock2.h>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <atlstr.h>
#include<iostream>
#include <atlstr.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
using namespace std;
// Danh sach những disk của máy tính.
vector<string> getListOfDrives() {
    vector<string> arrayOfDrives;
    char* szDrives = new char[MAX_PATH]();
    if (GetLogicalDriveStringsA(MAX_PATH, szDrives));
    for (int i = 0; i < 100; i += 4)
        if (szDrives[i] != (char)0)
            arrayOfDrives.push_back(string{ szDrives[i],szDrives[i + 1],szDrives[i + 2] });
    delete[] szDrives;
    return arrayOfDrives;
}

char* printNameComputer()
{
    char buffer[256] = "";
    DWORD size = sizeof(buffer);
    if (GetComputerNameA(buffer, &size))
    {
        return buffer;
    }
}

int main(int argc, char* argv[])
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr = inet_addr(argv[1]);

    // chuyen doi char[] sang int
    int port = strtol(argv[2], NULL, 10);

    addr.sin_port = htons(port);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    // Gui ten computer den server
    const char* msg = printNameComputer();
    send(client, msg, strlen(msg), 0);
    // Gui danh sach o dia den server
    const char* msg1 = "\n==================\nDanh sach o dia cua may ban la: \n";
    send(client, msg1, strlen(msg1), 0);
    vector<string> dsDisk = getListOfDrives();
    for (string currentDrive : dsDisk) {

        char* char_arr;
        char_arr = &currentDrive[0];
        send(client, char_arr, strlen(char_arr), 0);


    }
    // xuong dong để nhìn cho đẹp
    const char* br = "\n====================\n";
    send(client, br, strlen(br), 0);
    // In dung lượng từng ổ cứng một
    for (string currentDrive : dsDisk) {

        char* char_arr;
        // Chuyển từ string về char* (LPCSTR)
        char_arr = &currentDrive[0];
        LPCSTR diskPath = char_arr;
        DWORD sectorsPerCluster;
        DWORD bytesPerSector;
        DWORD numberOfFreeClusters;
        DWORD totalNumberOfClusters;
        if (GetDiskFreeSpaceA(diskPath, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters))
        {
            char subchr_arr[1000];
            sprintf(subchr_arr, "\nGetDiskFreeSpaceA:\nSectorsPerCluster:\t %lu\nBytesPerSector:\t %lu\nNumberOfFreeClusters:\t %lu\nTotalNumberOfClusters:\t %lu\n=================\n", sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters);
            char* xauphu1 = &currentDrive[0];
            char* msg2 = strcat(xauphu1, "\n");
            send(client, msg2, strlen(msg2), 0);
            send(client, subchr_arr, strlen(subchr_arr), 0);


        }


    }

    // Nhan du lieu tu server va in ra man hinh
    char buf[256];
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;

        // Them ky tu ket thuc xau va in ra man hinh
        buf[ret] = 0;
        printf("Du lieu tu server: %s\n", buf);
    }

    closesocket(client);
    WSACleanup();
}