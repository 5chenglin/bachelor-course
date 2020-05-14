/*
 * @Author: Chenglin Wu
 * @Date:   2020-05-10 15:23:50
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-05-14 08:13:17
 */
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#define COL pos / N
#define ROW pos % N

using namespace std;

const int N   = 10;
const int MAX = N * N;

class PAGING {
public:
    void init();
    string alloc();
    void recycle();
    void show_block();
    void show_job(const string &a);
    void show_table();
    void menu();
    void print(const int &cnt);

private:
    vector<vector<bool> > store;
    int total;
    map<string, vector<int> > table;
};


void PAGING::init() {
    cout << "程序初次运行，开始执行初始化。" << endl;
    total = 0;
    store.clear();
    table.clear();
    store.resize(N, vector<bool>(N, false));
    cout << "请输入已经占用的块号信息。" << endl;
    cout << "请输入个数： " << endl;
    int sum;
    cin >> sum;
    cout << "请依次输入块号，中间用空格隔开，输入回车以结束（请保证 块号 < "
         << MAX << "）： " << endl;
    for (int i = 0; i < sum; i++) {
        int pos = -1;
        cin >> pos;
        if (pos > MAX) { cout << "输入出错，程序已经终止。" << endl; }
        store[pos / N][pos % N] = true;
    }
    total += sum;
}

string PAGING::alloc() {
    cout << "请输入作业的名称： ";
    string name;
    cin >> name;
    if (table.count(name)) {
        cout << "输入错误，作业名已存在。" << endl;
        return "#";
    }
    cout << "请输入作业的大小： ";
    int sum = 0;
    cin >> sum;
    if (total + sum > MAX) { cout << "内存不足，分配失败。" << endl; }
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            _Bit_reference x = store[i][j];
            if (!x) {
                x = true;
                cnt++;
                table[name].push_back(i * N + j);
                if (cnt == sum) {
                    total += sum;
                    return name;
                }
            }
        }
    }
}

void PAGING::recycle() {
    if (!table.size()) {
        cout << "作业表为空，已退出。" << endl;
        return;
    }
    cout << "请输入要回收的作业的名称： ";
    while (true) {
        string name;
        cin >> name;
        if (table.count(name)) {
            for (auto pos : table[name]) {
                store[COL][ROW] = false;
                total--;
            }
            table.erase(name);
            cout << "回收成功。" << endl;
            return;
        } else {
            cout << "作业名称不存在，请重新输入：";
        }
    }
}

void PAGING::show_block() {
    for (int j = 0; j < N; j++) {
        if (j == 0) { cout << setw(3) << ' '; }
        cout << setw(3) << j;
        if (j == N - 1) cout << endl;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j == 0) { cout << setw(3) << i; }
            cout << setw(3) << store[i][j];
            if (j == N - 1) cout << endl;
        }
    }
}
void PAGING::show_job(const string &a) {
    cout << "----------" << endl;
    cout << "作业：" << a << " 所占用的物理块坐标如下：" << endl;
    for (auto pos : table[a]) { cout << "(" << COL << ", " << ROW << ")  "; }
    cout << endl;
}
void PAGING::show_table() {
    if (table.empty()) {
        cout << "作业表空。" << endl;
        return;
    }
    cout << "显示所有的作业表：" << endl;
    for (auto i : table) { show_job(i.first); }
    cout << "----------" << endl;
}
void PAGING::print(const int &cnt) {
    string out(cnt, '=');
    cout << out << endl;
}
void PAGING::menu() {
    init();
    while (true) {
        cout << "请选择功能：\n(0) 分配作业；(1) 回收作业；(2) 显示内存块；(3) "
                "显示作业表\n(-1) 退出"
             << endl;
        int op = -1;
        cin >> op;
        switch (op) {
            case 0: {
                string _name = alloc();
                if (_name == "#") break;
                show_job(_name);
                break;
            }
            case 1: {
                recycle();
                break;
            }
            case 2:{
                print((N+1)*3);
                show_block();
                print((N+1)*3);
                break;
            }
            case 3:{
                print((N+1)*3);
                show_table();
                print((N+1)*3);
                break;
            }
            default:exit(0);
        }
    }
}

int main(){
    PAGING work;
    work.menu();
    return 0;
}

