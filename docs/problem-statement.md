# Problem Statement

Urban traffic congestion represents a significant economic, environmental, and public safety challenge. Traditional traffic management systems suffer from two critical architectural shortcomings:
1. **Isolated Traffic Signals:** Traffic lights operate on static, uncoordinated fixed-time cycles regardless of real-time vehicular queues, leading to unnecessary delays and frequent stop-and-go idling.
2. **Uncoordinated Route Planning:** Commuters and navigation tools calculate routes based purely on free-flow road distance or static travel times, ignoring queue accumulations at downstream intersections.

This project implements an intelligent, autonomous **Urban Traffic Signal and Route Coordination System** in C11. It models a six-junction metropolitan network with ten bidirectional arterial corridors to achieve:
- Continuous real-time intersection monitoring and Level of Service (LOS A–F) grading.
- Dynamic queue-responsive green-phase allocation.
- Integrated route optimization balancing road congestion and junction signal wait times.
- Automated incident and blockage detection with dynamic rerouting.
- Corridor green-wave signal offset coordination to eliminate unnecessary vehicle stops.
- Empirical, quantified impact benchmarking measuring system-wide improvements over traditional baselines.
