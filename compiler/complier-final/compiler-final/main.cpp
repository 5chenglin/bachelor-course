#include <bits/stdc++.h>
#include "SYNTACTIC.h"
#include "SEMANTIC.h"
#include "LEXICAL.h"
using namespace std;

int main() {
    while (true) {
        printf(" \n");
        printf("##########################################################\n");
        printf(" (1)--------词法分析\n");
        printf(" (2)--------语法分析\n");
        printf(" (3)--------中间代码生成和目标代码生成\n");
        printf(" (0)--------退出\n");
        printf("###########################################################\n");
        int r1 = 0;
        scanf("%d", &r1);
        switch (r1) {
            case 0:
                exit(0);
            case 1:{
                LEXICAL work;
                 work.cifafenxi();
                break;
            }
            case 2:{
                SYNTACTIC work;
                work.yufafenxi();
                break;
            }
            case 3:{
                SEMANTIC work;
                work.zhongjiandaima();
                break;
            }
            default:
                printf("error,please input again");
                break;
        }
    }
}
