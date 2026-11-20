#ifndef TRAFFIC_COMMON_H
#define TRAFFIC_COMMON_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 6               /* Number of junctions (A to F) */
#define R 10              /* Number of bidirectional roads */
#define INF 1000000       /* Representation of infinity */
#define CYCLE_TIME 90     /* Standard traffic signal cycle length in seconds */
#define BASE_GREEN 30     /* Baseline fixed-time signal green duration in seconds */
#define PROGRESS_SPEED 40 /* Green wave target progression speed in km/h */

typedef struct
{
    char id;
    const char *name;
    int queue;          /* Queued vehicles waiting at intersection */
    int current_green;  /* Current allocated green light (seconds) */
    int adaptive_green; /* Recommended adaptive green light (seconds) */
    int offset_sec;     /* Coordinated signal offset (seconds) from cycle start */
    double delay_sec;   /* Average delay per vehicle at this junction */
    char los;           /* Level of Service grade: 'A', 'B', 'C', 'D', 'E', 'F' */
} Junction;

typedef struct
{
    int id;
    int a, b;           /* End-point junction indices */
    double distance_km; /* Road segment length in km */
    int speed_limit;    /* Speed limit in km/h */
    int free_time;      /* Free-flow travel time (minutes) */
    int capacity;       /* Vehicle capacity threshold */
    int vehicles;       /* Current active vehicle count */
    int closed;         /* 0 = Open, 1 = Blocked/Closed due to incident */
    double vc_ratio;    /* Volume-to-Capacity ratio */
    const char *status; /* "NORMAL", "MODERATE", "HEAVY", "CRITICAL", "CLOSED" */
} Road;

/* Terminal / Console Input Helpers */
int read_line(char *s, int n);
int read_number(const char *prompt, int *value);
int read_vertex(const char *prompt);

#endif /* TRAFFIC_COMMON_H */
