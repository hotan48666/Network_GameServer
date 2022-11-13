#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include<WinSock2.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

// Server 

int main()
{
	//초기화
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	
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
	ServerSockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ServerSockAddr.sin_port = htons(12345);
	
	//바인딩
	int Status = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	//에러처리
	if (Status == SOCKET_ERROR)
	{
		cout << "Socket bind error" << GetLastError() << endl;
		exit(-1);
	}

	//대기
	Status = listen(ServerSocket, 0);

	//에러처리
	if (Status == SOCKET_ERROR)
	{
		cout << "Socket listen error" << GetLastError() << endl;
		exit(-1);
	}

	cout << "Socket 대기중" << endl;

	//주소 구조체 정보 생성
	SOCKADDR_IN ClientAddrIn;
	memset(&ClientAddrIn, 0, sizeof(SOCKADDR_IN));
	int ClientAddrLength = sizeof(ClientAddrIn);

	//소켓 생성
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddrIn, &ClientAddrLength);

	//에러처리
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "ClientSocket make error" << GetLastError() << endl;
		exit(-1);
	}

	const char Message[] = "HelloWorld";
	
	//보내기
	int SendBytes = send(ClientSocket, Message, strlen(Message) + 1, 0);

	if (SendBytes <= 0)
	{
		cout << "SendBytes error" << GetLastError() << endl;
		exit(-1);
	}

	char Buffer[1024] = {0,};

	//받기
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


	closesocket(ClientSocket);
	closesocket(ServerSocket);
	WSACleanup();

	cout << "Server" << endl;
	return 0;
}