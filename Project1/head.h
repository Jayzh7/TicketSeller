#include<Windows.h>
#include<iostream>

using namespace std;
#define MAX_CUSTOMER_NUM 1000
#define MAX_ADMIN_NUM 10
#define MAX_MOVIE_NUM 6
#define MAX_PLAY_TIME 10

typedef struct movieinfo {
	SYSTEMTIME startTime[MAX_PLAY_TIME];
	SYSTEMTIME endTime[MAX_PLAY_TIME];
	int duration; // minutes
	int tickets;  //remaining tickets 
	string name;
}movieInfo[10];

void customer(int);
void orderTickets(string);
void showMovies();
void createCustomer(void);
void createAdmin(void);
void admin(int);
void initMovie(movieInfo, SYSTEMTIME, int, int, string);
void initMovieList(movieInfo);

