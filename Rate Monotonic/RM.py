# Lập lịch RM
def rm_schedule(tasks, duration=None):
    # Sắp xếp tasks theo chu kỳ tăng dần (ưu tiên cao nhất là task có chu kỳ nhỏ nhất)
    tasks = sorted(tasks, key=lambda x: x[1])

    if duration is None:
        max_period = max(t[1] for t in tasks)
        duration = 3 * max_period

    n = len(tasks)
    timeline = [0] * duration
    remaining_time = [0] * n
    release_time = [0] * n  # Thời điểm phát hành gần nhất cho từng task

    for t in range(duration):
        # Cập nhật các task được phát hành tại thời điểm t
        for i in range(n):
            if t % tasks[i][1] == 0:
                remaining_time[i] = tasks[i][0]
                release_time[i] = t

        # Chọn task ưu tiên cao nhất có remaining_time > 0
        for i in range(n):
            if remaining_time[i] > 0 and release_time[i] <= t:
                timeline[t] = i + 1  # Đánh dấu task chạy tại thời điểm t
                remaining_time[i] -= 1
                break  # Task ưu tiên cao nhất được chạy, ngắt các task thấp hơn

    return timeline, tasks


# Vẽ biểu đồ
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

def draw_timing_diagram(timeline, task_list):
    fig, ax = plt.subplots(figsize=(16, 6))
    n_tasks = len(task_list)
    colors = ['skyblue', 'lightgreen', 'salmon', 'orange', 'violet']
    end_time = len(timeline)

    # Vẽ mỗi task một dòng
    for i in range(n_tasks):
        y = n_tasks - i - 1  # để dòng trên cùng là Task 1
        start = None
        for t in range(end_time):
            if timeline[t] == i + 1:
                if start is None:
                    start = t
            else:
                if start is not None:
                    ax.broken_barh([(start, t - start)], (y - 0.4, 0.8),
                                   facecolors=colors[i % len(colors)])
                    for tt in range(start, t):
                        ax.text(tt + 0.2, y, str(tt), fontsize=8)
                    start = None
        if start is not None:
            ax.broken_barh([(start, end_time - start)], (y - 0.4, 0.8),
                           facecolors=colors[i % len(colors)])
            for tt in range(start, end_time):
                ax.text(tt + 0.2, y, str(tt), fontsize=8)

    # Vẽ đường phân cách chu kỳ riêng cho từng task
            
    for idx, task in enumerate(task_list):
        _, T = task
        # Đảo chiều để vẽ từ dòng cao xuống thấp
        reversed_idx = n_tasks - idx - 1
        for p in range(0, end_time + 1, T):
            ax.axvline(
                x=p,
                ymin=(reversed_idx) / n_tasks,
                ymax=(reversed_idx + 1) / n_tasks,
                color='black', linestyle='--', linewidth=1.5
            )

    # Cấu hình trục và nhãn
    ax.set_yticks(range(n_tasks))
    ax.set_yticklabels([f'Task {n_tasks - i}' for i in (range(n_tasks))], fontsize = 20)
    ax.set_xticks(range(0, end_time + 1))  # giây tròn
    ax.set_xticklabels([str(i) for i in range(0, end_time + 1)], fontsize = 12)
    ax.set_xlim(0, end_time)
    ax.set_xlabel("Time (seconds)", fontsize = 15)
    ax.set_title("RATE MONOTONIC SCHEDULING DIAGRAM", fontsize = 30)
    plt.grid(axis='x', linestyle=':', linewidth=0.5)

    # Thêm chú thích màu task
    legend = [mpatches.Patch(color=colors[i % len(colors)], label=f'Task {i + 1}') for i in range(n_tasks)]
    ax.legend(handles=legend)
    plt.tight_layout()
    plt.show()

# --- Ví dụ chạy thử ---
if __name__ == "__main__":
    # Mỗi task: (thời gian thực thi, chu kỳ)
    tasks = [(3,20), (2,5), (2,10)]  # Task 1, 2, 3

    timeline, tasks = rm_schedule(tasks)
    draw_timing_diagram(timeline, tasks)
