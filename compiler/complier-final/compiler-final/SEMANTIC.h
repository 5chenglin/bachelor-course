#pragma once
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#ifndef SEMANTIC_H_INCLUDED
#define SEMANTIC_H_INCLUDED


class SEMANTIC {
   public:
    void zhongjiandaima();
    void scaner1();
    void lrparser();
    void staBlock(int *nChain);    //语句块
    void staString(int *nChain);   //语句串
    void sta(int *nChain);         //语句
    void fuzhi();                  //赋值语句
    void tiaojian(int *nChain);    //条件语句
    void xunhuan();                //循环语句
    char *E();                     // Expresiion 表达式
    char *T();                     // Term项
    char *F();                     // Factor 因子
    char *newTemp();               //自动生成临时变量
    void backpatch(int p, int t);  //回填
    int merge(int p1, int p2);     //合并 p1 和 p2
    void emit(char *res, char *num1, char *op, char *num2);  //生成四元式
    void scanner();                                          //扫描
    char prog[80];        //存放所有输入字符
    char token[8];        //存放词组
    char ch;              //单个字符
    int syn, p, m, n, i;  // syn:种别编码
    double sum;
    int repeat;  //是否连续出现+,-
    int COunt;
    int isSignal;  //是否带正负号(0 不带，1 负号，2 正号）
    int isError;
    int isDecimal;   //是否是小数
    double decimal;  //小数
    int isExp;       //是否是指数
    int index;       //指数幂
    int isNegative;  //是否带负号
    double temp;
    int temp2;
    int nextq;
    int kk;  //临时变量的标号
    int ntc, nfc, nnc, nnb, nna;
    char *rwtab[9] = {"main", "int",  "float", "double", "char",
                      "if",   "else", "do",    "while"};
    struct {
        char result[10];  //字符串（字符数组）
        char arg1[10];
        char opera[10];
        char arg2[10];
    } fourCom[20];  //结构体数组
};

