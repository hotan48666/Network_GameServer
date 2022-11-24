/*

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS



#include<iostream>
#include<WinSock2.h>

using namespace std;

#pragma comment(lib, "WS2_32.lib")

typedef unsigned long long u64;
u64 GetMicroCounter();

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

SOCKET CreateServerSocket(int SOCK_Type)
{
	SOCKET ServerSocket = socket(AF_INET, SOCK_Type, 0);

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

void UDPFileClient_Main()
{
	u64 start, end;

	SocketInit();
	SOCKET Socket = CreateServerSocket(2);
	SOCKADDR_IN ServerSockAddr = CreateSockAddr("Server");
	SocketBind(Socket, ServerSockAddr);


	int FileSize = 0;
	int TotalBufferNum = 0;

	char Buffer[1024] = { 0, };

	SOCKADDR_IN ClientSockAddr = CreateSockAddr("Client");
	int ClientSockLength = sizeof(ClientSockAddr);


	start = GetMicroCounter();

	FILE* ImageFile = fopen("Test.jpg", "rb");

	fseek(ImageFile, 0, SEEK_END);
	FileSize = ftell(ImageFile);
	TotalBufferNum = FileSize / sizeof(Buffer) + 1;



	cout << "FileSize : " << FileSize << endl;
	cout << "TotalBufferNum : " << TotalBufferNum << endl;

	_snprintf(Buffer, sizeof(Buffer), "%d", FileSize);

	cout << "Buffer : " << Buffer << endl;

	//총 사이즈 넘김 
	int SendBytes = sendto(Socket, Buffer, strlen(Buffer), 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));

	while (!feof(ImageFile))
	{
		char Buffer[1024] = { 0, }; //2^10
		//Server
		size_t ReadSize = fread(Buffer, sizeof(char), sizeof(Buffer), ImageFile);

		//Client에 전송
		sendto(Socket, Buffer, (int)ReadSize, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));
	}

	end = GetMicroCounter();

	cout << "소요 시간 : " << end - start << endl;


	closesocket(Socket);
	WSACleanup();
}


int main()
{
	UDPFileClient_Main();
	return 0;
}


*/