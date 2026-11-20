# Asymptotic Complexity Analysis

| Module / Operation | Time Complexity | Auxiliary Space | Explanation |
| :--- | :---: | :---: | :--- |
| **Dijkstra's Algorithm** | $O(V^2)$ | $O(V)$ | Iterates $V$ times selecting minimum tentative distance among $V$ nodes using an adjacency matrix. |
| **Bellman-Ford Algorithm** | $O(V \cdot E)$ | $O(V)$ | Performs up to $V - 1$ relaxation passes over all $E$ bidirectional edges. |
| **Green Wave Offset Calculation** | $O(K)$ | $O(K)$ | Sequentially computes arrival progression times across $K \le V$ corridor junctions. |
| **Adaptive Signal Optimization** | $O(V)$ | $O(1)$ | Scans all $V$ junctions to calculate dynamic green splits and delay estimates. |
| **Congestion & Incident Scan** | $O(R + V)$ | $O(1)$ | Evaluates Volume-to-Capacity ratios for $R$ roads and delay thresholds for $V$ junctions. |
| **Dynamic Rerouting** | $O(V^2)$ | $O(V)$ | Re-runs Dijkstra after updating the blocked link and spillover vehicle distribution. |
| **Quick Sort (Hoare)** | Avg $O(R \log R)$, Worst $O(R^2)$ | $O(\log R)$ | Recursively partitions $R$ road records based on active vehicular load. |
| **Iterative Binary Search** | $O(\log R)$ | $O(1)$ | Logarithmic division of sorted road array with constant memory overhead. |
| **Impact Evaluation Benchmark** | $O(P \cdot V^2)$ | $O(V)$ | Runs routing and metric calculations across $P = 5$ representative commuter OD pairs. |

### Scalability Considerations

- For our 6-junction city model ($V = 6, R = 10$), the $O(V^2)$ adjacency matrix representation operates instantaneously (< 5 microseconds), ensuring deterministic real-time performance.
- In a full-scale metropolitan simulation with thousands of junctions ($V > 10^4$), Dijkstra can be adapted to an adjacency list with a Fibonacci or binary min-heap ($O((V + E) \log V)$).
