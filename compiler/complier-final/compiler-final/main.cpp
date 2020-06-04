#include<bits/stdc++.h>
using namespace std;
#define MAX 100
char inputstream[50]; //存储输入句子
int temp1=0; //数组下标
int right1; //判断输出信息
int m2=0,sum2=0;//sum 用于计算运算符的个数
//m 用于标记输入表达式中字符的个数
char JG='A';
char str[MAX];//用于存输入表达式
int tokene=0;//左括号的标志
char prog1[80],token1[8],ch1;
int syn1,p1,m1,n1,sum1;
char *rwtab1[6]= {"begin","if","then","while","do","end"};
int r1 ;
char prog[80]; //存放所有输入字符
char token[8]; //存放词组
char ch; //单个字符
int syn,p,m,n,i; //syn:种别编码
double sum;
int COunt;
int isSignal; //是否带正负号(0 不带，1 负号，2 正号）
int isError;
int isDecimal; //是否是小数
double decimal; //小数
int isExp; //是否是指数
int index; //指数幂
int isNegative; //是否带负号
double temp;
int temp2;
int repeat; //是否连续出现+,-
int nextq;
int kk; //临时变量的标号
int ntc,nfc,nnc,nnb,nna;
char *rwtab[9]= {"main","int","float","double","char","if","else","do","while"};
struct
{
    char result[10]; //字符串（字符数组）
    char arg1[10];
    char opera[10];
    char arg2[10];
} fourCom[20]; //结构体数组
void cifafenxi();
void yufafenxi();
void zhongjiandaima();
void scaner1();
void e();
void e1();
void t();
void t1();
void f();
void lrparser();
void staBlock(int *nChain); //语句块
void staString(int *nChain); //语句串
void sta(int *nChain); //语句
void fuzhi(); //赋值语句
void tiaojian(int *nChain); //条件语句
void xunhuan(); //循环语句
char* E(); //Expresiion 表达式
char* T(); //Term项
char* F(); //Factor 因子
char *newTemp(); //自动生成临时变量
void backpatch(int p,int t); //回填
int merge(int p1,int p2); //合并 p1 和 p2
void emit(char *res,char *num1,char *op,char *num2); //生成四元式
void scanner(); //扫描
void lrparser()
{
    int nChain;
    nfc=ntc=1;
    nextq=1;
    if(syn==1) //main
    {
        scanner();
        if(syn==26) //(
        {
            scanner();
            if(syn==27) //)
            {
                scanner();
                staBlock(&nChain);
            }
            else
                printf("缺少右括号\n");
        }
        else
            printf("缺少左括号\n");
    }
    else
        printf("缺少 main\n");
}
//<语句块> ::= '{'<语句串>'}'
void staBlock(int *nChain) //语句块
{
    if(syn==28) //{
    {
        scanner();
        staString(nChain);
//backpatch(*nChain,nextq);
        if(syn==29) //}
            scanner(); //读下一个
        else
            printf("缺少}号\n");
    }
    else
        printf("缺少{号\n");
}
//<语句串>::=<语句>{;<语句>};
void staString(int *nChain) //语句串
{
    sta(nChain);
    backpatch(*nChain,nextq);
    while(syn==31) //;
    {
        scanner();
        sta(nChain);
    }
//backpatch(*nChain,nextq-1);
}
void sta(int *nChain) //语句
{
    if(syn==10)
    {
        fuzhi();
//*nChain=0;
    }
    else if(syn==6) //if
    {
        tiaojian(nChain);
    }
    else if(syn==8) //do
        xunhuan();
}
//<条件语句>->if(<条件>)<语句块>
void tiaojian(int *nChain)
{
    char res[10],num1[10],num2[10],op[10];
    int nChainTemp;
//<条件>-><表达式><关系运算符><表达式>
    if(syn==6) //if
    {
        scanner();
//strcpy(num1,E());
        if(syn==26) //(
        {
            scanner();
            strcpy(num1,E());
            if((syn<=37)&&(syn>=32))
            {
                switch(syn)
                {
                case 32:
                    strcpy(op,">");
                    break;
                case 33:
                    strcpy(op,">=");
                    break;
                case 34:
                    strcpy(op,"<");
                    break;
                case 35:
                    strcpy(op,"<=");
                    break;
                case 36:
                    strcpy(op,"==");
                    break;
                case 37:
                    strcpy(op,"!=");
                    break;
                default:
                    printf("error");
                }
            }
            scanner();
            strcpy(num2,E());
            strcat(num1,op);
            strcat(num1,num2);
//nfc=nextq+1;
            ntc=nextq; //记住 if 语句位置
            emit("0","if",num1,"goto");
            nfc=nextq; //if 中表达式为假
            emit("0","","","goto");
//第一个 0 已回填
            backpatch(ntc,nextq); //ntc 链接的所有四元式都回填 nextq
        }
        if(syn==27) //)
            scanner();
        staBlock(&nChainTemp); //语句块
        *nChain=merge(nChainTemp,nfc);
    }
}
//<循环语句>::=do <语句块>while <条件>
void xunhuan()
{
    char res[10],num1[10],num2[10],op[10];
    int nChainTemp;
    if(syn==8) //do
    {
        nnc=nextq; //记住 if 语句位置，emit 之后 nextq 就变了
//emit("0","if",num1,"goto");
        scanner();
        staBlock(&nChainTemp); //语句块
        if(syn==9) //while
        {
            scanner();
            if(syn==26) //(
            {
                scanner();
                strcpy(num1,E());
                if((syn<=37)&&(syn>=32))
                {
                    switch(syn)
                    {
                    case 32:
                        strcpy(op,">");
                        break;
                    case 33:
                        strcpy(op,">=");
                        break;
                    case 34:
                        strcpy(op,"<");
                        break;
                    case 35:
                        strcpy(op,"<=");
                        break;
                    case 36:
                        strcpy(op,"==");
                        break;
                    case 37:
                        strcpy(op,"!=");
                        break;
                    default:
                        printf("error");
                    }
                }
                scanner();
                strcpy(num2,E());
                strcat(num1,op);
                strcat(num1,num2);
                nnb=nextq;
                emit("0","if",num1,"goto");
                backpatch(nnb,nnc);
                nna=nextq;
                emit("0","","","goto");
                backpatch(nna,nextq);
            }
            if(syn==27) //)
                scanner();
        }
    }
}
void fuzhi() //赋值语句只有 1 个操作数
{
    char res[10],num[10]; //num 操作数
    if(syn==10) //字符串
    {
        strcpy(res,token); //结果
        scanner();
        if(syn==21) //=
        {
            scanner();
            strcpy(num,E());
            emit(res,num,"=","");
        }
        else
        {
            printf("缺少=号\n");
        }
    }
}
char* E() //Expression 表达式
{
    char *res,*num1,*op,*num2;
    res=(char *)malloc(10);
    num1=(char *)malloc(10);
    op=(char *)malloc(10);
    num2=(char *)malloc(10);
    strcpy(num1,T());
    while((syn==22)||(syn==23)) //+ -
    {
        if(syn==22) //+
            strcpy(op,"+");
        else
            strcpy(op,"-");
        scanner();
        strcpy(num2,T());
        strcpy(res,newTemp());
        emit(res,num1,op,num2);
        strcpy(num1,res);
    }
    return num1;
}
char* T() //Term项
{
    char *res,*num1,*op,*num2;
    res=(char *)malloc(10);
    num1=(char *)malloc(10);
    op=(char *)malloc(10);
    num2=(char *)malloc(10);
    strcpy(num1,F());
    while((syn==24)||(syn==25)) //* /
    {
        if(syn==24)
            strcpy(op,"*");
        else
            strcpy(op,"/");
        scanner();
        strcpy(num2,F());
        strcpy(res,newTemp());
        emit(res,num1,op,num2);
        strcpy(num1,res);
    }
    return num1;
}
char* F() //Factor 因子
{
    char *res;
    res=(char *)malloc(10);
    if(syn==10) //字符串
    {
        strcpy(res,token);
        scanner();
    }
    else if(syn==20) //二进制数
    {
        itoa((int)sum,res,10); //整数转换为字符串
        scanner();
    }
    else if(syn==26) //(
    {
        scanner();
        res=E();
        if(syn==27) //)
        {
            scanner();
        }
        else isError=1;
    }
    else
        isError=1;
    return res;
}
char *newTemp()
{
    char *p;
    char varTemp[10];
    p=(char *)malloc(10);
    kk++;
    itoa(kk,varTemp,10);
    strcpy(p+1,varTemp);
    p[0]='T';
    return p;
}
//将 p 所链接的每个四元式的第四个分量都回填t
void backpatch(int p,int t)
{
    if(p>100000)
    return;
    int w,circle=p;
    while(circle) //circle 不为 0 的时候
    {
        w=atoi(fourCom[circle].result); //四元式 circle 第四分量内容
//strcpy(fourCom[circle].result,t); //把 t 填进四元式 circle 的第四分量
        sprintf(fourCom[circle].result,"%d",t);
        circle=w; //w 记录的是链条上下一个四元式，移动！
    }
    return;
}
int merge(int p1,int p2) //合并 p1 和 p2
{
    char circle,nResult;
    if(p2==0)
        nResult=p1;
    else
    {
        nResult=circle=p2;
        while(atoi(fourCom[circle].result)) //四元式第四个分量不为 0
        {
            circle=atoi(fourCom[circle].result);
//strcpy(fourCom[circle].result,p1);
            sprintf(fourCom[circle].result,"%s",p1);
        }
//目的是用 p1 的值覆盖 0
    }
    return nResult; //p2 是头，p1 覆盖 0，接在 p2 后边
}
void emit(char *res,char *num1,char *op,char *num2)
{
    strcpy(fourCom[nextq].result,res);
    strcpy(fourCom[nextq].arg1,num1);
    strcpy(fourCom[nextq].opera,op);
    strcpy(fourCom[nextq].arg2,num2);
    nextq++;
}
void scanner()
{
    sum=0;
    decimal=0;
    m=0;
    for(n=0; n<8; n++)
        token[n]=NULL;
    ch=prog[p++]; //从 prog 中读出一个字符到 ch 中
    while(ch==' '||ch=='\n') //跳过空字符（无效输入）
        ch=prog[p++];
    if(((ch>='a')&&(ch<='z'))||((ch>='A')&&(ch<='Z'))) //ch是字母字符
    {
        while(((ch>='a')&&(ch<='z'))||((ch>='A')&&(ch<='Z'))||((ch>='0')&&(ch<='9')))
        {
            token[m++]=ch; //ch=>token
            ch=prog[p++]; //读下一个字符
        }
        token[m++]='\0';
        p--; //回退一格
        syn=10; //标识符
//如果是"begin","if","then","while","do","end"标识符中的一个
        for(n=0; n<9; n++)
            if(strcmp(token,rwtab[n])==0)
            {
                syn=n+1;
                break;
            }
    }
    else if((ch>='0')&&(ch<='9'))
    {
IsNum:
        if(isSignal==1)
        {
//token[m++]='-';
        }
        while((ch>='0')&&(ch<='9'))
        {
            sum=sum*10+ch-'0'; //ch 中数字本身是当做字符存放的
            ch=prog[p++];
        }
        if(ch=='.')
        {
            isDecimal=1;
            ch=prog[p++];
            COunt=0; //之前忘了清零,123.123+123.123#两个浮点数就无法识别
            while((ch>='0')&&(ch<='9'))
            {
//pow(x,y)计算 x 的 y 次幂
                temp=(ch-'0')*pow(0.1,++COunt);
                decimal=decimal+temp;
//AddToDec();
                ch=prog[p++];
            }
            sum=sum+decimal;
        }
        if(ch=='e'||ch=='E')
        {
            isExp=1;
            ch=prog[p++];
            if(ch=='-')
            {
                isNegative=1;
                ch=prog[p++];
            }
            while((ch>='0')&&(ch<='9'))
            {
//指数
                index=index*10+ch-'0';
                ch=prog[p++];
            }
//10 的幂
//123e3 代表 123*10(3)
//sum=sum*pow(10,index);是错误的
            if(isNegative)
                sum=sum*pow(0.1,index);
            else
                sum=sum*pow(10,index);
        }
        if(isSignal==1)
        {
            sum=-sum;
            isSignal=0;
        }
        p--;
        syn=20;
    }
    else switch(ch)
        {
        case '<':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=')
            {
                syn=35;
                token[m++]=ch;
            }
            else
            {
                syn=34;
                p--;
            }
            break;
        case '>':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=')
            {
                syn=33;
                token[m++]=ch;
            }
            else
            {
                syn=32;
                p--;
            }
            break;
        case '=':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=')
            {
                syn=36;
                token[m++]=ch;
            }
            else
            {
                syn=21;
                p--;
            }
            break;
        case '+':
            temp2=prog[p];
            token[m++]=ch;
            if((temp2>='0')&&(temp2<='9')&&(repeat==1))
            {
                isSignal=2;
                ch=prog[p++];
                repeat=0;
                goto IsNum;
            }
            if(((temp2=='+')||(temp2=='-'))&&(repeat==0)) //如果重复出现符号，才将后边的+，-视为正负号
            {
                repeat=1;
//ch=prog[p++];
            }
            syn=22;
            break;
        case '-':
            temp2=prog[p];
            token[m++]=ch;
            if((temp2>='0')&&(temp2<='9')&&(repeat==1))
            {
                isSignal=1;
                ch=prog[p++]; //读"-"下一个字符
                repeat=0;
                goto IsNum; //转到数字的识别
            }
            if(((temp2=='+')||(temp2=='-'))&&(repeat==0))//的+，-视为正负号
            {
                repeat=1; //预言会重复
//ch=prog[p++]; //读下一个字符
            }
            syn=23;
            break;
        case '*':
            temp2=prog[p];
            token[m++]=ch;
            if(temp2=='+')
            {
                isSignal=2;
                repeat=1;
            }
            else if(temp2=='-')
            {
                isSignal=1;
                repeat=1;
            }
            syn=24;
            break;
        case '/':
            syn=25;
            token[m++]=ch;
            break;
        case '(':
            temp2=prog[p];
            token[m++]=ch;
            if(temp2=='+')
            {
                isSignal=2;
                repeat=1;
            }
            else if(temp2=='-')
            {
                isSignal=1;
                repeat=1;
            }
//如果重复出现符号，才将后边
            syn=26;
            break;
        case ')':
            syn=27;
            token[m++]=ch;
            break;
        case '{':
            syn=28;
            token[m++]=ch;
            break;
        case '}':
            syn=29;
            token[m++]=ch;
            break;
        case ',':
            syn=30;
            token[m++]=ch;
            break;
        case ';':
            syn=31;
            token[m++]=ch;
            break;
        case'#':
            syn=0;
            token[m++]=ch;
            break;
        default:
            syn=-1;
            break;
        }
}
void zhongjiandaima()
{
    p=0;
    COunt=0;
    isDecimal=0;
    index=0;
    repeat=0;
    kk=0;
    printf("\nPlease input your source string:\n");
    do
    {
        ch=getchar();
        prog[p++]=ch;
    }
    while(ch!='#');
    p=0;
    isError=0;
    scanner();
    lrparser();
    for(i=1; i<nextq; i++) //循环输出四元式
    {
        printf("(%3s, %3s ,%3s , %3s )\n",fourCom[i].opera,fourCom[i].arg1,fourCom[i].arg2,fourCom[i].result);
    }
    for(i=1; i<nextq; i++) //循环输出四元式
    {
        printf("\n%d\t",i);
        printf("(%3s, %3s ,%3s , %3s )\n",fourCom[i].opera,fourCom[i].arg1,fourCom[i].arg2,fourCom[i].result);
        if(strcmp(fourCom[i].opera,"=")==0)
        {
            printf("Move AX,%1s\n",fourCom[i].arg1);
            printf("Move %5s,Ax\n",fourCom[i].result);
        }
        else if(strcmp(fourCom[i].opera,"+")==0)
        {
            printf("Mov AX,%1s\n",fourCom[i].arg1);
            printf("ADD Ax,%1s\n",fourCom[i].arg2);
            printf("Mov %1s,Ax\n",fourCom[i].result);
        }
        else if(strcmp(fourCom[i].opera,"-")==0)
        {
            printf("Mov AX,%1s\n",fourCom[i].arg1);
            printf("SUB Ax,%1s\n",fourCom[i].arg2);
            printf("Mov %1s,Ax\n",fourCom[i].result);
        }
        else if(strcmp(fourCom[i].opera,"*")==0)
        {
            printf("Mov AL,%1s\n",fourCom[i].arg1);
            printf("MUL %1s\n",fourCom[i].arg2);
            printf("Mov %1s,Ax\n",fourCom[i].result);
        }
        else if(strcmp(fourCom[i].opera,"/")==0)
        {
            printf("Mov AX,%1s\n",fourCom[i].arg1);
            printf("DIv %1s\n",fourCom[i].arg2);
            printf("Mov %1s,AL\n",fourCom[i].result);
        }
        else if(strcmp(fourCom[i].arg2,"goto")==0&&strcmp(fourCom[i].arg1,"if")==0)
        {
            printf("cmp %1s\n",fourCom[i].opera);
            printf("jnc %1s\n",fourCom[i].result);
        }
        else
        {
            printf("jmp %1s\n",fourCom[i].result);
        }
    }
    return;
}
char Analysis_string[20];//分析栈
char Remain_string[20];//剩余串
char VN[20]= {'i','+','*','(',')','#'}; //终结符
char VT[20]= {'E','G','T','S','F'}; //非终结符
typedef struct type
{
    int len;
    char arr[5];
    char cap;
} type;
type ee,tt,gg,g1,ss,s1,fff,f1;
type Table[10][10];//预测分析表
int j=0,b=0,top=0,l;
void Output_stack()//输出分析栈
{
    int a;
    for(a=0; a<=top+1; a++)
        printf("%c",Analysis_string[a]);
    printf("\t\t");
}
void Output_remain()//输出剩余串
{
    int j;
    for(j=0; j<b; j++)
        printf(" ");
    for(j=b; j<=l; j++)
        printf("%c",Remain_string[j]);
    printf("\t\t\t");
}
string Pro[10]= {"E->TG","G->+TG","G->-TG","G->ε","T->FS","S->*FS","S->/FS","S->ε","F->(E)","F->i"}; //产生式集合
void Output_table()
{
    cout<<"本分析文法产生式为"<<endl;
    for(int i=0; i<10; i++)
        cout<<Pro[i]<<endl;
    cout<<"                        LL(1)分析表 "<<endl;
    cout<<"\t+\t-\t*\t/\t(\t)\ti\t#"<<endl;
    cout<<"E\t\t\t\t\tE->TG\t\tE->TG"<<endl;
    cout<<"T\t\t\t\t\tT->FS\t\tT->FS"<<endl;
    cout<<"G\tG->+TG\tG->-TG\t\t\t\tG->ε\t\tG->ε"<<endl;
    cout<<"F\t\t\t\t\tF->(E)\t\tF->i"<<endl;
    cout<<"S\tS->ε\tS->ε\tS->*FS\tS->/FS\t\tS->ε\t\tS->ε"<<endl;
}

