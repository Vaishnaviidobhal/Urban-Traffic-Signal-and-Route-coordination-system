# System Requirements

## Functional Requirements

1. **Network Topology Representation:** Store and model six urban junctions (A–F) and ten bidirectional arterial road segments with defined distances, speed limits, capacities, and base vehicular loads.
2. **Junction Monitoring & Level of Service:** Track queue lengths in real time, calculate average vehicle delay, assign Highway Capacity Manual (HCM) Level of Service (LOS A through F), and alert on critical bottlenecks.
3. **Adaptive Signal Control:** Compute dynamic green light splits bounded between 15s and 65s within a 90-second cycle length based on intersection queuing pressure.
4. **Congestion-Aware Routing:** Implement Dijkstra's algorithm to compute the fastest route between any origin and destination, evaluating composite weights of road travel time and intersection signal delays.
5. **Algorithmic Verification:** Execute the Bellman-Ford algorithm as an independent comparative baseline to verify path optimality, operation counts, and CPU timing.
6. **Incident & Blockage Detection:** Continuously monitor Volume-to-Capacity ($V/C$) ratios and speed degradation to detect heavy congestion ($V/C \ge 1.0$) and road closures.
7. **Dynamic Incident Rerouting:** Automatically calculate alternative diversion routes when a road closure or severe bottleneck occurs, bypassing blocked links.
8. **Corridor Signal Coordination (Green Wave):** Calculate progression travel times and synchronize signal offsets along arterial corridors to eliminate red-light stops for progressing platoons.
9. **Quantified Impact Evaluation:** Provide an A/B benchmark contrasting the intelligent coordinated system against a traditional fixed-time baseline across key metrics (travel time, delay, stops, throughput, emissions).
10. **Traffic Load Sorting & Search:** Sort road loads using a student-written Hoare Quick Sort and query loads using an iterative Binary Search.
11. **Report Generation:** Export comprehensive network states, signal plans, and road metrics to `traffic_report.csv`.

## Non-Functional Requirements & Constraints

- Portable C11 compliance with GCC compiling cleanly under `-Wall -Wextra -pedantic`.
- Zero external library dependencies (uses standard C libraries and math library).
- Cross-platform support for both Linux CLI and optional Windows GUI front-end.
