/*
 * @Author: Chenglin Wu
 * @Date:   2020-04-30 01:45:00
 * @Last Modified by:   Chenglin Wu
 * @Last Modified time: 2020-05-01 17:38:11
 */
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
using namespace std;
class memory {
   public:
    struct partition {
        int block_size;  // partition size
        int start_pos;   // partition start position
        bool status;     // status: (true, used), (false, empty)
        partition(int _bs = 0, int _sp = 0, bool _st = false)
            : block_size(_bs), start_pos(_sp), status(_st) {}
    };
    struct used {
        string name;     // partition name
        int block_size;  // partition size
        int start_pos;   // partition start position
        bool status;     // status: (true, used), (false, empty)
        used(string _n = string("*"), int _bs = 0, int _sp = 0,
             bool _st = false)
            : name(_n), block_size(_bs), start_pos(_sp), status(_st) {}
    };
    void init();
    bool alloc(list<partition>::iterator &pos, const string &_name,
               const int &_size);
    void input();
    void select();
    void switch_(string _n, int _s, int _i);
    void FF(const string &_name, const int &_size,
            const int num = -1);  // first fit
    void NF(const string &_name, const int &_size,
            const int num = -1);  // next fit
    void BF(const string &_name, const int &_size,
            const int num = -1);  // best fit

    bool display_menu();
    void recycle(const string &_name);
    void show();
    struct addr {
        bool operator()(const partition &a, const partition &b) {
            return a.start_pos < b.start_pos;
        }
        bool operator()(const used &a, const used &b) {
            return a.start_pos < b.start_pos;
        }
    } addr;


    struct size {
        bool operator()(const used &a, const used &b) {
            return a.block_size < b.block_size;
        }
        bool operator()(const partition &a, const partition &b) {
            return a.block_size < b.block_size;
        }
    } size;
    int SET;  // set an alloc algorithm
    int SIZE;
    int min_size;
    list<partition>::iterator pos;

   private:
    list<partition> empty_table;
    list<used> used_table;
    list<pair<string, int> > all_jobs;
};

void memory::init() {
    all_jobs.clear();
    empty_table.clear();
    used_table.clear();
    SET      = 3;
    min_size = 4;
    int N;
    cout << "Please input the size of the memory: " << endl;
    cin >> N;
    partition first(N, 0, false);
    empty_table.push_back(first);
}

void memory::FF(const string &_name, const int &_size, const int num) {
    empty_table.sort(addr);
    pos = empty_table.begin();
    if (alloc(pos, _name, _size))
        cout << "the " << (num == -1 ? used_table.size() + 1 : num)
             << "th job success!" << endl;
    else
        cout << "the " << (num == -1 ? used_table.size() + 1 : num)
             << "th job failed!" << endl;
}
void memory::NF(const string &_name, const int &_size, const int num) {
    if (alloc(pos, _name, _size))
        cout << "the " << (num == -1 ? used_table.size() + 1 : num)
             << "th job success!" << endl;
    else
        cout << "the " << (num == -1 ? used_table.size() + 1 : num)
             << "th job failed!" << endl;
    empty_table.sort(addr);
}
void memory::BF(const string &_name, const int &_size, const int num) {
    empty_table.sort(size);
    pos = empty_table.begin();
    if (alloc(pos, _name, _size))
        cout << "the " << (num == -1 ? used_table.size() + 1 : num)
             << "th job success!" << endl;
    else
        cout << "the " << (num == -1 ? used_table.size() + 1 : num)
             << "th job failed!" << endl;
}

