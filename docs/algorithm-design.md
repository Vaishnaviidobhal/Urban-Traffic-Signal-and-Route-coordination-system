# Algorithm Design

## 1. Integrated Routing Graph Model

The road network is modeled as a weighted graph $G = (V, E)$, where $V = \{A, B, C, D, E, F\}$ and $E$ represents the 10 bidirectional arterial segments.

### Edge Cost Function
Unlike traditional systems that evaluate only road distance, our edge weight combines:
$$\text{Weight}(u \to v) = \text{Travel Time}_{\text{road}}(u, v) + \text{Delay}_{\text{signal}}(v)$$

- **Road Travel Time:** Free-flow time adjusted by congestion delay when active volume exceeds capacity ($+1$ min per 10 excess vehicles).
- **Signal Wait Delay:** Calculated at target junction $v$ based on red phase duration and queued vehicles:
  $$\text{Delay}_{\text{signal}}(v) = \frac{(C - g)^2}{2C} + (\text{Queue} \times 2.2\text{s})$$

## 2. Shortest-Path Algorithms

- **Dijkstra's Algorithm ($O(V^2)$):** Employs an adjacency matrix and tentative distance array to iteratively find the global minimum travel time path for non-negative weights.
- **Bellman-Ford Algorithm ($O(V \cdot E)$):** Iterates over all edges for $V - 1$ passes, verifying the optimality of Dijkstra's solution and ensuring stability against anomalous weight drops.

## 3. Adaptive Signal Control

The signal controller dynamically allocates green time $g \in [15, 65]$ seconds within a 90-second cycle length:
$$g = \min(65, \max(15, 20 + 2 \times \text{queue}))$$
This dynamic reallocation reduces vehicle delay by clearing queues rapidly before overflow occurs.

## 4. Corridor Green Wave Coordination

For an arterial corridor $J_1 \to J_2 \to \dots \to J_k$ with progression speed $v = 40$ km/h:
$$\Delta t_{i, i+1} = \frac{d(J_i, J_{i+1})}{v} \times 3600 \text{ s}$$
$$\text{Offset}_1 = 0, \quad \text{Offset}_{i+1} = (\text{Offset}_i + \Delta t_{i, i+1}) \pmod C$$
By shifting the cycle start time at downstream intersections, vehicles traveling at the progression speed arrive during the green phase, cutting stops to zero.

## 5. Sorting and Search

- **Quick Sort:** Uses Hoare's partitioning scheme to sort the array of road structs in descending order of vehicle count in $O(R \log R)$ average time.
- **Iterative Binary Search:** Halves the search space in each iteration to locate target vehicle volumes in $O(\log R)$ time and $O(1)$ auxiliary space.
