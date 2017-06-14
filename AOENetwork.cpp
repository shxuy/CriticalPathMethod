//
//  AOENetwork.cpp
//  CriticalPathMethod
//
//  Created by Nick's Macbook Pro on 2017/6/5.
//  Copyright © 2017年 HaiTianYang. All rights reserved.
//

#include "AOENetwork.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <stack>
#include <queue>

using namespace std;

int AOENetwork::GetEventPos(const string &event)
{ //当前事件顶点 event 在 AOE 网络的事件顶点表中的位置
	for (int i = 0; i < EventsNum; i++)
		if (EventTable[i].name == event)
			return i;
	return -1; //若不存在，返回 -1
}

double AOENetwork::GetCost(const int pos1, const int pos2)
{ //返回活动耗费的时间
	if (pos1 != -1 && pos2 != -1) {
		ActivityNode * pointer = EventTable[pos1].firstout; //活动链表头指针
		while (pointer != NULL) {
			if (pointer->head == pos2)
				return pointer->cost; //找到对应活动
			else
				pointer = pointer->tailLink;
		}
	}
	return -1; //若不存在，返回 -1
}

int AOENetwork::GetFirstNeighbor(const int pos)
{ //取顶点pos(用它在事件顶点表中的位置表示，下同)的第一个正邻接表的邻接顶点
	if (pos != -1) { //顶点pos存在
		ActivityNode * pointer = EventTable[pos].firstout; //活动链表头指针
		if (pointer != NULL)
			return pointer->head;
	}
	return -1; //若不存在，返回 -1
}

int AOENetwork::GetNextNeighbor(const int pos1, const int pos2)
{ //取顶点pos1的某正邻接表邻接顶点pos2的下一个邻接顶点
	if (pos1 != -1) { //顶点pos1存在
		ActivityNode * pointer = EventTable[pos1].firstout; //活动链表头指针
		while (pointer != NULL) { //寻找某邻接顶点pos2
			if (pointer->head == pos2 && pointer->tailLink != NULL)
				return pointer->tailLink->head;
			else
				pointer = pointer->tailLink;
		}
	}
	return -1; //若不存在，返回 -1
}

int AOENetwork::GetFirstInverseNeighbor(const int pos)
{ //取顶点pos的第一个逆邻接表的邻接顶点
	if (pos != -1) { //顶点pos存在
		ActivityNode * pointer = EventTable[pos].firstin; //活动链表头指针
		if (pointer != NULL)
			return pointer->tail;
	}
	return -1; //若不存在，返回 -1
}

int AOENetwork::GetNextInverseNeighbor(const int pos1, const int pos2)
{ //取顶点pos1的某逆邻接表邻接顶点pos2的下一个邻接顶点
	if (pos1 != -1) { //顶点pos1存在
		ActivityNode * pointer = EventTable[pos1].firstin; //活动链表头指针
		while (pointer != NULL) { //寻找某邻接顶点pos2
			if (pointer->tail == pos2 && pointer->headLink != NULL)
				return pointer->headLink->tail;
			else
				pointer = pointer->headLink;
		}
	}
	return -1; //若不存在，返回 -1
}

