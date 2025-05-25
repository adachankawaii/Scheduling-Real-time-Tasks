#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_TASKS 10 // So luong tien trinh toi da
#define START_TIME 1 // Thoi gian bat dau lap lich
#define END_TIME 15  // Thoi gian ket thuc lap lich

// Cau truc mo ta mot tien trinh
typedef struct {
    char id;            // Ten tien trinh
    int exec_time;      // Thoi gian thuc thi
    int deadline;       // Deadline tuong doi
    int period;         // Chu ky lap lai
    int next_release;   // Thoi diem release tiep theo
    int remaining;      // Thoi gian con lai de chay
    int abs_deadline;   // Deadline tuyet doi cua lan hien tai
    int active;         // Co ton tai hay khong (1: ton tai, 0: da xoa)
} Task;

// Ham tinh do long leo (laxity) cua tien trinh
int laxity(Task* task, int current_time) {
    return task->abs_deadline - current_time - task->remaining;
}

// Thuat toan lap lich LLF
void llf_scheduler(Task tasks[], int n) {
    char gantt[END_TIME - START_TIME + 1]; // Mang ghi Gantt chart
    int gantt_idx = 0;

    for (int time = START_TIME; time <= END_TIME; time++) {
        // Kiem tra release cac tien trinh moi
        for (int i = 0; i < n; i++) {
            if (time - 1 >= tasks[i].next_release) { // Neu den thoi diem release
                tasks[i].remaining = tasks[i].exec_time; // Reset thoi gian chay
                tasks[i].abs_deadline = time - 1 + tasks[i].deadline; // Deadline tuyet doi
                tasks[i].active = 1; // Danh dau tien trinh ton tai
                tasks[i].next_release += tasks[i].period; // Cap nhat release lan sau
            }
        }

        int min_lax = INT_MAX; // Gia tri laxity nho nhat
        int selected = -1; // Luu tien trinh duoc chon

        // Tim tien trinh co laxity nho nhat
        for (int i = 0; i < n; i++) {
            if (tasks[i].active && tasks[i].remaining > 0) {
                int lax = laxity(&tasks[i], time - 1); // Tinh laxity
                if (lax < 0) { // Neu tien trinh da tre deadline
                    tasks[i].active = 0; // Vo hieu tien trinh
                    tasks[i].remaining = 0; // Xoa thoi gian con lai
                    continue;
                }
                if (lax < min_lax) { // Chon tien trinh co laxity nho nhat
                    min_lax = lax;
                    selected = i;
                }
            }
        }

        // Lap lich tien trinh duoc chon
        if (selected == -1) {
            gantt[gantt_idx++] = '_'; // CPU idle neu khong co tien trinh nao chay
        } else {
            gantt[gantt_idx++] = tasks[selected].id; // Ghi nhan tien trinh chay
            tasks[selected].remaining--; // Giam thoi gian chay cua tien trinh
            if (tasks[selected].remaining == 0) {
                tasks[selected].active = 0; // Danh dau tien trinh hoan thanh
            }
        }
    }

    // In Gantt chart
    printf("\n>> Gantt Chart (Time %d to %d):\n", START_TIME, END_TIME);
    for (int i = START_TIME; i <= END_TIME; i++) {
        printf(" %2d", i);
    }
    printf("\n");
    for (int i = 0; i < gantt_idx; i++) {
        printf("  %c", gantt[i]);
    }
    printf("\n");
}

// Ham main
int main() {
    Task tasks[MAX_TASKS]; // Mang chua cac tien trinh
    int n; // So tien trinh

    printf("Nhap so tien trinh: ");
    scanf("%d", &n);

    if (n > MAX_TASKS) { // Kiem tra gioi han so tien trinh
        printf("Vuot qua so tien trinh toi da.\n");
        return 1;
    }

    // Nhap tung tien trinh
    for (int i = 0; i < n; i++) {
        printf("\nTien trinh %d:\n", i + 1);
        printf("ID (A-Z): ");
        scanf(" %c", &tasks[i].id);
        printf("Exec time: ");
        scanf("%d", &tasks[i].exec_time);
        printf("Deadline: ");
        scanf("%d", &tasks[i].deadline);
        printf("Period: ");
        scanf("%d", &tasks[i].period);

        // Khoi tao gia tri ban dau
        tasks[i].next_release = 0;
        tasks[i].remaining = 0;
        tasks[i].abs_deadline = 0;
        tasks[i].active = 0;
    }

    // Goi ham lap lich LLF
    llf_scheduler(tasks, n);

    return 0;
}
