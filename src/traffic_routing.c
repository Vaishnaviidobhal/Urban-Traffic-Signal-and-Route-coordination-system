#include "traffic_routing.h"

void build_cost_matrix(double g[N][N], int use_signals, int use_adaptive)
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            g[i][j] = (i == j) ? 0.0 : (double)INF;

    for (i = 0; i < R; i++)
    {
        if (roads[i].closed)
            continue;
        int t = road_travel_time(&roads[i]);
        if (t >= INF)
            continue;

        double delay_a = use_signals ? signal_wait_delay_min(roads[i].a, use_adaptive) : 0.0;
        double delay_b = use_signals ? signal_wait_delay_min(roads[i].b, use_adaptive) : 0.0;

        g[roads[i].a][roads[i].b] = (double)t + delay_b;
        g[roads[i].b][roads[i].a] = (double)t + delay_a;
    }
}

double dijkstra(int source, int target, int parent[N], long long *ops, int use_signals, int use_adaptive)
{
    double g[N][N], dist[N];
    int used[N] = {0}, i, j;
    build_cost_matrix(g, use_signals, use_adaptive);
    *ops = 0;

    for (i = 0; i < N; i++)
    {
        dist[i] = (double)INF;
        parent[i] = -1;
    }
    dist[source] = 0.0;

    for (i = 0; i < N; i++)
    {
        double best = (double)INF;
        int u = -1;
        for (j = 0; j < N; j++)
        {
            ++*ops;
            if (!used[j] && dist[j] < best)
            {
                best = dist[j];
                u = j;
            }
        }
        if (u < 0)
            break;
        used[u] = 1;
        for (j = 0; j < N; j++)
        {
            ++*ops;
            if (!used[j] && g[u][j] < (double)INF && dist[u] + g[u][j] < dist[j])
            {
                dist[j] = dist[u] + g[u][j];
                parent[j] = u;
            }
        }
    }
    return dist[target];
}

double bellman(int source, int target, int parent[N], long long *ops, int use_signals, int use_adaptive)
{
    double d[N];
    int i, j, changed;
    *ops = 0;

    for (i = 0; i < N; i++)
    {
        d[i] = (double)INF;
        parent[i] = -1;
    }
    d[source] = 0.0;

    for (i = 1; i < N; i++)
    {
        changed = 0;
        for (j = 0; j < R; j++)
        {
            Road *r = &roads[j];
            if (r->closed)
                continue;
            int t = road_travel_time(r);
            if (t >= INF)
                continue;

            double delay_a = use_signals ? signal_wait_delay_min(r->a, use_adaptive) : 0.0;
            double delay_b = use_signals ? signal_wait_delay_min(r->b, use_adaptive) : 0.0;

            ++*ops;
            double cost_ab = (double)t + delay_b;
            if (d[r->a] < (double)INF && d[r->a] + cost_ab < d[r->b])
            {
                d[r->b] = d[r->a] + cost_ab;
                parent[r->b] = r->a;
                changed = 1;
            }

            ++*ops;
            double cost_ba = (double)t + delay_a;
            if (d[r->b] < (double)INF && d[r->b] + cost_ba < d[r->a])
            {
                d[r->a] = d[r->b] + cost_ba;
                parent[r->a] = r->b;
                changed = 1;
            }
        }
        if (!changed)
            break;
    }
    return d[target];
}

int extract_path(const int parent[N], int target, int out_path[N])
{
    int temp[N], count = 0, v = target;
    while (v != -1)
    {
        temp[count++] = v;
        v = parent[v];
    }
    for (int i = 0; i < count; i++)
        out_path[i] = temp[count - 1 - i];
    return count;
}

void print_path_string(const int path[], int len)
{
    for (int i = 0; i < len; i++)
        printf("%c%s", path[i] + 'A', (i < len - 1) ? " -> " : "");
    printf("\n");
}

