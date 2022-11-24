#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include <winsock2.h>
#include "TcpServer.h"

#pragma comment(lib, "WS2_32.lib")

#define ERRPR_MESSAGE_MAX 100

using namespace std;

void ErrorHandling(const char message[ERRPR_MESSAGE_MAX])
{
	cout << message << endl;
	exit(1);
}

void HelloServer()
{
	// ���� �ʱ�ȭ 
	WSADATA	wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}
		
	// ���� ����
	SOCKET hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	// ���� ���� 
	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi("5001"));

	// ���� ���ε�
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	// ���� ���
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	// ���ŷ ����� ���� ��ٸ�
	cout << "���� �����" << endl;

	SOCKADDR_IN  clntAddr;
	memset(&clntAddr, 0, sizeof(clntAddr));
	int szClntAddr = sizeof(clntAddr);

	// ����
	SOCKET hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	char message[] = "Hello World!";
	send(hClntSock, message, sizeof(message), 0);

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
}
