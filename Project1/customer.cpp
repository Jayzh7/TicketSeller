/*************************************************
Copyright:bit
Date:2017-05-08
Description:ʵ�ֹ�Ʊ����Ʊ��Ʊ
���鿴���Ʊ����Ϣ�Ĺ���
**************************************************/


#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<string>
#include<time.h>
#include<stdlib.h>
#include<map>
#include<sstream>
#include "head.h"
#include<iomanip>

using namespace std;

void cusMenu();
void showTicketInfo();
void refund();
void submitCustomerAdvice(string str);


map<string, int> ticketInfo;
movieinfo *MI;
processInfo *pi;



/*************************************************
Function:       // customer()
Description:    // �ú���ʵ��
Calls:          // void showMovies()
//void stime()
//void cusMenu()
//void orderTickets(string name, int num, HANDLE movie, int time)
//void submitCustomerAdvice(string str)
//void checkTicketInfo()
//void showTicketInfo()
//void refund()

Input:          // ������Ӧ���ܱ��
Output:         // ʵ�ֶ�Ӧ�ı����õĺ�������
ָʾ�û�������ز���

Others:         // ���롮1��     --->      ���붩Ʊϵͳ
���롮2��     --->      ����鿴��Ӱ��Ϣ
���롮3��     --->      �����������
���롮4��     --->      ����鿴�ѹ���ӰƱ��Ϣ
���롮5��     --->      ������Ʊϵͳ
���롮6��     --->      ������ݵ�Ӱ���鿴��Ϣ
���롮7��     --->      �˳��˿ͽ���
*************************************************/

void customer(int ID) {

	/*
	��������ĺ������ļ�ӳ����󣬴򿪵�Ӱ��Ϣ�Ĺ�������
	*/
	HANDLE hMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,      // access,���п��ܷ��ʵı�־
		FALSE,                    // �����ɵ�ǰ�����������½��̼̳�
		"movie");

	/*
	�򿪲��ɹ�����ӡ��"OPEN FILE MAPPING ERROR!"
	*/
	if (hMap == INVALID_HANDLE_VALUE) {
		cout << "OPEN FILE MAPPING ERROR!" << endl;
	}

	/*
	��������ĺ������ļ�ӳ����󣬴򿪽�����Ϣ�Ĺ�������
	*/
	HANDLE cstmInfoMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		"processInfo");

	/*
	�򿪲��ɹ�����ӡ��"OPEN FILE MAPPING ERROR!"
	*/
	if (cstmInfoMap == INVALID_HANDLE_VALUE) {
		cout << "OPEN FILE MAPPING ERROR!" << endl;
	}

	/*
	���ļ�ӳ������ȫ���򲿷�ӳ�䵽���̵�ַ�ռ䣬�ö�д�ڴ�ķ�ʽ�����ʹ����ļ�����
	*/
	LPVOID pData = MapViewOfFile(
		hMap,                     // hMapΪCreateFileMapping()���ص��ļ�ӳ�������
		FILE_MAP_WRITE,           // ���Զ�ȡ��д���ļ�
		0,                        // ��ʾ�ļ�ӳ����ʼƫ�Ƶĸ�32λ                                        
		0,                        // ��ʾ�ļ�ӳ����ʼƫ�Ƶĵ�32λ  
		sizeof(movieInfo));       // ָ��ӳ���ļ����ֽ���

								  /*
								  ���ļ�ӳ������ȫ���򲿷�ӳ�䵽���̵�ַ�ռ䣬�ö�д�ڴ�ķ�ʽ�����ʹ����ļ�����
								  */
	LPVOID processInfoData = MapViewOfFile(
		cstmInfoMap,
		FILE_MAP_WRITE,
		0,
		0,
		sizeof(processInfo));

	pi = reinterpret_cast<processInfo*>(processInfoData);      // ָ��ת������
	MI = reinterpret_cast<movieinfo*>(pData);


	int choice = 0;

	/* ��ʾ��ǰʱ�� */
	stime();

	/* ����ѡ�������Ӧ�����ĵ��� */
	while (choice != 7) {
		cusMenu();
		cin >> choice;

		/* ��Ʊ */
		if (choice == 1) {
			string mov = "";
			string strTime = "";

			cout << "��������Ҫ�ۿ��ĵ�Ӱ:" << endl;
			cin >> mov;
			cout << "�����볡��:" << endl;
			cin >> strTime;

			/*
			�������溯�����ź��������ļ�ӳ���������Ϊ���е�һ���������źŻ����󴴽�һ���¾��
			�򿪹�Ʊ����Ʊ���ź���
			*/
			HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, (mov + strTime).c_str());

			/*
			�����Ӱ�����ڣ���ӡ�� "INPUT ERROR!"
			*/
			if (movie == NULL) {
				cout << "INPUT ERROR!" << endl;
				cout << "Exiting .." << endl;
				continue;
			}

			/*
			��������ĵ�Ӱ������ӡ�� "�����빺������:"��������orderTickets
			*/
			cout << "�����빺������:";
			int numTickets;
			cin >> numTickets;
			orderTickets(mov, numTickets, movie, atoi(strTime.c_str()) - 1);
		}

		/* �鿴��Ӱ��Ϣ */
		else if (choice == 2) {
			showMovies();
		}

		/* ������� */
		else if (choice == 3) {
			cout << "Ոݔ��������Ҋ���h?" << endl;
			string text = "";
			cin >> text;
			cout << "���x���ķ�����" << endl;
			submitCustomerAdvice(text);
		}

		/* �ѹ���Ϣ */
		else if (choice == 4) {
			showTicketInfo();
		}

		/* ��Ʊ */
		else if (choice == 5) {
			refund();
		}

		/* ��ѯ */
		else if (choice == 6) {
			checkTicketInfo();
		}
	}

	UnmapViewOfFile(pData);
}



