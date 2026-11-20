#include "gui_common.h"
#include "gui_analytics.h"

void gui_handle_scenario(int kind)
{
    apply_scenario(kind);
    if (kind == 1)
        gui_show_text(">> Normal Daytime Operations Restored: Balanced flows across all corridors.");
    else if (kind == 2)
        gui_show_text(">> Capacity Shock Applied: Road C-D closed; severe spillover traffic on A-C & C-E.");
    else if (kind == 3)
        gui_show_text(">> Peak Surge Applied: All 10 roads and 6 junctions pushed beyond capacity.");
}

void gui_show_sort_search(void)
{
    Road values[R];
    char input[20], text[1500] =
        "========================================================================\r\n"
        "        ROAD TRAFFIC LOADS: QUICK SORT & ITERATIVE BINARY SEARCH        \r\n"
        "========================================================================\r\n"
        "Sorted Road Segments by Descending Load (Quick Sort - O(R log R)):\r\n";
    char row[100];
    int target, low = 0, high = R - 1, found = 0;

    GetWindowTextA(load_box, input, (int)sizeof(input));
    if (sscanf(input, "%d", &target) != 1)
    {
        gui_show_text("Please enter a valid integer vehicle count in the Load box (e.g. 26).");
        return;
    }

    memcpy(values, roads, sizeof(values));
    quick_sort_roads(values, 0, R - 1);

    for (int i = 0; i < R; i++)
    {
        snprintf(row, sizeof(row), "  Rank %2d: Road %c-%c | Load: %2d veh | Capacity: %2d | V/C: %.2f (%s)\r\n",
                 i + 1, values[i].a + 'A', values[i].b + 'A',
                 values[i].vehicles, values[i].capacity, values[i].vc_ratio, values[i].status);
        strcat(text, row);
    }
    strcat(text, "------------------------------------------------------------------------\r\n");

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (values[mid].vehicles == target)
        {
            snprintf(row, sizeof(row), ">> BINARY SEARCH MATCH FOUND (O(log R)): Road %c-%c has exactly %d vehicles.\r\n",
                     values[mid].a + 'A', values[mid].b + 'A', target);
            strcat(text, row);
            found = 1;
            break;
        }
        if (values[mid].vehicles < target)
            high = mid - 1;
        else
            low = mid + 1;
    }
    if (!found)
    {
        snprintf(row, sizeof(row), ">> No road segment currently has exactly %d vehicles.\r\n", target);
        strcat(text, row);
    }
    gui_show_text(text);
}

void gui_export_csv(void)
{
    export_traffic_report();
    gui_show_text(">> Successfully exported full network state to [traffic_report.csv].");
}
