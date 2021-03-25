#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <map>
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
    int chain_no;   //圈数
    vector<int>chain;   //每个圈的字母数
    char init_r1;        //初始转子t1位置
    char init_r2;        //初始转子t2位置
    char init_r3;        //初始转子t3位置
    char select_roll[3]; //选择的转子顺序
};

bool cmp(Table a,Table b){
    return a.n<b.n;
}
bool cmp2(Roll x,Roll y){
    return x.chain_no < y.chain_no;
}

//转子1：table1.  转子2：table2.  转子3：table3.
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
map<char,char> retable;// 反射器
map<char,char> board;// 插线板
vector<char>encTxt;     //明文数据
vector<char>decTxt;     //对应密文数据
vector<Roll>Dict_Roll(vector<Roll>(105456));
int Tree[26],Node[26];

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

int findRoot(int x){
    if(Tree[x]==-1)return x;
    else{
        int tmp = findRoot(Tree[x]);
        Tree[x]=tmp;
        Node[tmp] += Node[x];
        return tmp;
    }
}

char exchTab1(vector<Table>table,char c){ // 单表代替
    return table[c-'a'].c;
}
char exchTab2(vector<Table>table,char c){ // 单表代替
    for(int i=0;i<table.size();i++){
        if(table[i].c == c){
            return 'a'+i;
        }
    }
}