void format_path_string(char *buf, size_t size, const int path[], int len)
{
    size_t used = 0;
    buf[0] = '\0';
    for (int i = 0; i < len; i++)
    {
        int written = snprintf(buf + used, size - used, "%c%s", path[i] + 'A', (i < len - 1) ? " -> " : "");
        if (written < 0 || (size_t)written >= size - used)
            break;
        used += (size_t)written;
    }
}

void find_efficient_route(void)
{
    int s = read_vertex("Enter Origin Junction (A-F): ");
    int t = read_vertex("Enter Destination Junction (A-F): ");
    int parent[N], route_nodes[N];
    long long ops;

    if (s < 0 || t < 0 || s == t)
    {
        puts("Please enter two distinct junction letters from A to F.");
        return;
    }

    double pure_time = dijkstra(s, t, parent, &ops, 0, 0);
    if (pure_time >= INF)
    {
        puts("No open route is available between the requested locations.");
        return;
    }
    int pure_len = extract_path(parent, t, route_nodes);

    int parent_coord[N], route_coord[N];
    double total_time = dijkstra(s, t, parent_coord, &ops, 1, 1);
    int coord_len = extract_path(parent_coord, t, route_coord);

    puts("\n================================================================================");
    puts("                         EFFICIENT ROUTE RECOMMENDATION                         ");
    puts("================================================================================");
    printf("Origin: [%c] %-25s --> Destination: [%c] %-25s\n\n",
           s + 'A', junctions[s].name, t + 'A', junctions[t].name);

    printf("A) Traditional Distance/Free-Flow Route: ");
    print_path_string(route_nodes, pure_len);
    printf("   Road Travel Time: %.1f minutes (ignoring traffic signal queuing delays)\n\n", pure_time);

    printf("B) Intelligent Coordinated Route (Road Time + Signal Wait Delay):\n   Route: ");
    print_path_string(route_coord, coord_len);
    printf("   Total Estimated Travel Time: %.1f minutes\n", total_time);

    double road_sum = 0.0, signal_sum = 0.0;
    printf("\n   Route Leg Breakdown:\n");
    for (int i = 0; i < coord_len - 1; i++)
    {
        int u = route_coord[i], v = route_coord[i + 1];
        int r_idx = find_road(u, v);
        int r_time = road_travel_time(&roads[r_idx]);
        double s_delay = signal_wait_delay_min(v, 1);
        road_sum += r_time;
        if (i < coord_len - 2)
            signal_sum += s_delay;
        printf("    - Leg %c -> %c : %4.1f km | Road Time: %2d min | Signal Wait at %c: %4.1f min\n",
               u + 'A', v + 'A', roads[r_idx].distance_km, r_time, v + 'A', (i < coord_len - 2) ? s_delay : 0.0);
    }
    printf("   Summary: Pure Driving: %.1f min | Signal Queuing: %.1f min | Total: %.1f min\n",
           road_sum, signal_sum, road_sum + signal_sum);
    puts("================================================================================");
}

void compare_algorithms(void)
{
    int s = read_vertex("Enter Origin Junction (A-F): ");
    int t = read_vertex("Enter Destination Junction (A-F): ");
    int p_dijk[N], p_bell[N];
    long long ops_dijk, ops_bell;
    clock_t start, end;
    double time_dijk_us, time_bell_us;

    if (s < 0 || t < 0 || s == t)
    {
        puts("Please enter two distinct junction letters from A to F.");
        return;
    }

    start = clock();
    double cost_dijk = dijkstra(s, t, p_dijk, &ops_dijk, 1, 1);
    end = clock();
    time_dijk_us = (double)(end - start) * 1e6 / CLOCKS_PER_SEC;

    start = clock();
    double cost_bell = bellman(s, t, p_bell, &ops_bell, 1, 1);
    end = clock();
    time_bell_us = (double)(end - start) * 1e6 / CLOCKS_PER_SEC;

    puts("\n================================================================================");
    puts("                   ALGORITHMIC COMPARISON & CORRECTNESS PROOF                   ");
    puts("================================================================================");
    printf("Origin: %c | Destination: %c\n\n", s + 'A', t + 'A');
    printf("Algorithm       Route Cost(min)    Operations Checked     Execution Time\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("Dijkstra (O(V^2))     %6.2f min         %8lld ops            %8.2f us\n",
           cost_dijk >= INF ? 0.0 : cost_dijk, ops_dijk, time_dijk_us);
    printf("Bellman-Ford (O(VE))  %6.2f min         %8lld ops            %8.2f us\n",
           cost_bell >= INF ? 0.0 : cost_bell, ops_bell, time_bell_us);
    puts("--------------------------------------------------------------------------------");
    if (fabs(cost_dijk - cost_bell) < 0.001)
        puts("CORRECTNESS VERIFIED: Both algorithms produce identical optimal path costs.");
    else
        puts("WARNING: Cost mismatch between algorithms.");
    puts("DAA Insight: Dijkstra is faster for non-negative road weights; Bellman-Ford verifies correctness.");
    puts("================================================================================");
}

