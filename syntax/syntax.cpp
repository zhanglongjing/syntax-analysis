//LR1语法分析
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include <algorithm>
#define MAX_Count 100
#include <set>
#include <stack>
#include <iomanip>
#include <sstream>
#include <string>
#include<cstring>
#include <map>
using namespace std;

template <typename CFG_LR1> //定义模板

// 重载一下+号运算符，v1+v2=将v2插入到v1的后面
vector<CFG_LR1>& operator +(vector<CFG_LR1>& v1, vector<CFG_LR1>& v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
	return v1;
}

//非终结符集
struct VN_Set {
	string VN_name;
	set<string> FIRST; //First集合 
	set<string> FOLLOW; //follow集
};
//产生式
struct CFG {
	string start;//非终结符
	vector<string> next; //First集合 
};
//LR1项目集
 struct CFG_LR1 {
	 string start;//非终结符
	 vector<string> next; //First集合 
	 int num; //数目
	 vector<string> tail;//预测符
	 bool operator==(const CFG_LR1& rhs) //重载==，判断两个项目集是否一样
	 {
		 return (start == rhs.start && next == rhs.next &&
			 num == rhs.num && tail == rhs.tail);
	 }
 };

 int CFGCount = 0; //记录文法句数
 CFG cfg[MAX_Count];//产生式 
 VN_Set VN_First[MAX_Count];//非终结符集的First集合 
 set<string> VN;// 非终结符集合 
 set<string> VT;//终结符集合 
 int I_count = 0;//记录LR1项目数 
 vector<CFG_LR1> I[MAX_Count]; //项目集容器 
 map<string, int> mark_Follow;//用于标记Follow 
 map<string, int> GOTO[MAX_Count]; //GOTO集
 map<string, string> ACTION[MAX_Count]; //ACTION集


 bool cmp_vector(vector<CFG_LR1>& v1, vector<CFG_LR1>& v2)
 {
	 if (v1.size() != v2.size()) return false;
	 for (int i = 0; i < v2.size(); i++)
	 {
		 CFG_LR1 t;
		 t = v2[i];
		 vector<CFG_LR1>::iterator result = find(v1.begin(), v1.end(), t); //查找3
		 if (result == v1.end()) //没找到
			 return false;
	 }
	 return true;
 }

 //求FIRST集合 
 set<string> get_FIRST(string a) 
 { 
	 set<string> T;
	 for (int i = 0; i < CFGCount; i++) 
	 {
		 if (cfg[i].start == a) {  // a->.. 
			 for (int j = 0; j < cfg[i].next.size(); j++) 
			 {
				 if (VT.find(cfg[i].next[j]) != VT.end())
				 { //是终结符开头 
					 T.insert(cfg[i].next[j]);
					 //    T.erase("$");
					 break;
				 }
				 else
				 {
					 if (cfg[i].next[j] == cfg[i].start) 
					 {
						 break;
					 }
					 set<string> U = get_FIRST(cfg[i].next[j]);
					 T.insert(U.begin(), U.end());
					 if (U.find("$") != U.end()) { //U中含有$,继续查下个的first
						 if (j != cfg[i].next.size() - 1)
							 T.erase("$");
					 }
					 else 
					 {
						 break;
					 }
				 } 
			 }
		 }
	 }
	 return T;
 }

 //求FOLLOW
 set<string> get_FOLLOW(string a) 
 {
	 set<string> T;
	 //cout<<"现在在求"<<a<<" 的Follow"<<endl; 
	 mark_Follow[a]++;
	 if (mark_Follow[a] >= 2) {
		 return T;
	 }
	 set<string> temp;
	 if (a == cfg[0].start) {
		 T.insert("$");
	 }
	 for (int i = 0; i < CFGCount; i++) {
		 for (int j = 0; j < cfg[i].next.size(); j++) {
			 if (VT.find(cfg[i].next[j]) == VT.end() && a == cfg[i].next[j]) { //是非终结符，求FOLLOW集合 
				 if (j == cfg[i].next.size() - 1 && a != cfg[i].start) {//S->...a 
					 set<string> tt = get_FOLLOW(cfg[i].start);
					 T.insert(tt.begin(), tt.end());
				 }
				 for (int k = j + 1; k < cfg[i].next.size(); k++) {
					 if (VT.find(cfg[i].next[k]) != VT.end()) {//后面一个是终结符  S->..av.. 
						 T.insert(cfg[i].next[k]);
						 break;
					 }
					 else {
						 temp = get_FIRST(cfg[i].next[k]);
						 if (temp.find("$") != temp.end()) {//有$ S->..a B.. 
							 T.insert(temp.begin(), temp.end());
							 T.erase("$");
							 if (k == cfg[i].next.size() - 1) { //S->..a B
								 set<string> tt = get_FOLLOW(cfg[i].start);
								 T.insert(tt.begin(), tt.end());
								 break;
							 }
						 }
						 else {
							 T.insert(temp.begin(), temp.end());
							 break;
						 }
					 }
				 }
			 }
		 }
	 }//cout<<a<<" "<<mark_Follow[a]<<endl;
	 mark_Follow[a] = 0;
	 return T;
 }

 //输出first,follow集
 void Output_First_and_Follow() 
 {
	 set<string>::iterator it;
	 int count = 0;
	 cout << "*******************************************" << endl;
	 cout << "非终结符" <<"     " << "FIRST集" << "     " << "FOLLOW集" << endl;
	 for (it = VN.begin(); it != VN.end(); it++)
	 {

		 VN_First[count].VN_name = *it;
		 VN_First[count].FIRST = get_FIRST(*it);

		 mark_Follow[*it] = 0;
		 VN_First[count].FOLLOW = get_FOLLOW(*it);

		 //-----------输出FIRST-------------------- 
		 cout << setw(4) << VN_First[count].VN_name<< '\t'<<"       ";
		 set<string>::iterator it;
		 for (it = VN_First[count].FIRST.begin(); it != VN_First[count].FIRST.end(); it++) {
			 cout << *it<< " ";
		 }
		 cout << '\t' << "   ";
		 //------------输出FOLLOW--------------
		 set<string>::iterator it1;
		 for (it1 = VN_First[count].FOLLOW.begin(); it1 != VN_First[count].FOLLOW.end(); it1++) {
			 cout << *it1 << " ";
		 }cout << endl;
		 count++;
	 }cout << "*******************************************";
	 cout << endl << endl;
 }

 bool find_in_vector(vector<CFG_LR1> T, CFG_LR1 p) {
	 vector<CFG_LR1>::iterator it;
	 for (it = T.begin(); it != T.end(); it++) {
		 if (*it == p) {
			 return true;
		 }
	 }
	 return false;
 }

 //求闭包 
 vector<CFG_LR1> CLOSURE(CFG_LR1 I) {
	 vector<CFG_LR1> T;
	 //T.push_back(I);
	 if (I.num >= I.next.size()) { //归约项目A->α.或者接受项目 
		 return T;
	 }
	 else 
	 {
		 string temp = I.next[I.num];
		 if (VT.find(temp) != VT.end()) 
		 {     //点后面的是终结符 ,移进项目 A→α.aβ  
			 return T;
		 }
		 else 
		 {  //待约项目 
			 for (int i = 0; i < CFGCount; i++) 
			 {
				 if (cfg[i].start == temp) 
				 {
					 CFG_LR1 p;
					 p.start = cfg[i].start;
					 p.num = 0;//点在最前面 
					 p.next = cfg[i].next;
					 set<string> f1;
					 for (int j = I.num + 1; j < I.next.size(); j++)
					 {
						 set<string> f2;//用于暂存first 
						 if (VT.find(I.next[j]) != VT.end())
						 {
							 f2.insert(I.next[j]);
						 }
						 else 
						 {
							 f2 = get_FIRST(I.next[j]);
						 }
						 f1.insert(f2.begin(), f2.end());
						 if (f2.find("$") == f2.end()) 
						 {
							 break;
						 }
					 }
					 if (f1.size() == 0) 
					 {
						 p.tail = I.tail;
					 }
					 else 
					 {
						 vector<string> first_tail;
						 if (f1.find("$") != f1.end())
						 {
							 f1.erase("$");
							 copy(f1.begin(), f1.end(), back_inserter(first_tail));
							 first_tail.insert(first_tail.end(), I.tail.begin(), I.tail.end());
						 }
						 else 
						 {
							 copy(f1.begin(), f1.end(), back_inserter(first_tail));
						 }
						 p.tail = first_tail;
					 }
					 if (!find_in_vector(T, p)) 
					 {
						 T.push_back(p);
						 vector<CFG_LR1> ol = CLOSURE(p);
						 vector<CFG_LR1>::iterator z;
						 for (z = ol.begin(); z != ol.end(); z++) {
							 if (!find_in_vector(T, *z)) 
							 {
								 T.push_back(*z);
							 }
						 }
					 }
				 }
			 }
		 }
	 }
	 return T;
 }

 //展示项目集
 void printI(vector<CFG_LR1> I) 
 { 
	 vector<CFG_LR1>::iterator it;
	 for (it = I.begin(); it != I.end(); it++)
	 {
		 CFG_LR1 p = *it;
		 cout << setw(5) << p.start << "->";
		 vector<string>::iterator s;
		 for (int j = 0; j < p.next.size(); j++) 
		 {
			 if (j == p.num) cout << ".";
			 cout << p.next[j];
		 }if (p.num == p.next.size())cout << ".";
		 cout << ",";
		 for (int k = 0; k < p.tail.size(); k++) 
		 {
			 if (k >= 1)
			 {
				 cout <<"|" << p.tail[k];
			 }
			 else { cout << p.tail[k]; }
		 }
		 cout << endl;
	 }
 }

 //LR1分析表构造
 void LR1_Analyse() 
 {
	 CFG_LR1 p;
	 //初始项目 S’->.S ,# 
	 p.start = cfg[0].start + "^";
	 p.num = 0;//点在最前面 
	 p.tail.push_back("$");
	 p.next.push_back(cfg[0].start);
	 I[0] = CLOSURE(p);//求闭包后的I[0]
	 I[0].insert(I[0].begin(), p);//求闭包遇到了一些问题 
	 I_count = 1;
	 //计算项目集 
	 for (int i = 0; i < I_count; i++)
	 {//每个项目集  项目集I(i) 
		 cout << "-------------------------------" << endl;
		 cout << "计算项目集I_" << i << endl;
		 printI(I[i]);//展示项目集 
		 //---------求ACTION的r部分-------------- 
		 vector<CFG_LR1>::iterator t;
		 for (t = I[i].begin(); t != I[i].end(); t++) 
		 {
			 CFG_LR1 t2 = *t;
			 if (t2.num == t2.next.size()) 
			 {
				 int num = 0;
				 for (int xp = 0; xp < CFGCount; xp++) 
				 {
					 if (cfg[xp].start == t2.start && cfg[xp].next == t2.next) 
					 {
						 num = xp;
						 break;
					 }
				 }
				 std::stringstream ss;
				 ss << num;
				 string s = ss.str();
				 for (int q = 0; q < t2.tail.size(); q++) 
				 {
					 ACTION[i][t2.tail[q]] = "r" + s;
				 }
				 if (t2.num == 1 && t2.next[0] == cfg[0].start) 
				 {
					 ACTION[i]["$"] = "acc";
				 }
			 }
		 }
		 //-----对项目集进行变化，分别为输入非终结符和终结符
		 //每个非终结符
		 set<string>::iterator it;
		 for (it = VN.begin(); it != VN.end(); it++) 
		 {  
			 int num = 0;//用于判断不能输入的终结符
			 cout << "-------------------------------" << endl;
			 cout<<"项目集I_"<<i<<"输入"<<*it<<",";
			 vector<CFG_LR1> temp;
			 //cout<<"项目集大小"<<I[i].size()<<endl;
			 for (int j = 0; j < I[i].size(); j++) {
				 CFG_LR1 lr = I[i][j];
				 if (lr.num < lr.next.size() && lr.next[lr.num] == *it) 
				 {
					 //cout<<*it<<endl; 
					 vector<CFG_LR1> t2;
					 lr.num++;
					 t2 = CLOSURE(lr);
					 t2.push_back(lr);
					 temp = temp + t2;
				 }
				 else { num++; }
			 }
			 if (num == I[i].end()-I[i].begin()) 
			 { 
				 cout << "无法输入" << *it << endl;
				 cout << "-------------------------------" << endl<<endl;
			 }
			 //cout<<"temp.size"<< temp.size()<<endl;
			 if (temp.size() > 0) 
			 {
				 int k;
				 for (k = 0; k < I_count; k++) 
				 {//找一找项目集是否已经存在 
					 if (cmp_vector(I[k], temp)) 
					 {
						 break;
					 }
				 }
				 if (k == I_count) 
				 {
					 //产生了新的项目集 
					 I[I_count] = temp;
                     cout<<"得到新的项目集I_"<<I_count<<endl;
					 cout<<"*******************************" << endl << "I_" <<I_count<<": "<<endl;
                     printI(I[I_count]);//展示项目集
					 cout <<"关系：" << "  I_" << i << " -- " << *it << "->" << "I_" << I_count << endl;
					 cout<<"-------------------------------"<<endl<<endl;
					 GOTO[i][*it] = I_count;//更新goto表 
					 I_count++;
				 }
				 else 
				 {
					 //项目集已经存在，需要自己指向自己
					 cout << "  I_" << i << " -- " << *it << "->" << "I_" << k << endl << endl;
					 GOTO[i][*it] = k;
				 }
			 }
		 }
		 //对每个终结符
		 for (it = VT.begin(); it != VT.end(); it++) 
		 {
			 int num = 0;//用于判断不能输入的终结符
			 cout << "-------------------------------" << endl;
			 cout<<"项目集I_"<<i<<"输入"<<*it<<",";
			 vector<CFG_LR1> temp;
			 for (int j = 0; j < I[i].size(); j++) 
			 {
				 CFG_LR1 lr = I[i][j];
				 if(lr.num < lr.next.size() && lr.next[lr.num] == *it) 
				 {
					 vector<CFG_LR1> t2;
					 lr.num++;
					 t2 = CLOSURE(lr);//闭包求出的结果不包含本身 
					 t2.insert(t2.begin(), lr);//求闭包遇到了一些问题
					 //cout << "闭包：" << endl;
					 //showI(t2);
					 temp = temp + t2;
				 }
				 else
				 {
					 num++;
				 }
			 }
			 if (num == I[i].end() - I[i].begin())
			 {
				 cout << "无法输入" << *it << endl;
				 cout << "-------------------------------" << endl << endl;
			 }
			 //cout<<"temp.size"<< temp.size()<<endl;
			 if (temp.size() > 0) 
			 {
				 int k;
				 for (k = 0; k < I_count; k++) 
				 {//找一找项目集是否已经存在 
					 if (cmp_vector(I[k], temp)) 
					 {
						 break;
					 }
				 }
				 if (k == I_count) 
				 {
					 //产生了新的项目集 
					 I[I_count] = temp;
                     cout<<"得到新的项目集I"<<I_count<<endl;
					 cout << "*******************************" << endl << "I_" << I_count << ": "<<endl;
                     printI(I[I_count]);//展示项目集
					 cout <<"关系：" << "  I_" << i << " -- " << *it << "->" << "I_" << I_count << endl;
					 cout << "-------------------------------" << endl<<endl;
					 std::stringstream ss;
					 ss << I_count;
					 string s = ss.str();
					 ACTION[i][*it] = "S" + s;//更新ACTION表 
					 I_count++;
				 }
				 else 
				 {
					 //项目集已经存在，需要自己指向自己
					 cout << "  I_" << i << " -- " << *it << "->" << "I_" << k << endl << endl;
					 std::stringstream ss;
					 ss << k;
					 string s = ss.str();
					 ACTION[i][*it] = "S" + s;
				 }
			 }
		 }
	 }
 }


 //打印parsing table
 void print_ACTION_GOTO() 
 {
	 set<string>::iterator it;
	 cout << "-------------------------------------------------------------------------------" << endl;
	 cout << setw(27) << setiosflags(ios::right) << "ACTION";
	 cout << setw(38) << setiosflags(ios::left) << "  GOTO" << endl;
	 cout << "-------------------------------------------------------------------------------" << endl;
	 cout << setw(8) << "项目集" << "|";
	 for (it = VT.begin(); it != VT.end(); it++) 
	 {
		 cout << setw(4) << *it << setw(4) << "|";
	 }
	 cout << setw(4) << "$" << setw(4) << "|";
	 for (it = VN.begin(); it != VN.end(); it++) 
	 {
		 cout << setw(4) << *it << setw(4) << "|";
	 }
	 cout << endl;
	 for (int j = 0; j < I_count; j++) 
	 {
		 if (j >= 10)
		 {
			 cout << setw(6) << "I_" << j << "|";
		 }
		 else { cout << setw(6) << "I_" << j << setw(2) << "|"; }
		 for (it = VT.begin(); it != VT.end(); it++) 
		 {
			 cout << setw(4) << ACTION[j][*it] << setw(4) << "|";
		 }
		 cout << setw(4) << ACTION[j]["$"] << setw(4) << "|";
		 for (it = VN.begin(); it != VN.end(); it++) 
		 {
			 if (GOTO[j][*it])//GOTO表为0 
				 cout << setw(4) << GOTO[j][*it] << setw(4) << "|";
			 else 
			 {
				 cout << setw(4) << " " << setw(4) << "|";
			 }
		 }
		 cout << endl;
	 }
	 cout << "-------------------------------------------------------------------------------" << endl;
 }

 //对栈容器进行输出,i=0,返回status中的字符串,i=1,返回sign中的字符串，i=2返回inputStr中的字符串
 string vectTrancStr(int i, vector<int> status, vector<string> sign) {
	 string buf;
	 int count = 0;
	 //输出状态栈
	 if (i == 0) {
		 vector<int>::iterator it = status.begin();
		 //将数字转化为字符串
		 string str, tempStr;
		 for (it; it != status.end(); it++) 
		 {
			 stringstream ss;
			 ss << *it;
			 ss >> tempStr;
			 str += tempStr;
		 }
		 return str;
	 }
	 //输出符号栈
	 else if (i == 1) 
	 {
		 vector<string>::iterator it = sign.begin();
		 for (it; it != sign.end(); it++) 
		 {
			 buf += *it;
			 count++;
		 }
	 }
	 string str(buf);
	 return str;
 }

 //输入句子，开始分析,栈状态分析
 void Input_Analyse() 
 {
	 vector<int>  status;//定义状态栈
	 vector<string> sign;//定义符号栈
	 int step = 1;  //步骤
	 string input;
	 //cout << "请输入分析的字符串(请以#结尾)：";
	 //cin >> input;//输入待分析的句子 
	 ifstream infile;
	 //infile.open("D:\\Visualprogram\\syntax\\token1.txt"); //*i=i
	 //infile.open("D:\\Visualprogram\\syntax\\token2.txt"); //句子*i=*i
	 //infile.open("D:\\Visualprogram\\syntax\\token3.txt"); //句子ccdd
	 //infile.open("D:\\Visualprogram\\syntax\\token4.txt"); //句子+*i
	 infile.open("D:\\Visualprogram\\syntax\\token5.txt"); //句子cddd
	 getline(infile, input);
	 input = input + "$";
	 cout << "\n分析的字符串为：" << input << endl << endl;
	 status.push_back(0);//把状态0入栈
	 //把$加入符号栈
	 sign.push_back("$");
	 //输出初始栈状态
	 cout <<setw(10)<< "步骤" << setw(15) << "状态栈" << setw(15) << "符号栈" << setw(15) << "输入串" << setw(25) << "动作说明" << endl;
	 int s = 0;//初始状态
	 int oldStatus;//保存之前的状态
	 string input_s;  //获取初始符号
	 input_s = input.substr(0, 1);
	 while (ACTION[s][input_s] != "acc") 
	 {//如果action[s][input_s] =="accept" ，则分析成功
		 //获取字符串
		 string str = ACTION[s][input_s];
		 //如果str为空，报错并返回
		 if (str.size() == 0) 
		 {
			 cout << "出错";
			 return;
		 }
		 //获取S或r后面的数字 
		 stringstream ss;
		 ss << str.substr(1);
		 ss >> s;//新的状态号 
		 //如果是移进 
		 if (str.substr(0, 1) == "S") 
		 {
			 cout <<setw(10)<< step << setw(15) << vectTrancStr(0, status, sign) << setw(15) << vectTrancStr(1, status, sign)
				 << setw(15) << input << setw(15) << "A" << "CTION(" << status.back() << "," << input_s << ")=S" << s << "," << "状态" 
				 << s << "入栈" << endl;
			 sign.push_back(input_s); //输入符号入栈 
			 input.erase(0, 1);
			 status.push_back(s);//将状态数字入栈
		 }
		 //如果是规约 
		 else if (str.substr(0, 1) == "r") 
		 {
			 string kaitou;//产生式的头部 
			 kaitou = cfg[s].start;
			 int pop_num = cfg[s].next.size();//获取符号栈的出栈次数
			 string r;
			 stringstream ss;
			 ss << s;
			 ss >> r;
			 int oldStatus;//保存之前的状态
			 int status_size = status.size();
			 oldStatus = status[status_size - 1 - pop_num];
			 s = GOTO[oldStatus][kaitou];
			 cout <<setw(10)<< step << setw(15) << vectTrancStr(0, status, sign) << setw(15) << vectTrancStr(1, status, sign) 
				 << setw(15) << input << setw(15) << "产" << (string)"生式" + r + (string)"归约,GOTO(" << oldStatus << "," << kaitou
				 << ")=" << s << "入栈" << endl;
			 //对符号栈进行出栈和状态栈进行出栈
			 while (pop_num--) 
			 {
				 sign.pop_back();
				 status.pop_back();
			 }
			 sign.push_back(kaitou);//再对产生式的开始符号入栈
			 status.push_back(s);//再把新的状态入栈
		 }
		 step++; //步骤数加1
		 s = status.back();//获取栈顶状态
		 input_s = input.substr(0, 1);//获取输入的字符
	 }
	 cout <<setw(10)<< step << setw(15) << vectTrancStr(0, status, sign) << setw(15) << vectTrancStr(1, status, sign) 
		 << setw(15) << input << setw(15) << "a" << "cc:分析成功" << endl;
 }

 int main() 
 {
	 //创建一个文件输入流对象
	 ifstream inFile;
	 //打开文件
	 //inFile.open("D:\\Visualprogram\\syntax\\TEST1.txt
	 inFile.open("D:\\Visualprogram\\syntax\\TEST2.txt");
	 if (inFile) { //条件成立，则说明文件打开成功
		 cout << "File open scessful!" << endl;
	 }
	 else
		 cout << "File doesn't exist!" << endl;
	 string temp;
	 getline(inFile, temp);
	 for (int j = 0; j < temp.length(); j++) 
	 {
		 VT.insert(temp.substr(j, 1));
	 }
	 set<string>::iterator p;
	 cout << "终结符号:";
	 for (p = VT.begin(); p != VT.end(); p++) 
	 {
		 cout << *p << ",";
	 }
	 cout << endl << "原文法为：" << endl;
	 int count = 0;//文件行数 
	 while (getline(inFile, temp)) //按行读取文件内容 
	 {
		 cfg[count].start = temp[0];
		 for (int j = 3; j < temp.length(); j++) 
		 {
			 cfg[count].next.push_back(temp.substr(j, 1));
		 }
		 VN.insert(cfg[count].start);//非终结符 
		 cout <<setw(6)<< cfg[count].start << "->";
		 vector<string>::iterator it;
		 for (it = cfg[count].next.begin(); it != cfg[count].next.end(); it++) 
		 {
			 cout << *it;
		 }
		 cout << endl;
		 count++;
	 }
	 CFGCount = count;//文法句数
	 Output_First_and_Follow();//求First和Follow集合 
	 LR1_Analyse();//构造LR1分析表
	 cout << "打印分析表：" << endl;
	 print_ACTION_GOTO();//打印ACTION GOTO表 
	 Input_Analyse();//输入句子，开始分析 
	 return 0;
	 system("pause");
 }