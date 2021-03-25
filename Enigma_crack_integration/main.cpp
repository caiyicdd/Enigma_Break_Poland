#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <map>

using namespace std;

struct Table{       //   转子的数据结构
    char c;
    int n;
    Table(){}
    Table(char c,int n):
        c(c),n(n){}
};

bool cmp(Table a,Table b){      //用于对转子排序
    return a.n<b.n;
}

vector<Table> gener(){          // 生成随机转子
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

void printT(vector<Table>tab){      //打印转子
    for(int i=0;i<tab.size();i++){
        cout<<tab[i].c<<" ";
    }
    cout<<endl;
}

char exchTab1(vector<Table>table,char c){ // 单表代替  正向
    return table[c-'a'].c;
}
char exchTab2(vector<Table>table,char c){ // 单表代替  反向
    for(int i=0;i<table.size();i++){
        if(table[i].c == c){
            return 'a'+i;
        }
    }
}

void roveSigle(vector<Table>&tab){           //  单个转子转动
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
                                            //  所有转子按规则转
void roveAll(vector<Table>&table1,vector<Table>&table2,
             vector<Table>&table3,char& t1,char& t2,char& t3){
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
                                            //  加密数据
char encr(char c,vector<Table>table1,vector<Table>table2,vector<Table>table3,
          map<char,char>retable,map<char,char>board,char& t1,char& t2,char& t3){
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
                                            // 初始化转子位置
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
int main()
{
    vector<Table>table1;
    vector<Table>table2;
    vector<Table>table3;
    vector<Table> tab1 ={Table('v',1),Table('e',1),Table('t',1),Table('a',1),Table('b',1),Table('z',1),
                            Table('k',1),Table('x',1),Table('u',1),Table('i',1),Table('q',1),Table('s',1),
                            Table('f',1),Table('j',1),Table('d',1),Table('l',1),Table('w',1),Table('h',1),
                            Table('o',1),Table('c',1),Table('n',1),Table('m',1),Table('y',1),Table('r',1),
                            Table('g',1),Table('p',1)};
    vector<Table> tab2 ={Table('k',1),Table('c',1),Table('u',1),Table('m',1),Table('w',1),Table('i',1),
                            Table('g',1),Table('p',1),Table('s',1),Table('j',1),Table('o',1),Table('n',1),
                            Table('h',1),Table('f',1),Table('t',1),Table('x',1),Table('l',1),Table('e',1),
                            Table('y',1),Table('b',1),Table('v',1),Table('r',1),Table('q',1),Table('a',1),
                            Table('z',1),Table('d',1)};
    vector<Table> tab3 ={Table('k',1),Table('h',1),Table('s',1),Table('c',1),Table('x',1),Table('b',1),
                            Table('l',1),Table('n',1),Table('z',1),Table('e',1),Table('i',1),Table('y',1),
                            Table('w',1),Table('f',1),Table('o',1),Table('u',1),Table('q',1),Table('j',1),
                            Table('v',1),Table('r',1),Table('t',1),Table('p',1),Table('a',1),Table('g',1),
                            Table('d',1),Table('m',1)};

    map<char,char> retable;// 反射器
    for(int i=0;i<26;i++){
        retable.insert(pair<char,char>('a'+i,'a'+(i+13)%26));
    }
    map<char,char> board;// 插线板
    board['a'] = 'b';board['b'] = 'a';board['c'] = 'd';board['d'] = 'c';board['e'] = 'f';board['f'] = 'e';board['g'] = 'g';
    board['h'] = 'o';board['i'] = 'p';board['j'] = 'q';board['k'] = 'r';board['l'] = 's';board['m'] = 't';board['n'] = 'n';
    board['o'] = 'h';board['p'] = 'i';board['q'] = 'j';board['r'] = 'k';board['s'] = 'l';board['t'] = 'm';board['u'] = 'u';
    board['v'] = 'z';board['w'] = 'w';board['x'] = 'x';board['y'] = 'y';board['z'] = 'v';
    char t1,t2,t3;//初始转子位置，初始密钥
    char s1,s2,s3;//初始转子顺序
    cout<<"输入转子顺序（如：1 2 3）:";
    cin>>s1>>s2>>s3;
    cout<<"输入转子位置（如：a b z）:";
    cin>>t1>>t2>>t3;
    //  转子初始化
    if(s1 == '1'&& s2 == '2' && s3 == '3'){
        table1.assign(tab1.begin(),tab1.end());
        table2.assign(tab2.begin(),tab2.end());
        table3.assign(tab3.begin(),tab3.end());
    }else if(s1 == '2'&& s2 == '1' && s3 == '3'){
        table1.assign(tab2.begin(),tab2.end());
        table2.assign(tab1.begin(),tab1.end());
        table3.assign(tab3.begin(),tab3.end());
    }else if(s1 == '2'&& s2 == '3' && s3 == '1'){
        table1.assign(tab2.begin(),tab2.end());
        table2.assign(tab3.begin(),tab3.end());
        table3.assign(tab1.begin(),tab1.end());
    }else if(s1 == '3'&& s2 == '1' && s3 == '2'){
        table1.assign(tab3.begin(),tab3.end());
        table2.assign(tab1.begin(),tab1.end());
        table3.assign(tab2.begin(),tab2.end());
    }else if(s1 == '3'&& s2 == '2' && s3 == '1'){
        table1.assign(tab3.begin(),tab3.end());
        table2.assign(tab2.begin(),tab2.end());
        table3.assign(tab1.begin(),tab1.end());
    }else if(s1 == '1'&& s2 == '3' && s3 == '2'){
        table1.assign(tab1.begin(),tab1.end());
        table2.assign(tab3.begin(),tab3.end());
        table3.assign(tab2.begin(),tab2.end());
    }
    initRov(table1,t1);//设置转子初值
    initRov(table2,t2);
    initRov(table3,t3);

    char op;
    cout<<"加解密输入 1 / 生成测试数据输入 2  请输入:";
    cin>>op;
    if(op=='1'){
        string in;
        while(true){
                cout<<"输入要加密的内容:";
                cin>>in;
                const char *c = in.c_str();
                for(int j=0;c[j]!='\0';j++){
                    char en=encr(c[j],table1,table2,table3,retable,board,t1,t2,t3);
                    roveAll(table1,table2,table3,t1,t2,t3);//加密一个字母，转子转动一次
                    cout<<en;
                }
                cout<<endl;
            }
    }else if(op == '2'){
        vector<char>res(26);

        for(int i=0;i<26;i++){
            vector<Table>temp1;
            vector<Table>temp2;
            vector<Table>temp3;
            char t11=t1;
            char t22=t2;
            char t33=t3;
            temp1.assign(table1.begin(),table1.end());
            temp2.assign(table2.begin(),table2.end());
            temp3.assign(table3.begin(),table3.end());
            char c = 'a' + i;
            char en = encr(c,temp1,temp2,temp3,retable,board,t11,t22,t33);  //得到X(A0)(A3)Y中的X
            roveAll(temp1,temp2,temp3,t11,t22,t33);
            roveAll(temp1,temp2,temp3,t11,t22,t33);
            roveAll(temp1,temp2,temp3,t11,t22,t33);
            char de = encr(c,temp1,temp2,temp3,retable,board,t11,t22,t33);
            res[en-'a']=de;
        }
        for(int i = 0;i<26;i++){
            cout<<"'"<<res[i]<<"',";
        }
        cout<<endl;
    }

    return 0;
}
