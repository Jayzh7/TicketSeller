#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<string>
#include<time.h>
#include<map>
#include "head.h"

using namespace std;

void cusMenu();
void showTicketInfo();
void refund();

map<string, int> ticketInfo;
movieInfo *MI;

void customer(int ID) {

	HANDLE hMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		"movie");

	if (hMap == INVALID_HANDLE_VALUE) {
		cout << "OPEN FILE MAPPING ERROR!" << endl;
	}

	LPVOID pData = MapViewOfFile(
		hMap,
		FILE_MAP_WRITE,
		0,
		0,
		sizeof(movieInfo));

	MI = reinterpret_cast<movieInfo*>(pData);



	int choice = 0;
	stime();

	while (choice != 6) {
		cusMenu();
		cin >> choice;
		if (choice == 1) {
			string mov = "";
			cout << "请输入你要观看的电影:" << endl;
			cin >> mov;
			HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, mov.c_str());
			if (movie == NULL) {
				cout << "INPUT ERROR!" << endl;
				cout << "Exiting .." << endl;
				continue;
			}
			cout << "请输入购买数量:";
			int num;
			cin >> num;
			orderTickets(mov, num, movie);
		}
		else if (choice == 2) {
			showMovies();
		}
		else if (choice == 3) {

		}
		else if (choice == 4) {
			showTicketInfo();
		}
		else if (choice == 5) {
			refund();
		}
	}

	UnmapViewOfFile(pData);
}

void orderTickets(string name, int num, HANDLE movie) {
	HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");

	
	cout << "排队等待 .." << endl;
	Sleep(1000);
	WaitForSingleObject(mutex, INFINITE);
	cout << "购票中 .." << endl;
	Sleep(1000);
	DWORD returnStatus;
	map<string, int>::iterator it;
	for (int i = 0; i < num; i++) {
		returnStatus = WaitForSingleObject(movie, 3000);
		if (returnStatus == WAIT_TIMEOUT) {
			cout << "抱歉，票已售罄！" << endl;
			break;
		}
		else if (returnStatus == WAIT_OBJECT_0) {
			if (i == 0) {
				ticketInfo.insert(pair<string, int>(name, 1));
				it = ticketInfo.find(name);
			}
			else {
				it->second++;
			}
			cout << "恭喜成功买到第" << i+1 << "张票" << endl;
		}
	}
	
	

	ReleaseSemaphore(mutex, 1, NULL);
}

void showMovies() {
	
}

void stime()
{
	struct tm *ptr;
	time_t  it;
	it = time(NULL);
	ptr = localtime(&it);
	printf("\n现在的时间是%4d年%02d月%02d日%d:%d:%d\n", ptr->tm_year + 1900, ptr->tm_mon + 1, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
}

void cusMenu() {
	cout << "1: goupiao" << endl;
	cout << "2:movie info" << endl;
	cout << "3: advice" << endl;
	cout << "4: check out ticket info" << endl;
	cout << "5: exit" << endl;
}

void showTicketInfo() {
	map<string, int>::iterator it;
	cout << "影片名                      已购数量" << endl;
	for (it = ticketInfo.begin(); it != ticketInfo.end(); it++) {
		cout << it->first << "                      " << it->second << endl;
	}
}

void refund() {
	string name = "";
	cout << "请输入您想退票的电影名：";
	cin >> name;
	map<string, int>::iterator it;
	it = ticketInfo.find(name);
	if (it == ticketInfo.end()) {
		cout << "您未购买此电影的电影票，请重试" << endl;
	}
	else {
		cout << "请输入您想退票的数量：";
		int num;
		cin >> num;
		
		int refundNum = num > it->second ? it->second : num;

		it->second -= refundNum;
		HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");
		WaitForSingleObject(mutex, INFINITE);
		HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, name.c_str());
		for (int i = 0; i < num; i ++)
			ReleaseSemaphore(movie, 1, NULL);
		if (it->second == 0) {
			ticketInfo.erase(it);
		}
		cout << "您已成功退票" << refundNum << "张" << endl;
	}
}