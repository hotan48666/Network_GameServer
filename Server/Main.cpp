#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include<WinSock2.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

// Server 
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
		SockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		SockAddr.sin_port = htons(12345);
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

void SocketBind(SOCKET ServerSocket, SOCKADDR_IN ServerSockAddr)
{
	// 4. 바인딩
	int Status = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	//에러처리
	if (Status == SOCKET_ERROR)
	{
		cout << "Socket bind error" << GetLastError() << endl;
		exit(-1);
	}
}

void SocketListen(SOCKET ServerSocket)
{
	//대기
	int Status = listen(ServerSocket, 0);

	//에러처리
	if (Status == SOCKET_ERROR)
	{
		cout << "Socket listen error" << GetLastError() << endl;
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
	// 서버 주소 구조체 정보 생성
	SOCKADDR_IN ServerSockAddr = CreateSockAddr("Server");
	// 4. 바인딩
	SocketBind(ServerSocket, ServerSockAddr);
	// 5. 대기
	SocketListen(ServerSocket);
	// 클라 주소 구조체 정보 생성
	SOCKADDR_IN ClientAddrIn = CreateSockAddr("Client");
	int ClientAddrLength = sizeof(ClientAddrIn);
	// 6. 클라 소켓 생성
	SOCKET ClientSocket = ClientSocketAccept(ServerSocket, ClientAddrIn, ClientAddrLength);


	/////////////////
	

	const char Message[] = "HelloWorld";
	
	// 7. 보내기
	int SendBytes = send(ClientSocket, Message, strlen(Message) + 1, 0);

	if (SendBytes <= 0)
	{
		cout << "SendBytes error" << GetLastError() << endl;
		exit(-1);
	}

	char Buffer[1024] = {0,};

	// 8. 받기
	int RecvBytes = recv(ClientSocket, Buffer, sizeof(Buffer), 0);

	if (RecvBytes <= 0)
	{
		cout << "RecvBytes error" << GetLastError() << endl;
		exit(-1);
	}

	cout << "클라이언트로 받은 메세지 : " << Buffer << endl;
	cout << "클라이언트로 받은 바이트 : " << RecvBytes << endl;

	cout << "클라이언트로 받은 sizeof(Buffer) : " << sizeof(Buffer) << endl;
	cout << "클라이언트로 받은 strlen(Buffer) : " << strlen(Buffer) << endl;

	/////////////////

	closesocket(ClientSocket);
	closesocket(ServerSocket);
	WSACleanup();

	cout << "Server" << endl;
	return 0;
}