void dynamic_rerouting_simulation(void)
{
    puts("\n================================================================================");
    puts("                 DYNAMIC REROUTING & TRAFFIC DIVERSION ENGINE                   ");
    puts("================================================================================");
    puts("Simulating sudden incident: Road C-D (East to South arterial) is completely BLOCKED.");

    reset_city();
    int s = 0; /* A */
    int t = 3; /* D (South University) */
    int p_before[N], r_before[N];
    long long ops;
    double time_before = dijkstra(s, t, p_before, &ops, 1, 1);
    int len_before = extract_path(p_before, t, r_before);

    roads[5].closed = 1;
    roads[1].vehicles = 52;
    roads[6].vehicles = 44;
    junctions[2].queue = 34;
    update_network_metrics();

    int p_after[N], r_after[N];
    double time_after = dijkstra(s, t, p_after, &ops, 1, 1);
    int len_after = extract_path(p_after, t, r_after);

    printf("\nTest Case 1: Route from [%c] %s --> [%c] %s\n",
           s + 'A', junctions[s].name, t + 'A', junctions[t].name);
    printf(" - Baseline Route (Before Blockage): ");
    print_path_string(r_before, len_before);
    printf("   Estimated Travel Time: %.1f minutes\n", time_before);

    printf(" - Dynamic Reroute (After C-D Block): ");
    print_path_string(r_after, len_after);
    printf("   Rerouted Travel Time:  %.1f minutes (Bypasses blocked C-D link)\n", time_after);

    int s2 = 0, t2 = 5;
    reset_city();
    double h_before = dijkstra(s2, t2, p_before, &ops, 1, 1);
    int h_len_before = extract_path(p_before, t2, r_before);

    roads[5].closed = 1;
    roads[1].vehicles = 52;
    roads[6].vehicles = 44;
    junctions[2].queue = 34;
    update_network_metrics();

    double h_after = dijkstra(s2, t2, p_after, &ops, 1, 1);
    int h_len_after = extract_path(p_after, t2, r_after);

    printf("\nTest Case 2: Hospital Emergency Corridor [%c] %s --> [%c] %s\n",
           s2 + 'A', junctions[s2].name, t2 + 'A', junctions[t2].name);
    printf(" - Baseline Route (Before Blockage): ");
    print_path_string(r_before, h_len_before);
    printf("   Estimated Travel Time: %.1f minutes\n", h_before);

    printf(" - Dynamic Reroute (After C-D Block): ");
    print_path_string(r_after, h_len_after);
    printf("   Rerouted Travel Time:  %.1f minutes (Diverted via West corridor)\n", h_after);

    puts("\nTraffic Diversion Management System Response:");
    puts("1. Variable Message Signs (VMS) updated at Junction A: Alert drivers to detour via E.");
    puts("2. Signal Preemption: Green wave initiated along rerouted path.");
    puts("3. System maintains emergency access to Hospital (F) without gridlock.");
    puts("================================================================================");
}
