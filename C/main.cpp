#include <stdio.h>  
#include <stdlib.h>  
#include<string.h>  
  
/*一个结构，一个链表*/  
  
struct studentInfo        /*学生信息*/  
{  
    char num[13];            //学号  
    char name[20];          //姓名  
    char sex;              // 性别，'1'->男, '0'->女  
    int age;              //年龄  
    int score[3];        //三门课程的成绩  
    int sum;            //总成绩  
    double ave;        //三门课程的平均值  
    int Depart_Id;    //所在系的系号  
};  
  
struct Depart_Info           /*系信息，假设总共有3个系*/  
{  
    int id;                 //系号  
    char name[20];         //系名  
    char dean[10];        //系主任  
    char location[50];   //系的办公地点  
    struct Depart_Info *next;  
};  
  
void Menu();    //    主菜单（仅做展示使用）  
  
void Add_Yes_or_No(struct studentInfo *st, int *n);       //是否继续添加学生信息  
  
void Add_the_Student(struct studentInfo *st, int i);     //输入需要添加的学生的信息  
  
void Display_all_Stu(struct studentInfo *st, int i);    //输出所有学生的信息情况  
  
  
/*接下来是查找模块*/  
void Exact_Search(struct studentInfo *st, int n);   //精确查找  
  
void Search_Stu_Name(struct studentInfo *st, int i);     //姓名查找  
  
void Search_Result(struct studentInfo *st, int n);      //显示查询结果。  
  
void Fuzzy_Search(struct studentInfo *st, int n);      //模糊查找  
  
void Search_Id(struct studentInfo st[], int n);       //学号查找  
  
void Search_Age(struct studentInfo *st, int n);     //年龄查找  
  
void chain(struct Depart_Info **);  
  
void Search_Depart(struct studentInfo *st, int n, struct Depart_Info *p);       //查找某系的学生。  
	  
	void Search_Male(struct studentInfo *st, int n, struct Depart_Info *hhead);     //查找系所有男生信息  
	  
	  
	/*这里是加载或保存信息模块*/  
	void Download(struct studentInfo *st, int *n);     //加载文件中的信息。  
	  
	void Save(struct studentInfo *st, int n);       //保存所有同学的信息。  
	  
	  
	/*这里是后续功能*/  
	void ame(struct studentInfo st[], int n);           //修改学生信息  
	  
	void Del_Student(struct studentInfo *st, int *n);   //删除学生信息  
	  
	void xuesort(struct studentInfo st[], int *n);      //信息排序  
	  
	void Add_Stu(struct studentInfo *st, int *n);       //增加学生信息  
	  
	void Print_Line(int n);                             //为了美观，此函数可以打印些虚线。  
  
  
/*主函数大佬登场*/  
  
int main() {  
    struct studentInfo st[10000];  
    struct Depart_Info *head = NULL;  
    chain(&head);  
    int i = 0;  
    int elect;  
    printf("%42c 学生信息管理系统\n", ' ');  
    printf("%37c Designed by zzuli_Zhuomu\n", ' ');  
    printf("%28c Copyright (C) 2018 . All Rights Reserved.\n\n", ' ');  
    while (1) {  
  
        Menu();  
        while (scanf("%d", &elect) == 0) {  
            while (getchar() != '\n');  
            printf("输入存在错误，请重新输入。\n");  
        }  
        if (elect == 0)  
            break;  
        switch (elect) {  
            case 1:  
                Add_Yes_or_No(st, &i);  
                break;  
            case 2:  
                Display_all_Stu(st, i);  
                break;  
            case 3:  
                Search_Id(st, i);  
                break;  
            case 4:  
                Search_Age(st, i);  
                break;  
            case 5:  
                Search_Stu_Name(st, i);  
                break;  
            case 6:  
                Search_Depart(st, i, head);  
                break;  
            case 7:  
                Search_Male(st, i, head);  
                break;  
            case 8:  
                ame(st, i);  
                break;  
            case 9:  
                Del_Student(st, &i);  
                break;  
            case 10:  
                Add_Stu(st, &i);  
                break;  
            case 11:  
                Save(st, i);  
                break;  
            case 12:  
                Download(st, &i);  
                break;  
            case 13:  
                xuesort(st, &i);  
                break;  
            case 100:  
                Menu();  
                break;  
            default:  
                printf("输入操作错误，请重新输入\n");  
                break;  
        }  
    }  
    return 0;  
}  
  
