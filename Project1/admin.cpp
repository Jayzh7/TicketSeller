#include<iostream>
#include<string>
#include<Windows.h>
#include<conio.h>
#include "head.h"

using namespace std;

movieInfo* mi;

int seats[] = { 120, 120, 150, 200 ,100, 30};

void showTicketsLeft() {
	for (int i = 0; i < 6; i++) {
		if (mi[i]->duration == 0)
			break;
		cout << mi[i]->name << "  " << mi[i]->rmnTickets << "/" <<  mi[i]->ttlTickets << endl;
	}
}

void showCustomerAdvice() {

}
 
void showOnlineCustomer() {
	
}

void addMovieInfo() {
	string name;
	int duration;
	int startTime[10];
	int place[10];
	int time;
	cout << "��Ӱ��:";
	cin >> name;
	cout << "��Ӱʱ��:";
	cin >> duration;
	cout << "����";
	cin >> time;
	for (int i = 0; i < time; i++) {
		cout << "��" << i + 1 << "��������:";
		cin >> place[i];
		cout << "��" << i + 1 << "���Ŀ�ʼʱ��:";
		cin >> startTime[i];
	}
	
	int i;
	for (i = 0; i < 10; i++) {
		if(mi[i]->duration == 0){
			mi[i]->name = name;
			mi[i]->duration = duration;
			
			mi[i]->time = time;
			for (int j = 0; j < time; j++) {
				mi[i]->starttime[j] = startTime[j];
				mi[i]->place[j] = place[j];
				mi[i]->rmnTickets[j] = mi[i]->ttlTickets[j] = seats[place[j]];
			}
			
			
			cout << "¼��ɹ�" << endl;
			break;
		}
	}
	

}

void printMenu() {
	cout << "1: �鿴��Ʊ��" << endl;
	cout << "2: �鿴�˿ͽ���" << endl;
	cout << "3��¼���ӳ��Ϣ" << endl;

	cout << "10: ��ȫ�˳�" << endl;
}

void recordTicketsLeft() {
	

}
void admin(int ID) {
	string password = "";
	cout << "Please enter the password:" << endl;
	char ch = _getch();
	while (ch != 13) {
		password.push_back(ch);
		cout << "*";
		ch = _getch();
	}

	HANDLE hMap = OpenFileMapping(
		FILE_MAP_WRITE,
		FALSE,
		"movie");

	if (hMap == INVALID_HANDLE_VALUE) {
		cout << "OPEN FILE MAPPING ERROR" << endl;
	}

	LPVOID pData = MapViewOfFile(
		hMap,
		FILE_MAP_WRITE,
		0,
		0,
		sizeof(movieInfo));

	mi = reinterpret_cast<movieInfo*>(pData);

	//TODO check password
	stime();
	int choice;
	cout << "��ӭ����!" << endl;
	printMenu();

	while (cin >> choice) {
		if (choice == 10)
			break;
		if (choice == 1) {
			showTicketsLeft();
		}
		else if (choice == 2) {
			showCustomerAdvice();
		}
		else if (choice == 3) {
			addMovieInfo();
		}
		else if (choice == 4) {
			recordTicketsLeft();
		}
		else if (choice == 5){
			showOnlineCustomer();
		}else {
			cout << "Input error!  Please try again!" << endl;
		}
		printMenu();
	}
}

