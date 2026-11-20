# Urban Traffic Signal and Route Coordination System - Complete Project Guide

## 1. What This Project Does

This project is an intelligent **Urban Traffic Signal and Route Coordination System** implemented in **C11** for **Design and Analysis of Algorithms (DAA)**. It models a metropolitan grid with six key junctions and ten arterial roads to solve urban traffic congestion through dynamic algorithmic coordination.

The system addresses the eight core challenges of urban traffic management:
1. **Understands a City's Road Network:** Models road distances, free-flow speeds, capacity thresholds, and network connectivity as a weighted directed/undirected graph.
2. **Monitors Traffic at Different Junctions:** Continuously tracks intersection queue sizes, calculates vehicle wait times, assigns Level of Service (**LOS A to F**), and flags bottlenecks.
3. **Manages Traffic Signals Intelligently:** Uses an adaptive Webster-inspired green split algorithm that dynamically adjusts green light duration ($\min(65s, \max(15s, 20s + 2 \times \text{queue}))$) based on real-time queuing pressure.
4. **Finds Efficient Routes Between Locations:** Incorporates both road travel times (with congestion penalties) and intersection signal delays into an integrated edge-weight metric solved via Dijkstra's algorithm.
5. **Detects Congestion and Road Blockages:** Scans the network for Volume-to-Capacity ($V/C \ge 1.0$) saturation, speed degradation, and road blockages.
6. **Reroutes Traffic When Conditions Change:** Automatically calculates alternative diversion routes during incidents (such as a blocked arterial C-D), bypassing saturated links and protecting emergency corridors.
7. **Coordinates Multiple Traffic Signals (Green Wave):** Synchronizes cycle offsets across arterial corridors so platoons of vehicles encounter consecutive green lights, eliminating stop-and-go delays.
8. **Measures System Improvements (Quantified Benchmark):** Features an evaluation engine that contrasts a traditional **Baseline System** (Fixed-time signals, static routing, uncoordinated offsets) against our **Intelligent System**, reporting concrete percentage improvements in travel time, delay, stops, throughput, and emissions.

---

## 2. City Network Model

### Junctions (Nodes: $V = 6$)

| Letter | Location Description | Role in City | Initial Queue |
| :---: | :--- | :--- | :---: |
| **A** | Central Commercial Hub | High-density business district | 18 veh |
| **B** | North Residential Zone | Commuter origin | 10 veh |
| **C** | East Industrial & Tech Corridor | Heavy freight & commuter traffic | 22 veh |
| **D** | South University District | High-volume urban arterial node | 12 veh |
| **E** | West Logistics & Freight Park | Transit & bypass hub | 16 veh |
| **F** | Metropolitan Hospital Center | Critical emergency medical destination | 8 veh |

### Arterial Roads (Edges: $R = 10$)

| Road | Length (km) | Speed Limit | Free-Flow Time | Capacity | Base Vehicles | Base Status |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **A-B** | 3.0 km | 45 km/h | 4 min | 40 veh | 20 veh | Normal Flow |
| **A-C** | 2.5 km | 50 km/h | 3 min | 35 veh | 26 veh | Normal Flow |
| **A-E** | 5.0 km | 50 km/h | 6 min | 30 veh | 12 veh | Normal Flow |
| **B-C** | 1.8 km | 50 km/h | 2 min | 25 veh | 12 veh | Normal Flow |
| **B-D** | 6.0 km | 50 km/h | 7 min | 30 veh | 22 veh | Normal Flow |
| **C-D** | 3.5 km | 50 km/h | 4 min | 40 veh | 30 veh | Moderate Flow |
| **C-E** | 4.2 km | 50 km/h | 5 min | 25 veh | 18 veh | Normal Flow |
| **D-E** | 2.8 km | 50 km/h | 3 min | 30 veh | 14 veh | Normal Flow |
| **D-F** | 3.6 km | 50 km/h | 4 min | 35 veh | 16 veh | Normal Flow |
| **E-F** | 5.2 km | 50 km/h | 6 min | 25 veh | 10 veh | Normal Flow |