AOENetwork::AOENetwork(int maxSize) :EventsNum(0), EventsMaxNum(maxSize), ActivitiesNum(0)
{ //构造函数，maxSize是最大事件数量
#ifdef EXAMINEINPUT //检查用户输入是否正确，核心代码见 #else 和 #endif 之间
	int e; //记录事件顶点数量
	int a; //记录活动数量
	int i; //循环变量
	EventTable = new EventNode[EventsMaxNum]; //创建对用户不可见的事件顶点表
	cout << "Please input the number of events: ";
	while (!(cin >> e)) { //输入的不是数字
		cerr << "Please enter a number:";
		cin.clear();
		while (cin.get() != '\n')
			continue;
	}
	while (e <= 0 || e > maxSize) {
		if (e <= 0) //将要输入的事件数小于等于0，不合理
			cerr << "Please enter a positive number:";
		else //将要输入的事件数超过AOE网络的容量
			cerr << "Please enter a number less than or equal to " << maxSize << " :";
		while (cin.get() != '\n') //防止用户在这一行cin已成功读取的数据后面输入无效数据，影响后面数据的读取
			continue;
		while (!(cin >> e)) { //输入的不是数字
			cerr << "Please enter a number:";
			cin.clear();
			while (cin.get() != '\n')
				continue;
		}
	}
	while (cin.get() != '\n') //防止用户在这一行有效数据后面输入无效数据，影响后面数据的读取
		continue;
	cout << "Please input the names of all events in one line:" << endl;
	string eventName; //事件顶点名称
	int errInput = 0; //记录错误输入的数据个数
	for (i = 0; i < e; i++) {
		cin >> eventName;
		int status = InsertEvent(eventName); //检查插入事件结点是否成功
		if (status == 1) { //事件顶点表中已经存在这个事件
			cerr << eventName << " is already in the network, please try again later." << endl;
			errInput++; //记录错误输入的数据个数，给一个过会儿重新输入的机会
		}
		else if (status == 2) { //AOE网络已满，有数据输入不进去。因为e <= maxSize，所以这段代码永远不会被执行
			cerr << "Network is full, " << eventName << " can not be inserted." << endl;
			return; //直接返回
		}
	}
	while (cin.get() != '\n') //防止用户在这一行有效数据后面输入无效数据，影响后面数据的读取
		continue;
	int errAgain = 0; //重新输入的时候还是有错误，则记录下来。若第三次输入有错误，则记在errInput里；若第四次输入有错误，则记在errAgain里;若第五次输入有错误，则记在errInput里，以此类推。即errInput和errAgain交替记载错误
	while (errInput != 0 || errAgain != 0) {
		if (errInput != 0) { //错误次数记在errInput里
			if (errInput == 1)
				cout << "Please input an event." << endl;
			else
				cout << "Please input " << errInput << " events in one line." << endl;
			for (i = 0; i < errInput; i++) {
				cin >> eventName;
				int status = InsertEvent(eventName); //检查插入事件结点是否成功
				if (status == 1) { //事件顶点表中已经存在这个事件
					cerr << eventName << " is already in the network, please try again later." << endl;
					errAgain++; //记录错误输入的数据个数，给一个过会儿重新输入的机会
				}
				else if (status == 2) { //AOE网络已满，有数据输入不进去。因为e <= maxSize，所以这段代码永远不会被执行
					cerr << "Network is full, " << eventName << " can not be inserted." << endl;
					return; //直接返回
				}
			}
			while (cin.get() != '\n') //防止用户在这一行有效数据后面输入无效数据，影响后面数据的读取
				continue;
			errInput = 0; //清空errInput里的数据，为下次输入做准备
		}
		else { //错误次数记在errAgain里
			if (errAgain == 1)
				cout << "Please input an event." << endl;
			else
				cout << "Please input " << errAgain << " events in one line." << endl;
			for (i = 0; i < errAgain; i++) {
				cin >> eventName;
				int status = InsertEvent(eventName); //检查插入事件结点是否成功
				if (status == 1) { //事件顶点表中已经存在这个事件
					cerr << eventName << " is already in the network, please try again later." << endl;
					errInput++; //记录错误输入的数据个数，给一个过会儿重新输入的机会
				}
				else if (status == 2) { //AOE网络已满，有数据输入不进去。因为e <= maxSize，所以这段代码永远不会被执行
					cerr << "Network is full, " << eventName << " can not be inserted." << endl;
					return; //直接返回
				}
			}
			while (cin.get() != '\n') //防止用户在这一行有效数据后面输入无效数据，影响后面数据的读取
				continue;
			errAgain = 0; //清空errInput里的数据，为下次输入做准备
		}
	}
	cout << "Please input the number of activities: ";
	while (!(cin >> a)) { //输入的不是数字
		cerr << "Please enter a number:";
		cin.clear();
		while (cin.get() != '\n')
			continue;
	}
	while (a <= 0 || a > (EventsNum * (EventsNum - 1) / 2)) {
		if (a <= 0) //将要输入的活动数小于等于0，不合理
			cerr << "Please enter a positive number:";
		else //将要输入的活动数超过AOE网络的容量，即n(n-1)/2（n是顶点个数）。计算过程如下：AOE网络不存在回路，所以任意两个结点间最多只有一个有向边，即AOE网络最多有n(n-1)/2条边。
			cerr << "Please enter a number less than or equal to " << maxSize << " :";
		while (cin.get() != '\n') //防止用户在这一行cin已成功读取的数据后面输入无效数据，影响后面数据的读取
			continue;
		while (!(cin >> a)) { //输入的不是数字
			cerr << "Please enter a number:";
			cin.clear();
			while (cin.get() != '\n')
				continue;
		}
	}
	while (cin.get() != '\n') //防止用户在这一行有效数据后面输入无效数据，影响后面数据的读取
		continue;
	cout << "Please enter one activity for each line. " << endl;
	cout << "Syntax for each line:beginName endName cost" << endl;
	for (i = 0; i < a; i++) {
		string tail, head; //弧尾名称和弧头名称
		double cost; //活动耗费的时间
		cin >> tail >> head;
		while (!(cin >> cost)) { //输入的第三个信息不是数字
			cerr << "The cost should be a number. Please input all information of the activity again." << endl;
			cin.clear();
			while (cin.get() != '\n')
				continue;
			cin >> tail >> head;
		}
		int status = InsertActivity(tail, head, cost); //检查插入事件结点是否成功
		if (status != 0) { //插入失败
			if (status == 1) //AOE网络中没有名为 tail 的事件
				cerr << "There is no " << tail << " in the network. Please input all information of the activity again." << endl;
			else if (status == 2) //AOE网络中没有名为 head 的事件
				cerr << "There is no " << head << " in the network. Please input all information of the activity again." << endl;
			else if (status == 3) //tail 和 head 是同一个结点
				cerr << "The beginning and the end of the activity is the same event. Please input all information of the activity again." << endl;
			else if (status == 4) //已经存在从 tail 到 head 的活动
				cerr << "The activity from " << tail << " to " << head << " is already existed. Please input another activity." << endl;
			else if (status == 5) //权重应该是个正数
				cerr << "The cost should be a positive number. Please input all information of the activity again." << endl;
			while (cin.get() != '\n') //防止用户在这一行有效数据后面输入无效数据，影响后面数据的读取
				continue;
			i--; //给一个过会儿重新输入的机会
		}
	}
	while (cin.get() != '\n') //防止用户在这一行有效数据后面输入无效数据，影响后面数据的读取
		continue;
#else //不检查用户输入是否正确
	int e; //记录事件顶点数量
	int a; //记录活动数量
	int i; //循环变量
	EventTable = new EventNode[EventsMaxNum]; //创建对用户不可见的事件顶点表
	cout << "Please input the number of events: ";
	cin >> e;
	cout << "Please input the names of all events in one line:" << endl;
	for (i = 0; i < e; i++) {
		string eventName; //事件顶点名称
		cin >> eventName;
		InsertEvent(eventName);
	}
	cout << "Please input the number of activities: ";
	cin >> a;
	cout << "Please enter one edge for each line. " << endl;
	cout << "Syntax for each line:tailName headName cost" << endl;
	for (i = 0; i < a; i++) {
		string tail, head; //弧尾名称和弧头名称
		double cost; //活动耗费的时间
		cin >> tail >> head >> cost;
		InsertActivity(tail, head, cost);
	}
#endif
}

