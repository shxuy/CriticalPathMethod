//
//  main.cpp
//  CriticalPathMethod
//
//  Created by Nick's Macbook Pro on 2017/6/4.
//  Copyright © 2017年 HaiTianYang. All rights reserved.
//

#include <iostream>
#include "AOENetwork.hpp"
#define MAXEXENTS 100 //最大事件数

int main(void) {
	cout << "The program can help you find all criticlal path in an Acitivity on Edges-Network." << endl;
	cout << endl;
	AOENetwork process(MAXEXENTS); //建立一个AOE网络，要求用户输入事件数，各事件名称，活动数，各活动的开始事件，结束事件和耗费时间
	process.criticalAvitities(); //先检查AOE网络是否成立，如果不成立在屏幕上打印错误原因；如果成立，在屏幕上打印所有关键路径
	return 0;
}