int main()
{
    printf(" \n");
    printf(" \n");
    printf("#####################################################################\n");
    printf(" choice 1--------cifafenxi\n");
    printf(" choice 2--------yufafenxi\n");
    printf(" choice 3--------zhongjiandaima\n\n");
    printf("#####################################################################\n");
    scanf("%d",&r1);
    do
    {
        switch(r1)
        {
        case 1:
            cifafenxi();
            scanf("%d",&r1);
            break;
        case 2:
            yufafenxi() ;
            scanf("%d",&r1);
            break;
        case 3:
            zhongjiandaima();
            scanf("%d",&r1);
            break;
        default:
            printf("error,please input again");
            break;
        }
    }
    while(r1!=0);
}
void cifafenxi()
{
    p1=0;
    printf(" ------------------Welcome!!!(cifafenxi)-----------------");
    printf("\n please input a string(end with '#'):/n");
    do
    {
        scanf("%c",&ch1);
        prog1[p1++]=ch1;
    }
    while(ch1!='#');
    p1=0;
    do
    {
        scaner1();
        switch(syn1)
        {
        case 11:
            printf("( %-10d%5d )\n",sum1,syn1);
            break;
        case -1:
            printf("you have input a wrong string\n");
            exit(0);
        default:
            printf("( %-10s%5d )\n",token1,syn1);
            break;
        }
    }
    while(syn1!=0);
}
void scaner1()
{
    sum1=0;
    for(m1=0; m1<8; m1++)token1[m1++]=NULL;
    ch1=prog1[p1++];
    m1=0;
    while((ch1==' ')||(ch1=='\n'))ch1=prog1[p1++];
    if(((ch1<='z')&&(ch1>='a'))||((ch1<='Z')&&(ch1>='A')))
    {
        while(((ch1<='z')&&(ch1>='a'))||((ch1<='Z')&&(ch1>='A'))||((ch1>='0')&&(ch1<='9')))
        {
            token1[m1++]=ch1;
            ch1=prog1[p1++];
        }
        p1--;
        syn1=10;
        for(n1=0; n1<6; n1++)
            if(strcmp(token1,rwtab1[n1])==0)
            {
                syn1=n1+1;
                break;
            }
    }
    else if((ch1>='0')&&(ch1<='9'))
    {
        while((ch1>='0')&&(ch1<='9'))
        {
            sum1=sum1*10+ch1-'0';
            ch1=prog1[p1++];
        }
        p1--;
        syn1=11;
    }
    else switch(ch1)
        {
        case '<':
            token1[m1++]=ch1;
            ch1=prog1[p1++];
            if(ch1=='=')
            {
                syn1=22;
                token1[m1++]=ch1;
            }
            else
            {
                syn1=20;
                p1--;
            }
            break;
        case '>':
            token1[m1++]=ch1;
            ch1=prog1[p1++];
            if(ch1=='=')
            {
                syn1=24;
                token1[m1++]=ch1;
            }
            else
            {
                syn1=23;
                p1--;
            }
            break;
        case '+':
            token1[m1++]=ch1;
            ch1=prog1[p1++];
            if(ch1=='+')
            {
                syn1=17;
                token1[m1++]=ch1;
            }
            else
            {
                syn1=13;
                p1--;
            }
            break;
        case '-':
            token1[m1++]=ch1;
            ch1=prog1[p1++];
            if(ch1=='-')
            {
                syn1=29;
                token1[m1++]=ch1;
            }
            else
            {
                syn1=14;
                p1--;
            }
            break;
        case '!':
            ch1=prog1[p1++];
            if(ch1=='=')
            {
                syn1=21;
                token1[m1++]=ch1;
            }
            else
            {
                syn1=31;
                p1--;
            }
            break;
        case '=':
            token1[m1++]=ch1;
            ch1=prog1[p1++];
            if(ch1=='=')
            {
                syn1=25;
                token1[m1++]=ch1;
            }
            else
            {
                syn1=18;
                p1--;
            }
            break;
        case '*':
            syn1=15;
            token1[m1++]=ch1;
            break;
        case '/':
            syn1=16;
            token1[m1++]=ch1;
            break;
        case '(':
            syn1=27;
            token1[m1++]=ch1;
            break;
        case ')':
            syn1=28;
            token1[m1++]=ch1;
            break;
        case '{':
            syn1=5;
            token1[m1++]=ch1;
            break;
        case '}':
            syn1=6;
            token1[m1++]=ch1;
            break;
        case ';':
            syn1=26;
            token1[m1++]=ch1;
            break;
        case '\"':
            syn1=30;
            token1[m1++]=ch1;
            break;
        case '#':
            syn1=0;
            token1[m1++]=ch1;
            break;
        case ':':
            syn1=17;
            token1[m1++]=ch1;
            break;
        default:
            syn1=-1;
            break;
        }
    token1[m1++]='\0';
}
/**/
void yufafenxi()
{
    Output_table();
    int m,n,k=0,flag=0,fin=0;
    char ch,x;
    type cha;
    ee.cap='E';strcpy(ee.arr,"TG");ee.len=2;tt.cap='T';
    strcpy(tt.arr,"FS");tt.len=2;gg.cap='G';
    strcpy(gg.arr,"+TG");gg.len=3;g1.cap='G';g1.arr[0]='^';g1.len=1;ss.cap='S';
    strcpy(ss.arr,"*FS");ss.len=3;s1.cap='S';s1.arr[0]='^';s1.len=1;fff.cap='F';
    strcpy(fff.arr,"(E)");fff.len=3;f1.cap='F';f1.arr[0]='i';f1.len=1;
    for(m=0; m<=4; m++)
        for(n=0; n<=5; n++)
            Table[m][n].cap='N';
    Table[0][0]=ee;Table[0][3]=ee;Table[1][1]=gg;
    Table[1][4]=g1;Table[1][5]=g1;Table[2][0]=tt;
    Table[2][3]=tt;Table[3][1]=s1;Table[3][2]=ss;
    Table[3][4]=Table[3][5]=s1;Table[4][0]=f1;Table[4][3]=fff;
    printf("\n请输入由'i','+','*','(',')'构成的以'#'结束的字符串进行分析,\n");
    printf("要分析的字符串:");
    getchar();
    do{
        scanf("%c",&ch);
        if ((ch!='i') &&(ch!='+') &&(ch!='*')&&(ch!='(')&&(ch!=')')&&(ch!='#')){
            printf("输入串中有非法字符\n");
//            exit(1);
        }
        Remain_string[j]=ch;
        j++;
    }while(ch!='#');
    l=j;
    ch=Remain_string[0];
    Analysis_string[top]='#';
    Analysis_string[++top]='E';
    printf("步骤\t\t分析栈 \t\t剩余字符 \t\t所用产生式 \n");
    do{
        x=Analysis_string[top--];//x为当前栈顶字符
        printf("%d",k++);
        printf("\t\t");
        for(j=0; j<=5; j++) //判断是否为终结符
            if(x==VN[j]){
                flag=1;
                break;
            }
        if(flag==1){
            if(x=='#'){
                fin=1;
                printf("acc!\n");
//                getchar();getchar();
//                exit(1);
            }
            if(x==ch){
                Output_stack();Output_remain();
                printf("%c匹配\n",ch);
                ch=Remain_string[++b];flag=0;
            }
            else{
                Output_stack();Output_remain();
                printf("%c出错\n",ch);
//                exit(1);
            }
        }
        else{
            for(j=0; j<=4; j++)
                if(x==VT[j]){
                    m=j;break;
                }
            for(j=0; j<=5; j++)
                if(ch==VN[j]){
                    n=j;break;
                }
            cha=Table[m][n];
            if(cha.cap!='N'){
                Output_stack();Output_remain();
                printf("%c-",cha.cap);
                for(j=0; j<cha.len; j++)
                    printf("%c",cha.arr[j]);
                printf("\n");
                for(j=(cha.len-1); j>=0; j--)
                    Analysis_string[++top]=cha.arr[j];
                if(Analysis_string[top]=='^')
                    top--;
            }
            else{
                Output_stack();Output_remain();
                printf("%c出错\n",x);
//                exit(1);
            }
        }
    }
    while(fin==0);
//    printf("111111111111\n");
}

