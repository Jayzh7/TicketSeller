#include<iostream>
#include<Windows.h>
#include "head.h"

using namespace std;

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


	int choice;
	cout << "1: order tickets" << endl;
	cout << "2: check out available movies" << endl;
	cout << "Please input your choice:";


	cin >> choice;
	if (choice == 1) {
		orderTickets("Roman Holiday");
	}
	else if (choice == 2) {
		showMovies();
	}
}

void orderTickets(string name) {
	HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");
	HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, name.c_str());

	cout << "Waiting in line .." << endl;
	Sleep(1000);
	WaitForSingleObject(mutex, INFINITE);
	cout << "Buying ticket(s).." << endl;
	Sleep(1000);
	DWORD returnStatus = WaitForSingleObject(movie, 3000);
	if (returnStatus == WAIT_TIMEOUT) {
		cout << "Sorry, tickets are all sold out!" << endl;
	}
	else if (returnStatus == WAIT_OBJECT_0) {
		cout << "Congrats! You bought one ticket successfully" << endl;

	}

	ReleaseSemaphore(mutex, 1, NULL);

	system("pause");
}

void showMovies() {

}