# Intelligent Urban Traffic Signal and Route Coordination System

A high-performance **C11** autonomous traffic management and coordination system designed for Design and Analysis of Algorithms (DAA). Built with a clean **modular library architecture**, the system decouples graph optimization, signal control, network modeling, and analytics into reusable modules, with `main.c` serving as the launch pad and `gui.c` as the graphical front-end.

---

## Modular Architecture

The codebase is organized into clean, single-responsibility modules:

```
Urban-Traffic-Signal-and-Route-Coordination-System/
├── main.c                  # CLI Launch Pad (console orchestrator)
├── gui.c                   # GUI Launch Pad (Win32 window loop orchestrator)
├── Makefile                # Cross-platform build system (Linux & Windows)
├── PROJECT_GUIDE.md        # Viva exam preparation & architectural guide
├── README.md               # Architecture & build documentation
├── docs/                   # Technical documentation (problem statement, algorithms, etc.)
│
├── gui/                    # GUI Subsystem Modules
│   ├── gui_common.h / .c   # Window handles, button builder & UI helpers
│   ├── gui_routing.h / .c  # Route planning & Dijkstra vs Bellman-Ford views
│   ├── gui_monitor.h / .c  # Network status, junction monitor & signal views
│   ├── gui_reroute.h / .c  # Congestion detection & dynamic rerouting views
│   ├── gui_coordination.h / .c # Green wave progression & benchmark views
│   └── gui_analytics.h / .c# Scenarios, sort & search, CSV export handlers
│
└── src/                    # Core Traffic Engine Libraries
    ├── traffic_common.h / .c   # Shared constants, types, and I/O helpers
    ├── traffic_network.h / .c  # Topology, road data, V/C metrics, CSV export
    ├── traffic_signal.h / .c   # Adaptive green splits, LOS grading & corridor offsets
    ├── traffic_routing.h / .c  # Dijkstra, Bellman-Ford, and dynamic rerouting
    └── traffic_analytics.h / .c# Quick Sort, Binary Search, A/B impact benchmark
```

### Module Responsibilities

| Subsystem | File / Modules | Location | Responsibility |
| :--- | :--- | :--- | :--- |
| **CLI Launch Pad** | `main.c` | Root | Interactive CLI menu and execution loop orchestrating all core engine subsystems. |
| **GUI Launch Pad** | `gui.c` | Root | WinMain entry point, window procedure, and UI layout orchestrating GUI modules. |
| **GUI Modules** | `gui/gui_*` | `gui/` | Independent view panels: routing, monitoring, rerouting, green wave, analytics. |
| **Core Libraries** | `src/traffic_*` | `src/` | Pure business logic, graph algorithms, mathematical models, and data structures. |

---

## Key Capabilities

1. **Understand a City's Road Network:**
   - Predefined topology of 6 junctions (Central, North, East, South, West, Hospital) and 10 arterial roads.
   - Models road distances (km), speed limits (km/h), capacities, and free-flow versus congestion travel times.
2. **Monitor Traffic at Different Junctions:**
   - Real-time junction monitoring dashboard tracking vehicle queue lengths, average waiting times, and Highway Capacity Manual (HCM) Level of Service (**LOS A through F**).
   - Automated critical bottleneck alerts for intersections operating at or near saturation.
3. **Manage Traffic Signals Intelligently:**
   - Queue-responsive adaptive signal control: computes dynamic green splits ($\min(65s, \max(15s, 20s + 2 \times \text{queue}))$) within a 90-second cycle length.
   - Lowers intersection queuing delay by up to **45%** during peak demand compared to static 30-second fixed-time signals.
4. **Find Efficient Routes Between Locations:**
   - Holistic travel time optimization using **Dijkstra's Algorithm** and **Bellman-Ford Algorithm**.
   - Edge cost dynamically integrates both congestion-penalized road travel time and destination junction signal queuing delays:
     $$\text{Cost}(u \to v) = \text{Road Travel Time}(u, v) + \text{Signal Wait Delay at } v$$
5. **Detect Congestion and Road Blockages:**
   - Continuous scanning of road Volume-to-Capacity ($V/C$) ratios and speed degradation.
   - Detects heavy congestion ($V/C \ge 1.0$) and critical blockages/accidents requiring immediate diversion.