AOENetwork::~AOENetwork()
{ //析构函数
	for (int i = 0; i < EventsNum; i++) { //删除各边链表中的活动结点
		ActivityNode * pointer = EventTable[i].firstout;
		while (pointer != NULL) { //循环删除
			EventTable[i].firstout = pointer->tailLink;
			delete pointer;
			pointer = EventTable[i].firstout;
		}
	}
	delete[]EventTable; //释放顶点表数组空间
}

double AOENetwork::GetCost(const string event1, const string event2)
{ //返回活动耗费的时间
	return GetCost(GetEventPos(event1), GetEventPos(event2));
}

string AOENetwork::GetFirstNeighbor(const string event)
{ //取顶点event的第一个正邻接表的邻接顶点
	int position = GetFirstNeighbor(GetEventPos(event));
	if (position != 1) //找到了邻接顶点
		return EventTable[position].name;
	else
		return NULL; //如果找不到，返回空对象
}

string AOENetwork::GetNextNeighbor(const string event1, const string event2)
{ //取顶点event1的某正邻接表邻接顶点event2的下一个邻接顶点
	int position = GetNextNeighbor(GetEventPos(event1), GetEventPos(event2));
	if (position != 1) //找到了邻接顶点
		return EventTable[position].name;
	else
		return NULL; //如果找不到，返回空对象
}

