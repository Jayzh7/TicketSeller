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
	int grade;    //电影评分
	int duration; // 持续时间 minutes
	string type;   //电影类型
	int starttime[10];
	//endtime = startTime + duration
	int time;     //场次
	int place[5]; //每个场次对应的上映厅号
	int price;   //票价
	int ttlTickets[10]; //总票数
	int rmnTickets[10]; //剩余票数
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