/*
 * @Author: Chenglin Wu
 * @Date:   2020-04-12 23:47:32
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-04-13 12:08:30
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
        string id;
        int arrive_time;
        int serve_time;
        char status;

        JCB(string _id = "A", int arrive = 0, int serve = 0, char _status = 'W')
                : id(_id), arrive_time(arrive), serve_time(serve), status(_status) {};
    };

    void input_JCB();

    void init_job();

    void show_JCB_info();

    void erase_process();

    void SJF();

    static bool arrive_time_Sort(const list<JCB>::iterator a, const list<JCB>::iterator b) {
        return a->arrive_time < b->arrive_time;
    }

    struct serve_time_Sort {
        bool operator()(const list<JCB>::iterator a, const list<JCB>::iterator b) {
            return a->serve_time > b->serve_time;
        }
    };

    static void show_single_JCB(const list<JCB>::iterator p, int transfer_time = -1, double average_time = -1) {
        cout << p->id << " " << p->arrive_time << " " << p->serve_time << " " << p->status << " " << transfer_time
             << " " << average_time << endl;
    }

private:
    static int total_time;
    static int total_waiting;
    list<JCB> Queue;
    vector<list<JCB>::iterator> waitingQueue;
};

int Schedule::total_time = 0;
int Schedule::total_waiting = 0;

void Schedule::input_JCB() {
    ifstream in("job.txt");
    JCB temp;
    while (!in.eof()) {
        in >> temp.id >> temp.arrive_time >> temp.serve_time;
        Queue.push_back(temp);
    }
}

void Schedule::init_job() {
    // if queue is not empty, then pop all the elements.
    total_time = 0;
    Queue.clear();

    // input the process from the file
    Schedule::input_JCB();
}

void Schedule::show_JCB_info() {
    if (!Queue.empty()) {
        for (auto p : Queue)
            cout << p.id << " " << p.arrive_time << " " << p.serve_time << " " << p.status << endl;
    } else
        cout << "there is no process" << endl;
}


void Schedule::SJF() {
    waitingQueue.clear();
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'W') { waitingQueue.emplace_back(it); }
    }
    sort(waitingQueue.begin(), waitingQueue.end(), arrive_time_Sort);
    total_waiting = waitingQueue.size();
    priority_queue<list<JCB>::iterator, vector<list<JCB>::iterator>, serve_time_Sort> executingQueue;

    while (!executingQueue.empty())
        executingQueue.pop();
    while (true) {
        for (auto &cur : waitingQueue) {
            if (cur->arrive_time <= total_time && cur->status == 'W') {
                cur->status = 'R';
                executingQueue.push(cur);
            }
        }
        while (!executingQueue.empty()) {
            auto &cur = executingQueue.top();
            total_time += cur->serve_time;
            cur->status = 'R';
            double average = 1.0 * (total_time - cur->arrive_time) / cur->serve_time;
            cur->status = 'F';
            show_single_JCB(cur, total_time - (cur->arrive_time), average);
            executingQueue.pop();
            total_waiting--;
            break;
        }
        if (total_waiting == 0 && executingQueue.empty()) {
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
    Schedule work;  // define a work
    work.init_job();  // input the tasks
    work.show_JCB_info();   // show all the tasks
    work.SJF();  // call SJF
    work.show_JCB_info();

    return 0;
}
