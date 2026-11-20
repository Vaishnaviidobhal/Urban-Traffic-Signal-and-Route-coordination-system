#include "gui_common.h"
#include "gui_reroute.h"

void gui_show_detect(void)
{
    update_network_metrics();
    char text[2500] =
        "========================================================================\r\n"
        "                  CONGESTION & INCIDENT DETECTION SCAN                  \r\n"
        "========================================================================\r\n";
    char row[250];
    int incidents = 0, jams = 0;
    for (int i = 0; i < R; i++)
    {
        Road *r = &roads[i];
        if (r->closed)
        {
            snprintf(row, sizeof(row), "[BLOCKAGE DETECTED] Road %c-%c is CLOSED! Emergency diversion required.\r\n",
                     r->a + 'A', r->b + 'A');
            strcat(text, row);
            incidents++;
        }
        else if (r->vc_ratio >= 1.00)
        {
            snprintf(row, sizeof(row), "[HEAVY CONGESTION] Road %c-%c: %d/%d veh (V/C = %.2f) - Speed dropped!\r\n",
                     r->a + 'A', r->b + 'A', r->vehicles, r->capacity, r->vc_ratio);
            strcat(text, row);
            jams++;
        }
    }
    for (int i = 0; i < N; i++)
    {
        if (junctions[i].los >= 'E')
        {
            snprintf(row, sizeof(row), "[JUNCTION OVERLOAD] Junction %c (%s) in gridlock! Queue: %d veh (LOS %c)\r\n",
                     junctions[i].id, junctions[i].name, junctions[i].queue, junctions[i].los);
            strcat(text, row);
            jams++;
        }
    }
    if (incidents == 0 && jams == 0)
        strcat(text, "System Scan: All corridors and intersections are operating smoothly.\r\n");
    else
    {
        snprintf(row, sizeof(row), "\r\nActive Incidents: %d | Severe Congestion Bottlenecks: %d\r\n", incidents, jams);
        strcat(text, row);
    }
    gui_show_text(text);
}

void gui_show_reroute(void)
{
    char text[3000] =
        "========================================================================\r\n"
        "                DYNAMIC REROUTING & TRAFFIC DIVERSION ENGINE            \r\n"
        "========================================================================\r\n"
        "Simulating sudden blockage on Road C-D (East to South arterial)...\r\n\r\n";

    reset_city();
    int s = 0, t = 3, p_b[N], r_b[N], p_a[N], r_a[N];
    long long ops;
    double t_before = dijkstra(s, t, p_b, &ops, 1, 1);
    int len_b = extract_path(p_b, t, r_b);

    roads[5].closed = 1;
    roads[1].vehicles = 52;
    roads[6].vehicles = 44;
    junctions[2].queue = 34;
    update_network_metrics();

    double t_after = dijkstra(s, t, p_a, &ops, 1, 1);
    int len_a = extract_path(p_a, t, r_a);

    char buf1[80], buf2[80], row[500];
    format_path_string(buf1, sizeof(buf1), r_b, len_b);
    format_path_string(buf2, sizeof(buf2), r_a, len_a);

    snprintf(row, sizeof(row),
             "Test 1: Origin [%c] %s --> Destination [%c] %s\r\n"
             " - Route Before Blockage:  %s  (%.1f min)\r\n"
             " - Dynamic Reroute:        %s  (%.1f min - Bypasses closed C-D)\r\n\r\n",
             s + 'A', junctions[s].name, t + 'A', junctions[t].name,
             buf1, t_before, buf2, t_after);
    strcat(text, row);

    reset_city();
    int s2 = 0, t2 = 5;
    double h_before = dijkstra(s2, t2, p_b, &ops, 1, 1);
    int hlen_b = extract_path(p_b, t2, r_b);

    roads[5].closed = 1;
    roads[1].vehicles = 52;
    roads[6].vehicles = 44;
    junctions[2].queue = 34;
    update_network_metrics();

    double h_after = dijkstra(s2, t2, p_a, &ops, 1, 1);
    int hlen_a = extract_path(p_a, t2, r_a);

    format_path_string(buf1, sizeof(buf1), r_b, hlen_b);
    format_path_string(buf2, sizeof(buf2), r_a, hlen_a);

    snprintf(row, sizeof(row),
             "Test 2: Emergency Hospital Corridor [%c] %s --> [%c] %s\r\n"
             " - Route Before Blockage:  %s  (%.1f min)\r\n"
             " - Dynamic Reroute:        %s  (%.1f min - Diverted via West)\r\n\r\n",
             s2 + 'A', junctions[s2].name, t2 + 'A', junctions[t2].name,
             buf1, h_before, buf2, h_after);
    strcat(text, row);

    strcat(text, "Traffic Control Response:\r\n"
                 "1. VMS roadside signage dynamically detours approaching traffic.\r\n"
                 "2. Signal preemption allocates green waves along rerouted paths.\r\n"
                 "3. Eliminates gridlock and protects emergency access.\r\n");
    gui_show_text(text);
}
