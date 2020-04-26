/*
 * @Author: Chenglin Wu
 * @Date:   2020-04-25 19:55:28
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-04-26 20:42:38
 */
#include <cstdio>   // 包含库所用的某些宏和变量
#include <cstdlib>  // 包含库
#include <cstring>  // 包含字符串处理库
#define _KEY_WORD_END "wait for your expanding"  // 定义关键字结束标志
typedef struct {
    int typenum;
    char *word;
} WORD;
char input[255];       // 输入换缓冲区
char token[255] = "";  // 单词缓冲区
int p_input;           // 输入换缓冲区指针
int p_token;           // 单词缓冲区指针
char ch;               // 当前读入字符
char *rwtab[] = {"main",  "int", "char", "if",         "else", "for",
                 "while", "ID",  "NUM",  _KEY_WORD_END};  // 可扩充的关键字数组
WORD *scaner();  // 词法扫描函数，获得一个单词

using namespace std;

int main() {
    int over      = 1;
    WORD *oneword = new WORD;
    printf("    ***********************************    \n");
    printf("NAME: Chenglin Wu | School ID: 541707010134\n");
    printf("    ***********************************    \n");
    printf("Enter your original words (end with #):");
    scanf("%[^#]s", input);  // 读入源程序字符串到缓冲区，以#结束，允许多行输入

    p_input = 0;
    printf("Your words :\n%s\n", input);
    printf("The lex results are here:\n");
    while (over < 1000 && over != -1) {  //对源程序进行分析牞直至结束符#
        oneword = scaner();              //获得一个新单词
        if (oneword->typenum < 1000)
            printf("(%d, %s)\n", oneword->typenum,
                   oneword->word);  //打印种别码和单词自身的值
        over = oneword->typenum;
    }
    printf("\npress # to exit:");  //按#退出程序
    scanf("%[^#]s", input);
}

// 从输入缓冲区读取一个字符到ch中
char m_getch() {
    ch      = input[p_input];
    p_input = p_input + 1;
    return ch;
}
// 去掉空白符号
void getbc() {
    while (ch == ' ' || ch == 10) {
        ch      = input[p_input];
        p_input = p_input + 1;
    }
}
// 拼接单词
void concat() {
    token[p_token] = ch;
    p_token        = p_token + 1;
    token[p_token] = '\0';
}
// 判断是否字母
int letter() {
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
        return 1;
    else
        return 0;
}
// 判断是否数字
int digit() {
    if (ch >= '0' && ch <= '9')
        return 1;
    else
        return 0;
}
// 检索关键字表格
int reserve() {
    int i = 0;
    while (strcmp(rwtab[i], _KEY_WORD_END)) {
        if (!strcmp(rwtab[i], token)) { return i + 1; }
        i = i + 1;
    }
    return 10;
}
// 回退一个字符
void retract() { p_input = p_input - 1; }
// 数字转换成二进制
char *dtb() { return NULL; }


WORD *scaner() {
    WORD *myword    = new WORD;
    myword->typenum = 10;
    myword->word    = "";
    p_token         = 0;
    m_getch();
    getbc();

    if (letter()) {
        while (letter() || digit()) {
            concat();
            m_getch();
        }
        retract();
        myword->typenum = reserve();
        myword->word    = token;
        return (myword);
    } else if (digit()) {
        while (digit()) {
            concat();
            m_getch();
        }
        retract();
        myword->typenum = 20;
        myword->word    = token;
        return (myword);
    } else
        switch (ch) {
            case '=':
                m_getch();
                if (ch == '=') {
                    myword->typenum = 39;
                    myword->word    = "==";
                    return (myword);
                }
                retract();
                myword->typenum = 21;
                myword->word    = "=";
                return (myword);
                break;
            case '+':
                myword->typenum = 22;
                myword->word    = "+";
                return (myword);
                break;
            case '-':
                myword->typenum = 23;
                myword->word    = "-";
                return (myword);
                break;
            case '*':
                myword->typenum = 24;
                myword->word    = "*";
                return (myword);
                break;
            case '/':
                myword->typenum = 25;
                myword->word    = "/";
                return (myword);
                break;
            case '(':
                myword->typenum = 26;
                myword->word    = "(";
                return (myword);
                break;
            case ')':
                myword->typenum = 27;
                myword->word    = ")";
                return (myword);
                break;
            case '[':
                myword->typenum = 28;
                myword->word    = "[";
                return (myword);
                break;
            case ']':
                myword->typenum = 29;
                myword->word    = "]";
                return (myword);
                break;
            case '{':
                myword->typenum = 30;
                myword->word    = "{";
                return (myword);
                break;
            case '}':
                myword->typenum = 31;
                myword->word    = "}";
                return (myword);
                break;
            case ',':
                myword->typenum = 32;
                myword->word    = ",";
                return (myword);
                break;
            case ':':
                myword->typenum = 33;
                myword->word    = ":";
                return (myword);
                break;
            case ';':
                myword->typenum = 34;
                myword->word    = ";";
                return (myword);
                break;
            case '>':
                m_getch();
                if (ch == '=') {
                    myword->typenum = 37;
                    myword->word    = ">=";
                    return (myword);
                }
                retract();
                myword->typenum = 35;
                myword->word    = ">";
                return (myword);
                break;
            case '<':
                m_getch();
                if (ch == '=') {
                    myword->typenum = 38;
                    myword->word    = "<=";
                    return (myword);
                }
                retract();
                myword->typenum = 36;
                myword->word    = "<";
                return (myword);
                break;
            case '!':
                m_getch();
                if (ch == '=') {
                    myword->typenum = 40;
                    myword->word    = "!=";
                    return (myword);
                }
                retract();
                myword->typenum = -1;
                myword->word    = "ERROR";
                return (myword);
                break;
            case '\0':
                myword->typenum = 1000;
                myword->word    = "OVER";
                return (myword);
                break;
            default:
                myword->typenum = -1;
                myword->word    = "ERROR";
                return (myword);
                break;
        }
}
/*
Sample Input:

int main(){
    int i = 10;
    while(i)
        i=i-1;
}#

*/