/*************************************************
Function:       // �û��ύ���飨����������д���ݣ�
Description:    // ���˿��������ָ��������

Input:          // ͨ��customer()�еġ�else if (choice == 3) ����������
��ù˿ʹӼ����������Ϣ

Others:         // ��������Կ����������ڵ�����ͽ���
*************************************************/

void submitCustomerAdvice(string str) {

	/*
	���ڴ�ӳ���ļ����򿪹˿ͽ���Ĺ�������
	*/
	HANDLE hMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		"advice");

	/*
	���ڴ�ӳ���ļ�ӳ�䵽�����̵ĵ�ַ�ռ�
	*/
	LPVOID pfile = MapViewOfFile(
		hMap,
		FILE_MAP_WRITE,
		0,
		0,
		sizeof(msg_queue));

	msg_queue *p;;

	p = reinterpret_cast<msg_queue*>(pfile);

	/*
	���˿������������뻺����
	�ƶ�ָ�룬�ȴ���һλ�û�д�����
	*/
	p->text[p->num] = str;
	(p->r)++;
	(p->num)++;

}
/*************************************************
Function:       // orderTickets
Description:    // ʵ�ֵ�Ӱ��Ʊϵͳ�Ĺ�Ʊ����
Return:         // WAIT_TIMEOUT:��ʾƱ������
//WAIT_OBJECT_0����ʾ��Ʊ�ɹ�
Others:         // ��������������ָ�룬ָ����������������������м���ֵ
*************************************************/
void orderTickets(string name, int num, HANDLE movie, int time) {

	//��һ����Ϊmutex���ź��� 

	HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");
	int idMov = 0;
	do {
		//if (strcmp((MI[idMov]->name).c_str(), name.c_str()) == 0) {
		if (MI[idMov].name == name) {
			break;
		}
		idMov++;
	}

	//������ѯ�ĵ�Ӱ��Ϣ���ڱ���

	while (MI[idMov].duration != 0);

	cout << "�Ŷӵȴ� .." << endl;
	Sleep(600);

	//�ڶ�������ΪINFINITE��ʾһֱ�ȴ���ֱ��ӵ�л������  

	WaitForSingleObject(mutex, INFINITE);
	cout << "��Ʊ�� .." << endl;
	Sleep(600);
	DWORD returnStatus;

	//������һ����������������ָ�룬ָ����������������������м���ֵ��first�Ǽ���second��ֵ��

	map<string, int>::iterator it;
	for (int i = 0; i < num; i++) {

		//�ȴ�3�����ٴβ�ѯ�������״̬��Ӧ���߽��̽�������ֵ

		returnStatus = WaitForSingleObject(movie, 3000);

		//����ȴ�3��û����Ʊ����WAIT_TIMEOUT

		if (returnStatus == WAIT_TIMEOUT) {
			cout << "��Ǹ��Ʊ��������" << endl;
			break;
		}
		//����ȴ�3��û����Ʊ����WAIT_OBJECT_0
		else if (returnStatus == WAIT_OBJECT_0) {

			//��Ʊ�ɹ��Ļ�����������Ӱ��Ӧ���ε�Ʊ��������Ʊ��

			MI[idMov].rmnTickets[time]--;
			if (i == 0) {

				//���뵽��Ʊ��Ϣ�й˿�������Ӱ��һ��Ʊ�ε���Ϣ

				ticketInfo.insert(pair<string, int>(name, 1));

				//���й˿��ٴι�����ͬ��Ӱʱ��������Ʊ��Ϣ�в�ѯ��Ӧ����������Ӧ������������

				it = ticketInfo.find(name);
			}
			else {
				it->second++;





			}

			//���˿���Ʊ�ɹ����������ϲ�ɹ��򵽵�[i + 1]��Ʊ

			cout << "��ϲ�ɹ��򵽵�" << i + 1 << "��Ʊ" << endl;
		}
	}


	//�ͷ��ź���mutex

	ReleaseSemaphore(mutex, 1, NULL);
}

