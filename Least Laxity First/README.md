LLF Scheduler in C (Least Laxity First)

Description:
------------
This is a C implementation of a Least Laxity First (LLF) task scheduler. The program simulates real-time task scheduling based on task laxity (defined as: deadline - current_time - remaining_execution_time). It generates a Gantt chart to show how the CPU executes each task over time and lists which tasks finished on time or missed their deadlines.

Features:
---------
- Interactive input: Enter each task's information step-by-step.
- LLF-based scheduling algorithm.
- Gantt chart output to visualize task execution.
- Detailed reports on completed and missed tasks.

How It Works:
-------------
- The program simulates time from 0 up to a maximum time limit (default is 100).
- At each time unit:
    - It selects the task with the lowest laxity that has arrived and is still incomplete.
    - If no task is ready to run, the CPU stays idle for that time unit.
    - If any task has a negative laxity, it is marked as having missed its deadline.
- Tasks are executed one unit at a time until completion or deadline miss.

Input Format:
-------------
1. The program first asks for the number of tasks (maximum: 10).
2. For each task, the following information is entered interactively:
   - Task ID (a single uppercase character, e.g., A, B, C)
   - Arrival time (integer)
   - Execution time (integer)
   - Deadline (integer)

Example Input:
--------------
Enter number of tasks: 2

1:
Task ID (A-Z): A
Arrival time: 0
Exec time: 5
Deadline: 10

2:
Task ID (A-Z): B
Arrival time: 1
Exec time: 2
Deadline: 4

Output:
-------
- Gantt Chart: A visual timeline showing which task runs at each time unit.
- Completed Tasks: Tasks that finished before or at their deadline, with start and finish times.
- Missed Deadline Tasks: Tasks that couldn't be completed before their deadlines.

Example Output:
---------------
Gantt Chart:
 A A B B A A A _ _

>> Completed Tasks:
Task A: Start = 0, Finish = 7, Deadline = 10
Task B: Start = 2, Finish = 4, Deadline = 4

>> Missed Deadline Tasks:
None

How to Compile and Run:
------------------------
Use any standard C compiler (e.g., gcc):

Compile:
    gcc llf_scheduler.c -o llf

Run:
    ./llf

Limitations:
------------
- Only supports up to 10 tasks (can be changed via MAX_TASKS).
- Simulation runs for up to 100 time units (changeable via MAX_TIME).
- Input must be valid; no advanced error handling for invalid entries.
- Task IDs must be uppercase characters (A-Z).

License:
--------
This project is released for my OS project at HUST. Feel free to use, modify, and distribute.  