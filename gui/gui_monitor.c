#include "gui_common.h"
#include "gui_monitor.h"

void gui_show_network(void)
{
    update_network_metrics();
    char text[3000] =
        "========================================================================\r\n"
        "                CITY ROAD NETWORK TOPOLOGY & TRAFFIC STATUS             \r\n"
        "========================================================================\r\n"
        "Road   Dist     Speed    Free-Flow  Load/Cap     V/C    Travel-Time  Status\r\n"
        "------------------------------------------------------------------------\r\n";
    char row[200];
    for (int i = 0; i < R; i++)
    {
        Road *r = &roads[i];
        if (r->closed)
        {
            snprintf(row, sizeof(row), "%c-%c    %4.1f km  %2d km/h   %2d min    %2d/%-2d veh  %4.2f    -- min    [BLOCKED]\r\n",
                     r->a + 'A', r->b + 'A', r->distance_km, r->speed_limit, r->free_time,
                     r->vehicles, r->capacity, r->vc_ratio);
        }
        else
        {
            snprintf(row, sizeof(row), "%c-%c    %4.1f km  %2d km/h   %2d min    %2d/%-2d veh  %4.2f    %2d min    %-8s\r\n",
                     r->a + 'A', r->b + 'A', r->distance_km, r->speed_limit, r->free_time,
                     r->vehicles, r->capacity, r->vc_ratio, road_travel_time(r), r->status);
        }
        strcat(text, row);
    }
    strcat(text, "------------------------------------------------------------------------\r\n");
    strcat(text, "Junctions:\r\n");
    for (int i = 0; i < N; i++)
    {
        snprintf(row, sizeof(row), "  [%c] %-28s | Queue: %2d veh | LOS: %c (%.1fs delay)\r\n",
                 junctions[i].id, junctions[i].name, junctions[i].queue,
                 junctions[i].los, junctions[i].delay_sec);
        strcat(text, row);
    }
    gui_show_text(text);
}

void gui_show_monitor(void)
{
    update_network_metrics();
    char text[2500] =
        "========================================================================\r\n"
        "               REAL-TIME JUNCTION TRAFFIC MONITOR & LOS                 \r\n"
        "========================================================================\r\n"
        "Junction Name                         Queue    Avg Delay   LOS   Condition\r\n"
        "------------------------------------------------------------------------\r\n";
    char row[200];
    for (int i = 0; i < N; i++)
    {
        const char *cond = (junctions[i].los <= 'B') ? "Smooth Flow" :
                           (junctions[i].los == 'C') ? "Moderate Volume" :
                           (junctions[i].los == 'D') ? "Approaching Capacity" :
                           (junctions[i].los == 'E') ? "! HEAVY CONGESTION !" : "!! GRIDLOCK !!";
        snprintf(row, sizeof(row), "  [%c]    %-28s  %2d veh    %5.1f s      %c    %s\r\n",
                 junctions[i].id, junctions[i].name, junctions[i].queue,
                 junctions[i].delay_sec, junctions[i].los, cond);
        strcat(text, row);
    }
    strcat(text, "------------------------------------------------------------------------\r\n"
                 "HCM LOS Scale: A (<=10s) | B (10-20s) | C (20-35s) | D (35-55s) | E (55-80s) | F (>80s)\r\n");
    gui_show_text(text);
}

void gui_show_signals(void)
{
    update_network_metrics();
    char text[2500] =
        "========================================================================\r\n"
        "              INTELLIGENT ADAPTIVE TRAFFIC SIGNAL CONTROL               \r\n"
        "========================================================================\r\n"
        "Cycle Length: 90 seconds | Yellow: 3s | All-Red: 2s\r\n"
        "Junction Queue Fixed-Green Intelligent-Green  Delay(Fixed) Delay(Adapt) Action\r\n"
        "------------------------------------------------------------------------\r\n";
    char row[200];
    for (int i = 0; i < N; i++)
    {
        int q = junctions[i].queue;
        int fg = BASE_GREEN;
        int ag = junctions[i].adaptive_green;
        double d_fixed = ((double)(CYCLE_TIME - fg) * (CYCLE_TIME - fg)) / (2.0 * CYCLE_TIME) + (q * 2.2);
        double d_adapt = ((double)(CYCLE_TIME - ag) * (CYCLE_TIME - ag)) / (2.0 * CYCLE_TIME) + (q * 1.5);
        const char *act = (ag > fg) ? "EXTEND GREEN (+)" : (ag < fg) ? "REDUCE GREEN (-)" : "MAINTAIN (=)";
        snprintf(row, sizeof(row), "  [%c]    %2d veh   %2d sec         %2d sec        %4.1f s      %4.1f s    %-10s\r\n",
                 junctions[i].id, q, fg, ag, d_fixed, d_adapt, act);
        strcat(text, row);
    }
    strcat(text, "------------------------------------------------------------------------\r\n"
                 "Adaptive Rule: Green = clamp(15s, 20s + (Queue x 2.0s), 65s)\r\n"
                 "Benefit: Queue-responsive green allocation reduces intersection waiting times.\r\n");
    gui_show_text(text);
}
