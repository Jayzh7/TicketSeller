#include<Windows.h>
#include<iostream>

#ifndef headH
#define headH

using namespace std;
//���˿���
#define MAX_CUSTOMER_NUM 1000

//������Ա��
#define MAX_ADMIN_NUM 10

//����Ӱ��
#define MAX_MOVIE_NUM 6

//��󳡴�
#define MAX_PLAY_TIME 10

typedef struct movieinfo {
	string name;
	double grade;    //��Ӱ����
	int duration; // ����ʱ�� minutes
	string type;   //��Ӱ����
	int starttime[10];
	//endtime = startTime + duration
	int time;     //����
	int place[5];	 //ÿ�����ζ�Ӧ����ӳ����
	int price;   //Ʊ��
	int ttlTickets[10]; //��Ʊ��
	int rmnTickets[10]; //ʣ��Ʊ��
}movieInfo[10];

//++++++++����һ����������++++++++
struct msg_queue
{
	int f; // ͷ
	int r; //β
	int num; //����
	string text[100];
};

//������Ϣ�ṹ��
typedef struct processinfo {
	//�˿�����
	int num;
	//�˿ͽ��̵ľ������
	HANDLE cstmHandle[10];
	//�˿ͽ��̵�ID����
	DWORD cstmID[10];
}processInfo;


void customer(int);
void orderTickets(string, int, HANDLE, int);
void showMovies();
void createCustomer(void);
void createAdmin(void);
void admin(int);
void stime();
void  checkTicketInfo();
#endif