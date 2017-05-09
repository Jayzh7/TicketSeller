/*************************************************
Copyright:bit
Date:2017-05-08
Description:实现购票者买票退票
并查看相关票务信息的功能
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
Description:    // 该函数实现
Calls:          // void showMovies()
//void stime()
//void cusMenu()
//void orderTickets(string name, int num, HANDLE movie, int time)
//void submitCustomerAdvice(string str)
//void checkTicketInfo()
//void showTicketInfo()
//void refund()

Input:          // 输入相应功能编号
Output:         // 实现对应的被调用的函数功能
指示用户进行相关操作

Others:         // 输入‘1’     --->      进入订票系统
输入‘2’     --->      进入查看电影信息
输入‘3’     --->      进入意见或建议
输入‘4’     --->      进入查看已够电影票信息
输入‘5’     --->      进入退票系统
输入‘6’     --->      进入根据电影名查看信息
输入‘7’     --->      退出顾客界面
*************************************************/

void customer(int ID) {

	/*
	利用下面的函数打开文件映射对象，打开电影信息的共享缓冲区
	*/
	HANDLE hMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,      // access,所有可能访问的标志
		FALSE,                    // 不能由当前进程启动的新进程继承
		"movie");

	/*
	打开不成功，打印出"OPEN FILE MAPPING ERROR!"
	*/
	if (hMap == INVALID_HANDLE_VALUE) {
		cout << "OPEN FILE MAPPING ERROR!" << endl;
	}

	/*
	利用下面的函数打开文件映射对象，打开进程信息的共享缓冲区
	*/
	HANDLE cstmInfoMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		"processInfo");

	/*
	打开不成功，打印出"OPEN FILE MAPPING ERROR!"
	*/
	if (cstmInfoMap == INVALID_HANDLE_VALUE) {
		cout << "OPEN FILE MAPPING ERROR!" << endl;
	}

	/*
	该文件映射对象的全部或部分映射到进程地址空间，用读写内存的方式操作和处理文件数据
	*/
	LPVOID pData = MapViewOfFile(
		hMap,                     // hMap为CreateFileMapping()返回的文件映像对象句柄
		FILE_MAP_WRITE,           // 可以读取和写入文件
		0,                        // 表示文件映射起始偏移的高32位                                        
		0,                        // 表示文件映射起始偏移的低32位  
		sizeof(movieInfo));       // 指定映射文件的字节数

								  /*
								  该文件映射对象的全部或部分映射到进程地址空间，用读写内存的方式操作和处理文件数据
								  */
	LPVOID processInfoData = MapViewOfFile(
		cstmInfoMap,
		FILE_MAP_WRITE,
		0,
		0,
		sizeof(processInfo));

	pi = reinterpret_cast<processInfo*>(processInfoData);      // 指针转换整数
	MI = reinterpret_cast<movieinfo*>(pData);


	int choice = 0;

	/* 显示当前时间 */
	stime();

	/* 功能选项，及其相应函数的调用 */
	while (choice != 7) {
		cusMenu();
		cin >> choice;

		/* 购票 */
		if (choice == 1) {
			string mov = "";
			string strTime = "";

			cout << "请输入你要观看的电影:" << endl;
			cin >> mov;
			cout << "请输入场次:" << endl;
			cin >> strTime;

			/*
			利用下面函数打开信号量对象，文件映射对象句柄，为现有的一个已命名信号机对象创建一个新句柄
			打开购票者买票的信号量
			*/
			HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, (mov + strTime).c_str());

			/*
			如果电影不存在，打印出 "INPUT ERROR!"
			*/
			if (movie == NULL) {
				cout << "INPUT ERROR!" << endl;
				cout << "Exiting .." << endl;
				continue;
			}

			/*
			存在输入的电影名，打印出 "请输入购买数量:"，并调用orderTickets
			*/
			cout << "请输入购买数量:";
			int numTickets;
			cin >> numTickets;
			orderTickets(mov, numTickets, movie, atoi(strTime.c_str()) - 1);
		}

		/* 查看电影信息 */
		else if (choice == 2) {
			showMovies();
		}

		/* 意见或建议 */
		else if (choice == 3) {
			cout << "請輸入您的意見或建議?" << endl;
			string text = "";
			cin >> text;
			cout << "感謝您的反饋！" << endl;
			submitCustomerAdvice(text);
		}

		/* 已购信息 */
		else if (choice == 4) {
			showTicketInfo();
		}

		/* 退票 */
		else if (choice == 5) {
			refund();
		}

		/* 查询 */
		else if (choice == 6) {
			checkTicketInfo();
		}
	}

	UnmapViewOfFile(pData);
}