string AOENetwork::GetFirstInverseNeighbor(const string event)
{ //取顶点event的第一个逆邻接表的邻接顶点
	int position = GetFirstInverseNeighbor(GetEventPos(event));
	if (position != 1) //找到了邻接顶点
		return EventTable[position].name;
	else
		return NULL; //如果找不到，返回空对象
}

string AOENetwork::GetNextInverseNeighbor(const string event1, const string event2)
{ //取顶点event1的某逆邻接表邻接顶点event2的下一个邻接顶点
	int position = GetNextInverseNeighbor(GetEventPos(event1), GetEventPos(event2));
	if (position != 1) //找到了邻接顶点
		return EventTable[position].name;
	else
		return NULL; //如果找不到，返回空对象
}

int AOENetwork::InsertEvent(const string event)
{ //在AOE网络中插入一个新事件
	if (GetEventPos(event) != -1) //事件顶点表中已经存在这个事件
		return 1;
	if (isFull()) //AOE网络已满
		return 2;
	EventTable[EventsNum].name = event;
	EventTable[EventsNum].firstin = NULL;
	EventTable[EventsNum].firstout = NULL;
	EventsNum++;
	return 0; //插入成功
}

int AOENetwork::InsertActivity(const string event1, const string event2, const double cost)
{ //在AOE网络中插入一个新活动
	int pos1 = GetEventPos(event1); //事件 event1 在事件顶点表中的位置
	int pos2 = GetEventPos(event2); //事件 event2 在事件顶点表中的位置
	if (pos1 == -1) //AOE网络中没有名为 event1 的事件
		return 1;
	if (pos2 == -1) //AOE网络中没有名为 event2 的事件
		return 2;
	if (event1 == event2) //event1 和 event2 是同一个结点
		return 3;
	int neighbor = GetFirstNeighbor(pos1);
	while (neighbor != -1) { //事件 event1 的邻接顶点存在
		if (neighbor == pos2) //已经存在从 event1 到 event2 的活动
			return 4;
		neighbor = GetNextNeighbor(pos1, neighbor);
	}
	if (cost <= 0) //权重应该是个正数
		return 5;
	ActivityNode * activity = new ActivityNode;
	activity->tail = pos1;
	activity->head = pos2;
	activity->tailLink = NULL;
	activity->headLink = NULL;
	activity->cost = cost;
	ActivityNode * pointer = EventTable[pos1].firstout; //活动链表头指针
	if (pointer == NULL) //事件 event1 不是任何活动的开始结点
		EventTable[pos1].firstout = activity;
	else {
		while (pointer->tailLink != NULL)
			pointer = pointer->tailLink;
		pointer->tailLink = activity;
	}
	EventTable[pos1].outDegree++; //事件结点 event1 的出度加1
	pointer = EventTable[pos2].firstin; //活动链表头指针
	if (pointer == NULL) //事件 event2 不是任何活动的结束结点
		EventTable[pos2].firstin = activity;
	else {
		while (pointer->headLink != NULL)
			pointer = pointer->headLink;
		pointer->headLink = activity;
	}
	EventTable[pos2].inDegree++; //事件结点 event2 的入度加1
	ActivitiesNum++;
	return 0;
}

