#ifndef TRAFFIC_ANALYTICS_H
#define TRAFFIC_ANALYTICS_H

#include "traffic_common.h"
#include "traffic_network.h"
#include "traffic_signal.h"
#include "traffic_routing.h"

void detect_congestion_and_blockages(void);
void measure_system_improvements(void);
void quick_sort_roads(Road a[], int low, int high);
void sort_and_search_loads(void);
void apply_scenario(int kind);
void run_demonstration_scenario(void);

#endif /* TRAFFIC_ANALYTICS_H */
