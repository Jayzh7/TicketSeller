#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<iostream>
#include<shlwapi.h>
#include<iomanip>
#include<Psapi.h>
#include<WinBase.h>
#include<tlhelp32.h>
#include<stdlib.h>
#include "head.h"

#define SHOWMASK(dwTarget,type)\
if (TestSet(dwTarget, PAGE_##type))\
{cout << "," << #type; }

//wrapper function
#pragma comment(lib,"shlwapi.lib")
//
#pragma comment(lib,"kernel32.lib")
/*    process status api
a helper library that makes it easier for you to obtain
information about processes and device drivers
*/
#pragma comment(lib,"psapi.lib")

using namespace std;

//ȫ�ֱ��� �洢������Ϣ �����ڸ��������е���
processInfo* PI;

void createTester();

//��¼�����Ĺ���Ա�����͹˿�����
int admNum, cusNum;
//��¼�����Ĺ���Ա�͹˿͵Ľ��̾��
HANDLE admHandle[10], cusHandle[10];


inline bool TestSet(DWORD dwTarget, DWORD dwMask)
{
	return ((dwTarget &dwMask) == dwMask);
}




//�����ҳ�汣����ʽ
void ShowProtection(DWORD dwTarget)
{
	SHOWMASK(dwTarget, READONLY);
	SHOWMASK(dwTarget, GUARD);
	SHOWMASK(dwTarget, NOCACHE);
	SHOWMASK(dwTarget, READWRITE);
	SHOWMASK(dwTarget, WRITECOPY);
	SHOWMASK(dwTarget, EXECUTE);
	SHOWMASK(dwTarget, EXECUTE_READ);
	SHOWMASK(dwTarget, EXECUTE_READWRITE);
	SHOWMASK(dwTarget, EXECUTE_WRITECOPY);
	SHOWMASK(dwTarget, NOACCESS);
}

void createCustomer() {
	TCHAR szFilename[MAX_PATH];
	GetModuleFileName(NULL, szFilename, MAX_PATH);

	TCHAR szCmdLine[MAX_PATH];

	//create customer
	sprintf(szCmdLine, "\"%s\" customer %d", szFilename, cusNum);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(reinterpret_cast<void*>(&si), sizeof(si));

	si.cb = sizeof(si);
	si.lpTitle = "customer";

	BOOL bCreateOK = CreateProcess(
		szFilename,
		szCmdLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);

	if (!bCreateOK) {
		cout << " Create customer error!\n" << endl;
	}

	PI->cstmHandle[PI->num] = pi.hProcess;
	PI->cstmID[PI->num] = pi.dwProcessId;
	PI->num++;

}


void createAdmin() {
	TCHAR szFilename[MAX_PATH];
	GetModuleFileName(NULL, szFilename, MAX_PATH);

	TCHAR szCmdLine[MAX_PATH];

	//create customer
	sprintf(szCmdLine, "\"%s\" admin %d", szFilename, admNum);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(reinterpret_cast<void*>(&si), sizeof(si));
	si.cb = sizeof(si);

	BOOL bCreateOK = CreateProcess(
		szFilename,
		szCmdLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);

	if (!bCreateOK) {
		cout << " Create admin error!\n" << endl;
	}

}

/*��ʼ����Ӱ�б�
param
*/
void initMovieList(movieinfo* mi) {

	mi[0].name = "�鷳����";
	mi[0].grade = 8.1;
	mi[0].duration = 103;
	mi[0].type = "����";
	mi[0].starttime[0] = 1715;
	mi[0].starttime[0] = 2025;
	mi[0].time = 2;
	mi[0].place[0] = 1;
	mi[0].place[1] = 3;
	mi[0].price = 35;
	mi[0].ttlTickets[0] = 150;
	mi[0].rmnTickets[0] = 41;
	mi[0].ttlTickets[1] = 120;
	mi[0].rmnTickets[1] = 31;
	CreateSemaphore(NULL, 41, 150, "�鷳����1");
	CreateSemaphore(NULL, 31, 120, "�鷳����2");


	mi[1].name = "ϲ����";
	mi[1].grade = 8.9;
	mi[1].duration = 107;
	mi[1].type = "ϲ��";
	mi[1].starttime[0] = 1215;
	mi[1].starttime[1] = 1630;
	mi[1].starttime[2] = 2115;
	mi[1].time = 3;
	mi[1].place[0] = 2;
	mi[1].place[1] = 5;
	mi[1].place[2] = 1;
	mi[1].price = 44;
	mi[1].ttlTickets[0] = 120;
	mi[1].rmnTickets[0] = 25;
	mi[1].ttlTickets[1] = 150;
	mi[1].rmnTickets[1] = 31;
	mi[1].ttlTickets[2] = 180;
	mi[1].rmnTickets[2] = 45;
	CreateSemaphore(NULL, 25, 120, "ϲ����1");
	CreateSemaphore(NULL, 31, 150, "ϲ����2");
	CreateSemaphore(NULL, 45, 180, "ϲ����3");

	mi[2].name = "��ר��";
	mi[2].grade = 9.1;
	mi[2].duration = 120;
	mi[2].type = "����";
	mi[2].starttime[0] = 1905;
	mi[2].starttime[1] = 2155;
	mi[2].time = 2;
	mi[2].place[0] = 3;
	mi[2].place[0] = 2;
	mi[2].price = 54;
	mi[2].ttlTickets[0] = 180;
	mi[2].rmnTickets[0] = 39;
	mi[2].ttlTickets[1] = 150;
	mi[2].rmnTickets[1] = 29;
	CreateSemaphore(NULL, 39, 180, "��ר��1");
	CreateSemaphore(NULL, 29, 150, "��ר��2");

	mi[3].name = "�����ʦ";
	mi[3].grade = 8.6;
	mi[3].duration = 119;
	mi[3].type = "����";
	mi[3].starttime[0] = 1425;
	mi[3].starttime[1] = 1915;
	mi[3].starttime[2] = 2135;
	mi[3].time = 3;
	mi[3].place[0] = 4;
	mi[3].place[1] = 5;
	mi[3].place[2] = 2;
	mi[3].price = 54;
	mi[3].ttlTickets[0] = 150;
	mi[3].rmnTickets[0] = 25;
	mi[3].ttlTickets[1] = 120;
	mi[3].rmnTickets[1] = 20;
	mi[3].ttlTickets[2] = 180;
	mi[3].rmnTickets[2] = 45;
	CreateSemaphore(NULL, 25, 150, "�����ʦ1");
	CreateSemaphore(NULL, 20, 100, "�����ʦ2");
	CreateSemaphore(NULL, 45, 180, "�����ʦ3");

	for (int i = 4; i < 10; i++) {
		mi[i].duration = 0;
	}
	//init more movies
	//system("pause");
}

void initMovie(movieinfo mi, string name, double grade, int duration, string type, int startTime, int time, int place, int price, int ttl, int rmn) {
	mi.duration = duration;
	mi.name = name;
	//mi.ttlTickets = ttl;
	//mi.rmnTickets = rmn;
	mi.grade = grade;
	//CreateSemaphore(NULL, tickets, tickets, name.c_str());
}

void WalkVM(HANDLE hProcess)
{
	//���ϵͳ��Ϣ
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	MEMORY_BASIC_INFORMATION mbi;    //���������ڴ�ռ�Ļ�����Ϣ�ṹ
	ZeroMemory(&mbi, sizeof(mbi));   //���仺���������ڱ�����Ϣ


									 //ѭ������Ӧ�ó����ַ�ռ�
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
	while (pBlock < si.lpMaximumApplicationAddress)
	{
		//�����һ�������ڴ�����Ϣ
		if (VirtualQueryEx(
			hProcess, //A handle to the process whose memory information is queried
			pBlock,   //A pointer to the base address of the region of pages to be queried
			&mbi,     //structure used to return information about the specified page range
			sizeof(mbi)) == sizeof(mbi))
		{
			//�����Ľ�β���䳤��
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];
			StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);

			cout.fill('0');
			cout << hex << setw(8) << (DWORD)pBlock << "-" << hex << setw(8) << (DWORD)pEnd
				<< (strlen(szSize) == 7 ? "(" : "(") << szSize << ")";

			switch (mbi.State)
			{
			case MEM_COMMIT:
				printf("Committed");
				break;
			case MEM_FREE:
				printf("Free");
				break;
			case MEM_RESERVE:
				printf("Reserved");
			default:
				break;
			}

			//��ʾ����
			if (mbi.Protect == 0 && mbi.State != MEM_FREE)
			{
				mbi.Protect = PAGE_READONLY;
			}
			ShowProtection(mbi.Protect);

			switch (mbi.Type)
			{
			case MEM_IMAGE:
				printf(",Image");
				break;
			case MEM_MAPPED:
				printf(",Mapped");
				break;
			case MEM_PRIVATE:
				printf(",Private");
				break;
			default:
				break;
			}

			//�����ִ��ӳ��
			TCHAR szFilename[MAX_PATH];
			if (GetModuleFileName(
				(HMODULE)pBlock,  //ʵ�������ڴ��ģ����
				szFilename,       //��ȫָ�����ļ�ϵͳ
				MAX_PATH) > 0)    //ʵ��ʹ�õĻ���������
			{
				PathStripPath(szFilename);
				printf(",Module:%s", szFilename);
			}

			printf("\n");

			//�ƶ���ָ���Ի����һ����
			pBlock = pEnd;
		}
	}
}


int main(int argc, char** argv) {

	if (argc > 1 && strcmp(argv[1], "customer") == 0) {
		customer(atoi(argv[2]));
	}
	else if (argc > 1 && strcmp(argv[1], "admin") == 0) {
		admin(atoi(argv[2]));
	}
	else if (argc > 1 && strcmp(argv[1], "processinfo") == 0) {
		//��ʾ��ѡ���̵������ڴ�ʹ�������ˢ������7��
		HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, atoi(argv[2]));
		while (1) {
			cout << "-------------------------process ID : " << argv[2] << "---------------------" << endl;
			WalkVM(hp);
			Sleep(7000);
			system("cls");
		}
	}
	else {

		//++++++++������ӳ��(��Ӱ��Ϣ)++++++++
		HANDLE hMap = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(movieInfo),
			"movie");

		//++++++++������ӳ��(������Ϣ)++++++++
		HANDLE cstmInfoMap = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(processInfo),
			"processInfo");


		//++++++++������ӳ��(�˿ͽ���)++++++++
		HANDLE MapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(msg_queue),
			"advice");

		LPVOID pfile = MapViewOfFile(
			hMap,
			FILE_MAP_WRITE,
			0,
			0,
			sizeof(msg_queue));

		if (pfile != NULL) {
			ZeroMemory(pfile, sizeof(msg_queue));
		}

		LPVOID pData = MapViewOfFile(
			hMap,
			FILE_MAP_WRITE,
			0,
			0,
			sizeof(movieInfo));

		if (pData != NULL) {
			ZeroMemory(pData, sizeof(movieInfo));
		}

		LPVOID processInfoData = MapViewOfFile(
			cstmInfoMap,
			FILE_MAP_WRITE,
			0,
			0,
			sizeof(processInfo));

		//initialize process information struct
		PI = reinterpret_cast<processInfo*>(processInfoData);
		PI->num = 0;

		//initialize movie list
		movieinfo *mi = reinterpret_cast<movieinfo*>(pData);

		initMovieList(mi);

		UnmapViewOfFile(pData);

		//create mutex name "mutex"
		CreateSemaphore(NULL, 1, 1, "mutex");

		int id;
		cout << "*********************************" << endl;
		cout << "******Welcome to XX Cinema!******" << endl;
		cout << "*********************************" << endl;
		cout << "1: �˿�" << endl;
		cout << "2: ����Ա" << endl;
		cout << "3: ������" << endl;
		cout << "��ѡ��������:";
		while ((cin >> id)) {
			if (id == 1) {
				createCustomer();
				cout << "�����˿���.." << endl;
			}
			else if (id == 2) {
				createAdmin();
				cout << "��������Ա��.." << endl;
			}
			else if (id == 3) {
				//createTester();
				cout << "������������.." << endl;
			}
			else {
				cout << "input error, try again" << endl;
			}
			cout << "��ѡ��������";
		}
	}
}