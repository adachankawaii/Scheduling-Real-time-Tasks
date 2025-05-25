#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
 
typedef struct {
    int period;      // P – chu ky
    int wcet;        // C – thoi gian thuc thi toi da
    int deadline;    // D – han chot (tuong doi hoac tuyet doi)
} Task;

int max_deadline(Task tasks[], int n) {
    int max = tasks[0].deadline;
    for (int i = 1; i < n; ++i)
        if (tasks[i].deadline > max) max = tasks[i].deadline;
    return max;
}

void schedulability_test(Task tasks[], int n) {
    // Kiem tra kha thi cua bo task theo EDF: tong C_i / P_i <= 1
    double utilization = 0.0;
    for (int i = 0; i < n; ++i)
        utilization += (double)tasks[i].wcet / (double)tasks[i].period;

    printf("Kiem tra kha thi (EDF): U = %.2f  <=  1.00 ?  %s\n",
           utilization,
           utilization <= 1.0 ? "DAT" : "KHONG DAT");
}

Task *clone_tasks(Task *src, int n) {
    // Tao ban sao mang task
    Task *dst = (Task*)malloc(n * sizeof(Task));
    if (!dst) { perror("malloc"); exit(EXIT_FAILURE); }
    memcpy(dst, src, n * sizeof(Task));
    return dst;
}

void print_timeline(int *timeline, int n_tasks, int hyperperiod) {
    // In lich chay theo kieu Gantt
    for (int t = 0; t < n_tasks; ++t) {
        printf("T%d\t", t + 1);
        for (int time = 0; time < hyperperiod; ++time)
            printf("%c\t", timeline[time] == t + 1 ? '#' : '-');
        printf("\n");
    }
    printf("t\t");
    for (int time = 0; time < hyperperiod; ++time) printf("%d\t", time);
    printf("\n");
}

void earliest_deadline_first(Task tasks[], int n) {
    int hyperperiod = max_deadline(tasks, n);        // mo phong den D_max
    int *timeline   = (int*)calloc(hyperperiod, sizeof(int));
    int *next_start = (int*)calloc(n, sizeof(int));  // thoi diem release tiep theo
    Task *active    = clone_tasks(tasks, n);         // ban sao de cap nhat WCET con lai

    int current     = -1;    // task dang chay

    for (int time = 0; time < hyperperiod; ++time) {
        // Cap nhat job moi duoc release
        for (int i = 0; i < n; ++i) {
            if (time == next_start[i]) {
                active[i].wcet     = tasks[i].wcet;            // reset WCET
                active[i].deadline = time + tasks[i].deadline; // deadline tuyet doi
                next_start[i]     += tasks[i].period;          // thoi gian release ke tiep
            }
        }

        // Chon job co deadline gan nhat va van con WCET
        int chosen = -1, min_deadline = INT_MAX;
        for (int i = 0; i < n; ++i)
            if (active[i].wcet > 0 && active[i].deadline < min_deadline) {
                min_deadline = active[i].deadline;
                chosen = i;
            }

        if (chosen != -1) {                // Co job de chay
            active[chosen].wcet--;         // Giam WCET
            timeline[time] = chosen + 1;   // Danh dau task chay tai thoi diem nay
            current = chosen;
        } else {
            timeline[time] = 0;            // CPU idle
            current = -1;
        }
    }

    print_timeline(timeline, n, hyperperiod);

    // Giai phong bo nho
    free(timeline);  free(next_start);  free(active);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Cach dung: %s tasks.txt\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) { perror("fopen"); return 1; }

    char line[64];
    int n = 0;
    while (fgets(line, sizeof(line), file))
        if (line[0] != '\n') ++n; // Dem so dong (bo qua dong rong)
    rewind(file);

    Task *tasks = (Task*)malloc(n * sizeof(Task));
    if (!tasks) { perror("malloc"); return 1; }

    fgets(line, sizeof(line), file); // Bo qua dong tieu de
    for (int i = 0; i < n && fgets(line, sizeof(line), file); ++i)
        sscanf(line, "%d %d %d", &tasks[i].period, &tasks[i].wcet, &tasks[i].deadline);

    puts("\nDanh sach task:");
    for (int i = 0; i < n; ++i)
        printf("T%d: P=%d, C=%d, D=%d\n", i + 1,
               tasks[i].period, tasks[i].wcet, tasks[i].deadline);

    puts("\n================= Earliest Deadline First (EDF) =================\n");
    schedulability_test(tasks, n);
    earliest_deadline_first(tasks, n);

    free(tasks);
    return 0;
}
