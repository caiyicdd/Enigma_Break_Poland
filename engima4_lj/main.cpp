#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>
using namespace std;

struct Table{
    char c;
    int n;
    Table(){}
    Table(char c,int n):
        c(c),n(n){}
};
struct Roll{
    int chain_no;   //Ȧ��
    vector<int>chain;   //ÿ��Ȧ����ĸ��
    char init_r1;        //��ʼת��t1λ��
    char init_r2;        //��ʼת��t2λ��
    char init_r3;        //��ʼת��t3λ��
    char select_roll[3]; //ѡ���ת��˳��
};
vector<Roll>Dict_Roll(vector<Roll>(105456));
int Tree[26];
int Node[26];


bool cmp(Table a,Table b){
    return a.n<b.n;
}

vector<Table> gener(){

    vector<Table>table(26);
    vector <int> pri;
    for(int i=0;i<26;i++){
        table[i].c = 'a' + i;
        int p = rand();
        table[i].n = p;
    }
    sort(table.begin(),table.begin()+26,cmp);
    return table;
}

void printT(vector<Table>tab){
    for(int i=0;i<tab.size();i++){
        cout<<tab[i].c<<" ";
    }
    cout<<endl;
}
void printV(vector<char>tab){
    for(int i=0;i<tab.size();i++){
        cout<<tab[i]<<" ";
    }
    cout<<endl;
}
void printM(map<char,char>tab){
    map<char,char>::iterator iter;
    iter = tab.begin();
    while(iter != tab.end()){
        cout << iter->first << "-" << iter->second << endl;
        iter++;
    }
}

char exchTab1(vector<Table>table,char c){ // �������
    return table[c-'a'].c;
}
char exchTab2(vector<Table>table,char c){ // �������
    for(int i=0;i<table.size();i++){
        if(table[i].c == c){
            return 'a'+i;
        }
    }
}

void roveSigle(vector<Table>&tab){           //ת��ת��
    Table temp;
    temp.c = tab[0].c;
    temp.n = tab[0].n;
    for(int i=0;i<tab.size()-1;i++){
        tab[i].c= tab[i+1].c;
        tab[i].n= tab[i+1].n;
    }
    tab[tab.size()-1].c= temp.c;
    tab[tab.size()-1].n= temp.n;
}

void roveAll(vector<Table>&table1,vector<Table>&table2,vector<Table>&table3,char& t1,char& t2,char& t3){
    roveSigle(table1);
    t1 = t1 + 1;
    if(t1-'a' >= 26){
        t2 = t2 + 1;
        t1 = t1 % 26 + 'a';
        roveSigle(table2);
        if(t2-'a'>=26){
            t3 = t3 + 1;
            t2 = t2 % 26 + 'a';
            roveSigle(table3);
            if(t3-'a'>=26){
                t3 = t3%26 + 'a';
            }
        }
    }
}


char encr(char c,vector<Table>table1,vector<Table>table2,vector<Table>table3,map<char,char>retable,map<char,char>board,char& t1,char& t2,char& t3){
    char x = board[c];
    x = exchTab1(table1,x);
    x = exchTab1(table2,x);
    x = exchTab1(table3,x);
    x = retable[x];
    x = exchTab2(table3,x);
    x = exchTab2(table2,x);
    x = exchTab2(table1,x);
    x = board[x];
    return x;
}


void initRov(vector<Table>&table,char t){
    Table temp;
    for (int i=0;i<t-'a';i++){
        temp.c = table[0].c;
        temp.n = table[0].n;
        for(int j = 0;j<table.size()-1;j++){
            table[j].c = table[j+1].c;
            table[j].n = table[j+1].n;
        }
        table[table.size()-1].c=temp.c;
        table[table.size()-1].n=temp.n;
    }
}

int findRoot(int x){
    if(Tree[x]==-1)return x;
    else{
        int tmp = findRoot(Tree[x]);
        Tree[x]=tmp;
        Node[tmp] += Node[x];
        return tmp;
    }
}
bool cmp2(Roll x,Roll y){
    return x.chain_no < y.chain_no;
}

