#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

struct Task {
    int id;     // mã task
    int R;      // Thời gian phát hành
    int C;      // Worst-Case Execution Time
    int T;      // Period
    int D;      // Relative Deadline
};

// Một job cụ thể của task:
struct Job {
    int id;             // mã task gốc
    int rem;            // thời gian còn lại để hoàn thành
    int releaseTime;    // thời điểm phát hành
    int deadlineTime;   // thời điểm deadline tuyệt đối
    int Dline;          // thời điểm deadline tương đối = D
};

// Comparator để ưu tiên job có deadline nhỏ hơn
struct CmpDeadline {
    bool operator()(const Job &a, const Job &b) const {
        if (a.Dline != b.Dline)
            return a.Dline > b.Dline;
        return a.id > b.id;
    }
};

bool loadTasks(const string& filename,
        vector<Task>& tasks, int& simTime) 
{
    ifstream ifs(filename);
    if (!ifs) {
        cerr << "Cannot open " << filename << "\n";
        return false;
    }

    vector<string> lines;
    string line;
    while (getline(ifs, line)) {
        if (!line.empty())
            lines.push_back(line);
    }
    if (lines.empty()) {
        cerr << filename << " is empty\n";
        return false;
    }

    // Đọc dòng cuối lấy thời gian mô phỏng
    simTime = stoi(lines.back());
    lines.pop_back();

    int id = 1;
    for (auto& l : lines) {
        replace(l.begin(), l.end(), ',', ' ');
        istringstream iss(l);
        Task t;
        t.id = id++;
        iss >> t.R >> t.C >> t.T >> t.D;
        tasks.push_back(t);
    }
    return true;
}

void printSortedJobs(const vector<Job>& jobs) {
    cout << "-- Sorted jobs --\n";
    // Tiêu đề cột
    cout << left << setw(6) << "Task"   << "| "
        << left << setw(10) << "release" << "| "
        << left << setw(10) << "deadline" << "| "
        << left << setw(5) << "rem"      << "\n";
    // Dòng phân cách
    cout << string(6, '-') << "+-" 
        << string(10, '-') << "+-" 
        << string(10, '-') << "+-" 
        << string(5, '-')  << "\n";
    for (const auto &job : jobs) {
        cout << left << setw(6) << job.id
            << "| " << left << setw(10) << job.releaseTime
            << "| " << left << setw(10) << job.deadlineTime
            << "| " << left << setw(5)  << job.rem
            << "\n";
    }
    cout << "\n";
}

void printResponseTimeTest(const vector<Task>& tasks,
                            const vector<bool>& rtt)
{
    cout << "-- Response-Time Test --\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << "Task " << tasks[i].id << ": "
                    << (rtt[i] ? "Fail" : "OK") << "\n";
    }
    cout << "\n";
}

void printSchedule(const vector<int>& schedule) {
    cout << "-- Schedule (horizontal) --\n";
    const int labelW = 8;
    const int cellW  = 4;
    const size_t maxCols = 20;  // tối đa 20 ô mỗi dòng

    size_t n = schedule.size();
    for (size_t start = 0; start < n; start += maxCols) {
        size_t end = min(n, start + maxCols);

        // Tiêu đề thời gian
        cout << setw(labelW) << "Time";
        for (size_t i = start; i < end; ++i) {
            cout << "|" << setw(cellW) << i;
        }
        cout << "|\n";

        // Dòng phân cách
        cout << string(labelW, '-');
        for (size_t i = start; i < end; ++i) {
            cout << "+" << string(cellW, '-');
        }
        cout << "+\n";

        // Dòng ID task
        cout << setw(labelW) << "Task ID";
        for (size_t i = start; i < end; ++i) {
            cout << "|" << setw(cellW) << schedule[i];
        }
        cout << "|\n\n";
    }
}

// Sinh tất cả các job trong khoảng simTime
void generateJobs(const vector<Task>& tasks,
                  int simTime,
                  vector<Job>& jobs)
{
    for (auto &t : tasks) {
        for (int k = 0; t.R + t.T * k < simTime; ++k) {
            int rel = t.R + k * t.T;
            jobs.push_back(Job{t.id, t.C, rel, rel + t.D, t.D});
        }
    }
    // Sắp xếp theo thời điểm release để dễ duyệt
    sort(jobs.begin(), jobs.end(),
         [](auto &a, auto &b){
             return a.releaseTime < b.releaseTime;
         });

    // In ra danh sách các job đã sắp xếp
    printSortedJobs(jobs);
}

// Mô phỏng Deadline-Monotonic scheduling
bool simulateDM(const vector<Task>& tasks, int simTime) {
    vector<Job> jobs;
    generateJobs(tasks, simTime, jobs);
    priority_queue<Job, vector<Job>, CmpDeadline> readyQ;

    int t = 0, idx = 0;
    // Mảng lưu lịch (job.id chạy tại mỗi ms)
    vector<int> schedule(simTime, -1);

    while (t < simTime) {
        // Release tất cả job mới tại t
        while (idx < (int)jobs.size() && jobs[idx].releaseTime == t) {
            readyQ.push(jobs[idx++]);
        }
        if (!readyQ.empty()) {
            Job cur = readyQ.top(); readyQ.pop();
            // Thực thi 1 ms
            schedule[t] = cur.id;
            cur.rem--;
            // Kiểm tra deadline
            if (t+1 > cur.deadlineTime && cur.rem >= 0) {
                cout << "[MISS] Task " << cur.id
                          << " missed deadline at t=" << t+1 << "\n";
                return false;
            }
            // Nếu chưa xong, đẩy lại vào queue
            if (cur.rem > 0) {
                readyQ.push(cur);
            }
        }
        ++t;
    }

    // In ra lịch chạy (hàng ngang)
    printSchedule(schedule);
    
    return true;
}

// Kiểm tra khả năng lập lịch bằng Response-Time Analysis
#include <numeric>  // for iota

// Trả về vector<bool> báo miss deadline cho từng task (theo thứ tự ban đầu)
vector<bool> responseTimeTest(const vector<Task>& tasks) {
    int n = tasks.size();
    // Chỉ số các task theo thứ tự ban đầu
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    // Sắp xếp theo D tăng dần
    sort(idx.begin(), idx.end(),
              [&](int a, int b) { return tasks[a].D < tasks[b].D; });

    vector<bool> miss(n, false);
    // Với mỗi task theo thứ tự deadline-monotonic
    for (int k = 0; k < n; ++k) {
        int i = idx[k];
        int R = tasks[i].C;
        int R_prev = 0;
        // Lặp tính response time
        do {
            R_prev = R;
            int interference = 0;
            for (int j = 0; j < k; ++j) {
                int h = idx[j];
                interference +=
                  ((R_prev + tasks[h].T - 1) / tasks[h].T) * tasks[h].C;
            }
            R = tasks[i].C + interference;
        } while (R != R_prev && R <= tasks[i].D);

        // Nếu R > D thì miss
        miss[i] = (R > tasks[i].D);
    }
    return miss;
}

int main() {
    vector<Task> tasks;
    int simTime = 0;

    if (!loadTasks("tasks.txt", tasks, simTime))
        return 1;

    auto rtt = responseTimeTest(tasks);
    printResponseTimeTest(tasks, rtt);

    simulateDM(tasks, simTime);
    return 0;
}