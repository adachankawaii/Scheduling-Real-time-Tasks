# Deadline Monotonic Scheduling

This project implements Deadline‐Monotonic (DM) scheduling and Response‐Time Analysis (RTA) for a set of periodic real-time tasks in C++.

## Project Structure

- **Deadline_Monotonic.cpp**  
  - `loadTasks()` – load task set and simulation horizon from **tasks.txt**  
  - `responseTimeTest()` – perform RTA on each task  
  - `simulateDM()` – simulate DM scheduling and report any deadline misses   
- **tasks.txt**  
  Input file listing tasks (`R C T D`) and final line = simulation time.  

## Prerequisites

- C++11‐compatible compiler (g++, clang++, MSVC)
- (Optional) Visual Studio Code

## Building

### VS Code

1. Open this folder.  
2. Press `Ctrl+Shift+B` to run the **C/C++: g++.exe build active file** task.  

### Command Line (Windows)

```bat
g++ -std=c++11 Deadline_Monotonic.cpp -o Deadline_Monotonic.exe
```

### Command Line (Linux/macOS)

```sh
g++ -std=c++11 Deadline_Monotonic.cpp -o Deadline_Monotonic
```

## Usage

1. Prepare **tasks.txt** next to the executable.  
   Example:
   ```
   0 3 20 7
   0 2 5 4
   0 2 10 9
   20
   ```
2. Run:
   - Windows: `.\Deadline_Monotonic.exe`
   - Unix: `./Deadline_Monotonic`
3. Output:
   - Sorted job list  
   - RTA results (`OK` / `Fail`)  
   - Any “[MISS]” lines for deadline violations  
   - Horizontal schedule chart  

## License

This project is released for my OS project at HUST. Feel free to use, modify, and distribute.  