//void yufafenxi()
//{
//    printf(" ------------------Welcome!!!(yufafenxi)-----------------\n");
//    right1=1;
//    temp1=0;
////    cout<<"E->TE'"<<endl;
////    cout<<"T->FT'"<<endl;
////    cout<<"E'->+TE'"<<endl;
////    cout<<"T'->*FT'"<<endl;
////    cout<<"T'->^"<<endl;
////    cout<<"F->i"<<endl;
////    cout<<"F->(E)"<<endl;
//    cout<<"请输入您要分析的字符串以#结束(^为空字符)："<<endl;
//    cin>>inputstream;
//    e();
//    if((inputstream[temp1]=='#')&&right)
//        cout<<"分析成功"<<endl;
//    else
//        cout<<"分析失败"<<endl;
//    return;
//}
//void e()
//{
//    cout<<"E->TE'"<<endl;
//    t();
//    e1();
//}
//void e1()
//{
//    if(inputstream[temp1]=='+')
//    {
//        cout<<"E'->+TE'"<<endl;
//        temp1++;
//        t();
//        e1();
//    }
//    else if (inputstream[temp1]!='#'||inputstream[temp1]!=')')
//    {
//        cout<<"T'->^"<<endl;
//        return ;
//    }
//    else
//        right1=0;
//}
//void t()
//{
//    cout<<"T->FT'"<<endl;
//    f();
//    t1();
//}
//void t1()
//{
//    if(inputstream[temp1]=='*')
//    {
//        cout<<"T'->*FT'"<<endl;
//        temp1++;
//        f();
//        t1();
//    }
//    else if (inputstream[temp1]!='#'&&inputstream[temp1]!=')'&&inputstream[temp1]!='+')
//    {
//        cout<<"T'->^"<<endl;
//        right1=0;
//    }
//}
//void f()
//{
//    if(inputstream[temp1]=='i')
//    {
//        cout<<"F->i"<<endl;
//        temp1++;
//    }
//    else if(inputstream[temp1]=='(')
//    {
//        cout<<"F->(E)"<<endl;
//        temp1++;
//        e();
//        if(inputstream[temp1]==')')
//        {
//            cout<<"F->(E)"<<endl;
//            temp1++;
//        }
//        else
//            right1=0;
//    }
//    else right1 =0;
//}
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
main()
{
    x=x+i;
    y=y*k;
}#
*/
/*
main()
{
    x=x+3;
    y=y-x;
    j=i/4;
    c=a*b;
}#
*/