/*************************************************
Function:       // showMovies()
Description:    // ����ȫ����Ӱ��Ϣ����ʾ���
Output:         // MI[n].name->��Ӱ����
// MI[n].grade->����
// MI[n].duration->ʱ��
// MI[n].type->����
// MI[n].starttime[i]->��ӳʱ��
// MI[n].time->����
// MI[n].place[i]->��ӳ����
// MI[n].price->Ʊ��
// MI[n].ttlTickets[i]->��Ʊ
// MI[n].rmnTickets[i]->��Ʊ
*************************************************/

void showMovies() {
	printf("��������ĵ�Ӱ��Ϣ��:\n");
	for (int n = 0; n < 10; n++)
	{
		//�жϵ�Ӱ�Ƿ�������

		if (MI[n].duration == 0)
			break;

		//��Ӱ����ƥ�䣬���г���ѭ�������

		for (int i = 0; i < MI[n].time; i++)
		{
			//����뷽ʽ�����Ӱ�Ķ�Ӧ��Ϣ

			cout << left << setw(10) << "��Ӱ��" << setw(10) << "����" << setw(10) << "ʱ��" << setw(10) << "����" << setw(10) << "ʱ��" << setw(10) << "����"
				<< setw(10) << "��ӳ��" << setw(10) << "Ʊ��" << setw(10) << "��Ʊ" << setw(10) << "��Ʊ" << endl;
			cout << left << setw(10) << MI[n].name << setw(12) << MI[n].grade << setw(12) << MI[n].duration << setw(10) << MI[n].type << MI[n].starttime[i] / 100 << ":" << setw(12) << MI[n].starttime[i] % 100
				<< setw(12) << MI[n].time << setw(12) << MI[n].place[i] << setw(12) << MI[n].price << setw(12) << MI[n].ttlTickets[i] << setw(12) << MI[n].rmnTickets[i] << endl;
			//cout <<"��Ӱ����"<<mi[n].name<<"��*����"<<mi[n].grade<<"*ʱ��"<<mi[n].duration<<"*����"<<
			//mi[n].type<<"*����"<<mi[n].starttime<<"*����"<<mi[n].time<<"*��ӳ��"<<mi[n].place<<"*Ʊ��"<<mi[n].price<<"*��Ʊ"<<mi[n].ttlTickets<<"*��Ʊ"<< mi[n].rmnTickets<<endl;

			printf("\n");
		}
	}
}

/*************************************************
Function:       // checkTicketInfo()
Description:    // ������Ҫ��ѯ�ĵ�Ӱ���ƣ�����ɹ���ĵ�Ӱ���ε���Ϣ
Calls:          // �����������õĺ����嵥
Input:          // s->��Ҫ��ѯ�ĵ�Ӱ��
Output:         // MI[n].name->��Ӱ����
// MI[n].grade->����
// MI[n].duration->ʱ��
// MI[n].type->����
// MI[n].starttime[i]->��ӳʱ��
// MI[n].time->����
// MI[n].place[i]->��ӳ����
// MI[n].price->Ʊ��
// MI[n].ttlTickets[i]->��Ʊ
// MI[n].rmnTickets[i]->��Ʊ
Others:         // ����ǰ��ѯʱ���Ѿ�������Ӱ�ķ�ӳʱ�䣬�����ʾ���մ˵�Ӱ���Ρ�i+1���Ѿ���ʼ���ţ��Ѿ���Ʊ
*************************************************/

