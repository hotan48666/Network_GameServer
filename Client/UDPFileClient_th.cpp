#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include "SelectTcpClient_th.h"

#pragma comment(lib, "WS2_32.lib")


using namespace std;

void Flie()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN SendSockAddr;
	ZeroMemory(&SendSockAddr, sizeof(SendSockAddr));
	SendSockAddr.sin_family = PF_INET;
	SendSockAddr.sin_port = htons(4949);
	SendSockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	char Buffer[1024] = { 0, };

	strcpy(Buffer, "Get File"); // ������ �ʿ��� ������ ��û 

	int SentBytes = sendto(SendSocket, Buffer, (int)strlen(Buffer) + 1, 0, (SOCKADDR*)&SendSockAddr, sizeof(SendSockAddr));

	int RecvBytes = 0;
	int RecvBytesTotal = 0;

	FILE* OutputFile = fopen("���ɴ��2.jpg", "wb");

	do
	{
		int SendSockAddrLength = sizeof(SendSockAddr);
		RecvBytes = recvfrom(SendSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&SendSockAddr, &SendSockAddrLength); //���� ����

		size_t ReadSize = fwrite(Buffer, 1, RecvBytes, OutputFile);

		cout << "RecvBytes : " << ReadSize << endl;
		RecvBytesTotal += ReadSize;
		cout << "RecvBytesTotal : " << RecvBytesTotal << endl;

	} while ((strcmp(Buffer, "EndFile") != 0)); // ������ �����Ͱ� EndFile��� �� �������� ������ �ٺ��±��� �ν� 

	cout << "������ ������ : " << Buffer << endl;

	fclose(OutputFile);
	closesocket(SendSocket);
	WSACleanup();
}

//int main()
//{
//	SelectTcpClient_th();

//	return 0;
//}