//
//  AOENetwork.hpp
//  CriticalPathMethod
//
//  AOE 网络（Activity on Edges - Network）本质是一个特殊的带权有向图，这个有向图有且只有一个顶点没有前继顶点，有且只有一个顶点没有后继顶点
//  因为求解事件 vi 能够发生的最早时间 ee[i] 需要用到邻接表，求解事件 vi 允许的最晚时间 le[i] 需要用到逆邻接表，所以干脆使用十字链表法表示 AOE 网络
//  不建议用户直接操作活动结点类和事件结点类。
//
//  Created by Nick's Macbook Pro on 2017/6/5.
//  Copyright © 2017年 HaiTianYang. All rights reserved.
//

#ifndef AOENetwork_hpp
#define AOENetwork_hpp

#include <iostream>
#include <string>
#define PRECISION 1e-5 //时间间隔的精度
#define WIDTH 8 //输出各字符串的宽度，便于打印结果时对齐
#define EXAMINEINPUT //如果定义了EXAMINEINPUT，那么构造函数AOENetwork(int maxSize)将会检查用户输入是否有效
using namespace std;

class AOENetwork; //AOE网络类的前向引用声明

class ActivityNode { //活动结点的类定义，活动的在AOE网络是带权有向边，不建议用户直接操作
	friend class AOENetwork;
private: //向用户隐藏活动类的细节
	int tail; //弧尾的位置
	int head; //弧头的位置
	ActivityNode * tailLink = NULL; //弧尾相同的下一个弧结点
	ActivityNode * headLink = NULL; //弧头相同的下一个弧结点
	double cost = 0; //活动需要耗费的时间
	double earliest = -1; //活动的最早发生时间，-1表示还没计算过
	double latest = -1; //活动允许的最晚发生时间，-1表示还没计算过
	double duration = -1; //活动的最大可利用时间（时间余量）
};

class EventNode { //事件结点的类定义，事件在AOE网络中是顶点，不建议用户直接操作
	friend class ActivityNode;
	friend class AOENetwork;
private: //向用户隐藏事件类的细节
	string name; //事件名字
	ActivityNode * firstout = NULL; //以该顶点为弧尾的第一个弧结点
	ActivityNode * firstin = NULL; //以该顶点为弧头的第一个弧结点
	int inDegree = 0; //入度
	int outDegree = 0; //出度
	double earliest = -1; //事件的最早发生时间，-1表示还没计算过
	double latest = -1; //事件允许的最晚发生时间，-1表示还没计算过
};

class AOENetwork {
	friend class ActivityNode;
	friend class EventNode;
private:
	EventNode * EventTable; //事件顶点表，用于存放所有事件，这个数组对用户隐藏，即类的用户不知道某一事件在事件顶点表里的具体位置
	int EventsNum = 0; //当前事件数
	int EventsMaxNum; //最大事件数
	int ActivitiesNum; //当前活动数
	int GetEventPos(const string &event); //当前事件顶点 event 在 AOE 网络中的位置
	double GetCost(const int pos1, const int pos2); //返回活动耗费的时间
	int GetFirstNeighbor(const int pos); //取顶点pos(用它在事件顶点表中的位置表示，下同)的第一个正邻接表的邻接顶点
	int GetNextNeighbor(const int pos1, const int pos2); //取顶点pos1的某正邻接表邻接顶点pos2的下一个邻接顶点
	int GetFirstInverseNeighbor(const int pos); //取顶点pos的第一个逆邻接表的邻接顶点
	int GetNextInverseNeighbor(const int pos1, const int pos2); //取顶点pos1的某逆邻接表邻接顶点pos2的下一个邻接顶点
	void DFSassistant(int position, int visited[]); //深度优先搜索的辅助函数
public:
	AOENetwork(int maxSize); //构造函数，maxSize是最大事件数量
	~AOENetwork(); //析构函数
	bool isEmpty() const { return EventsNum == 0; } //判断AOE网络是否为空
	bool isFull() const { return EventsNum == EventsMaxNum; } //判断AOE网络是否已满
	int EventsNumber() { return EventsNum; } //返回AOE网络的事件数
	int ActivitiesNumber() { return ActivitiesNum; } //返回AOE网络的活动数
	double GetCost(const string event1, const string event2); //返回活动耗费的时间
	string GetFirstNeighbor(const string event); //取顶点event的第一个正邻接表的邻接顶点
	string GetNextNeighbor(const string event1, const string event2); //取顶点event1的某正邻接表邻接顶点event2的下一个邻接顶点
	string GetFirstInverseNeighbor(const string event); //取顶点event的第一个逆邻接表的邻接顶点
	string GetNextInverseNeighbor(const string event1, const string event2); //取顶点event1的某逆邻接表邻接顶点event2的下一个邻接顶点
	int InsertEvent(const string event); //在AOE网络中插入一个新事件
	int InsertActivity(const string event1, const string event2, const double cost); //在AOE网络中插入一个新活动
	int RemoveEvent(string event); //在AOE网络中删除一个事件
	int RemoveActivity(string event1, string event2); //在AOE网络中删除一个活动
	int DFS(const string event); //从事件顶点event出发进行深度优先搜索，并将结果打印在屏幕上
	int BFS(const string event); //从事件顶点event出发进行广度优先搜索，并将结果打印在屏幕上
	int TopologicalOrder(); //拓扑排序，并在屏幕上打印出一个拓扑排序的结果
	int examine(bool printError); //检查AOE网络是否成立，成立返回0，否则返回非零值。如果printError为真，则在屏幕上显示错误原因
	int criticalAvitities(); //求解出所有关键活动并打印在屏幕上，成功返回0，否则在屏幕上打印错误返回错误代码
};

#endif /* AOENetwork_hpp */

