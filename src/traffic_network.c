#include "traffic_network.h"
#include "traffic_signal.h"

Junction junctions[N] = {
    {'A', "Central Commercial Hub", 18, BASE_GREEN, BASE_GREEN, 0, 0.0, 'B'},
    {'B', "North Residential Zone", 10, BASE_GREEN, BASE_GREEN, 0, 0.0, 'A'},
    {'C', "East Industrial & Tech Corridor", 22, BASE_GREEN, BASE_GREEN, 0, 0.0, 'C'},
    {'D', "South University District", 12, BASE_GREEN, BASE_GREEN, 0, 0.0, 'A'},
    {'E', "West Logistics & Freight Park", 16, BASE_GREEN, BASE_GREEN, 0, 0.0, 'B'},
    {'F', "Metropolitan Hospital Center", 8, BASE_GREEN, BASE_GREEN, 0, 0.0, 'A'}};

Road roads[R] = {
    {0, 0, 1, 3.0, 45, 4, 40, 20, 0, 0.50, "NORMAL"},  /* A-B */
    {1, 0, 2, 2.5, 50, 3, 35, 26, 0, 0.74, "NORMAL"},  /* A-C */
    {2, 0, 4, 5.0, 50, 6, 30, 12, 0, 0.40, "NORMAL"},  /* A-E */
    {3, 1, 2, 1.8, 50, 2, 25, 12, 0, 0.48, "NORMAL"},  /* B-C */
    {4, 1, 3, 6.0, 50, 7, 30, 22, 0, 0.73, "NORMAL"},  /* B-D */
    {5, 2, 3, 3.5, 50, 4, 40, 30, 0, 0.75, "NORMAL"},  /* C-D */
    {6, 2, 4, 4.2, 50, 5, 25, 18, 0, 0.72, "NORMAL"},  /* C-E */
    {7, 3, 4, 2.8, 50, 3, 30, 14, 0, 0.47, "NORMAL"},  /* D-E */
    {8, 3, 5, 3.6, 50, 4, 35, 16, 0, 0.46, "NORMAL"},  /* D-F */
    {9, 4, 5, 5.2, 50, 6, 25, 10, 0, 0.40, "NORMAL"}   /* E-F */
};

int road_travel_time(const Road *r)
{
    if (r->closed)
        return INF;
    int congestion_delay = 0;
    if (r->vehicles > r->capacity)
    {
        congestion_delay = (r->vehicles - r->capacity + 9) / 10;
    }
    return r->free_time + congestion_delay;
}

int find_road(int u, int v)
{
    for (int i = 0; i < R; i++)
    {
        if ((roads[i].a == u && roads[i].b == v) || (roads[i].a == v && roads[i].b == u))
            return i;
    }
    return -1;
}

void update_network_metrics(void)
{
    int i;
    for (i = 0; i < R; i++)
    {
        roads[i].vc_ratio = (double)roads[i].vehicles / (double)roads[i].capacity;
        if (roads[i].closed)
            roads[i].status = "CLOSED";
        else if (roads[i].vc_ratio >= 1.30)
            roads[i].status = "CRITICAL";
        else if (roads[i].vc_ratio >= 1.00)
            roads[i].status = "HEAVY";
        else if (roads[i].vc_ratio >= 0.75)
            roads[i].status = "MODERATE";
        else
            roads[i].status = "NORMAL";
    }

    for (i = 0; i < N; i++)
    {
        int rec_green = 20 + junctions[i].queue * 2;
        if (rec_green > 65)
            rec_green = 65;
        if (rec_green < 15)
            rec_green = 15;
        junctions[i].adaptive_green = rec_green;

        int red = CYCLE_TIME - junctions[i].current_green;
        junctions[i].delay_sec = ((double)red * red) / (2.0 * CYCLE_TIME) + (junctions[i].queue * 2.2);
        junctions[i].los = calculate_los(junctions[i].delay_sec);
    }
}

void reset_city(void)
{
    int loads[R] = {20, 26, 12, 12, 22, 30, 18, 14, 16, 10};
    int queues[N] = {18, 10, 22, 12, 16, 8};
    int i;
    for (i = 0; i < R; i++)
    {
        roads[i].vehicles = loads[i];
        roads[i].closed = 0;
    }
    for (i = 0; i < N; i++)
    {
        junctions[i].queue = queues[i];
        junctions[i].current_green = BASE_GREEN;
        junctions[i].offset_sec = 0;
    }
    update_network_metrics();
}

void display_network(void)
{
    int i;
    update_network_metrics();
    puts("\n================================================================================");
    puts("                   URBAN ROAD NETWORK TOPOLOGY & TRAFFIC STATUS                  ");
    puts("================================================================================");
    printf("Road   Dist(km) Speed  Free-Flow  Vehicles/Cap   V/C   Travel-Time  Status\n");
    printf("--------------------------------------------------------------------------------\n");
    for (i = 0; i < R; i++)
    {
        Road *r = &roads[i];
        printf("%c-%c    %4.1f km  %2d km/h  %2d min     %2d/%-2d veh    %4.2f   ",
               r->a + 'A', r->b + 'A', r->distance_km, r->speed_limit, r->free_time,
               r->vehicles, r->capacity, r->vc_ratio);
        if (r->closed)
            printf("  -- min     [BLOCKED / CLOSED]\n");
        else
            printf("  %2d min     %-8s\n", road_travel_time(r), r->status);
    }
    puts("--------------------------------------------------------------------------------");
    puts("Network Junctions:");
    for (i = 0; i < N; i++)
        printf("  [%c] %-30s | Queue: %2d veh | LOS: %c (%4.1fs delay)\n",
               junctions[i].id, junctions[i].name, junctions[i].queue,
               junctions[i].los, junctions[i].delay_sec);
    puts("================================================================================");
}

void export_traffic_report(void)
{
    FILE *f = fopen("traffic_report.csv", "w");
    if (!f)
    {
        puts("Error: Could not open traffic_report.csv for writing.");
        return;
    }
    update_network_metrics();

    fprintf(f, "# ROAD NETWORK STATUS\n");
    fprintf(f, "road_id,origin,destination,distance_km,speed_limit,free_flow_min,vehicles,capacity,vc_ratio,travel_time_min,status\n");
    for (int i = 0; i < R; i++)
    {
        Road *r = &roads[i];
        fprintf(f, "%d,%c,%c,%.1f,%d,%d,%d,%d,%.2f,%d,%s\n",
                r->id, r->a + 'A', r->b + 'A', r->distance_km, r->speed_limit,
                r->free_time, r->vehicles, r->capacity, r->vc_ratio,
                r->closed ? 0 : road_travel_time(r), r->status);
    }

    fprintf(f, "\n# JUNCTION TRAFFIC SIGNALS & LOS\n");
    fprintf(f, "junction_id,name,queue_veh,fixed_green_sec,adaptive_green_sec,delay_sec,los,offset_sec\n");
    for (int i = 0; i < N; i++)
    {
        Junction *j = &junctions[i];
        fprintf(f, "%c,\"%s\",%d,%d,%d,%.1f,%c,%d\n",
                j->id, j->name, j->queue, j->current_green, j->adaptive_green,
                j->delay_sec, j->los, j->offset_sec);
    }

    fclose(f);
    puts(">> Successfully exported comprehensive traffic report to [traffic_report.csv].");
}
