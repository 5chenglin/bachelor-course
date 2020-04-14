/*
 * @Author: Chenglin Wu
 * @Date:   2020-04-12 23:47:32
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-04-13 18:14:14
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>

using namespace std;

class Schedule {
   public:
    struct JCB {
        string id;        //作业名称
        int arrive_time;  //到达时间
        int serve_time;   //服务时间
        char status;      //状态，共三种状态W, R, F

        JCB(string _id = "A", int arrive = 0, int serve = 0, char _status = 'W')
            : id(_id),
              arrive_time(arrive),
              serve_time(serve),
              status(_status){};
    };

    void input_JCB();

    void init_job();

    void show_JCB_info();

    void erase_job();

    void SJF();

    /// @brief 按照到达时间排序
    static bool arrive_time_Sort(const list<JCB>::iterator a,
                                 const list<JCB>::iterator b) {
        return a->arrive_time < b->arrive_time;
    }

    /// @brief 按照服务时间排序
    struct serve_time_Sort {
        bool operator()(const list<JCB>::iterator a,
                        const list<JCB>::iterator b) {
            return a->serve_time > b->serve_time;
        }
    };
    /// @brief 打印单个运行时的JCB信息
    static void show_single_running_JCB(const list<JCB>::iterator p) {
        cout << "作业名称：" << p->id << " 到达时间：" << p->arrive_time
             << " 服务时间：" << p->serve_time << " 状态：" << p->status
             << endl;
    }

    /// @brief 打印单个终止后的JCB信息
    static void show_single_finished_JCB(const list<JCB>::iterator p,
                                         int transfer_time   = -1,
                                         double average_time = -1) {
        cout << "作业名称：" << p->id << " 到达时间：" << p->arrive_time
             << " 服务时间：" << p->serve_time << " 状态：" << p->status
             << " 周转时间：" << transfer_time << " 带权周转时间"
             << average_time << endl;
    }

   private:
    static int total_time;     //当前时间
    static int total_waiting;  // waiting队列中的作业数
    list<JCB> Queue;           //存储所有输入的作业信息
    vector<list<JCB>::iterator>
        waitingQueue;  //存储所有状态为W的作业，只存放指针，方便修改。
};

// 初始化静态成员变量
int Schedule::total_time    = 0;
int Schedule::total_waiting = 0;

/*!
 * @brief 输入所有作业的信息
 * @param temp 临时JCB变量，保存单个作业信息。
 * @param in 从 job.txt 文件输入
 */
void Schedule::input_JCB() {
    ifstream in("job.txt");
    JCB temp;
    while (!in.eof()) {
        in >> temp.id >> temp.arrive_time >> temp.serve_time;
        Queue.push_back(temp);
    }
}

/*!
 * @brief 初始化所有输入的作业信息，将 status 修改为 W
 */
void Schedule::init_job() {
    total_time = 0;  //对总时间初始化
    Queue.clear();   //清空作业队列

    Schedule::input_JCB();  //开始输入作业信息
}

/*!
 * @brief 显示所有作业状态
 */
void Schedule::show_JCB_info() {
    cout << "\n******显示所有作业状态信息******\n";
    //遍历所有的作业队列，显示所有作业当前的状态信息。
    if (!Queue.empty()) {
        for (auto p : Queue)
            cout << "作业名称：" << p.id << " 到达时间：" << p.arrive_time
                 << " 服务时间：" << p.serve_time << " 状态：" << p.status
                 << endl;
    } else  //作业队列中任何作业
        cout << "there is no job" << endl;
}

/*!
 * @brief SJF调度算法核心
 * @param executingQueue
 * 待执行队列，采用优先队列进行存储，方便维护，按照服务时间排序
 */
void Schedule::SJF() {
    cout << "\n******开始使用SJF算法进行作业调度******\n";
    waitingQueue.clear();  //清空等待队列
    //遍历所有作业，找出状态为 W 的作业，放入等待队列
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'W') { waitingQueue.emplace_back(it); }
    }
    //对等待队列按照到达时间排序
    sort(waitingQueue.begin(), waitingQueue.end(), arrive_time_Sort);
    //待找到所有状态为 W 的作业后，计算出所有等待队列中的作业个数
    total_waiting = waitingQueue.size();
    priority_queue<list<JCB>::iterator, vector<list<JCB>::iterator>,
                   serve_time_Sort>
        executingQueue;
    //清空待执行队列
    while (!executingQueue.empty()) executingQueue.pop();
    // SJF核心处理部分
    while (true) {
        //按照当前时间找已经到达的作业，然后将作业状态改为 R ，放入待执行队列
        for (auto &cur : waitingQueue) {
            if (cur->arrive_time <= total_time && cur->status == 'W') {
                cur->status = 'R';
                executingQueue.push(cur);
            }
        }
        // 处理待执行队列中的作业，处理完一个作业后，计算对应的周转时间和带权周转时间，然后跳出循环。
        while (!executingQueue.empty()) {
            auto &cur = executingQueue.top();
            total_time += cur->serve_time;
            cur->status = 'R';
            show_single_running_JCB(cur);
            double average =
                    1.0 * (total_time - cur->arrive_time) / cur->serve_time;
            cur->status = 'F';
            show_single_finished_JCB(cur, total_time - (cur->arrive_time), average);
            executingQueue.pop();
            total_waiting--;//每完成一个作业，总数减一
            break;
        }
        //等待队列中的任务数为零，且 待执行队列为空 时，算法结束。
        if (total_waiting == 0 && executingQueue.empty()) {
            cout<<"******SJF算法结束******\n\n";
            break;
        }
    }
}

//删除作业状态为 F 的作业。
void Schedule::erase_job() {
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'F') Queue.erase(it);
    }
}

int main() {
    Schedule work;  // define a work
    work.init_job();  // input the tasks
    work.show_JCB_info();   // show all the tasks
    work.SJF();  // call SJF
    work.show_JCB_info();

    return 0;
}