/*************************************************
Function:       // 用户提交建议（往缓冲区中写数据）
Description:    // 将顾客意见存入指定缓存区

Input:          // 通过customer()中的‘else if (choice == 3) ’及相关语句
获得顾客从键盘输入的信息

Others:         // 管理与可以看到缓存区内的意见和建议
*************************************************/

void submitCustomerAdvice(string str) {

	/*
	打开内存映射文件，打开顾客建议的共享缓存区
	*/
	HANDLE hMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		"advice");

	/*
	将内存映射文件映射到本进程的地址空间
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
	将顾客输入的意见存入缓存区
	移动指针，等待下一位用户写入意见
	*/
	p->text[p->num] = str;
	(p->r)++;
	(p->num)++;

}
/*************************************************
Function:       // orderTickets
Description:    // 实现电影售票系统的购票功能
Return:         // WAIT_TIMEOUT:表示票已售罄
//WAIT_OBJECT_0：表示购票成功
Others:         // 迭代器，类似于指针，指向关联容器，关联容器中有键和值
*************************************************/
void orderTickets(string name, int num, HANDLE movie, int time) {

	//打开一个名为mutex的信号量 

	HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");
	int idMov = 0;
	do {
		//if (strcmp((MI[idMov]->name).c_str(), name.c_str()) == 0) {
		if (MI[idMov].name == name) {
			break;
		}
		idMov++;
	}

	//当所查询的电影信息仍在遍历

	while (MI[idMov].duration != 0);

	cout << "排队等待 .." << endl;
	Sleep(600);

	//第二个参数为INFINITE表示一直等待，直到拥有互斥对象  

	WaitForSingleObject(mutex, INFINITE);
	cout << "购票中 .." << endl;
	Sleep(600);
	DWORD returnStatus;

	//声明了一个迭代器，类似于指针，指向关联容器，关联容器中有键和值，first是键，second是值。

	map<string, int>::iterator it;
	for (int i = 0; i < num; i++) {

		//等待3秒钟再次查询，如果有状态响应或者进程结束返回值

		returnStatus = WaitForSingleObject(movie, 3000);

		//如果等待3秒没有买到票返回WAIT_TIMEOUT

		if (returnStatus == WAIT_TIMEOUT) {
			cout << "抱歉，票已售罄！" << endl;
			break;
		}
		//如果等待3秒没有买到票返回WAIT_OBJECT_0
		else if (returnStatus == WAIT_OBJECT_0) {

			//购票成功的话，则所购电影对应场次的票量减掉购票数

			MI[idMov].rmnTickets[time]--;
			if (i == 0) {

				//插入到买票信息中顾客所购电影第一张票次的信息

				ticketInfo.insert(pair<string, int>(name, 1));

				//若有顾客再次购买相同电影时，则在买票信息中查询相应的名字所对应的数量上增加

				it = ticketInfo.find(name);
			}
			else {
				it->second++;





			}

			//若顾客买票成功，则输出恭喜成功买到第[i + 1]张票

			cout << "恭喜成功买到第" << i + 1 << "张票" << endl;
		}
	}


	//释放信号量mutex

	ReleaseSemaphore(mutex, 1, NULL);
}

/*************************************************
Function:       // showMovies()
Description:    // 进行全部电影信息的显示输出
Output:         // MI[n].name->电影名字
// MI[n].grade->评分
// MI[n].duration->时长
// MI[n].type->类型
// MI[n].starttime[i]->放映时间
// MI[n].time->场次
// MI[n].place[i]->放映厅号
// MI[n].price->票价
// MI[n].ttlTickets[i]->总票
// MI[n].rmnTickets[i]->余票
*************************************************/

void showMovies() {
	printf("您所浏览的电影信息是:\n");
	for (int n = 0; n < 10; n++)
	{
		//判断电影是否遍历完毕

		if (MI[n].duration == 0)
			break;

		//电影名称匹配，进行场次循环的输出

		for (int i = 0; i < MI[n].time; i++)
		{
			//左对齐方式输出电影的对应信息

			cout << left << setw(10) << "电影名" << setw(10) << "评分" << setw(10) << "时长" << setw(10) << "类型" << setw(10) << "时间" << setw(10) << "场次"
				<< setw(10) << "放映厅" << setw(10) << "票价" << setw(10) << "总票" << setw(10) << "余票" << endl;
			cout << left << setw(10) << MI[n].name << setw(12) << MI[n].grade << setw(12) << MI[n].duration << setw(10) << MI[n].type << MI[n].starttime[i] / 100 << ":" << setw(12) << MI[n].starttime[i] % 100
				<< setw(12) << MI[n].time << setw(12) << MI[n].place[i] << setw(12) << MI[n].price << setw(12) << MI[n].ttlTickets[i] << setw(12) << MI[n].rmnTickets[i] << endl;
			//cout <<"电影名《"<<mi[n].name<<"》*评分"<<mi[n].grade<<"*时长"<<mi[n].duration<<"*类型"<<
			//mi[n].type<<"*开场"<<mi[n].starttime<<"*场次"<<mi[n].time<<"*放映厅"<<mi[n].place<<"*票价"<<mi[n].price<<"*总票"<<mi[n].ttlTickets<<"*余票"<< mi[n].rmnTickets<<endl;

			printf("\n");
		}
	}
}

