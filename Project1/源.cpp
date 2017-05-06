#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#define MAX_CUSTOMER_NUM 1000
#define MAX_ADMIN_NUM 10
#define MAX_MOVIE_NUM 6
using namespace std;

HANDLE cusHandle[MAX_CUSTOMER_NUM];
HANDLE admHandle[MAX_ADMIN_NUM];
int cusNum = 0;
int admNum = 0;

typedef struct movieinfo {
	SYSTEMTIME startTime;
	SYSTEMTIME endTime;
	int duration; // minutes
	string name;
}movieInfo[10];

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
	cusHandle[cusNum++] = pi.hProcess;
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
	admHandle[admNum++] = pi.hProcess;
}

void admin(int ID) {
	cout << "Please enter the password:" << endl;
	//TODO check password

}

void customer(int ID) {
	cout << "1: order tickets" << endl;
	cout << "2: check out available movies" << endl;
}

int main(int argc, char** argv) {

	if (argc > 1 && strcmp(argv[1], "customer") == 0) {
		customer(atoi(argv[2]));
	}
	else if (argc > 1 && strcmp(argv[1], "admin") == 0) {
		admin(atoi(argv[2]));
	}
	else {
		
		HANDLE hMap = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(sizeof(movieInfo)),
			"movie");

		LPVOID pData = MapViewOfFile(
			hMap,
			FILE_MAP_WRITE,
			0,
			0,
			sizeof(movieInfo));

		if (pData != NULL) {
			ZeroMemory(pData, sizeof(movieInfo));
		}

		//initialize movie list
		movieInfo *mi = reinterpret_cast<movieInfo*>(pData);
		for (int i = 0; i < MAX_MOVIE_NUM; i++) {
			mi[i]->duration = 0;
		}

		
		int id;
		cout << "*********************************" << endl;
		cout << "******Welcome to XX Cinema!******" << endl;
		cout << "*********************************" << endl;
		cout << "1: customer" << endl;
		cout << "2: administrator" << endl;
		cout << "Please choose your identity:";
		while ((cin >> id)) {
			if (id == 1) {
				createCustomer();
				cout << "Creating customer.." << endl;
			}
			else if (id == 2) {
				createAdmin();
				cout << "Creating administrator.." << endl;
			}
			else {
				cout << "input error, try again" << endl;
			}
			cout << "Please choose your identity:";
		}
	}
}