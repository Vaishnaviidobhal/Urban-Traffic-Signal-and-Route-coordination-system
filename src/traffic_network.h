#ifndef TRAFFIC_NETWORK_H
#define TRAFFIC_NETWORK_H

#include "traffic_common.h"

extern Junction junctions[N];
extern Road roads[R];

void reset_city(void);
void update_network_metrics(void);
int road_travel_time(const Road *r);
int find_road(int u, int v);
void display_network(void);
void export_traffic_report(void);

#endif /* TRAFFIC_NETWORK_H */
