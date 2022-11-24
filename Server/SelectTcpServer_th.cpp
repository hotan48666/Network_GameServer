#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include "SelectTcpServer_th.h"

#pragma comment(lib, "WS2_32.lib")

using namespace std;

//스레드 

void SelectTcpServer_th()
{
	WSAData  WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = PF_INET;
	ListenSockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //INADDR_ANY;
	ListenSockAddr.sin_port = htons(4949);

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	fd_set RecvList;
	fd_set BeckUpRecvList;
	FD_ZERO(&RecvList);

	FD_SET(ListenSocket, &RecvList);

	TIMEVAL TimeOut;
	TimeOut.tv_sec = 1;
	TimeOut.tv_usec = 10;

	while (true)
	{
		BeckUpRecvList = RecvList;
		int ChangeCount = select(0, &BeckUpRecvList, 0, 0, &TimeOut);
		if (ChangeCount == SOCKET_ERROR)
		{
			cout << "error" << endl;
			exit(-1); 
		}
		else if (ChangeCount == 0)
		{
			continue;
		}
		else
		{
			for (int i = 0; i < (int)RecvList.fd_count; ++i)
			{
				if (FD_ISSET(RecvList.fd_array[i], &BeckUpRecvList)) //변화 감지
				{
					if (RecvList.fd_array[i] == ListenSocket)
					{
						//연결처리
						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientSockAddrLength = sizeof(ClientSockAddr);
						SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
						 

						FD_SET(ClientSocket, &RecvList);
						cout << "Connect : " << ClientSocket << endl;
					}
					else
					{
						//메세지 처리
						char Buffer[1024] = { 0 , };
						int RecvLength = recv(RecvList.fd_array[i], Buffer, sizeof(Buffer) - 1, 0);

						if (RecvLength <= 0)
						{
							cout << "Disconnect : " << RecvList.fd_array[i] << endl;
							SOCKET DisconnectSocket = RecvList.fd_array[i];
							FD_CLR(DisconnectSocket, &RecvList);
							closesocket(DisconnectSocket);
						}
						else
						{
							cout << "Client Send : " << Buffer << endl;
							for (int j = 0; j < (int)RecvList.fd_count; ++j)
							{
								if (RecvList.fd_array[j] != ListenSocket)
								{
									int SendLength = send(RecvList.fd_array[j], Buffer, RecvLength, 0);
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
	
}