void checkTicketInfo()
{
	//����ṹ��ptr
	struct tm *ptr;
	time_t  it;
	it = time(NULL);

	//ptr��õ�ǰϵͳʱ��

	ptr = localtime(&it);

	int n;

	//����20���ֽڵ�����������ĵ�Ӱ����

	char s[20];
	printf("��������Ҫ�ۿ��ĵ�Ӱ���ƣ�");
	scanf("%s", s);
	//printf("%s", s);

	for (n = 0; n < 4; n++)
	{
		//��ѯ��Ҫ���ҵĵ�Ӱ������ϵͳ�ڵĵ�Ӱ��Ϣƥ��

		if (MI[n].name == s)
		{
			//������ѭ�������˿Ͳ�ѯ�ĵ�Ӱ��ƥ��ĵ�Ӱ��Ϣ

			for (int i = 0; i < MI[n].time; i++)
			{
				//�����ǰ��ѯ��ϵͳʱ���ڵ�Ӱ��ӳʱ��֮ǰ

				if (MI[n].starttime[i] >= (ptr->tm_hour * 100) + ptr->tm_min) //time
				{
					printf("����Ҫ�ۿ��ĵ�Ӱ��Ϣ���£�\n");

					//����뷽ʽ�����Ӱ�Ķ�Ӧ��Ϣ

					cout << left << setw(10) << "��Ӱ��" << setw(10) << "����" << setw(10) << "ʱ��" << setw(10) << "����" << setw(10) << "ʱ��" << setw(10) << "����"
						<< setw(10) << "��ӳ��" << setw(10) << "Ʊ��" << setw(10) << "��Ʊ" << setw(10) << "��Ʊ" << endl;
					cout << left << setw(10) << MI[n].name << setw(12) << MI[n].grade << setw(12) << MI[n].duration << setw(10) << MI[n].type << MI[n].starttime[i] / 100 << ":" << setw(12) << MI[n].starttime[i] % 100
						<< setw(12) << MI[n].time << setw(12) << MI[n].place[i] << setw(12) << MI[n].price << setw(12) << MI[n].ttlTickets[i] << setw(12) << MI[n].rmnTickets[i] << endl;
				}
				else

					//�������֮��û�з�����ƥ��ĵ�Ӱ��Ϣ��������˵�Ӱ����[i + 1]�Ѿ���ʼ���ţ��Ѿ���Ʊ

					cout << "�˵�Ӱ����" << i + 1 << "�Ѿ���ʼ���ţ��Ѿ���Ʊ" << endl;
			}
		}

	}
}

/*************************************************
Function:       // stime()
Description:    // �������ʱ��
Output:         // ptr->tm_year + 1900����
// ptr->tm_mon + 1����
// ptr->tm_mday����
// ptr->tm_hour��Сʱ
// ptr->tm_min������
// ptr->tm_sec����
Others:         // ����˵��
//tm_year��ʵ�����-1900
//tm_mon�Ǵ�0��ʼ�ģ�ȡֵ��Χ��[0,11]
*************************************************/

