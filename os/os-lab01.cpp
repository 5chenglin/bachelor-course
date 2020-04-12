/*
 * @Author: Chenglin Wu
 * @Date:   2020-04-12 23:47:32
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-04-13 02:41:31
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
using namespace std;
class Schedule {
   public:
    struct Process {
        int id;
        int arrive_time;
        int serve_time;
        char status;
        Process(int _id = 0, int arrive = 0, int serve = 0, char _status = 'W')
            : id(_id),
              arrive_time(arrive),
              serve_time(serve),
              status(_status){};
    };

    void input_process();
    void init_work();
    void show_process();
    static bool FCFS_cmp(const list<Process>::iterator a,
                         const list<Process>::iterator b) {
        return a->arrive_time < b->arrive_time;
    }
    void FCFS();

    static bool SJF_cmp(const list<Process>::iterator a,
                        const list<Process>::iterator b) {
        return a->serve_time < b->serve_time;
    }
    void SJF();

    void execute();
    void erase_process();
    static void show_single_process(const list<Process>::iterator p) {
        cout << p->id << " " << p->arrive_time << " " << p->serve_time << " "
             << p->status << endl;
    }

   private:
    list<Process> Queue;
    vector<list<Process>::iterator> waitingQueue;
};

void Schedule::input_process() {
    ifstream in("process.txt");
    Process temp;
    while (!in.eof()) {
        in >> temp.arrive_time >> temp.serve_time >> temp.status;
        Queue.push_back(temp);
    }
}

void Schedule::init_work() {
    // if queue is not empty, then pop all the elements.
    if (!Queue.empty()) Queue.clear();

    // input the process from the file
    Schedule::input_process();
}

void Schedule::show_process() {
    if (!Queue.empty()) {
        for (auto p : Queue)
            cout << p.id << " " << p.arrive_time << " " << p.serve_time << " "
                 << p.status << endl;
    } else
        cout << "there is no process" << endl;
}

void Schedule::FCFS() {
    waitingQueue.clear();
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'W') { waitingQueue.emplace_back(it); }
    }
    sort(waitingQueue.begin(), waitingQueue.end(), FCFS_cmp);
}
void Schedule::SJF() {
    waitingQueue.clear();
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'W') { waitingQueue.emplace_back(it); }
    }
    sort(waitingQueue.begin(), waitingQueue.end(), SJF_cmp);
}


void Schedule::execute() {
    for (auto& i : waitingQueue) {
        i->status = 'R';
        show_single_process(i);
        i->status = 'F';
    }
}

void Schedule::erase_process() {
    for (auto it = Queue.begin(); it != Queue.end(); ++it) {
        if ((*it).status == 'F') Queue.erase(it);
    }
}

int main() {
    Schedule work1, work2;  // define two work
    work1.input_process();  // input the tasks
    work1.show_process();   // show all the tasks
    work1.FCFS();  // using the FCFS algorithms sort the processes which is
                   // waiting status
    work1.show_process();
    work1.execute();  // execute
    work1.show_process();

    work2.input_process();
    work2.show_process();
    work2.FCFS();
    work2.show_process();
    work2.execute();
    work2.show_process();

    return 0;
}
