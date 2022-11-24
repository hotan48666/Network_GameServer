#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <process.h>
#include <Windows.h>
#include <vector>
#include <map>
#include "TestGameServer.h"
#include "MessagePacket.h"
#include "PlayerData.h"

#pragma comment(lib, "WS2_32.lib")

#define ERRPR_MESSAGE_MAX 100

using namespace std;

struct Vector3
{
	float X;
	float Y;
	float Z;
};

struct Stat
{
	int HP;
	int Attack;
};

struct PlayInfo
{
	int UserID;
	Vector3 position;
	Stat PlayerStat;
};

struct Packet
{

	PlayInfo info;
};

vector<SOCKET> UserList;

map<SOCKET, PlayerData*> PlayerList;

CRITICAL_SECTION ServerCS;

char Data[18] = { 0,};

unsigned WINAPI WorkThread(void* Arg)
{
	SOCKET ClientSocket = *(SOCKET*)Arg;
	char Buffer[1024] = { 0, };

	while (true)
	{
		int RecvLength = recv(ClientSocket, Buffer, sizeof(Buffer), 0);
		if (RecvLength <= 0)
		{
			//disconnect
			closesocket(ClientSocket);
			EnterCriticalSection(&ServerCS);
			PlayerList.erase(find(PlayerList.begin(), PlayerList.end(), ClientSocket));
			LeaveCriticalSection(&ServerCS);
			break;
		}
		else
		{
			for (auto Player : PlayerList)
			//for (int i = 0; i < UserList.size(); ++i)
			{
				int SentLength = send(Player.first, Buffer, (int)strlen(Buffer) + 1, 0);
				if (SentLength <= 0)
				{
					closesocket(ClientSocket);
					EnterCriticalSection(&ServerCS);
					PlayerList.erase(find(PlayerList.begin(), PlayerList.end(), ClientSocket));
					LeaveCriticalSection(&ServerCS);
					break;
				}
			}
		}
	}

	return 0;
}

void TestGameServer2()
{
	map<string, int> Test;

	Test["Hello"] = 1;
	Test["World"] = 2;
	Test["World"] = 3;

	for (auto data : Test)
	{
		cout << data.first << endl;
		cout << data.second << endl;
	}

	cout << Test["Hello"] << endl;
	cout << Test["World"] << endl;

}

void TestGameServer()
{
	InitializeCriticalSection(&ServerCS);

	// 1. Server�� ���� 
	// 2. Player�� World�� ����
	// 3. ���忡 �ִ� Player�� ������ ��� Ŭ���̾�Ʈ(�ڱ� �ڽ��� ����)�� ���� 
	// 4. �̸� ������ Player�� ���� ������ Player�� ������ (���� ������ Ŭ���̾�Ʈ)

	/*
		1...

			1��° Ŭ���̾�Ʈ�� ������ ������ ����

			������ ������ �ް� �÷��̾ ���� 

			���� ������ �ش� Ŭ���̾�Ʈ���� �ѱ�

			�ش� 1��° Ŭ���̾�Ʈ�� �ް� �÷��̾ ����

		2...
 
			2��° Ŭ���̾�Ʈ�� ������ ������ ���� 

			������ ������ �ް� �÷��̾ ����
			���������� Ŭ���̾�Ʈ���� �ѱ� 

			�̶� 1��° Ŭ���̾�Ʈ���� ���� ������ ���������� �ѱ�
			2��° Ŭ���̾�Ʈ���� �̹� ������ ���������� �ѱ�

			1��° Ŭ���̾�Ʈ�� ���� ������ ���������� �ް� �÷��̾� ����
			2��° Ŭ���̾�Ʈ�� �̹� ������ ���������� �ް� �÷��̾� ����

		3...

			3���� Ŭ���̾�Ʈ�� ������ ������ ����

			������ ������ �ް� �÷��̾ ����
			���������� Ŭ���̾�Ʈ���� �ѱ� 

			�̶� 1��° Ŭ���̾�Ʈ, 2��° Ŭ���̾�Ʈ ���� ���� ������ ���������� �ѱ�
			3��° Ŭ���̾�Ʈ ���� �̹� ������ ���������� �ѱ�

			1��° Ŭ���̾�Ʈ, 2��° Ŭ���̾�Ʈ�� ���� ������ ���������� �ް� �÷��̾� ����
			3��° Ŭ���̾�Ʈ�� �̹� ������ ���������� �ް� �÷��̾� ����
	
	
	*/



	InitializeCriticalSection(&ServerCS);

	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ListenSocketAddr;
	memset(&ListenSocketAddr, 0, sizeof(ListenSocketAddr));
	ListenSocketAddr.sin_family = PF_INET;
	ListenSocketAddr.sin_port = htons(4949);
	ListenSocketAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(ListenSocket, (SOCKADDR*)&ListenSocketAddr, sizeof(ListenSocketAddr));

	listen(ListenSocket, 0);

	while (true)
	{
		//���� ó��
		SOCKADDR_IN ClientSockAddr;
		memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
		int ClientSockAddrLength = sizeof(ClientSockAddr);
		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

		cout << "connect : " << ClientSocket << endl;
		EnterCriticalSection(&ServerCS);

		PlayerData* NewPlayer = new PlayerData();
		NewPlayer->MySocket = ClientSocket;
		PlayerList[ClientSocket] = NewPlayer;

		//PlayerList.push_back(ClientSocket);
		LeaveCriticalSection(&ServerCS);

		//S2C_RegisterID
		unsigned short Code = htons((unsigned short)MessagePacket::S2C_RegisterID);
		memcpy(&Data[0], &Code, sizeof(Code));

		SOCKET SendID = htonll(ClientSocket);
		memcpy(&Data[2], &SendID, sizeof(SendID));

		//unsigned short Code = htons((unsigned short)MessagePacket::S2C_RegisterID);
		//memcpy(&Data[10], &Code, 4);
		//unsigned short Code = htons((unsigned short)MessagePacket::S2C_RegisterID);
		//memcpy(&Data[14], &Code, 4);

		int SendBytes = send(ClientSocket, Data, sizeof(Data), 0);




		HANDLE ThreadHandle = (HANDLE)_beginthreadex(nullptr,
			0,
			WorkThread,
			(void*)&ClientSocket,
			0,
			nullptr
		);
	}


	closesocket(ListenSocket);

	DeleteCriticalSection(&ServerCS);


	WSACleanup();


}