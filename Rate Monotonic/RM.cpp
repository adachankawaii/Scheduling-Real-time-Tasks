#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Kiểm tra tính khả thi của hệ thống theo RM
bool is_schedulable(const vector<pair<int, int>>& tasks)
{
    int n = tasks.size();
    double U = 0;

    for (auto& task : tasks) {
        U += (double)task.first / task.second;
    }

    double bound = n * (pow(2.0, 1.0 / n) - 1);
    cout << "\nTổng tải hệ thống (U) = " << U << endl;
    cout << "Ngưỡng cho phép theo RM = " << bound << endl;

    return U <= bound;
}

// Hàm lập lịch Rate Monotonic
vector<int> rm_schedule(const vector<pair<int, int>>& tasks, int duration = -1) {
    int n = tasks.size();
    vector<pair<int, int>> sorted_tasks = tasks;

    // Sắp xếp theo chu kỳ tăng dần
    sort(sorted_tasks.begin(), sorted_tasks.end(), [](const pair<int, int>& a, const pair<int, int>& b)
    {
        return a.second < b.second;
    });

    if (duration == -1) {
        int max_period = 0;
        for (auto& task : sorted_tasks) {
            if (task.second > max_period)
                max_period = task.second;
        }
        duration = 3 * max_period;
    }

    vector<int> timeline(duration, 0);
    vector<int> remaining_time(n, 0);
    vector<int> release_time(n, 0);

    for (int t = 0; t < duration; ++t)
    {
        // Cập nhật các task được phát hành tại thời điểm t
        for (int i = 0; i < n; ++i)
        {
            if (t % sorted_tasks[i].second == 0)
            {
                remaining_time[i] = sorted_tasks[i].first;
                release_time[i] = t;
            }
        }

        // Chọn task có độ ưu tiên cao nhất (chu kỳ nhỏ nhất) còn thời gian
        for (int i = 0; i < n; ++i)
        {
            if (remaining_time[i] > 0 && release_time[i] <= t)
            {
                timeline[t] = i + 1; // Task i+1 được thực hiện tại thời điểm t
                remaining_time[i]--;
                break;
            }
        }
    }
    return timeline;
}

// --- Hàm main ---
int main()
{
    int n;
    cout << "Nhập số lượng tasks: ";
    cin >> n;

    vector<pair<int, int>> tasks(n);
    cout << "Nhập thời gian thực thi và chu kỳ của mỗi task (C T):\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "Task " << i + 1 << ": ";
        cin >> tasks[i].first >> tasks[i].second;
    }

    // Kiểm tra schedulability
    if (!is_schedulable(tasks))
    {
        cout << "\n!!! Hệ thống KHÔNG đảm bảo thực hiện đúng hạn theo Rate Monotonic.\n";
    }
    else
    {
        cout << "\n>>> Hệ thống CÓ THỂ thực hiện đúng hạn theo Rate Monotonic.\n";
    }

    // Thực hiện lập lịch
    vector<int> timeline = rm_schedule(tasks);

    // In kết quả lập lịch
    cout << "\nLịch thực hiện các task theo từng giây:\n";
    for (size_t i = 0; i < timeline.size(); ++i)
    {
        cout << timeline[i];
        if (i != timeline.size() - 1) cout << ", ";
    }
    cout << endl;

    return 0;
}
