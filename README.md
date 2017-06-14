# CriticalPathMethod
This repository is used to learn how to use github for me.

本程序用来求解关键路径。关键路径的原理可以参见国内任何一本《数据结构》教材有关图的那一章，这里不再赘述。本程序的代码主要改编自复旦大学出版社出版，施伯乐主编的《数据结构教程》里的内容。

**输入**  
第一行输入正整数 events（0 < events <= 100）,表示有 events 个事件，即AOE网络有 events 个顶点。  
第二行输入各个事件的名字，依次命名为1, 2, 3, 4, 5......（其实也可以命名为a, b, c, d.... 或者begin, event1, event2......）  
第三行输入正整数 activities（0 < activities <= 9800），表示有 activities 个活动，即AOE网络有 activities 条边。  
后面的 activities 行输入多组输入数据，每组输入的数据都是tail, head, weight，分别代表一条有向边的弧尾的名字，弧头的名字和权重。数据间有且只有一个空格。

**输出**  
第一行输出关键路径的长度
后面几行输出所有关键活动，每一行输出一个关键活动的弧尾的编号，弧头的编号和权重。数字与数字间有且只有一个空格。

**样例输入**  
9  
1 2 3 4 5 6 7 8 9  
13  
1 2 5  
1 3 7  
2 4 3  
3 4 6  
3 5 3  
4 5 4  
4 6 4  
4 7 4  
5 7 2  
5 8 5  
6 9 4  
7 9 5  
8 9 2  

**样例输出**  
Cost of the network: 24  
All critical activities:  
begin   end     cost      
1       3       7         
3       4       6         
4       5       4         
5       7       2         
5       8       5         
7       9       5         
8       9       2  