int AOENetwork::RemoveEvent(string event)
{ //在AOE网络中删除一个事件
	int position = GetEventPos(event);
	if (position == -1) //事件顶点表中找不到这个事件
		return 1;
	ActivityNode * pointer = EventTable[position].firstout;
	while (pointer != NULL) { //循环删除以事件顶点 evnet 为弧尾的有向边
		EventTable[position].firstout = pointer->tailLink;
		EventTable[pointer->head].inDegree--; //对应弧头的入度减1
		delete pointer;
		pointer = EventTable[position].firstout;
	}
	pointer = EventTable[position].firstin;
	while (pointer != NULL) { //循环删除以事件顶点 evnet 为弧头的有向边
		EventTable[position].firstin = pointer->headLink;
		EventTable[pointer->tail].outDegree--; //对应弧尾的出度减1
		delete pointer;
		pointer = EventTable[position].firstin;
	}
	for (int i = position; i < EventsNum - 1; i++) { //在事件顶点表中删除事件 event
		EventTable[i].name = EventTable[i + 1].name;
		EventTable[i].firstout = EventTable[i + 1].firstout;
		EventTable[i].firstin = EventTable[i + 1].firstin;
	}
	EventsNum--;
	return 0;
}

int AOENetwork::RemoveActivity(string event1, string event2)
{ //在AOE网络中删除一个活动
	int pos1 = GetEventPos(event1); //事件 event1 在事件顶点表中的位置
	int pos2 = GetEventPos(event2); //事件 event2 在事件顶点表中的位置
	if (pos1 == -1) //AOE网络中没有名为 event1 的事件
		return 1;
	if (pos2 == -1) //AOE网络中没有名为 event2 的事件
		return 2;
	if (event1 == event2) //event1 和 event2是同一个结点
		return 3;
	ActivityNode * pointer = EventTable[pos1].firstout;
	while (pointer != NULL) {
		if (pointer->head == pos2) { //找到了这个活动
			delete pointer;
			return 0; //成功返回
		}
		else
			pointer = pointer->tailLink;
	}
	ActivitiesNum--;
	return 4; //这个活动不存在
}

int AOENetwork::DFS(const string event)
{ //从事件顶点event出发进行深度优先搜索，并将结果打印在屏幕上
	int position = GetEventPos(event);
	if (position == -1) //事件顶点表中找不到这个事件结点
		return 1;
	int * visited = new int[EventsNum](); //创建辅助数组，并将所有元素初始化为0，意思是所有顶点都还没访问过
	DFSassistant(position, visited);
	delete[] visited;
	return 0;
}

void AOENetwork::DFSassistant(int position, int visited[])
{ //深度优先搜索的辅助函数，从位置为position的事件顶点出发，按深度优先查找次序访问所有尚未访问过的顶点
	cout << EventTable[position].name; //输出访问顶点信息
	visited[position] = 1; //该顶点访问标志修改为已访问过
	int adjacent = GetFirstNeighbor(position); //寻找刚刚访问过顶点的第一个邻接顶点
	while (adjacent != -1) { //若邻接顶点存在
		if (!visited[adjacent]) //若该邻接顶点未被访问过
			DFSassistant(adjacent, visited); //则从位于adjacent的邻接顶点开始进行递归访问
		adjacent = GetNextNeighbor(position, adjacent); //寻找位于position的顶点的下一个邻接顶点
	}
}

int AOENetwork::BFS(const string event)
{ //从事件顶点event出发进行广度优先搜索，并将结果打印在屏幕上
	int position = GetEventPos(event);
	if (position == -1) //事件顶点表中找不到这个事件结点
		return 1;
	int * visited = new int[EventsNum](); //创建辅助数组，并将所有元素初始化为0，意思是所有顶点都还没访问过
	cout << event; //输出访问顶点信息
	visited[position] = 1; //该顶点访问标志修改为已访问过
	queue<int> q; //实现分层访问的辅助存储结构——队列
	q.push(position); //顶点进队列
	while (!q.empty()) {
		position = q.front();
		q.pop(); //从队列中退出位于position的顶点
		int adjacent = GetFirstNeighbor(position); //寻找位于position的顶点的第一个邻接顶点
		while (adjacent != -1) { //邻接顶点存在
			if (!visited[adjacent]) { //邻接顶点未被访问过
				cout << EventTable[adjacent].name << endl; //访问邻接顶点
				visited[adjacent] = 1; //更改邻接顶点的访问标记
				q.push(adjacent); //邻接顶点进队列
			}
			adjacent = GetNextNeighbor(position, adjacent); //寻找位于position的顶点的下一个邻接顶点
		}
	}
	delete[] visited;
	return 0;
}

