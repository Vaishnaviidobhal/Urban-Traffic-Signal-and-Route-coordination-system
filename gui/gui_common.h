#ifndef GUI_COMMON_H
#define GUI_COMMON_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "traffic_common.h"
#include "traffic_network.h"
#include "traffic_signal.h"
#include "traffic_routing.h"
#include "traffic_analytics.h"

#define ID_SOURCE       101
#define ID_DESTINATION  102
#define ID_ROUTE        103
#define ID_COMPARE      104
#define ID_NETWORK      105
#define ID_MONITOR      106
#define ID_SIGNALS      107
#define ID_DETECT       108
#define ID_REROUTE      109
#define ID_GREENWAVE    110
#define ID_IMPACT       111
#define ID_NORMAL       112
#define ID_SHOCK        113
#define ID_STRESS       114
#define ID_LOAD         115
#define ID_SEARCH       116
#define ID_EXPORT       117

extern HWND output_box;
extern HWND source_box;
extern HWND destination_box;
extern HWND load_box;

int gui_get_selected(HWND box);
void gui_show_text(const char *text);
void gui_create_button(HWND parent, const char *label, int id, int x, int y, int width, int height);

#endif /* GUI_COMMON_H */
