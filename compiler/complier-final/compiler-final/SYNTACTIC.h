#pragma once
#include <cstring>
#include <iostream>
#include <string>
using namespace std;
#ifndef SYNTACTIC_H_INCLUDED
#define SYNTACTIC_H_INCLUDED

class SYNTACTIC {
   public:
    void yufafenxi();
    void Output_table();
    void Output_stack();
    void Output_remain();
    char Analysis_string[20];                      //分析栈
    char Remain_string[20];                        //剩余串
    char VN[20] = {'i', '+', '*', '(', ')', '#'};  //终结符
    char VT[20] = {'E', 'G', 'T', 'S', 'F'};       //非终结符
    typedef struct type {
        int len;
        char arr[5];
        char cap;
    } type;
    type ee, tt, gg, g1, ss, s1, fff, f1;
    type Table[10][10];  //预测分析表
    int j = 0, b = 0, top = 0, l;
    string Pro[10] = {"E->TG",  "G->+TG", "G->-TG", "G->ε", "T->FS", "S->*FS",
                      "S->/FS", "S->ε",   "F->(E)", "F->i"};  //产生式集合
};
//输出分析栈
void SYNTACTIC::Output_stack() {
    int a;
    for (a = 0; a <= top + 1; a++) printf("%c", Analysis_string[a]);
    printf("\t\t");
}

//输出剩余串
void SYNTACTIC::Output_remain() {
    int j;
    for (j = 0; j < b; j++) printf(" ");
    for (j = b; j <= l; j++) printf("%c", Remain_string[j]);
    printf("\t\t\t");
}

void SYNTACTIC::Output_table() {
    cout << "\t\t【本分析文法产生式为】" << endl;
    for (int i = 0; i < 10; i++) cout << "\t\t" << Pro[i] << endl;
    cout << "                        【LL(1)分析表】 " << endl;
    cout << "\t+\t-\t*\t/\t(\t)\ti\t#" << endl;
    cout << "E\t\t\t\t\tE->TG\t\tE->TG" << endl;
    cout << "T\t\t\t\t\tT->FS\t\tT->FS" << endl;
    cout << "G\tG->+TG\tG->-TG\t\t\t\tG->ε\t\tG->ε" << endl;
    cout << "F\t\t\t\t\tF->(E)\t\tF->i" << endl;
    cout << "S\tS->ε\tS->ε\tS->*FS\tS->/FS\t\tS->ε\t\tS->ε" << endl;
}
void SYNTACTIC::yufafenxi() {
    Output_table();
    int m, n, k = 0, flag = 0, fin = 0;
    char ch, x;
    type cha;
    ee.cap = 'E';
    strcpy(ee.arr, "TG");
    ee.len = 2;
    tt.cap = 'T';
    strcpy(tt.arr, "FS");
    tt.len = 2;
    gg.cap = 'G';
    strcpy(gg.arr, "+TG");
    gg.len    = 3;
    g1.cap    = 'G';
    g1.arr[0] = '^';
    g1.len    = 1;
    ss.cap    = 'S';
    strcpy(ss.arr, "*FS");
    ss.len    = 3;
    s1.cap    = 'S';
    s1.arr[0] = '^';
    s1.len    = 1;
    fff.cap   = 'F';
    strcpy(fff.arr, "(E)");
    fff.len   = 3;
    f1.cap    = 'F';
    f1.arr[0] = 'i';
    f1.len    = 1;
    for (m = 0; m <= 4; m++)
        for (n = 0; n <= 5; n++) Table[m][n].cap = 'N';
    Table[0][0] = ee;
    Table[0][3] = ee;
    Table[1][1] = gg;
    Table[1][4] = g1;
    Table[1][5] = g1;
    Table[2][0] = tt;
    Table[2][3] = tt;
    Table[3][1] = s1;
    Table[3][2] = ss;
    Table[3][4] = Table[3][5] = s1;
    Table[4][0]               = f1;
    Table[4][3]               = fff;
    printf("\n请输入由'i','+','*','(',')'构成的以'#'结束的字符串进行分析.\n\n");
    cout << "\t\t请输入：";
    getchar();
    do {
        scanf("%c", &ch);
        if ((ch != 'i') && (ch != '+') && (ch != '*') && (ch != '(') &&
            (ch != ')') && (ch != '#')) {
            printf("输入串中有非法字符\n");
            //            exit(1);
        }
        Remain_string[j] = ch;
        j++;
    } while (ch != '#');
    l                      = j;
    ch                     = Remain_string[0];
    Analysis_string[top]   = '#';
    Analysis_string[++top] = 'E';
    printf("\n步骤\t\t分析栈 \t\t剩余字符 \t\t所用产生式 \n");
    do {
        x = Analysis_string[top--];  // x为当前栈顶字符
        printf("%d", k++);
        printf("\t\t");
        for (j = 0; j <= 5; j++)  //判断是否为终结符
            if (x == VN[j]) {
                flag = 1;
                break;
            }
        if (flag == 1) {
            if (x == '#') {
                fin = 1;
                printf("acc!\n");
                //                getchar();getchar();
                //                exit(1);
            }
            if (x == ch) {
                Output_stack();
                Output_remain();
                printf("%c匹配\n", ch);
                ch   = Remain_string[++b];
                flag = 0;
            } else {
                Output_stack();
                Output_remain();
                printf("%c出错\n", ch);
                //                exit(1);
            }
        } else {
            for (j = 0; j <= 4; j++)
                if (x == VT[j]) {
                    m = j;
                    break;
                }
            for (j = 0; j <= 5; j++)
                if (ch == VN[j]) {
                    n = j;
                    break;
                }
            cha = Table[m][n];
            if (cha.cap != 'N') {
                Output_stack();
                Output_remain();
                printf("%c-", cha.cap);
                for (j = 0; j < cha.len; j++) printf("%c", cha.arr[j]);
                printf("\n");
                for (j = (cha.len - 1); j >= 0; j--)
                    Analysis_string[++top] = cha.arr[j];
                if (Analysis_string[top] == '^') top--;
            } else {
                Output_stack();Output_remain();
                printf("%c出错\n",x);
//                exit(1);
            }
        }
    } while (fin == 0);
}

#endif // SYNTACTIC_H_INCLUDED
