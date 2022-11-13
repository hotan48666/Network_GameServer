#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include<WinSock2.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

// Client 
void SocketInit()
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
}

SOCKET CreateServerSocket()
{
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	//에러처리
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "ServerSocket make error" << GetLastError() << endl;
		exit(-1);
	}

	return ServerSocket;
}

SOCKADDR_IN CreateSockAddr(string type)
{

	SOCKADDR_IN SockAddr;

	if (type == "Server")
	{
		memset(&SockAddr, 0, sizeof(SOCKADDR_IN));//초기화

		SockAddr.sin_family = PF_INET;
		SockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		SockAddr.sin_port = htons(5001);
	}
	else if (type == "Client")
	{
		memset(&SockAddr, 0, sizeof(SOCKADDR_IN));
	}
	else
	{
		cout << "Socket type error" << GetLastError() << endl;
		exit(-1);
	}

	return SockAddr;
}

void SocketConnect(SOCKET ServerSocket, SOCKADDR_IN ServerSockAddr)
{
	// 3. 서버 연결
	int Status = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	//에러처리
	if (Status == SOCKET_ERROR)
	{
		cout << "ServerSocket connect error" << GetLastError() << endl;
		exit(-1);
	}
}

SOCKET ClientSocketAccept(SOCKET ServerSocket, SOCKADDR_IN ClientAddrIn, int ClientAddrLength)
{
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddrIn, &ClientAddrLength);

	//에러처리
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "ClientSocket make error" << GetLastError() << endl;
		exit(-1);
	}

	return ClientSocket;
}

int main()
{
	// 1. 초기화
	SocketInit();
	// 2. 서버 소켓 생성
	SOCKET ServerSocket = CreateServerSocket();
	//주소 구조체 정보 생성
	SOCKADDR_IN ServerSockAddr = CreateSockAddr("Server");
	// 3. 서버 연결
	SocketConnect(ServerSocket, ServerSockAddr);


	/////////////////////////////

	const char Message[] = "give me message.";

	int SendBytes = send(ServerSocket, Message, strlen(Message) + 1, 0); //strlen(Message) + 1 널문자 하나 추가

	if (SendBytes <= 0)
	{
		cout << "SendBytes error" << GetLastError() << endl;
		exit(-1);
	}



	char Buffer[1024] = { 0, };

	int RecvBytes = recv(ServerSocket, Buffer, sizeof(Buffer), 0); //sizeof(Buffer) 원 사이즈로 받음 

	if (RecvBytes <= 0)
	{
		cout << "RecvBytes error" << GetLastError() << endl;
		exit(-1);
	}

	cout << "서버로 받은 메세지 : " << Buffer << endl;
	cout << "서버로 받은 바이트 : " << RecvBytes << endl;

	cout << "서버로 받은 sizeof(Buffer) : " << sizeof(Buffer) << endl;
	cout << "서버로 받은 strlen(Buffer) : " << strlen(Buffer) << endl;

	
	/////////////////////////////


	closesocket(ServerSocket);
	WSACleanup();

	cout << "Client" << endl;
	return 0;
}