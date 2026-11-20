#ifndef TRAFFIC_SIGNAL_H
#define TRAFFIC_SIGNAL_H

#include "traffic_common.h"
#include "traffic_network.h"

char calculate_los(double delay_sec);
double signal_wait_delay_min(int junction_idx, int use_adaptive);
void monitor_junctions(void);
void manage_signals_intelligently(void);
void coordinate_green_wave(void);
int calculate_corridor_offsets(const int corridor[], int len, int offsets[N], double *stops_without);

#endif /* TRAFFIC_SIGNAL_H */