/*************************************************
Function:       // checkTicketInfo()
Description:    // 根据所要查询的电影名称，输出可购买的电影场次等信息
Calls:          // 被本函数调用的函数清单
Input:          // s->所要查询的电影名
Output:         // MI[n].name->电影名字
// MI[n].grade->评分
// MI[n].duration->时长
// MI[n].type->类型
// MI[n].starttime[i]->放映时间
// MI[n].time->场次
// MI[n].place[i]->放映厅号
// MI[n].price->票价
// MI[n].ttlTickets[i]->总票
// MI[n].rmnTickets[i]->余票
Others:         // 若当前查询时间已经超出电影的放映时间，则会显示今日此电影场次【i+1】已经开始播放，已经无票
*************************************************/

void checkTicketInfo()
{
	//定义结构体ptr
	struct tm *ptr;
	time_t  it;
	it = time(NULL);

	//ptr获得当前系统时间

	ptr = localtime(&it);

	int n;

	//定义20个字节的数组存放输入的电影名称

	char s[20];
	printf("请输入你要观看的电影名称：");
	scanf("%s", s);
	//printf("%s", s);

	for (n = 0; n < 4; n++)
	{
		//查询所要查找的电影名称与系统内的电影信息匹配

		if (MI[n].name == s)
		{
			//按场次循环输出与顾客查询的电影所匹配的电影信息

			for (int i = 0; i < MI[n].time; i++)
			{
				//如果当前查询的系统时间在电影放映时间之前

				if (MI[n].starttime[i] >= (ptr->tm_hour * 100) + ptr->tm_min) //time
				{
					printf("您想要观看的电影信息如下：\n");

					//左对齐方式输出电影的对应信息

					cout << left << setw(10) << "电影名" << setw(10) << "评分" << setw(10) << "时长" << setw(10) << "类型" << setw(10) << "时间" << setw(10) << "场次"
						<< setw(10) << "放映厅" << setw(10) << "票价" << setw(10) << "总票" << setw(10) << "余票" << endl;
					cout << left << setw(10) << MI[n].name << setw(12) << MI[n].grade << setw(12) << MI[n].duration << setw(10) << MI[n].type << MI[n].starttime[i] / 100 << ":" << setw(12) << MI[n].starttime[i] % 100
						<< setw(12) << MI[n].time << setw(12) << MI[n].place[i] << setw(12) << MI[n].price << setw(12) << MI[n].ttlTickets[i] << setw(12) << MI[n].rmnTickets[i] << endl;
				}
				else

					//如果遍历之后没有发现相匹配的电影信息，则输出此电影场次[i + 1]已经开始播放，已经无票

					cout << "此电影场次" << i + 1 << "已经开始播放，已经无票" << endl;
			}
		}

	}
}

/*************************************************
Function:       // stime()
Description:    // 输出本地时间
Output:         // ptr->tm_year + 1900：年
// ptr->tm_mon + 1：月
// ptr->tm_mday：天
// ptr->tm_hour：小时
// ptr->tm_min：分钟
// ptr->tm_sec：秒
Others:         // 其它说明
//tm_year是实际年份-1900
//tm_mon是从0开始的，取值范围是[0,11]
*************************************************/