void roveSigle(vector<Table>&tab){           //转子转动
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

bool crack(char c,char en,char s1,char s2,char s3,char t1,char t2,char t3){        //破解程序，输入转子顺序（s1,s2,s3）与转子位置(t1,t2,t3),输出密钥
    bool res = false;
    vector<Table>tab1;
    vector<Table>tab2;
    vector<Table>tab3;
    if(s1 == '1'&& s2 == '2' && s3 == '3'){
        tab1.assign(table1.begin(),table1.end());
        tab2.assign(table2.begin(),table2.end());
        tab3.assign(table3.begin(),table3.end());
    }else if(s1 == '2'&& s2 == '1' && s3 == '3'){
        tab1.assign(table2.begin(),table2.end());
        tab2.assign(table1.begin(),table1.end());
        tab3.assign(table3.begin(),table3.end());
    }else if(s1 == '2'&& s2 == '3' && s3 == '1'){
        tab1.assign(table2.begin(),table2.end());
        tab2.assign(table3.begin(),table3.end());
        tab3.assign(table1.begin(),table1.end());
    }else if(s1 == '3'&& s2 == '1' && s3 == '2'){
        tab1.assign(table3.begin(),table3.end());
        tab2.assign(table1.begin(),table1.end());
        tab3.assign(table2.begin(),table2.end());
    }else if(s1 == '3'&& s2 == '2' && s3 == '1'){
        tab1.assign(table3.begin(),table3.end());
        tab2.assign(table2.begin(),table2.end());
        tab3.assign(table1.begin(),table1.end());
    }else if(s1 == '1'&& s2 == '3' && s3 == '2'){
        tab1.assign(table1.begin(),table1.end());
        tab2.assign(table3.begin(),table3.end());
        tab3.assign(table2.begin(),table2.end());
    }
    initRov(tab1,t1);//设置转子初值
    initRov(tab2,t2);
    initRov(tab3,t3);

    char test=encr(c,tab1,tab2,tab3,retable,board,t1,t2,t3);
    roveAll(tab1,tab2,tab3,t1,t2,t3);//加密一个字母，转子转动一次
    roveAll(tab1,tab2,tab3,t1,t2,t3);
    roveAll(tab1,tab2,tab3,t1,t2,t3);
    test=encr(test,tab1,tab2,tab3,retable,board,t1,t2,t3);
    if(en==test)res=true;
    return res;
}

vector<int> countRoll(vector<char>tab){
    for(int i=0;i<26;i++)Tree[i]=-1;    //辅助数组计算链中的圈
    for(int i=0;i<26;i++)Node[i]=1;     //辅助数组计算每个圈的字母数
    vector<int>roll;                    //记录每个链的圈
    for(int i=0;i<tab.size();i++){
        int a = i;
        int b = tab[i] - 'a';
        a = findRoot(a);
        b = findRoot(b);
        if(a!=b){
            Tree[a]=b;
        }
    }
    for(int i=0;i<26;i++){
        if(Tree[i]==-1){
            roll.push_back(Node[i]);
        }
    }
    sort(roll.begin(),roll.end(),greater<int>());
    return roll;
}

void out_file(vector<Roll>res){
    FILE *fin;                      //将所有可能的密钥输出到search.txt文件中，等待下一步暴力破解
    fopen_s(&fin,"search.txt","wt+");
    for(int i=0;i<res.size();i++){
        fprintf(fin,"%d %c %c %c %c %c %c ",res[i].chain_no,
                res[i].select_roll[0],res[i].select_roll[1],res[i].select_roll[2],
                res[i].init_r1,res[i].init_r2,res[i].init_r3);
        for(int j=0;j<res[i].chain.size();j++){
            fprintf(fin,"%d ",res[i].chain[j]);
        }
        fprintf(fin,"\n");
    }
    fclose(fin);
}

vector<vector<char>> select_filter(vector<int>roll){
    FILE *fp;
    fopen_s(&fp,"rollMap1.txt","r+");
    int index=0;
    while(!feof(fp)){
        int x;
        char t1,t2,t3,s1,s2,s3;
        fscanf(fp,"%d %c %c %c %c %c %c ",&x,&s1,&s2,&s3,&t1,&t2,&t3);
        Dict_Roll[index].chain_no=x;
        Dict_Roll[index].select_roll[0]=s1;
        Dict_Roll[index].select_roll[1]=s2;
        Dict_Roll[index].select_roll[2]=s3;
        Dict_Roll[index].init_r1=t1;
        Dict_Roll[index].init_r2=t2;
        Dict_Roll[index].init_r3=t3;

        for(int i = 0;i<Dict_Roll[index].chain_no;i++){
            int d;
            fscanf(fp,"%d ",&d);
            Dict_Roll[index].chain.push_back(d);
        }
        fscanf(fp,"\n");
        index++;
    }
    fclose(fp);
    sort(Dict_Roll.begin(),Dict_Roll.end(),cmp2);
    vector<vector<char>>res;
    for(int i=0;i<index;i++){       //输入圈数，每个圈的字母数
        if(Dict_Roll[i].chain_no == roll.size()){
            bool flag = true;
            for(int j = 0;j<Dict_Roll[i].chain_no;j++){
                if(Dict_Roll[i].chain[j]!=roll[j]){
                    flag = false;
                    break;
                }
            }
            if(flag == true){
                vector<char>temp;
                temp.push_back(Dict_Roll[i].select_roll[0]);
                temp.push_back(Dict_Roll[i].select_roll[1]);
                temp.push_back(Dict_Roll[i].select_roll[2]);
                temp.push_back(Dict_Roll[i].init_r1);
                temp.push_back(Dict_Roll[i].init_r2);
                temp.push_back(Dict_Roll[i].init_r3);
                res.push_back(temp);
                //out_file(Dict_Roll[i]);
            }
        }
    }
    cout<<"匹配圈子的数量总共有："<<res.size()<<endl;
    return res;
}



int main()
{
    for(int i=0;i<26;i++){                      //反射器设置
        retable.insert(pair<char,char>('a'+i,'a'+(i+13)%26));
    }
    //      插线板设置
    board['a'] = 'b';board['b'] = 'a';board['c'] = 'd';board['d'] = 'c';board['e'] = 'f';board['f'] = 'e';board['g'] = 'g';
    board['h'] = 'o';board['i'] = 'p';board['j'] = 'q';board['k'] = 'r';board['l'] = 's';board['m'] = 't';board['n'] = 'n';
    board['o'] = 'h';board['p'] = 'i';board['q'] = 'j';board['r'] = 'k';board['s'] = 'l';board['t'] = 'm';board['u'] = 'u';
    board['v'] = 'z';board['w'] = 'w';board['x'] = 'x';board['y'] = 'y';board['z'] = 'v';

    //   通过每日收集的6个日密钥得到的26个字母的转换表
    vector<char>cc1 = {'m','f','l','w','p','x','s','i','h','a','n','g','b','v','e','u','z','c','y','o','j','q','d','t','k','r'};//321zzz
    vector<char>cc2 = {'c','b','o','q','d','x','w','s','f','v','n','i','p','r','y','z','k','a','e','g','u','m','j','t','h','l'};//123zaa
    vector<char>cc3 = {'g','t','o','e','b','s','x','p','z','l','c','u','h','y','k','r','v','f','m','d','w','j','q','a','i','n'};//123zzz
    vector<char>cc4 = {'o','v','m','c','n','r','f','g','u','x','e','p','h','y','s','d','k','l','w','j','b','q','a','z','i','t'};//123aaa
    vector<char>cc5 = {'z','o','v','d','c','m','w','b','k','e','l','j','u','n','g','h','y','t','s','i','p','r','f','x','q','a'};//132aaa
    vector<char>cc6 = {'v','i','y','z','k','e','w','q','o','r','p','u','s','d','m','n','c','a','x','b','f','l','h','g','t','j'};//213aaa
    vector<char>cc7 = {'m','r','o','p','v','c','g','k','q','d','w','u','f','b','a','x','l','s','e','n','z','t','j','h','y','i'};//231aaa
    vector<char>cc8 = {'e','j','c','q','w','o','u','h','i','r','k','b','s','g','z','l','a','p','f','t','y','n','d','x','v','m'};//312aaa
    vector<char>cc9 = {'c','u','s','b','v','l','f','p','j','g','t','z','i','k','r','q','h','x','a','y','n','m','e','d','o','w'};//321aaa
    vector<char>cc = {'l','d','p','x','i','f','b','k','a','z','j','o','g','c','s','u','w','h','t','m','q','r','v','e','y','n'};//321zaa


    vector<int>cal_roll = countRoll(cc);
    cout<<"圈子：";
    for(int i=0;i<cal_roll.size();i++)cout<<cal_roll[i]<<" ";cout<<endl;
    vector<vector<char>>res_table = select_filter(cal_roll);

    for(int i=0;i<res_table.size();i++){
        int flag = true;
        char s1 = res_table[i][0];
        char s2 = res_table[i][1];
        char s3 = res_table[i][2];
        char t1 = res_table[i][3];
        char t2 = res_table[i][4];
        char t3 = res_table[i][5];
        for(int j = 0;j<26;j++){
            char c = 'a' + j;
            char en = cc[j];
            if(!crack(c,en,s1,s2,s3,t1,t2,t3)){
                flag = false;
                break;
            }
        }
        if(flag == true){
            cout<<"破解Enigma密钥 :"<<endl;
            cout<<"转子顺序： "<<s1<<" "<<s2<<" "<<s3<<endl;
            cout<<"转子位置： "<<t1<<" "<<t2<<" "<<t3<<endl;
        }
    }
    return 0;
}
/*
测试数据集：
{'c','b','o','q','d','x','w','s','f','v','n','i','p','r','y','z','k','a','e','g','u','m','j','t','h','l'}; 密钥：1 2 3 z a a
{'m','f','l','w','p','x','s','i','h','a','n','g','b','v','e','u','z','c','y','o','j','q','d','t','k','r'};密钥：3 2 1 z z z
{'g','t','o','e','b','s','x','p','z','l','c','u','h','y','k','r','v','f','m','d','w','j','q','a','i','n'};密钥：1 2 3 z z z
*/
/*
FILE *fp;
    fopen_s(&fp,"search.txt","r+");
    int index=0;
    vector<vector<char>>res_table;
    while(!feof(fp)){
        int x;
        char t1,t2,t3,s1,s2,s3;
        fscanf(fp,"%d %c %c %c %c %c %c ",&x,&s1,&s2,&s3,&t1,&t2,&t3);
        vector<char>tmp;
        tmp.push_back(s1);
        tmp.push_back(s2);
        tmp.push_back(s3);
        tmp.push_back(t1);
        tmp.push_back(t2);
        tmp.push_back(t3);
        res_table.push_back(tmp);

        for(int i = 0;i<x;i++){
            int d;
            fscanf(fp,"%d ",&d);
        }
        fscanf(fp,"\n");
        index++;
    }
    fclose(fp);

    cout<<index<<endl;
    cout<<res_table.size()<<endl;

*/
