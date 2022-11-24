#include<iostream>
#include<process.h>
#include<Windows.h>

using namespace std;

int Gold = 0;

CRITICAL_SECTION GoldCS;



unsigned WINAPI Increase(void* Arg)
{
	for (int i = 0; i < 10000; i++)
	{
		EnterCriticalSection(&GoldCS);
		Gold++;
		LeaveCriticalSection(&GoldCS);
	}

	return 0;
}

unsigned WINAPI Decrease(void* Arg)
{
	for (int i = 0; i < 10000; i++)
	{
		EnterCriticalSection(&GoldCS);
		Gold--;
		LeaveCriticalSection(&GoldCS);
	}

	return 0;
}

//typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
unsigned WINAPI Thread(void* Arg)
{
	for (int i = 0; i < 100; i++)
	{
		printf("%d \n", i);
	}

	return 0;
}

int main()
{
	InitializeCriticalSection(&GoldCS);

	unsigned int ThreadID = 0;

	char Message[] = "¾È³çÇÏ¼¼¿ä.";
	int number = 1234; 
	//HANDLE ThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, Thread, nullptr, CREATE_SUSPENDED, &ThreadID);
	HANDLE ThreadHandle1 = (HANDLE)_beginthreadex(0, 0, Increase, nullptr, 0, &ThreadID);
	HANDLE ThreadHandle2 = (HANDLE)_beginthreadex(0, 0, Decrease, nullptr, 0, &ThreadID);

	Sleep(3000);

	//ResumeThread(ThreadHandle);

	//Sleep(10);

	//SuspendThread(ThreadHandle);

	//cout << "zzzzzzz " << endl;

	//Sleep(300);

	//ResumeThread(ThreadHandle);

	//TerminateThread(ThreadHandle, -1);


	//WaitForSingleObject(ThreadHandle, 10);
	

	if (ThreadHandle1 == 0)
	{
		cout << "error " << endl;
	}
	else
	{
		cout << ThreadHandle1 << endl;

	}
	
	

	cout << Gold << endl;

	DeleteCriticalSection(&GoldCS);

	return 0;
}