#include<iostream>
#include<Windows.h>

using namespace std;

void customer(int);
void orderTickets(string);
void showMovies();

void customer(int ID) {
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

void showMovies() {

}