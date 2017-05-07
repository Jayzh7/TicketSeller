#include<Windows.h>
#include<iostream>

#ifndef headH
#define headH

using namespace std;
#define MAX_CUSTOMER_NUM 1000
#define MAX_ADMIN_NUM 10
#define MAX_MOVIE_NUM 6
#define MAX_PLAY_TIME 10

typedef struct movieinfo {
	string name;
	int grade;    //��Ӱ����
	int duration; // ����ʱ�� minutes
	string type;   //��Ӱ����
	int starttime[10];
	//endtime = startTime + duration
	int time;     //����
	int place[5]; //ÿ�����ζ�Ӧ����ӳ����
	int price;   //Ʊ��
	int ttlTickets[10]; //��Ʊ��
	int rmnTickets[10]; //ʣ��Ʊ��
}movieInfo[7];

void customer(int);
void orderTickets(string, int, HANDLE);
void showMovies();
void createCustomer(void);
void createAdmin(void);
void admin(int);
void initMovie(movieInfo, SYSTEMTIME, int, int, string);
void initMovieList(movieInfo);
void stime();
#endif