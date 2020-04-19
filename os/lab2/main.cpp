/*
 * @Author: Chenglin Wu
 * @Date:   2020-04-18 07:45:54
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-04-19 23:51:10
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
        string id;        //作业名称
        int arrive_time;  //到达时间
        int serve_time;   //服务时间
        int serve_temp;   //临时服务时间
        int priority;     //优先权
        char status;      //状态，共三种状态W, R, F

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

    void SJF();

    void HPF();

    /// @brief 按照到达时间排序
    static bool arrive_time_Sort(const list<PCB>::iterator a,
                                 const list<PCB>::iterator b) {
        return a->arrive_time < b->arrive_time;
    }

    /// @brief 按照优先级排序
    struct serve_time_Sort {
        bool operator()(const list<PCB>::iterator a,
                        const list<PCB>::iterator b) {
            if (a->priority == b->priority) {
                //                if (a->arrive_time == b->arrive_time)
                return a->id > b->id;
                //                return a->serve_temp > b->serve_temp;
            }
            return a->priority < b->priority;
        }
    };
    /// @brief 打印单个运行时的PCB信息
    static void show_single_running_JCB(const list<PCB>::iterator p) {
        cout << "作业名称：" << p->id << " 执行时间：" << total_time
             << " 剩余服务时间：" << p->serve_temp << " 剩余优先权："
             << p->priority << " 状态：" << p->status << "（将要运行时）"
             << endl;
    }

    /// @brief 打印变为就绪状态后的PCB信息
    static void show_single_waiting_JCB(const list<PCB>::iterator p,
                                        int transfer_time   = -1,
                                        double average_time = -1) {
        cout << "作业名称：" << p->id << " 执行时间：" << total_time
             << " 剩余服务时间：" << p->serve_temp << " 剩余优先权："
             << p->priority << " 状态：" << p->status << "（一个时间片结束后）"
             << endl;
    }

    /// @brief 打印单个终止后的PCB信息
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
    static int total_time;     //当前时间
    static int total_waiting;  // waiting队列中的进程数
    list<PCB> Queue;           //存储所有输入的进程信息
    vector<list<PCB>::iterator>
        waitingQueue;  //存储所有状态为W的作业，只存放指针，方便修改。
};

// 初始化静态成员变量
int Schedule::total_time    = 0;
int Schedule::total_waiting = 0;

/*!
 * @brief 输入所有作业的信息
 * @param temp 临时PCB变量，保存单个作业信息。
 * @param in 从 process.txt 文件输入
 */
void Schedule::input_PCB() {
    ifstream in("process.txt");
    PCB temp;
    while (!in.eof()) {
        in >> temp.id >> temp.arrive_time >> temp.serve_time >> temp.priority;
        temp.serve_temp = temp.serve_time;
        Queue.push_back(temp);
    }
}

/*!
 * @brief 初始化所有输入的进程信息，将 status 修改为 W
 */
void Schedule::init_process() {
    total_time = 0;  //对总时间初始化
    Queue.clear();   //清空进程队列

    Schedule::input_PCB();  //开始输入进程信息
}

/*!
 * @brief 显示所有进程状态
 */
void Schedule::show_PCB_info() {
    cout << "\n******显示所有进程状态信息******\n";
    //遍历所有的进程队列，显示所有进程当前的状态信息。
    if (!Queue.empty()) {
        for (auto p : Queue)
            cout << "作业名称：" << p.id << " 到达时间：" << p.arrive_time
                 << " 服务时间：" << p.serve_time << " 优先权：" << p.priority
                 << " 状态：" << p.status << endl;
    } else  //进程队列中无进程
        cout << "there is no job" << endl;
}

/*!
 * @brief HPF调度算法核心
 * @param executingQueue
 * 待执行队列，采用优先队列进行存储，方便维护，按照服务时间排序
 *
 */
void Schedule::HPF() {
    cout << "\n******开始使用HPF算法进行进程调度******\n";
    waitingQueue.clear();  //清空等待队列
    //遍历所有进程，找出状态为 W 的进程，放入等待队列
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'W') {
            //            cout << "before it address: " << (*it).id << " " <<
            //            &*it << endl;
            waitingQueue.emplace_back(it);
        }
    }
    //对等待队列按照到达时间排序
    sort(waitingQueue.begin(), waitingQueue.end(), arrive_time_Sort);
    //待找到所有状态为 W 的进程后，计算出所有等待队列中的进程个数
    total_waiting = waitingQueue.size();
    priority_queue<list<PCB>::iterator, vector<list<PCB>::iterator>,
                   serve_time_Sort>
        executingQueue;
    //清空待执行队列
    while (!executingQueue.empty()) executingQueue.pop();
    // HPF核心处理部分
    while (true) {
        //按照当前时间找已经到达的进程，然后将进程状态改为 R ，放入待执行队列
        for (auto &cur : waitingQueue) {
            if (cur->arrive_time <= total_time && cur->status == 'W') {
                //                cout << "it address: " << (*cur).id << " " <<
                //                &*cur << endl;
                cur->status = 'R';
                //                cout << "---" << endl;
                //                show_single_running_JCB(cur);
                //                cout << "---" << endl;
                executingQueue.push(cur);
            }
        }
        // 处理待执行队列中的进程，处理完一个进程后，计算对应的周转时间和带权周转时间，然后跳出循环。
        if (!executingQueue.empty()) {
            auto cur = executingQueue.top();
            executingQueue.pop();
            if (cur->serve_temp == 0) {
                //                cout << "it address: " << (*cur).id << &*cur
                //                << endl;
                cur->status = 'F';
                total_waiting--;  //每完成一个进程，总数减一
                int trans  = total_time - cur->arrive_time;
                double ave = 1.0 * trans / cur->serve_time;
                show_single_finished_JCB(cur, trans, ave);
            } else {
                //                cout<<"it address: "<< (*cur).id<<&*cur
                //                <<endl;
                show_single_running_JCB(cur);
                cur->serve_temp--;
                cur->priority--;
                total_time++;
                cur->status = 'W';
                show_single_waiting_JCB(cur);
                if (cur->serve_temp == 0) {
                    //                cout << "it address: " << (*cur).id << &*cur
                    //                << endl;
                    cur->status = 'F';
                    total_waiting--;//每完成一个进程，总数减一
                    int trans = total_time - cur->arrive_time;
                    double ave = 1.0*trans/cur->serve_time;
                    show_single_finished_JCB(cur, trans, ave);
                }
            }
        }

        //等待队列中的任务数为零，且 待执行队列为空 时，算法结束。
        if (total_waiting == 0 && executingQueue.empty()) {
            cout<<"******HPF算法结束******\n\n";
            break;
        }
    }
}

//删除作业状态为 F 的进程。
void Schedule::erase_process() {
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'F') Queue.erase(it);
    }
}

int main() {
    Schedule work;  // define a work
    work.init_process();  // input the processes
    work.show_PCB_info();   // show all the processes
    work.HPF();  // call HPF
    work.show_PCB_info();

    return 0;
}