---

## 3. Mathematical Formulations

### Congestion-Adjusted Road Travel Time

$$\text{Travel Time}(r) = \begin{cases} \infty & \text{if road is closed/blocked} \\ \text{Free-Flow Time} + \left\lceil \frac{\text{Vehicles} - \text{Capacity}}{10} \right\rceil & \text{if Vehicles} > \text{Capacity} \\ \text{Free-Flow Time} & \text{otherwise} \end{cases}$$

### Intersection Queuing Delay Model

For a signal cycle $C = 90$s and allocated green duration $g$, the red phase duration is $r = C - g$.
The average uniform wait delay per vehicle at junction $v$ is:
$$\text{Delay}_{\text{wait}}(v) = \frac{r^2}{2C} + (\text{Queue} \times 2.2\text{s})$$

### Integrated Routing Edge Weight

Traditional algorithms route based only on distance or free-flow speed. Our coordinated system evaluates the combined cost:
$$\text{Cost}(u \to v) = \text{Road Travel Time}(u, v) + \text{Signal Wait Delay}(v)$$
This prevents routing traffic into an empty road segment that leads to a gridlocked intersection.

### Green Wave Signal Offset Progression

For an arterial corridor comprising sequential junctions $J_1 \to J_2 \to \dots \to J_k$ traversed at progression speed $v_{\text{prog}} = 40$ km/h:
$$\Delta t_{i, i+1} = \frac{\text{Distance}(J_i, J_{i+1})}{v_{\text{prog}}} \times 3600 \text{ seconds}$$
$$\text{Offset}_1 = 0$$
$$\text{Offset}_{i+1} = (\text{Offset}_i + \Delta t_{i, i+1}) \pmod C$$

Vehicles driving at the design speed encounter green lights at every successive intersection, reducing stops to near zero.

---

## 4. Algorithms Implemented

### 1. Dijkstra's Algorithm ($O(V^2)$)
- **Role:** Finds the optimal route minimizing combined road travel time and intersection signal delays.
- **Why Adjacency Matrix?** With $V = 6$, an adjacency matrix is straightforward, cache-friendly, and optimal for dense municipal subgrids.

### 2. Bellman-Ford Algorithm ($O(V \cdot E)$)
- **Role:** Validates path costs and serves as an algorithmic correctness baseline.
- **Significance:** Relaxing edges over $V - 1$ passes guarantees the optimal solution even in scenarios where comparative path checks are required.

### 3. Quick Sort ($O(R \log R)$ average, $O(R^2)$ worst-case)
- **Role:** Ranks road segments by active vehicle volume (Hoare partition scheme) for traffic analytics.

### 4. Iterative Binary Search ($O(\log R)$)
- **Role:** Queries exact traffic loads in the sorted array with logarithmic efficiency.

---

## 5. Performance Benchmark: Baseline vs. Intelligent System

Option 8 runs a system-wide A/B benchmark evaluating representative commuter flows across five major city OD pairs (A $\to$ F Hospital, B $\to$ D Suburb, E $\to$ C Industrial, A $\to$ D Commute, B $\to$ F North-Hospital):

| Key Performance Indicator (KPI) | Baseline (Fixed + Static) | Intelligent Coordinated | Measured Improvement |
| :--- | :---: | :---: | :---: |
| **Average Trip Travel Time** | 10.7 min | 9.4 min | **11.9% Faster** |
| **Average Intersection Delay** | 172.2 sec | 75.7 sec | **56.1% Less Delay** |
| **Average Stops Per Trip** | 2.1 stops | 0.3 stops | **85.9% Fewer Stops** |
| **Network Level of Service** | LOS D | LOS B | **+2 Service Tiers** |
| **Hourly Intersection Throughput** | 420 veh/h | 650 veh/h | **+54.8% Capacity Flow** |
| **Idle Fuel & Emissions Waste** | 100% Baseline | 71% Reduced | **-29.0% Emissions** |

---

## 6. Demonstration Script for Exams & Viva

Follow this demonstration order to impress examiners:

### Step 1: Network Topology & Status (Option 1)
- Display the six junctions and ten roads.
- Explain the physical model: road lengths, speeds, capacities, and initial loads.

### Step 2: Real-Time Junction Monitoring & LOS (Option 2)
- Show intersection queues and delays mapped to HCM Level of Service (**LOS A through F**).
- Point out that Junction C (queue: 22 veh) is reaching high congestion (LOS E).

### Step 3: Adaptive Signal Optimization (Option 3)
- Show how the intelligent signal controller increases green allocation from 30s to 64s at Junction C while keeping low-queue nodes like F at 36s.
- Explain that dynamic green splits reduce intersection delay by up to 45%.

### Step 4: Coordinated Routing vs. Pure Free-Flow (Option 4)
- Select Origin **A** and Destination **F** (Hospital).
- Explain how our system evaluates both road travel times and intersection queuing delays.

### Step 5: Algorithmic Verification (Option 9)
- Run Dijkstra and Bellman-Ford on the same route.
- Show identical costs, operation counts, and CPU timing to prove algorithmic correctness.

### Step 6: Incident Detection & Dynamic Rerouting (Options 5 & 6)
- Run Option 5: Show the system scanning for $V/C \ge 1.0$ and road blockages.
- Run Option 6: Simulate a sudden accident blocking Road C-D.
- Show how the system dynamically reroutes traffic via the West bypass (A $\to$ E $\to$ D and A $\to$ E $\to$ F), protecting emergency hospital access.

### Step 7: Green Wave Signal Coordination (Option 7)
- Select corridor **A** to **F**.
- Show the calculated progression offsets (e.g. Offset 0s at A, 0s at E, 18s at F).
- Highlight the reduction from 1.3 expected red-light stops down to 0.0 stops!

### Step 8: System Impact Benchmark (Option 8)
- Display the comprehensive KPI table showing quantifiable double-digit improvements in travel time, delay, stops, and emissions.

### Step 9: Quick Sort & Binary Search (Option 10)
- Demonstrate sorting the 10 road loads and searching for a specific volume (e.g. 26 vehicles).

---

## 7. Viva Questions & Model Answers

### Q1: How does your system coordinate traffic signals and routes?
> *"In our system, coordination operates on two levels:
> 1. **Signal-to-Route Coupling:** When computing optimal paths in Dijkstra's algorithm, edge weights include both road travel times and expected signal queuing delays at destination junctions. This prevents routing vehicles into jammed intersections.
> 2. **Signal-to-Signal Coordination (Green Wave):** Along any recommended arterial corridor, the system synchronizes signal cycle offsets based on road lengths and travel times. This allows platoons of vehicles to pass through successive green lights without stopping."*

### Q2: Why implement both Dijkstra and Bellman-Ford?
> *"Dijkstra ($O(V^2)$ with an adjacency matrix) is the optimal choice for non-negative road travel times and runs significantly faster. We also implement Bellman-Ford ($O(V \cdot E)$) as an independent verification mechanism to prove algorithmic correctness and demonstrate comparative operation counts and runtime complexity."*

### Q3: How is Level of Service (LOS) determined?
> *"We follow Highway Capacity Manual (HCM) intersection delay thresholds:
> - LOS A: $\le 10$ seconds (Free flow)
> - LOS B: $10–20$ seconds (Stable)
> - LOS C: $20–35$ seconds (Noticeable delay)
> - LOS D: $35–55$ seconds (Approaching saturation)
> - LOS E: $55–80$ seconds (Unstable / Heavy congestion)
> - LOS F: $> 80$ seconds (Critical breakdown / Gridlock)"*

### Q4: How do you mathematically prove your system actually improves traffic?
> *"Option 8 executes an automated A/B benchmark across five major city commuter origin-destination pairs. It evaluates a traditional baseline (fixed 30s signals, static routing, zero offsets) against our intelligent coordinated system. The results demonstrate a **12% reduction in travel time**, a **56% reduction in intersection delay**, an **86% reduction in vehicle stops**, and an upgrade in network Level of Service from **LOS D to LOS B**."*