void SEMANTIC::lrparser() {
    int nChain;
    nfc = ntc = 1;
    nextq     = 1;
    if (syn == 1)  // main
    {
        scanner();
        if (syn == 26)  //(
        {
            scanner();
            if (syn == 27)  //)
            {
                scanner();
                staBlock(&nChain);
            } else
                printf("缺少右括号\n");
        } else
            printf("缺少左括号\n");
    } else
        printf("缺少 main\n");
}
//<语句块> ::= '{'<语句串>'}'
//语句块
void SEMANTIC::staBlock(int *nChain) {
    if (syn == 28)  //{
    {
        scanner();
        staString(nChain);
        // backpatch(*nChain,nextq);
        if (syn == 29)  //}
            scanner();  //读下一个
        else
            printf("缺少}号\n");
    } else
        printf("缺少{号\n");
}
//<语句串>::=<语句>{;<语句>};
//语句串
void SEMANTIC::staString(int *nChain) {
    sta(nChain);
    backpatch(*nChain, nextq);
    while (syn == 31)  //;
    {
        scanner();
        sta(nChain);
    }
    // backpatch(*nChain,nextq-1);
}
//语句
void SEMANTIC::sta(int *nChain) {
    if (syn == 10) {
        fuzhi();
        //*nChain=0;
    } else if (syn == 6) {  // if
        tiaojian(nChain);
    } else if (syn == 8) {
        // do
        xunhuan();
    }
}
//<条件语句>->if(<条件>)<语句块>
void SEMANTIC::tiaojian(int *nChain) {
    char res[10], num1[10], num2[10], op[10];
    int nChainTemp;
    //<条件>-><表达式><关系运算符><表达式>
    if (syn == 6)  // if
    {
        scanner();
        // strcpy(num1,E());
        if (syn == 26) {
            //(
            scanner();
            strcpy(num1, E());
            if ((syn <= 37) && (syn >= 32)) {
                switch (syn) {
                    case 32:
                        strcpy(op, ">");
                        break;
                    case 33:
                        strcpy(op, ">=");
                        break;
                    case 34:
                        strcpy(op, "<");
                        break;
                    case 35:
                        strcpy(op, "<=");
                        break;
                    case 36:
                        strcpy(op, "==");
                        break;
                    case 37:
                        strcpy(op, "!=");
                        break;
                    default:
                        printf("error");
                }
            }
            scanner();
            strcpy(num2, E());
            strcat(num1, op);
            strcat(num1, num2);
            // nfc=nextq+1;
            ntc = nextq;  //记住 if 语句位置
            emit("0", "if", num1, "goto");
            nfc = nextq;  // if 中表达式为假
            emit("0", "", "", "goto");
            //第一个 0 已回填
            backpatch(ntc, nextq);  // ntc 链接的所有四元式都回填 nextq
        }
        if (syn == 27) {  // )
            scanner();
        }
        staBlock(&nChainTemp);  //语句块
        *nChain = merge(nChainTemp, nfc);
    }
}
//<循环语句>::=do <语句块>while <条件>
void SEMANTIC::xunhuan() {
    char res[10], num1[10], num2[10], op[10];
    int nChainTemp;
    if (syn == 8) {
        // do
        nnc = nextq;  //记住 if 语句位置，emit 之后 nextq 就变了
        // emit("0","if",num1,"goto");
        scanner();
        staBlock(&nChainTemp);  //语句块
        if (syn == 9) {         // while
            scanner();
            if (syn == 26) {  //(
                scanner();
                strcpy(num1, E());
                if ((syn <= 37) && (syn >= 32)) {
                    switch (syn) {
                        case 32:
                            strcpy(op, ">");
                            break;
                        case 33:
                            strcpy(op, ">=");
                            break;
                        case 34:
                            strcpy(op, "<");
                            break;
                        case 35:
                            strcpy(op, "<=");
                            break;
                        case 36:
                            strcpy(op, "==");
                            break;
                        case 37:
                            strcpy(op, "!=");
                            break;
                        default:
                            printf("error");
                    }
                }
                scanner();
                strcpy(num2, E());
                strcat(num1, op);
                strcat(num1, num2);
                nnb = nextq;
                emit("0", "if", num1, "goto");
                backpatch(nnb, nnc);
                nna = nextq;
                emit("0", "", "", "goto");
                backpatch(nna, nextq);
            }
            if (syn == 27) {  // )

                scanner();
            }
        }
    }
}
void SEMANTIC::fuzhi()  // 赋值语句只有 1 个操作数
{
    char res[10], num[10];   // num 操作数
    if (syn == 10) {         // 字符串
        strcpy(res, token);  // 结果
        scanner();
        if (syn == 21) {  // =
            scanner();
            strcpy(num, E());
            emit(res, num, "=", "");
        } else {
            printf("缺少=号\n");
        }
    }
}
// Expression 表达式
char *SEMANTIC::E() {
    char *res, *num1, *op, *num2;
    res  = (char *) malloc(10);
    num1 = (char *) malloc(10);
    op   = (char *) malloc(10);
    num2 = (char *) malloc(10);
    strcpy(num1, T());
    while ((syn == 22) || (syn == 23)) {  // + -
        if (syn == 22)                    // +
            strcpy(op, "+");
        else
            strcpy(op, "-");
        scanner();
        strcpy(num2, T());
        strcpy(res, newTemp());
        emit(res, num1, op, num2);
        strcpy(num1, res);
    }
    return num1;
}
// Term项
char *SEMANTIC::T() {
    char *res, *num1, *op, *num2;
    res  = (char *) malloc(10);
    num1 = (char *) malloc(10);
    op   = (char *) malloc(10);
    num2 = (char *) malloc(10);
    strcpy(num1, F());
    while ((syn == 24) || (syn == 25)) {  // * /
        if (syn == 24)
            strcpy(op, "*");
        else
            strcpy(op, "/");
        scanner();
        strcpy(num2, F());
        strcpy(res, newTemp());
        emit(res, num1, op, num2);
        strcpy(num1, res);
    }
    return num1;
}
// Factor 因子
char *SEMANTIC::F() {
    char *res;
    res = (char *) malloc(10);
    if (syn == 10) {  // 字符串
        strcpy(res, token);
        scanner();
    } else if (syn == 20) {  // 二进制数
        // itoa((int) sum, res, 10);  // 整数转换为字符串
        int summ = sum + 0.5;
        //        cout<<"test***"<<sum<<"***test"<<endl;
        sprintf(res, "%d", summ);
        scanner();
    } else if (syn == 26) {  // (
        scanner();
        res = E();
        if (syn == 27) {  //  )
            scanner();
        } else
            isError = 1;
    } else
        isError = 1;
    return res;
}
char *SEMANTIC::newTemp() {
    char *p;
    char varTemp[10];
    p = (char *) malloc(10);
    kk++;
    //    cout<<"test***"<<kk<<"***test"<<endl;
    sprintf(varTemp, "%d", kk);

    //    itoa(kk, varTemp, 10);
    strcpy(p + 1, varTemp);
    p[0] = 'T';
    return p;
}
//将 p 所链接的每个四元式的第四个分量都回填t
void SEMANTIC::backpatch(int p, int t) {
    if (p > 20000) return;
    int w, circle = p;
    while (circle) {                       // circle 不为 0 的时候
        w = atoi(fourCom[circle].result);  //四元式 circle 第四分量内容
        // strcpy(fourCom[circle].result,t); //把 t 填进四元式 circle 的第四分量
        sprintf(fourCom[circle].result, "%d", t);
        circle = w;  // w 记录的是链条上下一个四元式，移动！
    }
    return;
}
//合并 p1 和 p2
int SEMANTIC::merge(int p1, int p2) {
    char circle, nResult;
    if (p2 == 0)
        nResult = p1;
    else {
        nResult = circle = p2;
        while (atoi(fourCom[circle].result)) {  // 四元式第四个分量不为 0
            circle = atoi(fourCom[circle].result);
            // strcpy(fourCom[circle].result,p1);
            sprintf(fourCom[circle].result, "%s", p1);
        }
        //目的是用 p1 的值覆盖 0
    }
    return nResult;  // p2 是头，p1 覆盖 0，接在 p2 后边
}
void SEMANTIC::emit(char *res, char *num1, char *op, char *num2) {
    strcpy(fourCom[nextq].result, res);
    strcpy(fourCom[nextq].arg1, num1);
    strcpy(fourCom[nextq].opera, op);
    strcpy(fourCom[nextq].arg2, num2);
    nextq++;
}