void stime()
{
	//定义ptr结构

	struct tm *ptr;
	time_t  it;
	it = time(NULL);

	//获得当前系统时间

	ptr = localtime(&it);

	//打印当前时间

	printf("\n现在的时间是%4d年%02d月%02d日%d:%d:%d\n", ptr->tm_year + 1900, ptr->tm_mon + 1, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
}

/*************************************************
Function:       // cusMenu()
Description:    //实现电影售票系统中顾客的选择页面
Calls:          // void showMovies();
// void showTicketInfo();
//void refund();
//void checkTicketInfo();
Input:         //输入1：跳转到售票界面
//输入2：调用showMovies()函数，实现显示电影信息功能
//输入3：跳转到意见反馈界面
//输入4：调用showTicketInfo()函数，实现显示电影售票情况信息
//输入5：调用refund()函数，实现退票功能
//输入6：调用checkTicketInfo()函数，实现查询电影场次及余票信息
//输入7：跳出顾客页面
*************************************************/

void cusMenu() {

	//输入1：跳转到订票界面

	cout << "1: 订票" << endl;

	//输入2：调用showMovies()函数，实现显示电影信息功能

	cout << "2: 查看电影信息" << endl;

	//输入3：跳转到意见反馈界面

	cout << "3: 意见或建议" << endl;

	//输入4：调用showTicketInfo()函数，实现显示电影售票情况信息

	cout << "4: 查看已购电影票信息" << endl;

	//输入5：调用refund()函数，实现退票功能

	cout << "5: 退票" << endl;

	//输入6：调用checkTicketInfo()函数，实现查询电影场次及余票信息

	cout << "6: 根据电影名查询" << endl;

	//输入7：跳出顾客页面

	cout << "7: 退出" << endl;
}

/*************************************************
Function:       //showTicketInfo()
Description:    // 显示目前系统内已经存在的电影购票情况
Output:         // it->first：表示电影的名称
// it->second：表示电影的售票量
*************************************************/

void showTicketInfo() {

	////声明了一个迭代器，类似于指针，指向关联容器，关联容器中有键和值，first是键，second是值。

	map<string, int>::iterator it;

	//输出当前系统中订票信息中的影票名及已售票数

	cout << "影片名                      已购数量" << endl;
	cout << "------------------------------------" << endl;

	//定义一个bool变量 movieExist判断有没有买票

	bool movieExist = false;

	//购票成功的话，遍历输出买票信息，并将movieExist的值置为true

	for (it = ticketInfo.begin(); it != ticketInfo.end(); it++) {

		//输出当前系统中订票信息中的影票名及已售票数

		cout << it->first << "                      " << it->second << endl;
		movieExist = true;
	}

	//如果购票没有成功则movieExist的值仍为false

	if (!movieExist) {
		cout << "您没有购票信息！" << endl;
	}
}

/*************************************************
Function:       //refund
Description:    // 实现电影的退票处理
Calls:          // ticketInfo
Input:          // name->想要退票的名字
// num->想要退票的数量
Output:         // 对输出参数的说明。
Others:         // 其它说明
*************************************************/

void refund() {

	/*
	指示用户输入相关信息
	*/
	string name = "";
	string cc = "";
	cout << "请输入您想退票的电影名：";
	cin >> name;
	cout << "请输入您购买的场次:";
	cin >> cc;

	/*
	迭代器就是指针，指向这个关联容器啊，关联容器有键和值，first就是键，second就是值
	it相当于一个指针，指向电影的名称和购买的数量
	*/
	map<string, int>::iterator it;
	it = ticketInfo.find(name);

	/*
	查询了所有的电影也没找到该用户购买的电影票
	打印 "您未购买此电影的电影票，请重试"
	*/
	if (it == ticketInfo.end()) {
		cout << "您未购买此电影的电影票，请重试" << endl;
	}

	/*
	输入退票数量，释放信号量
	*/
	else {
		cout << "请输入您想退票的数量：";
		int num;
		cin >> num;

		/*
		没有该电影
		*/
		for (int i = 0; i < 10; i++) {
			if (MI[i].duration == 0)
				break;

			/*
			找到该用户所买的电影票
			将本场次电影余票数量相应的增加
			*/
			if (MI[i].name == name) {
				MI[i].rmnTickets[stoi(cc) - 1] += num;
				break;
			}
		}

		/*
		用户不能退比自己购买的票数多的票
		若用户退的票数大于所购买的票数，则只能退其购买的票数
		*/
		int refundNum = num > it->second ? it->second : num;

		it->second -= refundNum;

		/*
		互斥信号量
		该用户等待无人买票卖票时进行操作
		*/
		HANDLE mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, "mutex");

		WaitForSingleObject(mutex, INFINITE);      // 当semEmpty为有信号状态时，函数返回

		HANDLE movie = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, (name.append(cc)).c_str());

		/*
		释放用户购买的票数的信号量
		*/

		for (int i = 0; i < refundNum; i++) {
			ReleaseSemaphore(movie, 1, NULL);
		}

		/*
		把插入的那一条删除
		*/
		if (it->second == 0) {
			ticketInfo.erase(it);
		}

		/*
		退票成功，释放互斥信号量
		*/
		//TODO write share memory
		//释放信号量
		ReleaseSemaphore(mutex, 1, NULL);
		cout << "您已成功退票" << refundNum << "张" << endl;
	}
}
