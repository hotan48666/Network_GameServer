#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include "SelectTcpClient_th.h"
#include <process.h>
#include <Windows.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

unsigned WINAPI WorkThread(void* Arg)
{
	SOCKET ServerSocket;
	char Buffer[1024] = { 0, };

	ServerSocket = *(SOCKET*)Arg;

	while (true)
	{
		int RecvBytes = recv(ServerSocket, Buffer, sizeof(Buffer), 0);
		if (RecvBytes <= 0)
		{
			//¼­¹ö¶û ²÷±è
			break;
		}
		else
		{
			cout << Buffer << endl;
		}
	}

	return 0;
}


void SelectTcpClient_th()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_port = htons(4949);
	ServerSockAddr.sin_addr.S_un.S_addr = inet_addr("218.145.204.176");

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	HANDLE WorkHandle = (HANDLE)_beginthreadex(nullptr, 0, WorkThread, (void*)&ServerSocket, 0, nullptr);

	while (true)
	{
		char Message[1024] = { 0, };
		cin >> Message;
		int SentBytes = send(ServerSocket, Message, (int)strlen(Message) + 1, 0);
	}

	WSACleanup();
}