int AOENetwork::TopologicalOrder()
{ //拓扑排序，并在屏幕上打印出一个拓扑排序的结果
	stack<int> s; //存放入度为0的顶点
	int i; //循环变量
	int * indegree = new int[EventsNum];
	for (i = 0; i < EventsNum; i++)
		indegree[i] = EventTable[i].inDegree; //拷贝一份入度数组，拓扑排序时用
	for (i = 0; i < EventsNum; i++) {
		if (EventTable[i].inDegree == 0) //入度为0的事件顶点依次进栈
			s.push(i);
	}
	for (i = 0; i < EventsNum; i++) {
		if (s.empty()) { //存在有向环，未能形成拓扑排序，直接返回
			cerr << "Network has a cycle!" << endl;
			delete[] indegree;
			return 1;
		}
		else {
			int position = s.top();
			s.pop(); //入度为零的事件顶点出栈
			cout << EventTable[position].name << endl; //输出入度为0的事件顶点信息
			ActivityNode * link = EventTable[position].firstout; //位于position的事件顶点的边链表的头指针
			while (link) { // link不等于NULL，存在出边
				if (--indegree[link->head] == 0) //入度减1为零的顶点进栈
					s.push(link->head);
				link = link->tailLink; //取位于position的事件顶点的下一条边出边
			}
		}
	}
	delete[] indegree;
	return 0;
}

