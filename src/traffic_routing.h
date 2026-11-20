#ifndef TRAFFIC_ROUTING_H
#define TRAFFIC_ROUTING_H

#include "traffic_common.h"
#include "traffic_network.h"
#include "traffic_signal.h"

void build_cost_matrix(double g[N][N], int use_signals, int use_adaptive);
double dijkstra(int source, int target, int parent[N], long long *ops, int use_signals, int use_adaptive);
double bellman(int source, int target, int parent[N], long long *ops, int use_signals, int use_adaptive);
int extract_path(const int parent[N], int target, int out_path[N]);
void print_path_string(const int path[], int len);
void format_path_string(char *buf, size_t size, const int path[], int len);
void find_efficient_route(void);
void compare_algorithms(void);
void dynamic_rerouting_simulation(void);

#endif /* TRAFFIC_ROUTING_H */
