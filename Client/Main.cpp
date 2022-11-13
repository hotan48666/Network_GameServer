#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include<WinSock2.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

// Client 

int main()
{
	//초기화
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	//에러처리
	if (Result > 0)
	{
		cout << "Socket init error" << GetLastError() << endl;
		exit(-1);
	}

	//생성
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	//에러처리
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "ServerSocket make error" << GetLastError() << endl;
		exit(-1);
	}

	//주소 구조체 정보 생성
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN));//초기화

	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(12345);

	int Status = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	if (Status == SOCKET_ERROR)
	{
		cout << "ServerSocket connect error" << GetLastError() << endl;
		exit(-1);
	}

	char Buffer[1024] = {0,};

	int RecvBytes = recv(ServerSocket, Buffer, sizeof(Buffer), 0);

	if (RecvBytes <= 0)
	{
		cout << "RecvBytes error" << GetLastError() << endl;
		exit(-1);
	}

	cout << "서버로 받은 메세지 : " << Buffer << endl;
	cout << "서버로 받은 바이트 : " << RecvBytes << endl;

	cout << "서버로 받은 sizeof(Buffer) : " << sizeof(Buffer) << endl;
	cout << "서버로 받은 strlen(Buffer) : " << strlen(Buffer) << endl;

	int SendBytes = send(ServerSocket, Buffer, strlen(Buffer) + 1, 0);

	if (SendBytes <= 0)
	{
		cout << "SendBytes error" << GetLastError() << endl;
		exit(-1);
	}



	closesocket(ServerSocket);
	WSACleanup();

	cout << "Client" << endl;
	return 0;
}