void stime()
{
	//����ptr�ṹ

	struct tm *ptr;
	time_t  it;
	it = time(NULL);

	//��õ�ǰϵͳʱ��

	ptr = localtime(&it);

	//��ӡ��ǰʱ��

	printf("\n���ڵ�ʱ����%4d��%02d��%02d��%d:%d:%d\n", ptr->tm_year + 1900, ptr->tm_mon + 1, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
}

/*************************************************
Function:       // cusMenu()
Description:    //ʵ�ֵ�Ӱ��Ʊϵͳ�й˿͵�ѡ��ҳ��
Calls:          // void showMovies();
// void showTicketInfo();
//void refund();
//void checkTicketInfo();
Input:         //����1����ת����Ʊ����
//����2������showMovies()������ʵ����ʾ��Ӱ��Ϣ����
//����3����ת�������������
//����4������showTicketInfo()������ʵ����ʾ��Ӱ��Ʊ�����Ϣ
//����5������refund()������ʵ����Ʊ����
//����6������checkTicketInfo()������ʵ�ֲ�ѯ��Ӱ���μ���Ʊ��Ϣ
//����7�������˿�ҳ��
*************************************************/

void cusMenu() {

	//����1����ת����Ʊ����

	cout << "1: ��Ʊ" << endl;

	//����2������showMovies()������ʵ����ʾ��Ӱ��Ϣ����

	cout << "2: �鿴��Ӱ��Ϣ" << endl;

	//����3����ת�������������

	cout << "3: �������" << endl;

	//����4������showTicketInfo()������ʵ����ʾ��Ӱ��Ʊ�����Ϣ

	cout << "4: �鿴�ѹ���ӰƱ��Ϣ" << endl;

	//����5������refund()������ʵ����Ʊ����

	cout << "5: ��Ʊ" << endl;

	//����6������checkTicketInfo()������ʵ�ֲ�ѯ��Ӱ���μ���Ʊ��Ϣ

	cout << "6: ���ݵ�Ӱ����ѯ" << endl;

	//����7�������˿�ҳ��

	cout << "7: �˳�" << endl;
}

/*************************************************
Function:       //showTicketInfo()
Description:    // ��ʾĿǰϵͳ���Ѿ����ڵĵ�Ӱ��Ʊ���
Output:         // it->first����ʾ��Ӱ������
// it->second����ʾ��Ӱ����Ʊ��
*************************************************/

void showTicketInfo() {

	////������һ����������������ָ�룬ָ����������������������м���ֵ��first�Ǽ���second��ֵ��

	map<string, int>::iterator it;

	//�����ǰϵͳ�ж�Ʊ��Ϣ�е�ӰƱ��������Ʊ��

	cout << "ӰƬ��                      �ѹ�����" << endl;
	cout << "------------------------------------" << endl;

	//����һ��bool���� movieExist�ж���û����Ʊ

	bool movieExist = false;

	//��Ʊ�ɹ��Ļ������������Ʊ��Ϣ������movieExist��ֵ��Ϊtrue

	for (it = ticketInfo.begin(); it != ticketInfo.end(); it++) {

		//�����ǰϵͳ�ж�Ʊ��Ϣ�е�ӰƱ��������Ʊ��

		cout << it->first << "                      " << it->second << endl;
		movieExist = true;
	}

	//�����Ʊû�гɹ���movieExist��ֵ��Ϊfalse

	if (!movieExist) {
		cout << "��û�й�Ʊ��Ϣ��" << endl;
	}
}

/*************************************************
Function:       //refund
Description:    // ʵ�ֵ�Ӱ����Ʊ����
Calls:          // ticketInfo
Input:          // name->��Ҫ��Ʊ������
// num->��Ҫ��Ʊ������
Output:         // �����������˵����
Others:         // ����˵��
*************************************************/

void refund() {

	/*
	ָʾ�û����������Ϣ
	*/
	string name = "";
	string cc = "";
	cout << "������������Ʊ�ĵ�Ӱ����";
	cin >> name;
	cout << "������������ĳ���:";
	cin >> cc;

	/*
	����������ָ�룬ָ��������������������������м���ֵ��first���Ǽ���second����ֵ
	it�൱��һ��ָ�룬ָ���Ӱ�����ƺ͹��������
	*/
	map<string, int>::iterator it;
	it = ticketInfo.find(name);

	/*
	��ѯ�����еĵ�ӰҲû�ҵ����û�����ĵ�ӰƱ
	��ӡ "��δ����˵�Ӱ�ĵ�ӰƱ��������"
	*/
	if (it == ticketInfo.end()) {
		cout << "��δ����˵�Ӱ�ĵ�ӰƱ��������" << endl;
	}

	/*
	������Ʊ�������ͷ��ź���
	*/
	else {
		cout << "������������Ʊ��������";
		int num;
		cin >> num;

		/*
		û�иõ�Ӱ
		*/
		for (int i = 0; i < 10; i++) {
			if (MI[i].duration == 0)
				break;

			/*
			�ҵ����û�����ĵ�ӰƱ
			�������ε�Ӱ��Ʊ������Ӧ������
			*/
			if (MI[i].name == name) {
				MI[i].rmnTickets[stoi(cc) - 1] += num;
				break;
			}
		}

		/*
		�û������˱��Լ������Ʊ�����Ʊ
		���û��˵�Ʊ�������������Ʊ������ֻ�����乺���Ʊ��
		*/
		int refundNum = num > it->second ? it->second : num;

		it->second -= refundNum;

		/*
		�����ź���
		���û��ȴ�������Ʊ��Ʊʱ���в���
		*/
		HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");

		WaitForSingleObject(mutex, INFINITE);      // ��semEmptyΪ���ź�״̬ʱ����������

		HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, (name.append(cc)).c_str());

		/*
		�ͷ��û������Ʊ�����ź���
		*/

		for (int i = 0; i < refundNum; i++) {
			ReleaseSemaphore(movie, 1, NULL);
		}

		/*
		�Ѳ������һ��ɾ��
		*/
		if (it->second == 0) {
			ticketInfo.erase(it);
		}

		/*
		��Ʊ�ɹ����ͷŻ����ź���
		*/
		//TODO write share memory
		//�ͷ��ź���
		ReleaseSemaphore(mutex, 1, NULL);
		cout << "���ѳɹ���Ʊ" << refundNum << "��" << endl;
	}
}