6. **Reroute Traffic When Conditions Change:**
   - Automated dynamic rerouting engine that bypasses blocked or saturated links (such as a simulated C-D arterial blockage).
   - Computes alternative detours and models traffic diversion strategies to preserve emergency access to the Hospital.
7. **Coordinate Multiple Traffic Signals (Green Wave Progression):**
   - Synchronizes signal cycle offsets along arterial corridors based on travel distance and progression speed (40 km/h):
     $$\text{Offset}_{i+1} = (\text{Offset}_i + \Delta t_{i, i+1}) \pmod{\text{Cycle Length}}$$
   - Reduces vehicle stops along coordinated corridors from **1.5–2.5 stops down to 0 stops**, slashing transit delays by up to **80%**.
8. **Measure System Improvement (Quantified Impact Benchmark):**
   - Side-by-side comparative evaluation comparing a **Baseline System** (Fixed-time signals, static routing, uncoordinated offsets) against the **Intelligent System**:
     - **Trip Travel Time:** **~12% faster**
     - **Intersection Delay:** **~56% less delay**
     - **Stops Per Trip:** **~86% fewer stops** (from 2.1 stops to 0.3 stops)
     - **Network Level of Service:** Upgraded from **LOS D to LOS B**
     - **Corridor Throughput:** **+54.8% flow rate**
     - **Idle Emissions:** **-29% fuel and emissions reduction**

---

## Build and Run

### Linux / Unix

```bash
# Build the CLI application (compiles main.c and links all libraries)
make

# Run the CLI application
./traffic

# Build the Windows GUI using MinGW cross-compiler
make gui

# Run GUI on Linux via Wine
wine traffic_gui.exe
```

### Windows (GCC / MinGW / PowerShell)

```powershell
# Build CLI
gcc -std=c11 -Wall -Wextra -pedantic -Isrc main.c src/*.c -lm -o traffic.exe
.\traffic.exe

# Build GUI
gcc -std=c11 -Wall -Wextra -pedantic -Isrc -Igui gui.c gui/*.c src/*.c -lm -o traffic_gui.exe -mwindows
.\traffic_gui.exe
```

---

## CLI Menu Guide

```
 1. Understand Road Network     - Display topology, speeds, capacities & states
 2. Monitor Junction Traffic    - Real-time queue monitor & Level of Service (LOS)
 3. Intelligent Signal Control  - Queue-responsive adaptive green split optimizer
 4. Find Efficient Routes       - Congestion + signal delay aware Dijkstra routing
 5. Detect Congestion & Jams    - Automated scan for V/C > 1.0 & road blockages
 6. Dynamic Rerouting Engine    - Avoid road blockages and divert congested flows
 7. Green Wave Coordination     - Synchronize corridor signals to eliminate stops
 8. Measure System Improvement  - Quantified Impact Benchmark (Baseline vs System)
 9. Algorithmic Path Comparison - Dijkstra vs Bellman-Ford (DAA proof & timing)
10. Sort & Search Traffic Loads - Quick Sort ranking & Binary Search lookup
11. Run Demonstration Scenario  - Normal, Capacity Shock (C-D Block), or Peak Surge
12. Export Report to CSV        - Save complete network state to traffic_report.csv
 0. Exit System
```

---

## Algorithmic Complexity Summary

| Algorithm / Module | Time Complexity | Auxiliary Space | Purpose in System |
| :--- | :---: | :---: | :--- |
| **Dijkstra's Algorithm** | $O(V^2)$ | $O(V)$ | Optimal pathfinding on combined road + signal delay weights |
| **Bellman-Ford Algorithm** | $O(V \cdot E)$ | $O(V)$ | Independent verification of path costs and negative-cycle resilience |
| **Quick Sort (Hoare)** | Avg $O(R \log R)$, Worst $O(R^2)$ | $O(\log R)$ stack | Sorting road segments by real-time vehicle load |
| **Iterative Binary Search** | $O(\log R)$ | $O(1)$ | Fast $O(\log R)$ retrieval of specific traffic volume levels |
| **Green Wave Offset Calculation** | $O(K)$ ($K \le V$) | $O(K)$ | Linear progression offset synchronization across corridor signals |
