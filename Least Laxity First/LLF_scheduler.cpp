#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_TASKS 10
#define MAX_TIME 100

// Cau truc luu thong tin cua mot tien trinh
typedef struct {
    char id;           // Ten tien trinh (vi du: 'A', 'B', ...)
    int arrival;       // Thoi diem den
    int exec_time;     // Thoi gian thuc thi
    int deadline;      // Han deadline
    int remaining;     // Thoi gian con lai can thuc thi
    int completed;     // 0: chua xong, 1: da xong, -1: bi tre deadline
    int start_time;    // Thoi diem bat dau thuc thi
} Task;

// Ham tinh do long leo (laxity)
int laxity(Task* task, int current_time) {
    return task->deadline - current_time - task->remaining;
}

// Kiem tra tat ca tien trinh da hoan thanh chua
int all_done(Task tasks[], int n) {
    for (int i = 0; i < n; i++) {
        if (tasks[i].completed == 0) return 0;  // Neu con tien trinh chua xong thi tra ve 0
    }
    return 1;  // Tat ca da xong
}

// Ham lap lich theo thuat toan LLF (Least Laxity First)
void llf_scheduler(Task tasks[], int n) {
    int time = 0;  // Bien dem thoi gian
    char gantt_chart[MAX_TIME];  // Luu lai Gantt chart
    int gantt_len = 0;  // Do dai Gantt chart

    while (!all_done(tasks, n) && time < MAX_TIME) {
        int min_laxity = INT_MAX;  // Do long leo nho nhat
        int selected = -1;         // Chi so tien trinh duoc chon

        // Duyet tat ca tien trinh
        for (int i = 0; i < n; i++) {
            // Neu tien trinh da den va chua hoan thanh
            if (tasks[i].arrival <= time && tasks[i].completed == 0) {
                int lax = laxity(&tasks[i], time);  // Tinh laxity
                if (lax < 0) {
                    // Neu laxity < 0 thi danh dau la bi tre deadline
                    tasks[i].completed = -1;
                    continue;
                }
                // Chon tien trinh co laxity nho nhat
                if (lax < min_laxity) {
                    min_laxity = lax;
                    selected = i;
                }
            }
        }

        // Neu khong co tien trinh nao de chay thi CPU nhan
        if (selected == -1) {
            gantt_chart[gantt_len++] = '_'; // Danh dau idle
            time++;
            continue;
        }

        // Ghi lai thoi diem bat dau cua tien trinh (neu chua co)
        if (tasks[selected].start_time == -1)
            tasks[selected].start_time = time;

        // Thuc thi tien trinh trong 1 don vi thoi gian
        tasks[selected].remaining--;
        gantt_chart[gantt_len++] = tasks[selected].id;

        // Neu tien trinh thuc thi xong thi danh dau hoan thanh
        if (tasks[selected].remaining == 0)
            tasks[selected].completed = 1;

        time++;
    }

    // In Gantt chart
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_len; i++) {
        printf(" %c", gantt_chart[i]);
    }
    printf("\n");

    // In thong tin cac tien trinh hoan thanh
    printf("\n>> Completed Tasks:\n");
    for (int i = 0; i < n; i++) {
        if (tasks[i].completed == 1) {
            printf("Task %c: Start = %d, Finish = %d, Deadline = %d\n",
                   tasks[i].id,
                   tasks[i].start_time,
                   tasks[i].start_time + tasks[i].exec_time,
                   tasks[i].deadline);
        }
    }

    // In thong tin cac tien trinh bi tre deadline
    printf("\n>> Missed Deadline Tasks:\n");
    for (int i = 0; i < n; i++) {
        if (tasks[i].completed == -1) {
            printf("Task %c: Arrival = %d, Exec = %d, Deadline = %d\n",
                   tasks[i].id,
                   tasks[i].arrival,
                   tasks[i].exec_time,
                   tasks[i].deadline);
        }
    }
}

// Ham main de chay chuong trinh
int main() {
    Task tasks[MAX_TASKS];
    int n;

    printf("Nhap so tien trinh: ");
    scanf("%d", &n);
    getchar(); // Loai bo ky tu '\n' sau khi nhap so

    if (n > MAX_TASKS) {
        printf("So tien trinh vuot qua gioi han (%d).\n", MAX_TASKS);
        return 1;
    }

    for (int i = 0; i < n; i++) {
	    char id;
	    int arrival, exec_time, deadline;
	
	    printf("\n%d:\n", i + 1);
	
	    // Nhap Task ID
	    printf("Task ID (A-Z): ");
	    char buffer[10];
	    fgets(buffer, sizeof(buffer), stdin); // doc ca dong
	    id = buffer[0]; // lay ky tu dau tien
	
	    // Nhap Arrival
	    printf("Arrival time: ");
	    scanf("%d", &arrival);
	    getchar(); // loai bo \n
	
	    // Nhap Exec time
	    printf("Exec time: ");
	    scanf("%d", &exec_time);
	    getchar(); // loai bo \n
	
	    // Nhap Deadline
	    printf("Deadline: ");
	    scanf("%d", &deadline);
	    getchar(); // loai bo \n
	
	    // Luu vao cau truc
	    tasks[i].id = id;
	    tasks[i].arrival = arrival;
	    tasks[i].exec_time = exec_time;
	    tasks[i].deadline = deadline;
	    tasks[i].remaining = exec_time;
	    tasks[i].completed = 0;
	    tasks[i].start_time = -1;
	}

    llf_scheduler(tasks, n);

    return 0;
}