int AOENetwork::examine(bool printError)
{ //检查AOE网络是否成立，成立返回0，否则返回非零值。如果printError为真，则在屏幕上显示错误原因
	int i; //循环变量
	if (isEmpty()) { //AOE网络为空
		if (printError)
			cerr << "The network is empty." << endl;
		return 1;
	}
	for (i = 0; i < EventsNum; i++) {
		if (EventTable[i].firstout == NULL && EventTable[i].firstin == NULL) { //存在没有用到的事件结点
			if (printError)
				cerr << EventTable[i].name << " is not used." << endl;
			return 2;
		}
	}
	int zeroOutDegree = 0; //统计出度为0的事件结点个数
	int zeroInDegree = 0; //统计入度为0的事件结点个数
	for (i = 0; i < EventsNum; i++) {
		if (EventTable[i].outDegree == 0)
			zeroOutDegree++;
		if (EventTable[i].inDegree == 0)
			zeroInDegree++;
	}
	if (zeroInDegree == 0) { //所有结点都是某条有向边的弧头，AOE网络没有开始结点
		if (printError)
			cerr << "There is no beginning event." << endl;
		return 3;
	}
	else if (zeroInDegree > 1) { //入度为0的事件结点不止一个，AOE网络有多个开始结点
		if (printError)
			cerr << "There are more than one beginning events." << endl;
		return 4;
	}
	if (zeroOutDegree == 0) { //所有结点都是某条有向边的弧尾，AOE网络没有结束结点
		if (printError)
			cerr << "There is no terminal event." << endl;
		return 5;
	}
	else if (zeroOutDegree > 1) {//入度为0的事件结点不止一个，AOE网络有多个开始结点
		if (printError)
			cerr << "There are more than one terminal events." << endl;
		return 6;
	}
	int beginPos; //开始结点在事件顶点表中的位置
	for (beginPos = 0; beginPos < EventsNum; beginPos++) {
		if (EventTable[beginPos].firstin == NULL) //找到AOE网络的开始结点
			break;
	}
	//下面的代码用深度优先搜索检查AOE网络是否联通
	int position = beginPos;
	int * visited = new int[EventsNum](); //创建辅助数组，并将所有元素初始化为0，意思是所有顶点都还没访问过
	visited[position] = 1; //该顶点访问标志修改为已访问过
	queue<int> q; //实现分层访问的辅助存储结构——队列
	q.push(position); //顶点进队列
	while (!q.empty()) {
		position = q.front();
		q.pop(); //从队列中退出位于position的顶点
		int adjacent = GetFirstNeighbor(position); //寻找位于position的顶点的第一个邻接顶点
		while (adjacent != -1) { //邻接顶点存在
			if (!visited[adjacent]) { //邻接顶点未被访问过
				visited[adjacent] = 1; //更改邻接顶点的访问标记
				q.push(adjacent); //邻接顶点进队列
			}
			adjacent = GetNextNeighbor(position, adjacent); //寻找位于position的顶点的下一个邻接顶点
		}
	}
	int sum = 0; //统计有多少顶点被访问过，即计算开始结点所在的最大联通分量有几个顶点
	for (i = 0; i < EventsNum; i++)
		sum += visited[i];
	delete[] visited;
	if (sum != EventsNum) { //开始结点所在的最大联通分量的顶点数小于AOE网络的顶点数，说明AOE网络不联通
		if (printError)
			cerr << "The network is not connected." << endl;
		return 7;
	}
	//下面的代码用拓扑排序检查AOE网络是否存在有向环
	stack<int> s; //存放入度为0的顶点
	int * indegree = new int[EventsNum];
	for (i = 0; i < EventsNum; i++)
		indegree[i] = EventTable[i].inDegree; //拷贝一份入度数组，拓扑排序时用
	for (i = 0; i < EventsNum; i++) {
		if (EventTable[i].inDegree == 0) //入度为0的事件顶点依次进栈
			s.push(i);
	}
	for (i = 0; i < EventsNum; i++) {
		if (s.empty()) { //存在有向环，未能形成拓扑排序，直接返回
			delete[] indegree;
			if (printError)
				cerr << "The network has a cycle." << endl;
			return 8;
		}
		else {
			int position = s.top();
			s.pop(); //入度为零的事件顶点出栈
			ActivityNode * link = EventTable[position].firstout; //位于position的事件顶点的边链表的头指针
			while (link) { // link不等于NULL，存在出边
				if (--indegree[link->head] == 0) //入度减1为零的顶点进栈
					s.push(link->head);
				link = link->tailLink; //取位于position的事件顶点的下一条边出边
			}
		}
	}
	delete[] indegree;
	return 0;
}

