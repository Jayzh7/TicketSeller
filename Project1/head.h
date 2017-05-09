#include<Windows.h>
#include<iostream>

#ifndef headH
#define headH

using namespace std;
//最大顾客数
#define MAX_CUSTOMER_NUM 1000

//最大管理员数
#define MAX_ADMIN_NUM 10

//最大电影数
#define MAX_MOVIE_NUM 6

//最大场次
#define MAX_PLAY_TIME 10

typedef struct movieinfo {
	string name;
	double grade;    //电影评分
	int duration; // 持续时间 minutes
	string type;   //电影类型
	int starttime[10];
	//endtime = startTime + duration
	int time;     //场次
	int place[5];	 //每个场次对应的上映厅号
	int price;   //票价
	int ttlTickets[10]; //总票数
	int rmnTickets[10]; //剩余票数
}movieInfo[10];

//++++++++定义一个缓存区存++++++++
struct msg_queue
{
	int f; // 头
	int r; //尾
	int num; //数量
	string text[100];
};

//进程信息结构体
typedef struct processinfo {
	//顾客数量
	int num;
	//顾客进程的句柄数组
	HANDLE cstmHandle[10];
	//顾客进程的ID数组
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