void Menu() {  
    printf("----------------------------------学生管理系统帮助菜单-------------------------------------\n\n");  
    printf("\t->说明：请用户按照菜单索引进行相应个性化操作\n");  
    printf("\t-> 1 录入学生信息;            2 显示所有学生信息；     3 按学号查询学生信息。\n");  
    printf("\t-> 4 按年龄查询学生信息;      5 按姓名查询学生信息；   6 查询指定学生的所在系的信息。\n");  
    printf("\t-> 7 查某系里男生的信息;      8 修改学生信息；         9 删除学生信息。\n");  
	    printf("\t-> 10 添加学生信息;           11 保存学生信息至文件；  12 从文件载入学生信息。\n");  
    printf("\t-> 13 按平均成绩排序(Z-A)；   0 退出系统。\n\n");  
    printf("-------------------------------------------------------------------------------------------\n");  
}  
  
void Add_the_Student(struct studentInfo *st, int i) {  
  
    printf("请输入第%d个学生的信息\n", i + 1);  
    printf("请输入该学生的学号：");  
    scanf("%s", st[i].num);  
    printf("请输入姓名：");  
    scanf("%s", st[i].name);  
    printf("请输入性别(1->男生，0->女生)：");  
    scanf(" %c", &st[i].sex);  
    printf("请输入年龄：");  
    while (scanf("%d", &st[i].age) != EOF)        //读取年龄并判断是否存在输入错误。  
    {  
        if (st[i].age < 0 || st[i].age > 200)  
            printf("输入错误，请重新输入！！\n");  
        else  
            break;  
    }  
    printf("请分别输入三门课的成绩（高等数学、英语、C语言），中间用空格隔开。\n");  
    st[i].sum = 0;  
    int k;  
    for (k = 0; k <= 2; k++) {  
        scanf("%d", &st[i].score[k]);  
        st[i].sum += st[i].score[k];  
    }  
    st[i].ave = 1.0 * st[i].sum / 3.0;  
    printf("请输入学生所在系的ID（ 1 ->计算机与科学与技术、 2 ->网络工程、 3 ->通信工程 ）。\n");  
    while (scanf("%d", &st[i].Depart_Id) != EOF)         //读取系id并判断是否存在输入错误。  
    {  
        if (st[i].Depart_Id <= 0 || st[i].Depart_Id > 3)  
            printf("输入错误，请重新输入！\n");  
        else  
            break;  
    }  
}  
  
void Add_Stu(struct studentInfo *st, int *n) {  
    int i, mark = 1;  
    printf("请先输入你想添加的学生的学号，系统将自动查找是否存在此学生:");  
    char str[100];  
    scanf("%s", str);  
    for (i = 0; i < *n; i++) {  
        if (strcmp(st[i].num, str) == 0) {  
            printf("你添加的学生信息已经存在。\n");  
            mark = 0;  
            break;  
        }  
    }  
    if (mark) {  
        printf("该学生信息不存在，您可以继续输入。\n");  
        Add_the_Student(st, *n);  
    }  
}  
  
void Add_Yes_or_No(struct studentInfo *st, int *n) {  
    char Option = 'y';  
    while (Option == 'y') {  
        Add_the_Student(st, (*n)++);  
        printf("恭喜您，添加成功！继续添加请输入：y，否则，请输入n。\n");  
        scanf(" %c", &Option);  
    }  
}  
  
