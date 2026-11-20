/* Urban Traffic Signal & Route Coordination System - GUI Launch Pad
 *
 * Windows Graphical Entry Point & Orchestrator for GUI Subsystems:
 * - gui_common       : Window handles, control IDs & UI helpers
 * - gui_routing      : Route finding & Dijkstra vs Bellman-Ford comparison
 * - gui_monitor      : Road network status, junction monitor & adaptive signals
 * - gui_reroute      : Congestion detection & dynamic incident rerouting
 * - gui_coordination : Green wave progression & system impact benchmark
 * - gui_analytics    : Demonstration scenarios, sort & search, CSV export
 */

#include "gui/gui_common.h"
#include "gui/gui_routing.h"
#include "gui/gui_monitor.h"
#include "gui/gui_reroute.h"
#include "gui/gui_coordination.h"
#include "gui/gui_analytics.h"

static LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_COMMAND)
    {
        switch (LOWORD(wparam))
        {
        case ID_ROUTE:
            gui_show_route();
            return 0;
        case ID_COMPARE:
            gui_show_compare();
            return 0;
        case ID_NETWORK:
            gui_show_network();
            return 0;
        case ID_MONITOR:
            gui_show_monitor();
            return 0;
        case ID_SIGNALS:
            gui_show_signals();
            return 0;
        case ID_DETECT:
            gui_show_detect();
            return 0;
        case ID_REROUTE:
            gui_show_reroute();
            return 0;
        case ID_GREENWAVE:
            gui_show_greenwave();
            return 0;
        case ID_IMPACT:
            gui_show_impact();
            return 0;
        case ID_NORMAL:
            gui_handle_scenario(1);
            return 0;
        case ID_SHOCK:
            gui_handle_scenario(2);
            return 0;
        case ID_STRESS:
            gui_handle_scenario(3);
            return 0;
        case ID_SEARCH:
            gui_show_sort_search();
            return 0;
        case ID_EXPORT:
            gui_export_csv();
            return 0;
        }
    }
    if (message == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show)
{
    WNDCLASSA wc = {0};
    HWND window;
    MSG message;
    (void)previous;
    (void)command;

    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = "TrafficCoordinationGui";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassA(&wc);

    reset_city();

    window = CreateWindowA("TrafficCoordinationGui",
                           "Urban Traffic Signal & Route Coordination System",
                           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                           100, 60, 780, 720, NULL, NULL, instance, NULL);

    /* Row 1: Route origin/destination & primary pathfinding */
    CreateWindowA("STATIC", "Origin:", WS_VISIBLE | WS_CHILD, 20, 20, 50, 20, window, NULL, instance, NULL);
    source_box = CreateWindowA("EDIT", "A", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_UPPERCASE, 75, 17, 45, 25, window, (HMENU)(INT_PTR)ID_SOURCE, instance, NULL);

    CreateWindowA("STATIC", "Destination:", WS_VISIBLE | WS_CHILD, 135, 20, 80, 20, window, NULL, instance, NULL);
    destination_box = CreateWindowA("EDIT", "F", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_UPPERCASE, 220, 17, 45, 25, window, (HMENU)(INT_PTR)ID_DESTINATION, instance, NULL);

    gui_create_button(window, "Find Route", ID_ROUTE, 280, 15, 105, 28);
    gui_create_button(window, "Compare (Dijk vs BF)", ID_COMPARE, 395, 15, 160, 28);
    gui_create_button(window, "Road Network", ID_NETWORK, 565, 15, 175, 28);

    /* Row 2: Real-time intelligence & coordination */
    gui_create_button(window, "Junction Monitor", ID_MONITOR, 20, 55, 135, 28);
    gui_create_button(window, "Adaptive Signals", ID_SIGNALS, 165, 55, 135, 28);
    gui_create_button(window, "Detect Incidents", ID_DETECT, 310, 55, 135, 28);
    gui_create_button(window, "Dynamic Reroute", ID_REROUTE, 455, 55, 135, 28);
    gui_create_button(window, "Green Wave", ID_GREENWAVE, 600, 55, 140, 28);

    /* Row 3: Evaluation, scenarios & load search */
    gui_create_button(window, "Impact Benchmark", ID_IMPACT, 20, 95, 140, 28);
    gui_create_button(window, "Normal", ID_NORMAL, 170, 95, 75, 28);
    gui_create_button(window, "Shock", ID_SHOCK, 255, 95, 75, 28);
    gui_create_button(window, "Surge", ID_STRESS, 340, 95, 75, 28);

    CreateWindowA("STATIC", "Load:", WS_VISIBLE | WS_CHILD, 430, 98, 40, 20, window, NULL, instance, NULL);
    load_box = CreateWindowA("EDIT", "26", WS_VISIBLE | WS_CHILD | WS_BORDER, 475, 95, 45, 25, window, (HMENU)(INT_PTR)ID_LOAD, instance, NULL);
    gui_create_button(window, "Sort & Search", ID_SEARCH, 530, 95, 115, 28);
    gui_create_button(window, "Export CSV", ID_EXPORT, 655, 95, 85, 28);

    /* Output display console */
    output_box = CreateWindowA("EDIT",
                               "Welcome to the Urban Traffic Signal & Route Coordination System.\r\n"
                               "Select an action button above to run traffic monitoring, routing, or green wave coordination.",
                               WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
                               20, 140, 720, 520, window, NULL, instance, NULL);

    ShowWindow(window, show);
    UpdateWindow(window);

    while (GetMessageA(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    return 0;
}
