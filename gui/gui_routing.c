#include "gui_common.h"
#include "gui_routing.h"

void gui_show_route(void)
{
    int s = gui_get_selected(source_box), t = gui_get_selected(destination_box);
    int p[N], path_nodes[N];
    long long ops;
    char path_buf[128], text[2000];

    if (s < 0 || s >= N || t < 0 || t >= N || s == t)
    {
        gui_show_text("Please enter two distinct letters from A to F in the Origin and Destination boxes.");
        return;
    }

    double total_cost = dijkstra(s, t, p, &ops, 1, 1);
    if (total_cost >= INF)
    {
        gui_show_text("No open route is available between the selected junctions.");
        return;
    }

    int len = extract_path(p, t, path_nodes);
    format_path_string(path_buf, sizeof(path_buf), path_nodes, len);

    snprintf(text, sizeof(text),
             "========================================================================\r\n"
             "                 INTELLIGENT ROUTE RECOMMENDATION                       \r\n"
             "========================================================================\r\n"
             "Origin: [%c] %s\r\nDestination: [%c] %s\r\n\r\n"
             "OPTIMAL RECOMMENDED ROUTE:\r\n  %s\r\n\r\n"
             "Total Estimated Travel Time: %.1f minutes\r\n"
             "Dijkstra Algorithm Operations: %lld ops\r\n\r\n"
             "Route Leg Breakdown (Road Travel Time + Intersection Queuing Delay):\r\n",
             s + 'A', junctions[s].name, t + 'A', junctions[t].name,
             path_buf, total_cost, ops);

    char leg_buf[200];
    for (int i = 0; i < len - 1; i++)
    {
        int u = path_nodes[i], v = path_nodes[i + 1];
        int r_idx = find_road(u, v);
        int r_time = road_travel_time(&roads[r_idx]);
        double s_delay = (i < len - 2) ? signal_wait_delay_min(v, 1) : 0.0;
        snprintf(leg_buf, sizeof(leg_buf),
                 "  * Leg %c -> %c : %.1f km | Driving: %d min | Signal Wait at %c: %.1f min\r\n",
                 u + 'A', v + 'A', roads[r_idx].distance_km, r_time, v + 'A', s_delay);
        strcat(text, leg_buf);
    }
    strcat(text, "\r\nNote: Evaluates both road congestion and junction traffic light delays.\r\n");
    gui_show_text(text);
}

void gui_show_compare(void)
{
    int s = gui_get_selected(source_box), t = gui_get_selected(destination_box);
    int p_dijk[N], p_bell[N];
    long long ops_dijk, ops_bell;
    char text[1500];

    if (s < 0 || s >= N || t < 0 || t >= N || s == t)
    {
        gui_show_text("Please enter two distinct letters from A to F in the Origin and Destination boxes.");
        return;
    }

    clock_t start = clock();
    double cost_dijk = dijkstra(s, t, p_dijk, &ops_dijk, 1, 1);
    clock_t end = clock();
    double time_dijk = (double)(end - start) * 1e6 / CLOCKS_PER_SEC;

    start = clock();
    double cost_bell = bellman(s, t, p_bell, &ops_bell, 1, 1);
    end = clock();
    double time_bell = (double)(end - start) * 1e6 / CLOCKS_PER_SEC;

    snprintf(text, sizeof(text),
             "========================================================================\r\n"
             "             ALGORITHMIC COMPARISON & CORRECTNESS PROOF                 \r\n"
             "========================================================================\r\n"
             "Origin: %c | Destination: %c\r\n\r\n"
             "Algorithm          Route Cost       Operations      CPU Time\r\n"
             "------------------------------------------------------------------------\r\n"
             "Dijkstra (O(V^2))     %5.1f min        %6lld ops       %8.2f us\r\n"
             "Bellman-Ford (O(VE))  %5.1f min        %6lld ops       %8.2f us\r\n"
             "------------------------------------------------------------------------\r\n"
             "%s\r\n\r\n"
             "DAA Complexity Analysis:\r\n"
             "- Dijkstra is optimal for non-negative road travel times.\r\n"
             "- Bellman-Ford verifies correctness and guards against negative cycles.\r\n",
             s + 'A', t + 'A',
             cost_dijk >= INF ? 0.0 : cost_dijk, ops_dijk, time_dijk,
             cost_bell >= INF ? 0.0 : cost_bell, ops_bell, time_bell,
             fabs(cost_dijk - cost_bell) < 0.001 ? "VERIFICATION: Both algorithms produce identical optimal route costs." : "WARNING: Cost discrepancy detected.");
    gui_show_text(text);
}