void Display_all_Stu(struct studentInfo *st, int i) {  
    int j;  
    Print_Line(155);  
    printf("%-15s%-20s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "编号", "学号", "姓名", "性别", "年龄", "数学", "英语", "C语言",  
	           "总成绩", "系号");  
	    Print_Line(155);  
	    for (j = 0; j < i; j++) {  
	        printf("%-15d%-20s%-15s%-15c", j + 1, st[j].num, st[j].name, st[j].sex);  
	        printf("%-15d%-15d", st[j].age, st[j].score[0]);  
	        printf("%-15d%-15d%-15d", st[j].score[1], st[j].score[2], st[j].sum);  
	        printf("%-15d\n", st[j].Depart_Id);  
	    }  
	    Print_Line(155);  
	}  
	  
	void Search_Stu_Name(struct studentInfo *st, int i) {  
	    int n;  
    printf("\t**---------------------------------------------------------**\n");  
	    printf("\t **   请选择查找姓名的方式：1.精确查找、2.模糊查找、3.退出    **\n");  
	    printf("\t  **----------------------------------------------------**\n");  
	    scanf("%d", &n);  
	    switch (n) {  
	        case 1:  
	            Exact_Search(st, i);  
	            break;  
	        case 2:  
	            Fuzzy_Search(st, i);  
	            break;  
	        default:  
	            break;  
	    }  
	}  
	  
	void Search_Id(struct studentInfo st[], int n) {           //学号查找  
    int i, mark = 1;  
    printf("请输入你想精确查找的学生的学号\n");  
    char str[40];  
    scanf("%s", str);  
    for (i = 0; i < n; i++) {  
        if (strcmp(str, st[i].num) == 0) {  
            mark = 0;  
            Search_Result(st, i);  
        }  
    }  
    if (mark)  
        printf("查询无果，系统将自动返回首页。如需其他操作，请直接输入菜单中的数字。\n");  
	}  
	  
	void Exact_Search(struct studentInfo *st, int n) {          //精确查找函数。  
	    int i, mark = 1;  
	    printf("请输入你想精确查找的学生姓名\n");  
	    char str[40];  
	    scanf("%s", str);  
	    for (i = 0; i < n; i++) {  
	        if (strcmp(str, st[i].name) == 0) {  
	            mark = 0;  
	            Search_Result(st, i);  
	        }  
	    }  
	    if (mark)  
	        printf("查询无果，系统将自动返回首页。\n");  
	}  
	  
	void Fuzzy_Search(struct studentInfo *st, int n) {          //模糊查找函数。  
	    int mark = 1, i;  
	    printf("请输入你想模糊查找的学生的姓名\n");  
	    char str[40];  
	    scanf("%s", str);  
	    for (i = 0; i < n; i++) {  
	        if (strstr(st[i].name, str) != 0) {  
	            Search_Result(st, i);  
	            mark = 0;  
	        }  
	    }  
	    if (mark)  
	        printf("查询无果，系统将自动返回首页。\n");  
	}  
  
