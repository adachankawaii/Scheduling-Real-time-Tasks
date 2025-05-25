import tkinter as tk
from tkinter import ttk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.colors as mcolors

# Dark mode colors for UI
LEFT_BG_COLOR = "#2e2e2e"
LEFT_FG_COLOR = "#f0f0f0"
ENTRY_BG = "#4a4a4a"
BTN_BG = "#555555"
BTN_ACTIVE_BG = "#666666"

# Gantt chart background (light)
CHART_BG_COLOR = "#ffffff"

# Distinct colors for tasks
DISTINCT_COLORS = list(mcolors.TABLEAU_COLORS.values())

class SchedulerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("RTA Scheduler")
        self.root.state('zoomed')  # Fullscreen

        # Left frame for input (25% width)
        self.left_frame = tk.Frame(root, bg=LEFT_BG_COLOR, width=350)
        self.left_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=10)

        self.task_entries = []
        self.task_count = 0

        self.add_task_button = tk.Button(self.left_frame, text="Add Task", command=self.add_task_entry,
                                         bg=BTN_BG, fg=LEFT_FG_COLOR, activebackground=BTN_ACTIVE_BG)
        self.add_task_button.pack(pady=5)

        self.remove_task_button = tk.Button(self.left_frame, text="Delete Task", command=self.remove_task_entry,
                                            bg=BTN_BG, fg=LEFT_FG_COLOR, activebackground=BTN_ACTIVE_BG)
        self.remove_task_button.pack(pady=5)

        # Scheduling algorithm buttons
        self.edf_button = tk.Button(self.left_frame, text="EDF Scheduler", command=self.schedule_edf,
                                     bg=BTN_BG, fg=LEFT_FG_COLOR, activebackground=BTN_ACTIVE_BG)
        self.edf_button.pack(pady=5)

        self.rm_button = tk.Button(self.left_frame, text="RM Scheduler", command=self.schedule_rm,
                                     bg=BTN_BG, fg=LEFT_FG_COLOR, activebackground=BTN_ACTIVE_BG)
        self.rm_button.pack(pady=5)

        self.dm_button = tk.Button(self.left_frame, text="DM Scheduler", command=self.schedule_dm,
                                     bg=BTN_BG, fg=LEFT_FG_COLOR, activebackground=BTN_ACTIVE_BG)
        self.dm_button.pack(pady=5)

        self.llf_button = tk.Button(self.left_frame, text="LLF Scheduler", command=self.schedule_llf,
                                     bg=BTN_BG, fg=LEFT_FG_COLOR, activebackground=BTN_ACTIVE_BG)
        self.llf_button.pack(pady=5)

        # Right frame for Gantt chart (75% width)
        self.right_frame = tk.Frame(root, bg=CHART_BG_COLOR)
        self.right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

        self.canvas = None

    def add_task_entry(self):
        frame = tk.Frame(self.left_frame, bg=LEFT_BG_COLOR)
        frame.pack(pady=3)
        label = tk.Label(frame, text=f"T{self.task_count} C:", bg=LEFT_BG_COLOR, fg=LEFT_FG_COLOR)
        label.pack(side=tk.LEFT)
        exec_entry = tk.Entry(frame, width=10, bg=ENTRY_BG, fg=LEFT_FG_COLOR, insertbackground=LEFT_FG_COLOR)
        exec_entry.pack(side=tk.LEFT)

        label_d = tk.Label(frame, text="D:", bg=LEFT_BG_COLOR, fg=LEFT_FG_COLOR)
        label_d.pack(side=tk.LEFT)
        deadline_entry = tk.Entry(frame, width=10, bg=ENTRY_BG, fg=LEFT_FG_COLOR, insertbackground=LEFT_FG_COLOR)
        deadline_entry.pack(side=tk.LEFT)

        label_p = tk.Label(frame, text="P:", bg=LEFT_BG_COLOR, fg=LEFT_FG_COLOR)
        label_p.pack(side=tk.LEFT)
        period_entry = tk.Entry(frame, width=10, bg=ENTRY_BG, fg=LEFT_FG_COLOR, insertbackground=LEFT_FG_COLOR)
        period_entry.pack(side=tk.LEFT)

        self.task_entries.append((frame, exec_entry, deadline_entry, period_entry))
        self.task_count += 1

    def remove_task_entry(self):
        if self.task_entries:
            last = self.task_entries.pop()
            last[0].destroy()
            self.task_count -= 1

    def get_tasks(self):
        tasks = []
        for i, (_, exec_entry, deadline_entry, period_entry) in enumerate(self.task_entries):
            try:
                c = int(exec_entry.get())
                d = int(deadline_entry.get())
                p = int(period_entry.get())
                tasks.append({'id': i, 'exec': c, 'deadline': d, 'period': p, 'release': 0, 'remain': 0})
            except:
                continue
        return tasks

    def draw_chart(self, timeline, tasks, title):
        for widget in self.right_frame.winfo_children():
            widget.destroy()

        fig, ax = plt.subplots(figsize=(14, 5))
        fig.patch.set_facecolor(CHART_BG_COLOR)
        ax.set_facecolor(CHART_BG_COLOR)
        ax.tick_params(colors='black')
        ax.spines['bottom'].set_color('black')
        ax.spines['left'].set_color('black')
        ax.set_title(title, color='black')
        ax.set_xlabel("Time", color='black')
        ax.set_yticks(range(len(tasks)))
        # Add dashed horizontal lines between tasks

        ax.set_yticklabels([f"T{i}" for i in range(len(tasks))], color='black')
        ax.set_xticks(range(0, 17))  # Mốc thời gian đầy đủ
        ax.grid(axis='x', linestyle='--', color='gray', linewidth=0.5)  # Lưới dọc
        ax.set_axisbelow(True)  # Đặt lưới ở dưới các thanh

        ax.set_yticks(range(len(tasks)))
        ax.set_yticklabels([f"T{i}" for i in range(len(tasks))], color='black')
        for i in range(len(tasks) + 1):
            ax.axhline(i - 0.5, color='gray', linestyle='--', linewidth=0.8)


        colors = DISTINCT_COLORS[:len(tasks)]
        bars = {}

        for t, task_id in enumerate(timeline):
            if task_id is not None:
                if (task_id, t - 1) in bars:
                    bars[(task_id, t - 1)][1] += 1
                else:
                    bars[(task_id, t)] = [t, 1]

        for (task_id, start), (_, duration) in bars.items():
            ax.broken_barh([(start, duration)], (task_id - 0.4, 0.8),
                           facecolors=colors[task_id], edgecolors='black', label=f"T{task_id}")

        handles, labels = ax.get_legend_handles_labels()
        by_label = dict(zip(labels, handles))
        ax.legend(by_label.values(), by_label.keys(), loc='upper right', facecolor=CHART_BG_COLOR)

        canvas = FigureCanvasTkAgg(fig, master=self.right_frame)
        canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        canvas.draw()

    def schedule_llf(self):
        tasks = self.get_tasks()
        if not tasks:
            return

        start_time = 0
        end_time = 16
        timeline = []

        for t in range(start_time, end_time):
            for task in tasks:
                if t >= task['release'] and task['remain'] == 0:
                    task['remain'] = task['exec']
                    task['deadline_time'] = task['release'] + task['deadline']

            min_laxity = float('inf')
            selected = None
            for task in tasks:
                if task['remain'] > 0:
                    laxity = task['deadline_time'] - t - task['remain']
                    if laxity < min_laxity:
                        min_laxity = laxity
                        selected = task

            if selected:
                selected['remain'] -= 1
                timeline.append(selected['id'])
                if selected['remain'] == 0:
                    selected['release'] += selected['period']
            else:
                timeline.append(None)

        self.draw_chart(timeline, tasks, "Gantt Chart - LLF")

    def schedule_edf(self):
        tasks = self.get_tasks()
        if not tasks:
            return

        timeline = []
        for t in range(16):
            for task in tasks:
                if t >= task['release'] and task['remain'] == 0:
                    task['remain'] = task['exec']
                    task['absolute_deadline'] = t + task['deadline']

            selected = None
            min_deadline = float('inf')
            for task in tasks:
                if task['remain'] > 0 and task['absolute_deadline'] < min_deadline:
                    min_deadline = task['absolute_deadline']
                    selected = task

            if selected:
                selected['remain'] -= 1
                timeline.append(selected['id'])
                if selected['remain'] == 0:
                    selected['release'] += selected['period']
            else:
                timeline.append(None)

        self.draw_chart(timeline, tasks, "Gantt Chart - EDF")

    def schedule_rm(self):
        tasks = self.get_tasks()
        if not tasks:
            return

        timeline = []
        for t in range(16):
            for task in tasks:
                if t >= task['release'] and task['remain'] == 0:
                    task['remain'] = task['exec']

            selected = None
            min_period = float('inf')
            for task in tasks:
                if task['remain'] > 0 and task['period'] < min_period:
                    min_period = task['period']
                    selected = task

            if selected:
                selected['remain'] -= 1
                timeline.append(selected['id'])
                if selected['remain'] == 0:
                    selected['release'] += selected['period']
            else:
                timeline.append(None)

        self.draw_chart(timeline, tasks, "Gantt Chart - RM")

    def schedule_dm(self):
        tasks = self.get_tasks()
        if not tasks:
            return

        timeline = []
        for t in range(16):
            for task in tasks:
                if t >= task['release'] and task['remain'] == 0:
                    task['remain'] = task['exec']

            selected = None
            min_deadline = float('inf')
            for task in tasks:
                if task['remain'] > 0 and task['deadline'] < min_deadline:
                    min_deadline = task['deadline']
                    selected = task

            if selected:
                selected['remain'] -= 1
                timeline.append(selected['id'])
                if selected['remain'] == 0:
                    selected['release'] += selected['period']
            else:
                timeline.append(None)

        self.draw_chart(timeline, tasks, "Gantt Chart - DM")

if __name__ == "__main__":
    root = tk.Tk()
    
    def on_closing():
        plt.close('all')  # Đóng tất cả các figure của matplotlib
        root.destroy()    # Hủy cửa sổ chính

    root.protocol("WM_DELETE_WINDOW", on_closing)
    app = SchedulerApp(root)
    root.mainloop()
