/*
 * @Author: Chenglin Wu
 * @Date:   2020-04-18 07:45:54
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-04-20 00:26:43
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>

using namespace std;

class Schedule {
   public:
    struct PCB {
        string id;
        int arrive_time;
        int serve_time;
        int serve_temp;
        int priority;
        char status;

        PCB(string _id = "A", int arrive = 0, int serve = 0,
            int _priority = (1 << 31), char _status = 'W')
            : id(_id),
              arrive_time(arrive),
              serve_time(serve),
              serve_temp(serve),
              priority(_priority),
              status(_status){};
    };

    void input_PCB();

    void init_process();

    void show_PCB_info();

    void erase_process();

    void DHPF();


    static bool arrive_time_Sort(const list<PCB>::iterator a,
                                 const list<PCB>::iterator b) {
        return a->arrive_time < b->arrive_time;
    }


    struct serve_time_Sort {
        bool operator()(const list<PCB>::iterator a,
                        const list<PCB>::iterator b) {
            if (a->priority == b->priority) { return a->id > b->id; }
            return a->priority < b->priority;
        }
    };

    static void show_single_running_JCB(const list<PCB>::iterator p) {
        cout << "作业名称：" << p->id << " 执行时间：" << total_time
             << " 剩余服务时间：" << p->serve_temp << " 剩余优先权："
             << p->priority << " 状态：" << p->status << "（将要运行时）"
             << endl;
    }


    static void show_single_waiting_JCB(const list<PCB>::iterator p,
                                        int transfer_time   = -1,
                                        double average_time = -1) {
        cout << "作业名称：" << p->id << " 执行时间：" << total_time
             << " 剩余服务时间：" << p->serve_temp << " 剩余优先权："
             << p->priority << " 状态：" << p->status << "（一个时间片结束后）"
             << endl;
    }


    static void show_single_finished_JCB(const list<PCB>::iterator p,
                                         int transfer_time   = -1,
                                         double average_time = -1) {
        cout << "作业名称：" << p->id << " 到达时间：" << p->arrive_time
             << " 服务时间：" << p->serve_time << " 剩余优先权：" << p->priority
             << " 周转时间： " << transfer_time << " 带权周转时间： "
             << average_time << " 状态：" << p->status << "（进程终止后）"
             << endl;
    }

   private:
    static int total_time;
    static int total_waiting;
    list<PCB> Queue;
    vector<list<PCB>::iterator> waitingQueue;
};


int Schedule::total_time    = 0;
int Schedule::total_waiting = 0;


void Schedule::input_PCB() {
    ifstream in("process.txt");
    PCB temp;
    while (!in.eof()) {
        in >> temp.id >> temp.arrive_time >> temp.serve_time >> temp.priority;
        temp.serve_temp = temp.serve_time;
        Queue.push_back(temp);
    }
}


void Schedule::init_process() {
    total_time = 0;
    Queue.clear();

    Schedule::input_PCB();
}


void Schedule::show_PCB_info() {
    cout << "\n******显示所有进程状态信息******\n";

    if (!Queue.empty()) {
        for (auto p : Queue)
            cout << "作业名称：" << p.id << " 到达时间：" << p.arrive_time
                 << " 服务时间：" << p.serve_time << " 优先权：" << p.priority
                 << " 状态：" << p.status << endl;
    } else
        cout << "there is no job" << endl;
}


void Schedule::DHPF() {
    cout << "\n******开始使用DHPF算法进行进程调度******\n";
    waitingQueue.clear();

    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'W') { waitingQueue.emplace_back(it); }
    }
    sort(waitingQueue.begin(), waitingQueue.end(), arrive_time_Sort);
    total_waiting = waitingQueue.size();
    priority_queue<list<PCB>::iterator, vector<list<PCB>::iterator>,
                   serve_time_Sort>
        executingQueue;
    while (!executingQueue.empty()) executingQueue.pop();
    while (true) {
        for (auto &cur : waitingQueue) {
            if (cur->arrive_time <= total_time && cur->status == 'W') {
                cur->status = 'R';
                executingQueue.push(cur);
            }
        }

        if (!executingQueue.empty()) {
            auto cur = executingQueue.top();
            executingQueue.pop();
            if (cur->serve_temp == 0) {
                cur->status = 'F';
                total_waiting--;
                int trans  = total_time - cur->arrive_time;
                double ave = 1.0 * trans / cur->serve_time;
                show_single_finished_JCB(cur, trans, ave);
            } else {
                show_single_running_JCB(cur);
                cur->serve_temp--;
                cur->priority--;
                total_time++;
                cur->status = 'W';
                show_single_waiting_JCB(cur);
                if (cur->serve_temp == 0) {
                    cur->status = 'F';
                    total_waiting--;
                    int trans  = total_time - cur->arrive_time;
                    double ave = 1.0 * trans / cur->serve_time;
                    show_single_finished_JCB(cur, trans, ave);
                }
            }
        }
        if (total_waiting == 0 && executingQueue.empty()) {
            cout << "******DHPF算法结束******\n\n";
            break;
        }
    }
}


void Schedule::erase_process() {
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'F') Queue.erase(it);
    }
}

int main() {
    Schedule work;
    work.init_process();
    work.show_PCB_info();
    work.DHPF();
    work.show_PCB_info();
    return 0;
}