bool memory::alloc(list<partition>::iterator &pos, const string &_name,
                   const int &_size) {
    if (pos == empty_table.begin()) {
        for (auto &i : empty_table) {
            if (i.block_size - _size > min_size) {
                i.block_size -= _size;
                used_table.emplace_back(_name, _size, i.start_pos, true);
                i.start_pos += _size;
                //            empty_table.sort(addr);
                return true;
            }
        }
    } else {
        for (auto &it = pos; it != empty_table.end(); it++) {
            if (it->block_size - _size > min_size) {
                it->block_size -= _size;
                used_table.emplace_back(_name, _size, it->start_pos, true);
                it->start_pos += _size;
                //            empty_table.sort(addr);
                return true;
            }
        }
        auto left = empty_table.begin()--;
        for (auto &it = pos; it != left; it--) {
            if (it->block_size - _size > min_size) {
                it->block_size -= _size;
                used_table.emplace_back(_name, _size, it->start_pos, true);
                it->start_pos += _size;
                //            empty_table.sort(addr);
                return true;
            }
        }
    }

    return false;
}
void memory::select() {
    cout << "Please select an alloc algorithm" << endl;
    cout << "(0) fitst fit, FF\n(1) next fit, NF\n(2) best fit, BF \n(others) "
            "EXIT"
         << endl;
    cin >> SET;
}
void memory::switch_(string _n, int _s, int _i) {
    switch (SET) {
        case 0:
            FF(_n, _s, _i);
            break;
        case 1:
            NF(_n, _s, _i);
            break;
        case 2:
            BF(_n, _s, _i);
            break;
        default: {
            cout << "You choose a wrong algorithm, the program will be "
                    "aborted!"
                 << endl;
            exit(0);
        }
    }
}
void memory::input() {
    select();
    int N;
    cout << "Please input the number of your jobs: ";
    cin >> N;
    cout << "Please enter the name and block size, separated by a space."
         << endl;
    for (int i = 0; i < N; i++) {
        string name;
        int _size = 0;
        cin >> name >> _size;
        // check the name
        bool flag = false;
        for (auto j : used_table) {
            if (j.name == name) {
                cout << "The name is not available!" << endl;
                cout << "Please input again." << endl;
                flag = true;
                break;
            }
        }
        if (flag) {
            i--;
            continue;
        }
        switch_(name, _size, i);
    }
    while (display_menu()) {}
}

bool memory::display_menu() {
    cout << "Choose an operation." << endl;
    cout << "(0) recycle a job\n(1) show the memory\n(2) add a job" << endl;
    int op;
    cin >> op;
    switch (op) {
        case 0: {
            cout << "Please input the name of the job to be recycled: ";
            string _n;
            cin >> _n;
            recycle(_n);
            break;
        }
        case 1: {
            show();
            break;
        }
        case 2: {
            string _n;
            int _s, i;
            cout << "Please enter:" << endl;
            cout << "@@ name: ";
            cin >> _n;
            cout << "@@ size: ";
            cin >> _s;
            switch_(_n, _s, -1);
            break;
        }
        default: {
            cout << "Wrong input! The program will be aborted!" << endl;
            exit(0);
        }
    }
    return op == 0 || op == 1 || op == 2;
}
void memory::recycle(const string &_name) {
    for (auto i = used_table.begin(); i != used_table.end(); i++) {
        if (i->name == _name) {
            empty_table.sort(addr);
            cout << "### Before:" << endl;
            show();
            for (auto j = empty_table.begin(); j != empty_table.end(); j++) {
                auto &last = j;
                last++;
                if (last != empty_table.end() &&
                    j->start_pos + j->block_size == i->start_pos &&
                    j->start_pos + j->block_size + i->block_size ==
                        last->start_pos) {
                    j->block_size += i->block_size + last->block_size;
                    j->status = false;
                    empty_table.erase(last);

                } else if (j->start_pos + j->block_size == i->start_pos) {
                    j->block_size += i->block_size;
                    j->status = false;

                } else if (last != empty_table.end() &&
                           last->start_pos == i->start_pos + i->block_size) {
                    last->start_pos = i->start_pos;
                    last->block_size += i->block_size;
                    last->status = false;

                } else {
                    empty_table.emplace_back(i->block_size, i->start_pos,
                                             false);
                }
                used_table.erase(i);
                break;
            }
            cout << "### After:" << endl;
            show();
            return;
        }
    }
    cout << "Job name does not exist!" << endl;
}
void memory::show() {
    empty_table.sort(addr);
    used_table.sort(addr);
    cout << "Empty table:" << endl;
    cout << "block id | block size | start position | status " << endl;
    int count = 0;
    for (auto i : empty_table) {
        cout << setw(8) << count++ << setw(10 + 3) << i.block_size
             << setw(sizeof("start position") - 1 + 3) << i.start_pos
             << setw(6 + 3) << i.status << endl;
    }
    cout << "=============" << endl;
    cout << "Job table:" << endl;
    cout << "job name | block size | start position | status " << endl;
    count = 0;
    for (auto i : used_table) {
        cout << setw(8) << i.name << setw(10 + 3) << i.block_size
             << setw(sizeof("start position") - 1 + 3) << i.start_pos
             << setw(6 + 3) << i.status << endl;
    }
}
int main() {
    memory work1;
    work1.init();
    work1.input();

    return 0;
}

