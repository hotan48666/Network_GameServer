#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <iostream>
#include <WinSock2.h>
#include <process.h>
#include <Windows.h>
#include <map>
#include "TestGameClient.h"
#include "MessagePacket.h"
#include "PlayerData.h"

using namespace std;

#pragma comment(lib, "WS2_32.lib")

char Data[18] = { 0, };

map<SOCKET, PlayerData*> PlayerList;

void ProcessPacket(char* Packet)
{

	//S2C_RegisterID
	unsigned short Code = 0;
	memcpy(&Code, &Data[0], sizeof(Code));

	SOCKET SendID = 0;
	memcpy(&SendID, &Data[2], sizeof(SendID));

	Code = ntohs(Code);
	SendID = ntohll(SendID);

	switch ((MessagePacket)Code)
	{

		case MessagePacket::S2C_RegisterID:
			PlayerData* NewPlayer = new PlayerData();
			NewPlayer->MySocket = SendID;
			PlayerList[SendID] = NewPlayer;
			break;

		case MessagePacket::S2C_Spawn:
		
			break;
		case MessagePacket::S2C_Destroy:
			
			break;


		default:
			cout << "Not found code. " << Code << endl;
			break;
	}


	cout << "Code : " << Code << endl;
	cout << "SendID : " << SendID << endl;

}

unsigned WINAPI TESTWorkThread(void* Arg)
{
	SOCKET ServerSocket;
	char Buffer[1024] = { 0, };

	ServerSocket = *(SOCKET*)Arg;

	while (true)
	{
		int RecvBytes = recv(ServerSocket, Data, sizeof(Data), 0);
		//받는거 확인처리 로직 있어야 함

		if (RecvBytes <= 0)
		{
			break;
		}
		else
		{
			//패킷 처리(정확히 받았다.) 
			ProcessPacket(Data);
		}
	}

	return 0;
}

void TestGameClient()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_port = htons(4949);
	ServerSockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	HANDLE WorkHandle = (HANDLE)_beginthreadex(nullptr, 0, TESTWorkThread, (void*)&ServerSocket, 0, nullptr);

	while (true)
	{
		char Message[1024] = { 0, };
		cin >> Message;
		int SentBytes = send(ServerSocket, Message, (int)strlen(Message) + 1, 0);
	}

	WSACleanup();
};