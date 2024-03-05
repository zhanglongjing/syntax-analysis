//LR1�﷨����
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

template <typename CFG_LR1> //����ģ��

// ����һ��+���������v1+v2=��v2���뵽v1�ĺ���
vector<CFG_LR1>& operator +(vector<CFG_LR1>& v1, vector<CFG_LR1>& v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
	return v1;
}

//���ս����
struct VN_Set {
	string VN_name;
	set<string> FIRST; //First���� 
	set<string> FOLLOW; //follow��
};
//����ʽ
struct CFG {
	string start;//���ս��
	vector<string> next; //First���� 
};
//LR1��Ŀ��
 struct CFG_LR1 {
	 string start;//���ս��
	 vector<string> next; //First���� 
	 int num; //��Ŀ
	 vector<string> tail;//Ԥ���
	 bool operator==(const CFG_LR1& rhs) //����==���ж�������Ŀ���Ƿ�һ��
	 {
		 return (start == rhs.start && next == rhs.next &&
			 num == rhs.num && tail == rhs.tail);
	 }
 };

 int CFGCount = 0; //��¼�ķ�����
 CFG cfg[MAX_Count];//����ʽ 
 VN_Set VN_First[MAX_Count];//���ս������First���� 
 set<string> VN;// ���ս������ 
 set<string> VT;//�ս������ 
 int I_count = 0;//��¼LR1��Ŀ�� 
 vector<CFG_LR1> I[MAX_Count]; //��Ŀ������ 
 map<string, int> mark_Follow;//���ڱ��Follow 
 map<string, int> GOTO[MAX_Count]; //GOTO��
 map<string, string> ACTION[MAX_Count]; //ACTION��


 bool cmp_vector(vector<CFG_LR1>& v1, vector<CFG_LR1>& v2)
 {
	 if (v1.size() != v2.size()) return false;
	 for (int i = 0; i < v2.size(); i++)
	 {
		 CFG_LR1 t;
		 t = v2[i];
		 vector<CFG_LR1>::iterator result = find(v1.begin(), v1.end(), t); //����3
		 if (result == v1.end()) //û�ҵ�
			 return false;
	 }
	 return true;
 }

 //��FIRST���� 
 set<string> get_FIRST(string a) 
 { 
	 set<string> T;
	 for (int i = 0; i < CFGCount; i++) 
	 {
		 if (cfg[i].start == a) {  // a->.. 
			 for (int j = 0; j < cfg[i].next.size(); j++) 
			 {
				 if (VT.find(cfg[i].next[j]) != VT.end())
				 { //���ս����ͷ 
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
					 if (U.find("$") != U.end()) { //U�к���$,�������¸���first
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

 //��FOLLOW
 set<string> get_FOLLOW(string a) 
 {
	 set<string> T;
	 //cout<<"��������"<<a<<" ��Follow"<<endl; 
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
			 if (VT.find(cfg[i].next[j]) == VT.end() && a == cfg[i].next[j]) { //�Ƿ��ս������FOLLOW���� 
				 if (j == cfg[i].next.size() - 1 && a != cfg[i].start) {//S->...a 
					 set<string> tt = get_FOLLOW(cfg[i].start);
					 T.insert(tt.begin(), tt.end());
				 }
				 for (int k = j + 1; k < cfg[i].next.size(); k++) {
					 if (VT.find(cfg[i].next[k]) != VT.end()) {//����һ�����ս��  S->..av.. 
						 T.insert(cfg[i].next[k]);
						 break;
					 }
					 else {
						 temp = get_FIRST(cfg[i].next[k]);
						 if (temp.find("$") != temp.end()) {//��$ S->..a B.. 
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

 //���first,follow��
 void Output_First_and_Follow() 
 {
	 set<string>::iterator it;
	 int count = 0;
	 cout << "*******************************************" << endl;
	 cout << "���ս��" <<"     " << "FIRST��" << "     " << "FOLLOW��" << endl;
	 for (it = VN.begin(); it != VN.end(); it++)
	 {

		 VN_First[count].VN_name = *it;
		 VN_First[count].FIRST = get_FIRST(*it);

		 mark_Follow[*it] = 0;
		 VN_First[count].FOLLOW = get_FOLLOW(*it);

		 //-----------���FIRST-------------------- 
		 cout << setw(4) << VN_First[count].VN_name<< '\t'<<"       ";
		 set<string>::iterator it;
		 for (it = VN_First[count].FIRST.begin(); it != VN_First[count].FIRST.end(); it++) {
			 cout << *it<< " ";
		 }
		 cout << '\t' << "   ";
		 //------------���FOLLOW--------------
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

 //��հ� 
 vector<CFG_LR1> CLOSURE(CFG_LR1 I) {
	 vector<CFG_LR1> T;
	 //T.push_back(I);
	 if (I.num >= I.next.size()) { //��Լ��ĿA->��.���߽�����Ŀ 
		 return T;
	 }
	 else 
	 {
		 string temp = I.next[I.num];
		 if (VT.find(temp) != VT.end()) 
		 {     //���������ս�� ,�ƽ���Ŀ A����.a��  
			 return T;
		 }
		 else 
		 {  //��Լ��Ŀ 
			 for (int i = 0; i < CFGCount; i++) 
			 {
				 if (cfg[i].start == temp) 
				 {
					 CFG_LR1 p;
					 p.start = cfg[i].start;
					 p.num = 0;//������ǰ�� 
					 p.next = cfg[i].next;
					 set<string> f1;
					 for (int j = I.num + 1; j < I.next.size(); j++)
					 {
						 set<string> f2;//�����ݴ�first 
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

 //չʾ��Ŀ��
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

 //LR1��������
 void LR1_Analyse() 
 {
	 CFG_LR1 p;
	 //��ʼ��Ŀ S��->.S ,# 
	 p.start = cfg[0].start + "^";
	 p.num = 0;//������ǰ�� 
	 p.tail.push_back("$");
	 p.next.push_back(cfg[0].start);
	 I[0] = CLOSURE(p);//��հ����I[0]
	 I[0].insert(I[0].begin(), p);//��հ�������һЩ���� 
	 I_count = 1;
	 //������Ŀ�� 
	 for (int i = 0; i < I_count; i++)
	 {//ÿ����Ŀ��  ��Ŀ��I(i) 
		 cout << "-------------------------------" << endl;
		 cout << "������Ŀ��I_" << i << endl;
		 printI(I[i]);//չʾ��Ŀ�� 
		 //---------��ACTION��r����-------------- 
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
		 //-----����Ŀ�����б仯���ֱ�Ϊ������ս�����ս��
		 //ÿ�����ս��
		 set<string>::iterator it;
		 for (it = VN.begin(); it != VN.end(); it++) 
		 {  
			 int num = 0;//�����жϲ���������ս��
			 cout << "-------------------------------" << endl;
			 cout<<"��Ŀ��I_"<<i<<"����"<<*it<<",";
			 vector<CFG_LR1> temp;
			 //cout<<"��Ŀ����С"<<I[i].size()<<endl;
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
				 cout << "�޷�����" << *it << endl;
				 cout << "-------------------------------" << endl<<endl;
			 }
			 //cout<<"temp.size"<< temp.size()<<endl;
			 if (temp.size() > 0) 
			 {
				 int k;
				 for (k = 0; k < I_count; k++) 
				 {//��һ����Ŀ���Ƿ��Ѿ����� 
					 if (cmp_vector(I[k], temp)) 
					 {
						 break;
					 }
				 }
				 if (k == I_count) 
				 {
					 //�������µ���Ŀ�� 
					 I[I_count] = temp;
                     cout<<"�õ��µ���Ŀ��I_"<<I_count<<endl;
					 cout<<"*******************************" << endl << "I_" <<I_count<<": "<<endl;
                     printI(I[I_count]);//չʾ��Ŀ��
					 cout <<"��ϵ��" << "  I_" << i << " -- " << *it << "->" << "I_" << I_count << endl;
					 cout<<"-------------------------------"<<endl<<endl;
					 GOTO[i][*it] = I_count;//����goto�� 
					 I_count++;
				 }
				 else 
				 {
					 //��Ŀ���Ѿ����ڣ���Ҫ�Լ�ָ���Լ�
					 cout << "  I_" << i << " -- " << *it << "->" << "I_" << k << endl << endl;
					 GOTO[i][*it] = k;
				 }
			 }
		 }
		 //��ÿ���ս��
		 for (it = VT.begin(); it != VT.end(); it++) 
		 {
			 int num = 0;//�����жϲ���������ս��
			 cout << "-------------------------------" << endl;
			 cout<<"��Ŀ��I_"<<i<<"����"<<*it<<",";
			 vector<CFG_LR1> temp;
			 for (int j = 0; j < I[i].size(); j++) 
			 {
				 CFG_LR1 lr = I[i][j];
				 if(lr.num < lr.next.size() && lr.next[lr.num] == *it) 
				 {
					 vector<CFG_LR1> t2;
					 lr.num++;
					 t2 = CLOSURE(lr);//�հ�����Ľ������������ 
					 t2.insert(t2.begin(), lr);//��հ�������һЩ����
					 //cout << "�հ���" << endl;
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
				 cout << "�޷�����" << *it << endl;
				 cout << "-------------------------------" << endl << endl;
			 }
			 //cout<<"temp.size"<< temp.size()<<endl;
			 if (temp.size() > 0) 
			 {
				 int k;
				 for (k = 0; k < I_count; k++) 
				 {//��һ����Ŀ���Ƿ��Ѿ����� 
					 if (cmp_vector(I[k], temp)) 
					 {
						 break;
					 }
				 }
				 if (k == I_count) 
				 {
					 //�������µ���Ŀ�� 
					 I[I_count] = temp;
                     cout<<"�õ��µ���Ŀ��I"<<I_count<<endl;
					 cout << "*******************************" << endl << "I_" << I_count << ": "<<endl;
                     printI(I[I_count]);//չʾ��Ŀ��
					 cout <<"��ϵ��" << "  I_" << i << " -- " << *it << "->" << "I_" << I_count << endl;
					 cout << "-------------------------------" << endl<<endl;
					 std::stringstream ss;
					 ss << I_count;
					 string s = ss.str();
					 ACTION[i][*it] = "S" + s;//����ACTION�� 
					 I_count++;
				 }
				 else 
				 {
					 //��Ŀ���Ѿ����ڣ���Ҫ�Լ�ָ���Լ�
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


 //��ӡparsing table
 void print_ACTION_GOTO() 
 {
	 set<string>::iterator it;
	 cout << "-------------------------------------------------------------------------------" << endl;
	 cout << setw(27) << setiosflags(ios::right) << "ACTION";
	 cout << setw(38) << setiosflags(ios::left) << "  GOTO" << endl;
	 cout << "-------------------------------------------------------------------------------" << endl;
	 cout << setw(8) << "��Ŀ��" << "|";
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
			 if (GOTO[j][*it])//GOTO��Ϊ0 
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

 //��ջ�����������,i=0,����status�е��ַ���,i=1,����sign�е��ַ�����i=2����inputStr�е��ַ���
 string vectTrancStr(int i, vector<int> status, vector<string> sign) {
	 string buf;
	 int count = 0;
	 //���״̬ջ
	 if (i == 0) {
		 vector<int>::iterator it = status.begin();
		 //������ת��Ϊ�ַ���
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
	 //�������ջ
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

 //������ӣ���ʼ����,ջ״̬����
 void Input_Analyse() 
 {
	 vector<int>  status;//����״̬ջ
	 vector<string> sign;//�������ջ
	 int step = 1;  //����
	 string input;
	 //cout << "������������ַ���(����#��β)��";
	 //cin >> input;//����������ľ��� 
	 ifstream infile;
	 //infile.open("D:\\Visualprogram\\syntax\\token1.txt"); //*i=i
	 //infile.open("D:\\Visualprogram\\syntax\\token2.txt"); //����*i=*i
	 //infile.open("D:\\Visualprogram\\syntax\\token3.txt"); //����ccdd
	 //infile.open("D:\\Visualprogram\\syntax\\token4.txt"); //����+*i
	 infile.open("D:\\Visualprogram\\syntax\\token5.txt"); //����cddd
	 getline(infile, input);
	 input = input + "$";
	 cout << "\n�������ַ���Ϊ��" << input << endl << endl;
	 status.push_back(0);//��״̬0��ջ
	 //��$�������ջ
	 sign.push_back("$");
	 //�����ʼջ״̬
	 cout <<setw(10)<< "����" << setw(15) << "״̬ջ" << setw(15) << "����ջ" << setw(15) << "���봮" << setw(25) << "����˵��" << endl;
	 int s = 0;//��ʼ״̬
	 int oldStatus;//����֮ǰ��״̬
	 string input_s;  //��ȡ��ʼ����
	 input_s = input.substr(0, 1);
	 while (ACTION[s][input_s] != "acc") 
	 {//���action[s][input_s] =="accept" ��������ɹ�
		 //��ȡ�ַ���
		 string str = ACTION[s][input_s];
		 //���strΪ�գ���������
		 if (str.size() == 0) 
		 {
			 cout << "����";
			 return;
		 }
		 //��ȡS��r��������� 
		 stringstream ss;
		 ss << str.substr(1);
		 ss >> s;//�µ�״̬�� 
		 //������ƽ� 
		 if (str.substr(0, 1) == "S") 
		 {
			 cout <<setw(10)<< step << setw(15) << vectTrancStr(0, status, sign) << setw(15) << vectTrancStr(1, status, sign)
				 << setw(15) << input << setw(15) << "A" << "CTION(" << status.back() << "," << input_s << ")=S" << s << "," << "״̬" 
				 << s << "��ջ" << endl;
			 sign.push_back(input_s); //���������ջ 
			 input.erase(0, 1);
			 status.push_back(s);//��״̬������ջ
		 }
		 //����ǹ�Լ 
		 else if (str.substr(0, 1) == "r") 
		 {
			 string kaitou;//����ʽ��ͷ�� 
			 kaitou = cfg[s].start;
			 int pop_num = cfg[s].next.size();//��ȡ����ջ�ĳ�ջ����
			 string r;
			 stringstream ss;
			 ss << s;
			 ss >> r;
			 int oldStatus;//����֮ǰ��״̬
			 int status_size = status.size();
			 oldStatus = status[status_size - 1 - pop_num];
			 s = GOTO[oldStatus][kaitou];
			 cout <<setw(10)<< step << setw(15) << vectTrancStr(0, status, sign) << setw(15) << vectTrancStr(1, status, sign) 
				 << setw(15) << input << setw(15) << "��" << (string)"��ʽ" + r + (string)"��Լ,GOTO(" << oldStatus << "," << kaitou
				 << ")=" << s << "��ջ" << endl;
			 //�Է���ջ���г�ջ��״̬ջ���г�ջ
			 while (pop_num--) 
			 {
				 sign.pop_back();
				 status.pop_back();
			 }
			 sign.push_back(kaitou);//�ٶԲ���ʽ�Ŀ�ʼ������ջ
			 status.push_back(s);//�ٰ��µ�״̬��ջ
		 }
		 step++; //��������1
		 s = status.back();//��ȡջ��״̬
		 input_s = input.substr(0, 1);//��ȡ������ַ�
	 }
	 cout <<setw(10)<< step << setw(15) << vectTrancStr(0, status, sign) << setw(15) << vectTrancStr(1, status, sign) 
		 << setw(15) << input << setw(15) << "a" << "cc:�����ɹ�" << endl;
 }

 int main() 
 {
	 //����һ���ļ�����������
	 ifstream inFile;
	 //���ļ�
	 //inFile.open("D:\\Visualprogram\\syntax\\TEST1.txt
	 inFile.open("D:\\Visualprogram\\syntax\\TEST2.txt");
	 if (inFile) { //������������˵���ļ��򿪳ɹ�
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
	 cout << "�ս����:";
	 for (p = VT.begin(); p != VT.end(); p++) 
	 {
		 cout << *p << ",";
	 }
	 cout << endl << "ԭ�ķ�Ϊ��" << endl;
	 int count = 0;//�ļ����� 
	 while (getline(inFile, temp)) //���ж�ȡ�ļ����� 
	 {
		 cfg[count].start = temp[0];
		 for (int j = 3; j < temp.length(); j++) 
		 {
			 cfg[count].next.push_back(temp.substr(j, 1));
		 }
		 VN.insert(cfg[count].start);//���ս�� 
		 cout <<setw(6)<< cfg[count].start << "->";
		 vector<string>::iterator it;
		 for (it = cfg[count].next.begin(); it != cfg[count].next.end(); it++) 
		 {
			 cout << *it;
		 }
		 cout << endl;
		 count++;
	 }
	 CFGCount = count;//�ķ�����
	 Output_First_and_Follow();//��First��Follow���� 
	 LR1_Analyse();//����LR1������
	 cout << "��ӡ������" << endl;
	 print_ACTION_GOTO();//��ӡACTION GOTO�� 
	 Input_Analyse();//������ӣ���ʼ���� 
	 return 0;
	 system("pause");
 }