void SEMANTIC::scanner() {
    sum     = 0;
    decimal = 0;
    m       = 0;
    for (n = 0; n < 8; n++) token[n] = NULL;
    ch = prog[p++];                  // 从 prog 中读出一个字符到 ch 中
    while (ch == ' ' || ch == '\n')  // 跳过空字符（无效输入）
        ch = prog[p++];
    if (((ch >= 'a') && (ch <= 'z')) ||
        ((ch >= 'A') && (ch <= 'Z'))) {  // ch是字母字符
        while (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) ||
               ((ch >= '0') && (ch <= '9'))) {
            token[m++] = ch;         // ch=>token
            ch         = prog[p++];  // 读下一个字符
        }
        token[m++] = '\0';
        p--;       // 回退一格
        syn = 10;  // 标识符
        // 如果是"begin","if","then","while","do","end"标识符中的一个
        for (n = 0; n < 9; n++)
            if (strcmp(token, rwtab[n]) == 0) {
                syn = n + 1;
                break;
            }
    } else if ((ch >= '0') && (ch <= '9')) {
    IsNum:
        if (isSignal == 1) {
            // token[m++]='-';
        }
        while ((ch >= '0') && (ch <= '9')) {
            sum = sum * 10 + ch - '0';  // ch 中数字本身是当做字符存放的
            ch = prog[p++];
        }
        if (ch == '.') {
            isDecimal = 1;
            ch        = prog[p++];
            COunt = 0;  // 之前忘了清零,123.123+123.123#两个浮点数就无法识别
            while ((ch >= '0') && (ch <= '9')) {
                // pow(x,y)计算 x 的 y 次幂
                temp    = (ch - '0') * pow(0.1, ++COunt);
                decimal = decimal + temp;
                // AddToDec();
                ch = prog[p++];
            }
            sum = sum + decimal;
        }
        if (ch == 'e' || ch == 'E') {
            isExp = 1;
            ch    = prog[p++];
            if (ch == '-') {
                isNegative = 1;
                ch         = prog[p++];
            }
            while ((ch >= '0') && (ch <= '9')) {
                // 指数
                index = index * 10 + ch - '0';
                ch    = prog[p++];
            }
            // 10 的幂
            // 123e3 代表 123*10(3)
            // sum=sum*pow(10,index);是错误的
            if (isNegative)
                sum = sum * pow(0.1, index);
            else
                sum = sum * pow(10, index);
        }
        if (isSignal == 1) {
            sum      = -sum;
            isSignal = 0;
        }
        p--;
        syn = 20;
    } else
        switch (ch) {
            case '<':
                m          = 0;
                token[m++] = ch;
                ch         = prog[p++];
                if (ch == '=') {
                    syn        = 35;
                    token[m++] = ch;
                } else {
                    syn = 34;
                    p--;
                }
                break;
            case '>':
                m          = 0;
                token[m++] = ch;
                ch         = prog[p++];
                if (ch == '=') {
                    syn        = 33;
                    token[m++] = ch;
                } else {
                    syn = 32;
                    p--;
                }
                break;
            case '=':
                m          = 0;
                token[m++] = ch;
                ch         = prog[p++];
                if (ch == '=') {
                    syn        = 36;
                    token[m++] = ch;
                } else {
                    syn = 21;
                    p--;
                }
                break;
            case '+':
                temp2      = prog[p];
                token[m++] = ch;
                if ((temp2 >= '0') && (temp2 <= '9') && (repeat == 1)) {
                    isSignal = 2;
                    ch       = prog[p++];
                    repeat   = 0;
                    goto IsNum;
                }
                if (((temp2 == '+') || (temp2 == '-')) &&
                    (repeat ==
                     0)) {  // 如果重复出现符号，才将后边的+，-视为正负号
                    repeat = 1;
                    // ch=prog[p++];
                }
                syn = 22;
                break;
            case '-':
                temp2      = prog[p];
                token[m++] = ch;
                if ((temp2 >= '0') && (temp2 <= '9') && (repeat == 1)) {
                    isSignal = 1;
                    ch       = prog[p++];  // 读"-"下一个字符
                    repeat   = 0;
                    goto IsNum;  // 转到数字的识别
                }
                if (((temp2 == '+') || (temp2 == '-')) &&
                    (repeat == 0)) {  // 的+，-视为正负号
                    repeat = 1;       // 预言会重复
                    // ch=prog[p++]; // 读下一个字符
                }
                syn = 23;
                break;
            case '*':
                temp2      = prog[p];
                token[m++] = ch;
                if (temp2 == '+') {
                    isSignal = 2;
                    repeat   = 1;
                } else if (temp2 == '-') {
                    isSignal = 1;
                    repeat   = 1;
                }
                syn = 24;
                break;
            case '/':
                syn        = 25;
                token[m++] = ch;
                break;
            case '(':
                temp2      = prog[p];
                token[m++] = ch;
                if (temp2 == '+') {
                    isSignal = 2;
                    repeat   = 1;
                } else if (temp2 == '-') {
                    isSignal = 1;
                    repeat   = 1;
                }
                // 如果重复出现符号，才将后边
                syn = 26;
                break;
            case ')':
                syn        = 27;
                token[m++] = ch;
                break;
            case '{':
                syn        = 28;
                token[m++] = ch;
                break;
            case '}':
                syn        = 29;
                token[m++] = ch;
                break;
            case ',':
                syn        = 30;
                token[m++] = ch;
                break;
            case ';':
                syn        = 31;
                token[m++] = ch;
                break;
            case '#':
                syn        = 0;
                token[m++] = ch;
                break;
            default:
                syn = -1;
        }
}
void SEMANTIC::zhongjiandaima() {
    p         = 0;
    COunt     = 0;
    isDecimal = 0;
    index     = 0;
    repeat    = 0;
    kk        = 0;
    cout << "\t\t【语义分析 中间代码生成 目标代码分析】" << endl;
    cout << "\t\t请选择输入源程序的方式：\n\t\t(0) "
            "程序框输入：直接输入源代码。\n\t\t(1) "
            "文件输入：将源程序保存在[source_code.txt]文件中。\n\t\t(others) "
            "Exit."
         << endl;
    cout << "\t\t[warning]请确保以'#'结束！" << endl;
    int option = 1;
    cin >> option;
    switch (option) {
        case 0: {
            cout << "请输入：" << endl;
            do {
                ch        = getchar();
                prog[p++] = ch;
            } while (ch != '#');
            break;
        }
        case 1: {
            ifstream fin("source_code.txt");
            string str, sumstr;
            while (fin >> str) {
                sumstr += str;
                if (str[str.length() - 1] == '#') break;
            }
            for (int i = 0; i < sumstr.length(); i++) { prog[p++] = sumstr[i]; }
            break;
        }
        default: {
            cout << "\t\t[info]:程序已终止！" << endl;
            exit(0);
        }
    }
    //    printf("\nPlease input your source string:\n");
    //    do {
    //        ch        = getchar();
    //        prog[p++] = ch;
    //    } while (ch != '#');
    p       = 0;
    isError = 0;
    scanner();
    lrparser();
    cout << "\n\t\t\t【四元式结果】" << endl;
    for (i = 1; i < nextq; i++) {  // 循环输出四元式
        printf("\t\t[%d] (%3s, %3s ,%3s , %3s )\n", i, fourCom[i].opera,
               fourCom[i].arg1, fourCom[i].arg2, fourCom[i].result);
    }
    cout << "\n\t\t\t【目标代码】" << endl;
    for (i = 1; i < nextq; i++)  // 循环输出四元式
    {
        printf("\n\t[%d]\t", i);
        printf("(%3s, %3s ,%3s , %3s )\n", fourCom[i].opera, fourCom[i].arg1,
               fourCom[i].arg2, fourCom[i].result);
        if (strcmp(fourCom[i].opera, "=") == 0) {
            printf("\tMOV AX, %1s\n", fourCom[i].arg1);
            printf("\tMOV %s, AX\n", fourCom[i].result);
        }
        if (strcmp(fourCom[i].opera, "+") == 0) {
            printf("\tMOV AX, %1s\n", fourCom[i].arg1);
            printf("\tADD AX, %1s\n", fourCom[i].arg2);
            printf("\tMOV %1s, AX\n", fourCom[i].result);
        }
        if (strcmp(fourCom[i].opera, "-") == 0) {
            printf("\tMOV AX, %1s\n", fourCom[i].arg1);
            printf("\tSUB AX, %1s\n", fourCom[i].arg2);
            printf("\tMOV %1s, AX\n", fourCom[i].result);
        }
        if (strcmp(fourCom[i].opera, "*") == 0) {
            printf("\tMOV AL, %1s\n", fourCom[i].arg1);
            printf("\tMUL %1s\n", fourCom[i].arg2);
            printf("\tMOV %1s, AX\n", fourCom[i].result);
        }
        if (strcmp(fourCom[i].opera, "/") == 0) {
            printf("\tMOV AX, %1s\n", fourCom[i].arg1);
            printf("\tDIV %1s\n", fourCom[i].arg2);
            printf("\tMOV %1s, AL\n", fourCom[i].result);
        }
        if (strcmp(fourCom[i].arg2, "goto") == 0) {
            if (strcmp(fourCom[i].arg1, "if") == 0) {
                printf("\tCMP %1s\n", fourCom[i].opera);
                printf("\tJNC %1s\n", fourCom[i].result);
            } else {
                printf("\tJMP %1s\n", fourCom[i].result);
            }
        }
    }
    return;
}

#endif // SEMANTIC_H_INCLUDED