int AOENetwork::criticalAvitities()
{ //求解出所有关键活动并打印在屏幕上，成功返回0，否则在屏幕上打印错误返回错误代码
	int exam = examine(true); //检查AOE网络是否成立，如果不成立，在屏幕上打印错误
	if (exam != 0) //AOE网络不成立
		return exam; //返回错误代码
	int begin = 0, end = 0; //开始结点和结束结点的位置
	int i; //循环变量
	for (i = 0; i < EventsNum; i++) {
		if (EventTable[i].inDegree == 0) //找到开始结点
			begin = i;
		if (EventTable[i].outDegree == 0) //找到结束结点
			end = i;
	}
	//第一步，利用拓扑排序求解各事件顶点的最早发生时间和关键路径长度（即完成整个工程的最短时间）
	EventTable[begin].earliest = 0; //递推公式第一步
	stack<int> s; //存放入度为0的顶点
	int * indegree = new int[EventsNum];
	for (i = 0; i < EventsNum; i++)
		indegree[i] = EventTable[i].inDegree; //拷贝一份入度数组，拓扑排序时用
	for (i = 0; i < EventsNum; i++) {
		if (EventTable[i].inDegree == 0) //入度为0的事件顶点依次进栈
			s.push(i);
	}
	for (i = 0; i < EventsNum; i++) {
		if (s.empty()) { //存在有向环，未能形成拓扑排序，直接返回
			delete[] indegree;
			return 8;
		}
		else {
			int position = s.top();
			s.pop(); //入度为0的事件顶点出栈
			ActivityNode * link = EventTable[position].firstout; //位于position的事件顶点的边链表的头指针
			while (link) { // link不等于NULL，存在出边
				if (--indegree[link->head] == 0) //入度减1为零的顶点进栈
					s.push(link->head);
				if (EventTable[link->head].earliest < EventTable[position].earliest + GetCost(position, link->head))
					EventTable[link->head].earliest = EventTable[position].earliest + GetCost(position, link->head);
				link = link->tailLink; //取位于position的事件顶点的下一条边出边
			}
		}
	}
	delete[] indegree;
	double costOfNetwork = EventTable[end].earliest; //关键路径长度（即完成整个工程的最短时间）
													 //第二步，利用逆拓扑排序和逆邻接表求解各事件顶点允许的最晚发生时间
	while (!s.empty()) //清空栈s，以防前面的程序错误影响接下来的计算。
		s.pop();
	EventTable[end].latest = costOfNetwork; //递推公式第一步
	for (i = 0; i < EventsNum; i++) //为接下来求各事件顶点允许的最晚发生时间的最小值做初始化准备
		EventTable[i].latest = costOfNetwork;
	int * outdegree = new int[EventsNum];
	for (i = 0; i < EventsNum; i++)
		outdegree[i] = EventTable[i].outDegree; //拷贝一份出度数组，逆拓扑排序时用
	for (i = 0; i < EventsNum; i++) {
		if (EventTable[i].outDegree == 0) //出度为0的事件顶点依次进栈
			s.push(i);
	}
	for (i = 0; i < EventsNum; i++) {
		if (s.empty()) { //存在有向环，未能形成拓扑排序，直接返回
			delete[] outdegree;
			return 8;
		}
		else {
			int position = s.top();
			s.pop(); //出度为0的事件顶点出栈
			ActivityNode * link = EventTable[position].firstin; //位于position的事件顶点的边链表的头指针
			while (link) { // link不等于NULL，存在入边
				if (--outdegree[link->tail] == 0) //出度减1为零的顶点进栈
					s.push(link->tail);
				if (EventTable[link->tail].latest > EventTable[position].latest - GetCost(link->tail, position))
					EventTable[link->tail].latest = EventTable[position].latest - GetCost(link->tail, position);
				link = link->headLink; //取位于position的事件顶点的下一条边入边
			}
		}
	}
	delete[] outdegree;
	for (i = 0; i < EventsNum; i++) { //第三步，计算各活动结点的最早开始时间,允许的最晚开始时间，顺便计算该活动的最大可利用时间，即可拖延的时间
		ActivityNode * neighbor = EventTable[i].firstout; //获取事件顶点i的第一个邻接顶点
		while (neighbor) { //邻接顶点存在
			neighbor->earliest = EventTable[i].earliest; //各活动结点的最早开始时间
			neighbor->latest = EventTable[neighbor->head].latest - neighbor->cost; //各活动结点允许的最晚开始时间
			neighbor->duration = neighbor->latest - neighbor->earliest; //该活动的最大可利用时间，即可拖延的时间
			neighbor = neighbor->tailLink; //取事件顶点i的下一个邻接顶点
		}
	}
	cout << endl; //最后一步，在屏幕上输出结果
	cout << "Cost of the network: " << costOfNetwork << endl;
	cout << "All critical activities:" << endl;
	cout << left << setw(WIDTH) << "begin" << setw(WIDTH) << "end" << setw(WIDTH) << "cost" << endl;
	for (i = 0; i < EventsNum; i++) {
		ActivityNode * neighbor = EventTable[i].firstout; //获取事件顶点i的第一个邻接顶点
		while (neighbor) { //邻接顶点存在
			if (neighbor->duration < PRECISION) //如果时间间隔小于设置精度，即可认为时间间隔为0，这个活动时关键活动
				cout << left << setw(WIDTH) << EventTable[i].name << setw(WIDTH) << EventTable[neighbor->head].name << setw(WIDTH) << neighbor->cost << endl;
			neighbor = neighbor->tailLink; //取事件顶点i的下一个邻接顶点
		}
	}
	return 0;
}