int main()
{
    srand(time(NULL));
    /* ����ת��
    vector<Table> table1 = gener();
    vector<Table> table2 = gener();
    vector<Table> table3 = gener();
    */
    vector<Table> table1 ={Table('v',1),Table('e',1),Table('t',1),Table('a',1),Table('b',1),Table('z',1),
                            Table('k',1),Table('x',1),Table('u',1),Table('i',1),Table('q',1),Table('s',1),
                            Table('f',1),Table('j',1),Table('d',1),Table('l',1),Table('w',1),Table('h',1),
                            Table('o',1),Table('c',1),Table('n',1),Table('m',1),Table('y',1),Table('r',1),
                            Table('g',1),Table('p',1)};
    vector<Table> table2 ={Table('k',1),Table('c',1),Table('u',1),Table('m',1),Table('w',1),Table('i',1),
                            Table('g',1),Table('p',1),Table('s',1),Table('j',1),Table('o',1),Table('n',1),
                            Table('h',1),Table('f',1),Table('t',1),Table('x',1),Table('l',1),Table('e',1),
                            Table('y',1),Table('b',1),Table('v',1),Table('r',1),Table('q',1),Table('a',1),
                            Table('z',1),Table('d',1)};
    vector<Table> table3 ={Table('k',1),Table('h',1),Table('s',1),Table('c',1),Table('x',1),Table('b',1),
                            Table('l',1),Table('n',1),Table('z',1),Table('e',1),Table('i',1),Table('y',1),
                            Table('w',1),Table('f',1),Table('o',1),Table('u',1),Table('q',1),Table('j',1),
                            Table('v',1),Table('r',1),Table('t',1),Table('p',1),Table('a',1),Table('g',1),
                            Table('d',1),Table('m',1)};

    map<char,char> retable;// ������
    for(int i=0;i<26;i++){
        retable.insert(pair<char,char>('a'+i,'a'+(i+13)%26));
    }
    map<char,char> board;// ���߰�
    board['a'] = 'b';board['b'] = 'a';board['c'] = 'd';board['d'] = 'c';board['e'] = 'f';board['f'] = 'e';board['g'] = 'g';
    board['h'] = 'o';board['i'] = 'p';board['j'] = 'q';board['k'] = 'r';board['l'] = 's';board['m'] = 't';board['n'] = 'n';
    board['o'] = 'h';board['p'] = 'i';board['q'] = 'j';board['r'] = 'k';board['s'] = 'l';board['t'] = 'm';board['u'] = 'u';
    board['v'] = 'z';board['w'] = 'w';board['x'] = 'x';board['y'] = 'y';board['z'] = 'v';
    char t1,t2,t3;//��ʼת��λ�ã���ʼ��Կ

    int index = 0;

    //   ��һ��ת��˳��table1,table2,table3
    vector<Table> table1_tmp = table1;
    vector<Table> table2_tmp = table2;
    vector<Table> table3_tmp = table3;
    char c='a';
    for(int m=0;m<26;m++){
        for(int n=0;n<26;n++){
            for(int l=0;l<26;l++){
                map<char,char>temp;
                for(int i=0;i<26;i++){          //�����õ�A0A3��Ӧ��ϵ
                    t1 = 'a'+ l;
                    t2 = 'a'+ n;
                    t3 = 'a'+ m;
                    table1_tmp = table1;
                    table2_tmp = table2;
                    table3_tmp = table3;
                    initRov(table1_tmp,t1);//����ת�ӳ�ֵ
                    initRov(table2_tmp,t2);
                    initRov(table3_tmp,t3);
                    char en0 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    char en3 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    temp.insert(pair<char,char>(en0,en3));
                    //cout<<c<<" - "<<en0<<" - "<<en3<<endl;
                    c = c + 1;
                    if(c>'z'){
                        c = 'a';
                    }
                }

                for(int i=0;i<26;i++)Tree[i]=-1;    //��������������е�Ȧ
                for(int i=0;i<26;i++)Node[i]=1;     //�����������ÿ��Ȧ����ĸ��

                vector<int>roll;                    //��¼ÿ������Ȧ
                map<char,char>::iterator iter = temp.begin();
                while(iter!=temp.end()){
                    char x = iter->first;
                    char y = iter->second;
                    int a = x - 'a';
                    int b = y - 'a';
                    a = findRoot(a);
                    b = findRoot(b);
                    if(a!=b){
                        Tree[a]=b;
                    }
                    iter++;
                }
                for(int i=0;i<26;i++){
                    if(Tree[i]==-1){
                        roll.push_back(Node[i]);
                    }
                }
                sort(roll.begin(),roll.end(),greater<int>());

                Dict_Roll[index].chain_no = roll.size();
                Dict_Roll[index].chain = roll;
                Dict_Roll[index].init_r1 = 'a'+ l;
                Dict_Roll[index].init_r2 = 'a'+ n;
                Dict_Roll[index].init_r3 = 'a'+ m;
                Dict_Roll[index].select_roll[0] = '1';
                Dict_Roll[index].select_roll[1] = '2';
                Dict_Roll[index].select_roll[2] = '3';
                index++;
                if(index%1000==0)cout<<index<<endl;
            }
        }
    }

    //   �ڶ���ת��˳��table1,table3,table2
    table1_tmp = table1;
    table2_tmp = table3;
    table3_tmp = table2;

    c='a';
    for(int m=0;m<26;m++){
        for(int n=0;n<26;n++){
            for(int l=0;l<26;l++){
                map<char,char>temp;
                for(int i=0;i<26;i++){          //�����õ�A0A3��Ӧ��ϵ
                    t1 = 'a'+ l;
                    t2 = 'a'+ n;
                    t3 = 'a'+ m;
                    table1_tmp = table1;
                    table2_tmp = table3;
                    table3_tmp = table2;
                    initRov(table1_tmp,t1);//����ת�ӳ�ֵ
                    initRov(table2_tmp,t2);
                    initRov(table3_tmp,t3);
                    char en0 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    char en3 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    temp.insert(pair<char,char>(en0,en3));
                    //cout<<c<<" - "<<en0<<" - "<<en3<<endl;
                    c = c + 1;
                    if(c>'z'){
                        c = 'a';
                    }
                }

                for(int i=0;i<26;i++)Tree[i]=-1;    //��������������е�Ȧ
                for(int i=0;i<26;i++)Node[i]=1;     //�����������ÿ��Ȧ����ĸ��

                vector<int>roll;                    //��¼ÿ������Ȧ
                map<char,char>::iterator iter = temp.begin();
                while(iter!=temp.end()){
                    char x = iter->first;
                    char y = iter->second;
                    int a = x - 'a';
                    int b = y - 'a';
                    a = findRoot(a);
                    b = findRoot(b);
                    if(a!=b){
                        Tree[a]=b;
                    }
                    iter++;
                }
                for(int i=0;i<26;i++){
                    if(Tree[i]==-1){
                        roll.push_back(Node[i]);
                    }
                }
                sort(roll.begin(),roll.end(),greater<int>());

                Dict_Roll[index].chain_no = roll.size();
                Dict_Roll[index].chain = roll;
                Dict_Roll[index].init_r1 = 'a'+ l;
                Dict_Roll[index].init_r2 = 'a'+ n;
                Dict_Roll[index].init_r3 = 'a'+ m;
                Dict_Roll[index].select_roll[0] = '1';
                Dict_Roll[index].select_roll[1] = '3';
                Dict_Roll[index].select_roll[2] = '2';
                index++;
                if(index%1000==0)cout<<index<<endl;
            }
        }
    }

    //   ������ת��˳��table2,table1,table3
    table1_tmp = table2;
    table2_tmp = table1;
    table3_tmp = table3;
    c='a';
    for(int m=0;m<26;m++){
        for(int n=0;n<26;n++){
            for(int l=0;l<26;l++){
                map<char,char>temp;
                for(int i=0;i<26;i++){          //�����õ�A0A3��Ӧ��ϵ
                    t1 = 'a'+ l;
                    t2 = 'a'+ n;
                    t3 = 'a'+ m;
                    table1_tmp = table2;
                    table2_tmp = table1;
                    table3_tmp = table3;
                    initRov(table1_tmp,t1);//����ת�ӳ�ֵ
                    initRov(table2_tmp,t2);
                    initRov(table3_tmp,t3);
                    char en0 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    char en3 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    temp.insert(pair<char,char>(en0,en3));
                    //cout<<c<<" - "<<en0<<" - "<<en3<<endl;
                    c = c + 1;
                    if(c>'z'){
                        c = 'a';
                    }
                }

                for(int i=0;i<26;i++)Tree[i]=-1;    //��������������е�Ȧ
                for(int i=0;i<26;i++)Node[i]=1;     //�����������ÿ��Ȧ����ĸ��

                vector<int>roll;                    //��¼ÿ������Ȧ
                map<char,char>::iterator iter = temp.begin();
                while(iter!=temp.end()){
                    char x = iter->first;
                    char y = iter->second;
                    int a = x - 'a';
                    int b = y - 'a';
                    a = findRoot(a);
                    b = findRoot(b);
                    if(a!=b){
                        Tree[a]=b;
                    }
                    iter++;
                }
                for(int i=0;i<26;i++){
                    if(Tree[i]==-1){
                        roll.push_back(Node[i]);
                    }
                }
                sort(roll.begin(),roll.end(),greater<int>());

                Dict_Roll[index].chain_no = roll.size();
                Dict_Roll[index].chain = roll;
                Dict_Roll[index].init_r1 = 'a'+ l;
                Dict_Roll[index].init_r2 = 'a'+ n;
                Dict_Roll[index].init_r3 = 'a'+ m;
                Dict_Roll[index].select_roll[0] = '2';
                Dict_Roll[index].select_roll[1] = '1';
                Dict_Roll[index].select_roll[2] = '3';
                index++;
                if(index%1000==0)cout<<index<<endl;
            }
        }
    }

    //   ������ת��˳��table2,table3,table1
    table1_tmp = table2;
    table2_tmp = table3;
    table3_tmp = table1;
    c='a';
    for(int m=0;m<26;m++){
        for(int n=0;n<26;n++){
            for(int l=0;l<26;l++){
                map<char,char>temp;
                for(int i=0;i<26;i++){          //�����õ�A0A3��Ӧ��ϵ
                    t1 = 'a'+ l;
                    t2 = 'a'+ n;
                    t3 = 'a'+ m;
                    table1_tmp = table2;
                    table2_tmp = table3;
                    table3_tmp = table1;
                    initRov(table1_tmp,t1);//����ת�ӳ�ֵ
                    initRov(table2_tmp,t2);
                    initRov(table3_tmp,t3);
                    char en0 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    char en3 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    temp.insert(pair<char,char>(en0,en3));
                    //cout<<c<<" - "<<en0<<" - "<<en3<<endl;
                    c = c + 1;
                    if(c>'z'){
                        c = 'a';
                    }
                }

                for(int i=0;i<26;i++)Tree[i]=-1;    //��������������е�Ȧ
                for(int i=0;i<26;i++)Node[i]=1;     //�����������ÿ��Ȧ����ĸ��

                vector<int>roll;                    //��¼ÿ������Ȧ
                map<char,char>::iterator iter = temp.begin();
                while(iter!=temp.end()){
                    char x = iter->first;
                    char y = iter->second;
                    int a = x - 'a';
                    int b = y - 'a';
                    a = findRoot(a);
                    b = findRoot(b);
                    if(a!=b){
                        Tree[a]=b;
                    }
                    iter++;
                }
                for(int i=0;i<26;i++){
                    if(Tree[i]==-1){
                        roll.push_back(Node[i]);
                    }
                }
                sort(roll.begin(),roll.end(),greater<int>());

                Dict_Roll[index].chain_no = roll.size();
                Dict_Roll[index].chain = roll;
                Dict_Roll[index].init_r1 = 'a'+ l;
                Dict_Roll[index].init_r2 = 'a'+ n;
                Dict_Roll[index].init_r3 = 'a'+ m;
                Dict_Roll[index].select_roll[0] = '2';
                Dict_Roll[index].select_roll[1] = '3';
                Dict_Roll[index].select_roll[2] = '1';
                index++;
                if(index%1000==0)cout<<index<<endl;
            }
        }
    }

    //   ������ת��˳��table3,table1,table2
    table1_tmp = table3;
    table2_tmp = table1;
    table3_tmp = table2;
    c='a';
    for(int m=0;m<26;m++){
        for(int n=0;n<26;n++){
            for(int l=0;l<26;l++){
                map<char,char>temp;
                for(int i=0;i<26;i++){          //�����õ�A0A3��Ӧ��ϵ
                    t1 = 'a'+ l;
                    t2 = 'a'+ n;
                    t3 = 'a'+ m;
                    table1_tmp = table3;
                    table2_tmp = table1;
                    table3_tmp = table2;
                    initRov(table1_tmp,t1);//����ת�ӳ�ֵ
                    initRov(table2_tmp,t2);
                    initRov(table3_tmp,t3);
                    char en0 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    char en3 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    temp.insert(pair<char,char>(en0,en3));
                    //cout<<c<<" - "<<en0<<" - "<<en3<<endl;
                    c = c + 1;
                    if(c>'z'){
                        c = 'a';
                    }
                }

                for(int i=0;i<26;i++)Tree[i]=-1;    //��������������е�Ȧ
                for(int i=0;i<26;i++)Node[i]=1;     //�����������ÿ��Ȧ����ĸ��

                vector<int>roll;                    //��¼ÿ������Ȧ
                map<char,char>::iterator iter = temp.begin();
                while(iter!=temp.end()){
                    char x = iter->first;
                    char y = iter->second;
                    int a = x - 'a';
                    int b = y - 'a';
                    a = findRoot(a);
                    b = findRoot(b);
                    if(a!=b){
                        Tree[a]=b;
                    }
                    iter++;
                }
                for(int i=0;i<26;i++){
                    if(Tree[i]==-1){
                        roll.push_back(Node[i]);
                    }
                }
                sort(roll.begin(),roll.end(),greater<int>());

                Dict_Roll[index].chain_no = roll.size();
                Dict_Roll[index].chain = roll;
                Dict_Roll[index].init_r1 = 'a'+ l;
                Dict_Roll[index].init_r2 = 'a'+ n;
                Dict_Roll[index].init_r3 = 'a'+ m;
                Dict_Roll[index].select_roll[0] = '3';
                Dict_Roll[index].select_roll[1] = '1';
                Dict_Roll[index].select_roll[2] = '2';
                index++;
                if(index%1000==0)cout<<index<<endl;
            }
        }
    }

    //   ������ת��˳��table3,table2,table1
    table1_tmp = table3;
    table2_tmp = table2;
    table3_tmp = table1;
    c='a';
    for(int m=0;m<26;m++){
        for(int n=0;n<26;n++){
            for(int l=0;l<26;l++){
                map<char,char>temp;
                for(int i=0;i<26;i++){          //�����õ�A0A3��Ӧ��ϵ
                    t1 = 'a'+ l;
                    t2 = 'a'+ n;
                    t3 = 'a'+ m;
                    table1_tmp = table3;
                    table2_tmp = table2;
                    table3_tmp = table1;
                    initRov(table1_tmp,t1);//����ת�ӳ�ֵ
                    initRov(table2_tmp,t2);
                    initRov(table3_tmp,t3);
                    char en0 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    roveAll(table1_tmp,table2_tmp,table3_tmp,t1,t2,t3);
                    char en3 = encr(c,table1_tmp,table2_tmp,table3_tmp,retable,board,t1,t2,t3);
                    temp.insert(pair<char,char>(en0,en3));
                    //cout<<c<<" - "<<en0<<" - "<<en3<<endl;
                    c = c + 1;
                    if(c>'z'){
                        c = 'a';
                    }
                }

                for(int i=0;i<26;i++)Tree[i]=-1;    //��������������е�Ȧ
                for(int i=0;i<26;i++)Node[i]=1;     //�����������ÿ��Ȧ����ĸ��

                vector<int>roll;                    //��¼ÿ������Ȧ
                map<char,char>::iterator iter = temp.begin();
                while(iter!=temp.end()){
                    char x = iter->first;
                    char y = iter->second;
                    int a = x - 'a';
                    int b = y - 'a';
                    a = findRoot(a);
                    b = findRoot(b);
                    if(a!=b){
                        Tree[a]=b;
                    }
                    iter++;
                }
                for(int i=0;i<26;i++){
                    if(Tree[i]==-1){
                        roll.push_back(Node[i]);
                    }
                }
                sort(roll.begin(),roll.end(),greater<int>());

                Dict_Roll[index].chain_no = roll.size();
                Dict_Roll[index].chain = roll;
                Dict_Roll[index].init_r1 = 'a'+ l;
                Dict_Roll[index].init_r2 = 'a'+ n;
                Dict_Roll[index].init_r3 = 'a'+ m;
                Dict_Roll[index].select_roll[0] = '3';
                Dict_Roll[index].select_roll[1] = '2';
                Dict_Roll[index].select_roll[2] = '1';
                index++;
                if(index%1000==0)cout<<index<<endl;
            }
        }
    }

/*
    for(int i=0;i<index;i++){
        cout<<Dict_Roll[i].chain_no<<" : "<<Dict_Roll[i].init_r1<<"-"<<Dict_Roll[i].init_r2<<"-"<<Dict_Roll[i].init_r3<<"    ";
        for(int j=0;j<Dict_Roll[i].chain.size();j++){
            cout<<Dict_Roll[i].chain[j]<<" ";
        }
        cout<<endl;
    }
*/
    cout<<index<<endl;
    sort(Dict_Roll.begin(),Dict_Roll.end(),cmp2);

    FILE *fp;
    fopen_s(&fp,"rollMap1.txt","wt+");
    for(int i=0;i<index;i++){
        fprintf(fp,"%d %c %c %c %c %c %c ",Dict_Roll[i].chain_no,
                Dict_Roll[i].select_roll[0],Dict_Roll[i].select_roll[1],Dict_Roll[i].select_roll[2],
                Dict_Roll[i].init_r1,Dict_Roll[i].init_r2,Dict_Roll[i].init_r3);
        for(int j=0;j<Dict_Roll[i].chain.size();j++){
            fprintf(fp,"%d ",Dict_Roll[i].chain[j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    for(int i=0;i<5;i++){
        cout<<Dict_Roll[i].chain_no<<" : "<<Dict_Roll[i].select_roll[0]<<":"<<Dict_Roll[i].select_roll[1]<<":"<<Dict_Roll[i].select_roll[2]<<"  "
        <<Dict_Roll[i].init_r1<<"-"<<Dict_Roll[i].init_r2<<"-"<<Dict_Roll[i].init_r3<<"    ";
        for(int j=0;j<Dict_Roll[i].chain.size();j++){
            cout<<Dict_Roll[i].chain[j]<<" ";
        }
        cout<<endl;
    }

    for(int i=105455;i>105450;i--){
        cout<<Dict_Roll[i].chain_no<<" : "<<Dict_Roll[i].select_roll[0]<<":"<<Dict_Roll[i].select_roll[1]<<":"<<Dict_Roll[i].select_roll[2]<<"  "
        <<Dict_Roll[i].init_r1<<"-"<<Dict_Roll[i].init_r2<<"-"<<Dict_Roll[i].init_r3<<"    ";
        for(int j=0;j<Dict_Roll[i].chain.size();j++){
            cout<<Dict_Roll[i].chain[j]<<" ";
        }
        cout<<endl;
    }

    cout<<index<<endl;
    return 0;
}
/*

    ofstream outfile("rollMap.txt",ios::binary);
    int size1 = Dict_Roll.size();
    outfile.write((const char*)&Dict_Roll[0],size1 * sizeof(Roll));
    outfile.close();

*/
