#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#define MAX_CUSTOMER_NUM 1000
#define MAX_ADMIN_NUM 10
#define MAX_MOVIE_NUM 6
#define MAX_PLAY_TIME 10
using namespace std;

HANDLE cusHandle[MAX_CUSTOMER_NUM];
HANDLE admHandle[MAX_ADMIN_NUM];
int cusNum = 0;
int admNum = 0;

typedef struct movieinfo {
	SYSTEMTIME startTime[MAX_PLAY_TIME];
	SYSTEMTIME endTime[MAX_PLAY_TIME];
	int duration; // minutes
	int tickets;  //remaining tickets 
	string name;
}movieInfo[10];

void createCustomer();
void createAdmin();
void admin();
void customer();
void orderTickets(string );
void initMovie(movieinfo *mi, SYSTEMTIME startTime, int duration, int tickets, string name);
void initMovieList(movieInfo*);


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
	cout << "Dear Admin, Welcome!" << endl;
	cout << "What do you wanna do?" << endl;
	cout << "1: check out remaining tickets" << endl;
	cout << "2: check out customers' advice" << endl;


}

void customer(int ID) {
	int choice;
	cout << "1: order tickets" << endl;
	cout << "2: check out available movies" << endl;
	cout << "Please input your choice:";
	cin >> choice;
	if (choice == 1) {
		orderTickets("Roman Holiday");
		getchar();
	}
}

void orderTickets(string name){
	HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");
	HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, name.c_str());

	cout << "Waiting in line .." << endl;
	Sleep(1000);
	WaitForSingleObject(mutex, INFINITE);
	cout << "Buying ticket(s).." << endl;
	Sleep(1000);
	DWORD returnStatus = WaitForSingleObject(movie, 1);
	if (returnStatus == WAIT_TIMEOUT) {
		cout << "Sorry, tickets are all sold out!" << endl;
	}
	else if (returnStatus == WAIT_OBJECT_0) {
		cout << "Congrats! You bought one ticket successfully" << endl;
	}

	ReleaseSemaphore(mutex, 1, NULL);

	system("pause");
}

void initMovieList(movieInfo* mi) {
	SYSTEMTIME st;
	GetSystemTime(&st);
	initMovie(mi[0], st, 90, 3, "Roman Holiday");
	initMovie(mi[1], st, 80, 100, "Gone with wind");
	//init more movies
}

void initMovie(movieinfo *mi, SYSTEMTIME startTime, int duration,int tickets, string name) {
	mi->duration = duration;
	mi->name = name;
	mi->tickets = tickets;
	CreateSemaphore(NULL, tickets, tickets, name.c_str());
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

		initMovieList(mi);
		
		UnmapViewOfFile(pData);

		//create mutex name "mutex"
		CreateSemaphore(NULL, 1, 1, "mutex");
		
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