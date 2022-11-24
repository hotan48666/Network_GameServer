#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include <winsock2.h>

#pragma comment(lib, "WS2_32.lib")

#define ERRPR_MESSAGE_MAX 100

using namespace std;

void ErrorHandling(const char message[ERRPR_MESSAGE_MAX])
{
	cout << message << ", " << GetLastError() << endl;
	exit(1);
}



int SelectTcpServer()
{

	WSAData  WsaData;
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
	{
		ErrorHandling("초기화 에러");
	}

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		ErrorHandling("소켓 생성 에러");
	}

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = PF_INET;
	ListenSockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	ListenSockAddr.sin_port = htons(12000);

	if (bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("소켓 바인딩 에러");
	}

	if (listen(ListenSocket, 5) == SOCKET_ERROR)
	{
		ErrorHandling("소켓 바인딩 에러");
	}


	fd_set Reads; // 파일 디스크럽터 
	fd_set Copys;
	FD_ZERO(&Reads); // 초기화 
	FD_SET(ListenSocket, &Reads); // Reads에 ListenSocket 추가

	TIMEVAL TimeOut;
	TimeOut.tv_sec = 1;    //1초
	TimeOut.tv_usec = 100000;  //1/1,000,000 * 10 //1.1초

	while (true)
	{
		Copys = Reads; // 카피 
		int fd_num = select(0, &Copys, 0, 0, &TimeOut); //
		if (fd_num == SOCKET_ERROR)
		{
			ErrorHandling("select 소켓 생성 에러");
		}
		else if (fd_num == 0)
		{
			ErrorHandling("TimeOut");
		}
		else
		{
			for (int i = 0; i < (int)Reads.fd_count; ++i) 
			{
				if (FD_ISSET(Reads.fd_array[i], &Copys)) //검사
				{
					if (Reads.fd_array[i] == ListenSocket)
					{
						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientSockAddrLength = sizeof(ClientSockAddr);
						SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
						FD_SET(ClientSocket, &Reads);
						cout << "Connect : " << ClientSocket << endl;
					}
					else
					{
						char Buffer[1024] = { 0 , };
						int RecvLength = recv(Reads.fd_array[i], Buffer, sizeof(Buffer) - 1, 0);

						if (RecvLength <= 0)
						{
							cout << "Disconnect : " << Reads.fd_array[i] << endl;
							SOCKET DisconnectSocket = Reads.fd_array[i];
							FD_CLR(DisconnectSocket, &Reads);
							closesocket(DisconnectSocket);
						}
						else
						{
							cout << "Client Send : " << Buffer << endl;
							for (int j = 0; j < (int)Reads.fd_count; ++j)
							{
								if (Reads.fd_array[j] != ListenSocket)
								{
									int SendLength = send(Reads.fd_array[j], Buffer, RecvLength, 0);
								}
							}
						}
					}
				}
			}
		}

	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}