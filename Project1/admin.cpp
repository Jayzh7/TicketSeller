#include<iostream>
#include<string>
#include<Windows.h>
#include<conio.h>
#include "head.h"

using namespace std;

movieInfo* mi;
int seats[] = { 120, 120, 150, 200 };

void showTicketsLeft() {

}

void showCustomerAdvice() {

}
 

void addMovieInfo() {
	string name;
	int duration, time;
	int startTime[10];
	int place[10];
	cout << "��Ӱ��:";
	cin >> name;
	cout << "��Ӱʱ��:";
	cin >> duration;
	cout << "��ӳ����:";
	cin >> time;
	for (int i = 0; i < time; i++) {
		cout << "��ӳ��:";
		cin >> place[i];
		cout << "��" << i + 1 << "����ʼʱ��:";
		cin >> startTime[i];
	}
	int i;
	for (i = 0; i < 10; i++) {
		if(mi[i]->duration == 0){
			mi[i]->name = name;
			mi[i]->duration = duration;
			mi[i]->time = time;
			for (int j = 0; j < time; j++)
				mi[i]->starttime[j] = startTime[j];
			cout << "¼��ɹ�" << endl;
			break;
		}
	}
	

}

void printMenu() {
	cout << "1: �鿴��Ʊ" << endl;
	cout << "2: �鿴�˿ͽ���" << endl;
	cout << "3��¼���ӳ��Ϣ" << endl;
	cout << "4: ��ȫ�˳�" << endl;
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
		if (choice == 4)
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
		else {
			cout << "Input error!  Please try again!" << endl;
		}
		printMenu();
	}
	//system("pause");
}