void Search_Result(struct studentInfo *st, int n) {  
    Print_Line(155);  
    printf("%-20s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "学号", "姓名", "性别", "年龄", "数学", "英语", "C语言", "总成绩", "系号");  
	    Print_Line(155);  
	    printf("%-20s%-15s%-15c", st[n].num, st[n].name, st[n].sex);  
	    printf("%-15d%-15d", st[n].age, st[n].score[0]);  
	    printf("%-15d%-15d%-15d", st[n].score[1], st[n].score[2], st[n].sum);  
	    printf("%-15d\n", st[n].Depart_Id);  
	    Print_Line(155);  
	}  
	  
	void Search_Age(struct studentInfo *st, int n) {  
	    int i, shu, mark = 1;  
	    char ch;  
	    printf("请输入查找学生的年龄：\n");  
	    while (scanf("%d", &shu) == 0) {  
	        scanf("%c", &ch);  
	        printf("输入格式错误，请重新输入,,,,\n");  
	    }  
	  
	    for (i = 0; i < n; i++) {  
	        if (st[i].age == shu) {  
	            mark = 0;  
	            Search_Result(st, i);  
	        }  
	  
	    }  
	    if (mark)  
	        printf("没找到这个年龄的学生，系统已完成查询操作，自动返回首页\n如有需要请重新查询年龄！\n");  
}  
  
void chain(struct Depart_Info **head) {  
  
    struct Depart_Info *p, *t;  
    p = *head;  
    t = (struct Depart_Info *) malloc(sizeof(struct Depart_Info));  
    *head = t;  
    p = t;  
    strcpy(p->dean, "wanngwu");  
    p->id = 1;  
    strcpy(p->name, "计算机系");  
    strcpy(p->location, "计算机楼");  
  
    t = (struct Depart_Info *) malloc(sizeof(struct Depart_Info));  
    p->next = t;  
    p = p->next;  
  
    strcpy(t->dean, "zhangsan");  
    p->id = 2;  
    strcpy(t->name, "网络工程系");  
    strcpy(t->location, "计算机楼");  
  
    t = (struct Depart_Info *) malloc(sizeof(struct Depart_Info));  
    p->next = t;  
    p = p->next;  
  
    strcpy(p->dean, "lisi");  
    p->id = 3;  
    strcpy(p->name, "通信工程系");  
    strcpy(p->location, "计算机楼");  
    t = (struct Depart_Info *) malloc(sizeof(struct Depart_Info));  
    p->next = NULL;  
  
}  
  
void Search_Depart(struct studentInfo *st, int n, struct Depart_Info *p) {  
    int i, flag = 0;  
    char str[100];  
    struct Depart_Info *head;  
    printf("请输入你想查询的学生学号，我们将根据您输入的学号来查询所在系：\n");  
    scanf("%s", str);  
    printf("\n");  
    for (i = 0; i < n; i++) {  
        if (strcmp(st[i].num, str) == 0) {  
            head = p;  
            while (head != NULL) {  
                if (head->id == st[i].Depart_Id) {  
                    Print_Line(95);  
                    printf("%-20s%-20s%-15s%-15s%-15s\n", "姓名", "性别", "系名", "系主任", "系的办公地点");  
	                    printf("%-20s%-20c%-15s%-15s", st[i].name, st[i].sex, head->name, head->dean);  
                    printf("%-15s\n", head->location);  
                    Print_Line(95);  
                    flag = 1;  
                }  
                head = head->next;  
            }  
        }  
    }  
    if (flag == 0)  
        printf("查无结果。\n");  
}  
  
void Download(struct studentInfo *st, int *n) {  
    FILE *fp = NULL;  
    int i;  
    if ((fp = fopen("F:\\zzuli_C\\StudentData.txt", "r")) == 0) {  
        *n = 0;  
        exit(1);  
    }  
    for (i = 0; fread(&st[i], sizeof(struct studentInfo), 1, fp) != 0; i++);  
    *n = i;  
    fclose(fp);  
    printf("恭喜你，下载成功。\n");  
}  
  
void Save(struct studentInfo *st, int n) {  
    FILE *fp;  
    if ((fp = fopen("F:\\zzuli_C\\StudentData.txt", "w")) == 0) {  
        printf("不好意思，打开失败。");  
        exit(1);  
    }  
    fwrite(st, n * sizeof(st[0]), 1, fp);  
    fclose(fp);  
    printf("恭喜你，保存成功。\n");  
}  
  
void Search_Male(struct studentInfo *st, int n, struct Depart_Info *p) {  
    struct Depart_Info *head;  
    int i, flag = 0, save = 1;  
    int mark;  
    char ch;  
    printf("请输入系号来查询该系所有男生：\n");  
    while (scanf("%d", &mark) == 0) {  
        scanf("%c", &ch);  
        printf("输入格式错误，请重新输入。\n");  
    }  
    for (i = 0; i < n; i++) {  
       if (st[i].Depart_Id == mark && st[i].sex == '1') {  
           head = p;  
           while (head != NULL) {  
               if (head->id == mark) {  
                   if (save) {  
                       Print_Line(105);  
                       printf("%-20s%-20s%-20s%-15s%-15s%-15s\n", "学号", "姓名", "性别", "系名", "系主任", "办公地点");  
                       Print_Line(105);  
                       save = 0;  
                   }  
                   printf("%-20s%-20s%-20c%-15s%-20s", st[i].num, st[i].name, st[i].sex, head->name, head->dean);  
                    printf("%-15s\n", head->location);  
                    flag = 1;  
                }  
                head = head->next;  
            }  
        }  
    }  
    Print_Line(105);  
    if (flag == 0)  
        printf("查无结果\n");  
}  
  
void ame(struct studentInfo st[], int n) {  
    char str[100];  
    printf("请输入你想修改的学生的学号:\n");  
    scanf("%s", str);  
    int i, j, mark = 1;  
    for (i = 0; i < n; i++) {  
        if (strcmp(st[i].num, str) == 0) {  
            mark = 0;  
            printf("请输入修改后的学号：");  
            scanf("%s", st[i].num);  
            printf("请输入要修改的性别：");  
            scanf(" %c", &st[i].sex);  
            printf("请输入要修改的姓名：");  
            scanf("%s", st[i].name);  
            printf("请输入要修改的年龄：");  
            scanf("%d", &st[i].age);  
            printf("请分别输入修改后的三门课成绩（高等数学、英语、C语言），中间用空格隔开。：");  
            for (j = 0; j <= 2; j++) {  
                scanf("%d", &st[i].score[j]);  
            }  
            st[i].sum = 0;  
            st[i].sum = st[i].score[1] + st[i].score[2] + st[i].score[3];  
            printf("请输入修改后的系ID：\n");  
            scanf("%d", &st[i].Depart_Id);  
            printf("恭喜您，修改成功。");  
        }  
    }  
    if (mark)  
        printf("没有此学生信息。\n");  
}  
  
  
void Del_Student(struct studentInfo *st, int *n) {  
    int i, j, mask, k;  
    char str[110], ch;  
    printf("%-35c---------------------------------------------\n", ' ');  
    printf("%-35c请输入数字以选择删除符合此类条件的信息。\n", ' ');  
    printf("%-35c1学号 2姓名 3年龄 4系ID 5 性别\n", ' ');  
    printf("%-35c---------------------------------------------\n", ' ');  
    while (scanf("%d", &j) == 0) {  
        scanf("%c", &ch);  
    }  
    if (j < 3) {  
        printf("请根据你的要求输入对应的 学号 或 姓名：\n");  
        scanf("%s", str);  
    } else {  
        printf("请根据你的要求输入对应的 年龄、系Id 或 性别：\n");  
        printf("请输入整数：\n");  
        scanf("%d", &mask);  
    }  
  
    if (j == 1) {  
        for (i = 0; i < *n; i++) {  
            if (strcmp(st[i].num, str) == 0) {  
                for (k = i; k < *n; k++)  
                    st[k] = st[k + 1];  
                (*n)--;  
            }  
        }  
    } else if (j == 2) {  
        for (i = 0; i < *n; i++) {  
            if (strcmp(st[i].name, str) == 0) {  
                for (k = i; k < *n; k++)  
                    st[k] = st[k + 1];  
                (*n)--;  
            }  
        }  
    } else if (j == 3) {  
        for (i = 0; i < *n; i++) {  
            if (st[i].age == mask) {  
                for (k = i; k < *n; k++)  
                    st[k] = st[k + 1];  
                (*n)--;  
            }  
        }  
    } else if (j == 4) {  
        for (i = 0; i < *n; i++) {  
            if (st[i].Depart_Id == mask) {  
                for (k = i; k < *n; k++)  
                    st[k] = st[k + 1];  
                (*n)--;  
            }  
        }  
    } else if (j == 5) {  
        for (i = 0; i < *n; i++) {  
            if (st[i].sex == mask) {  
                for (k = i; k < *n; k++)  
                    st[k] = st[k + 1];  
                (*n)--;  
            }  
        }  
    } else {  
        printf("没有找到符合该条件的学生。\n");  
    }  
}  
  
int cmp(const void *a, const void *b) {  
    struct studentInfo c = *(struct studentInfo *) a;  
    struct studentInfo d = *(struct studentInfo *) b;  
    if (c.ave != d.ave)  
        return c.ave < d.ave;  
    else {  
        return c.age > d.age;  
    }  
}  
  
void xuesort(struct studentInfo st[], int *n) {  
    int i;  
    char ch;  
    qsort(st, *n, sizeof(st[0]), cmp);  
    printf("排序成功.\n");  
    printf("是否显示排序结果（1.显示、0.返回主菜单):\n");  
    while (scanf("%d", &i) == 0) {  
        scanf("%c", &ch);  
        printf("请输入你的选择\n");  
    }  
    int j = *n;  
    if (i == 1)  
        Display_all_Stu(st, j);  
}  
  
void Print_Line(int n) {  
    int k = 0;  
    while (++k < n)printf("-");  
    printf("\n");  
}  

