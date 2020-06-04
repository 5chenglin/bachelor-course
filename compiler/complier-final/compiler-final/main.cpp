#include <bits/stdc++.h>
#include <unistd.h>
#include <windows.h>
#include "LEXICAL.h"
#include "SEMANTIC.h"
#include "SYNTACTIC.h"
using namespace std;

int main() {
    while (true) {
        system("cls");
        cout << "\t\t【Mini C 编译器】" << endl;
        printf("\t===========================================\n");
        printf("\t(1)--------词法分析\n");
        printf("\t(2)--------语法分析\n");
        printf("\t(3)--------语义分析 中间代码生成 目标代码分析\n");
        printf("\t(0)--------退出\n");
        printf("\t===========================================\n");
        int r1 = 0;
        scanf("%d", &r1);
        switch (r1) {
            case 0:
                exit(0);
            case 1: {
                LEXICAL work;
                work.cifafenxi();
                break;
            }
            case 2: {
                SYNTACTIC work;
                work.yufafenxi();
                break;
            }

            case 3: {
                SEMANTIC work;
                work.zhongjiandaima();
                break;
            }
            default: {
                printf("[warning]输入错误，请重新输入！程序将会自动重新加载。");
                Sleep(2000);
                break;
            }
        }
    }
}
/*
main()
{
    if(x>3)
    {
        x=x+i;
        y=y*k;
    }
}#
*/
/*
begin x:=1; y:=1+2;end #
*/

/*
main(){
    x = x+3;
    y = y-x;
    j = i/4;
    c = a*b;
}#
*/
