/* Urban Traffic Signal and Route Coordination System - Launch Pad
 *
 * C11 Main Entry Point & Orchestrator for Modular Subsystems:
 * - traffic_network   : Topology, road states, capacities & serialization
 * - traffic_signal    : Queue monitoring, adaptive green splits & green waves
 * - traffic_routing   : Congestion + delay aware Dijkstra & Bellman-Ford
 * - traffic_analytics : Congestion detection, rerouting, sorting & benchmarking
 */

#include "src/traffic_common.h"
#include "src/traffic_network.h"
#include "src/traffic_signal.h"
#include "src/traffic_routing.h"
#include "src/traffic_analytics.h"

static void display_menu(void)
{
    puts("\n================================================================================");
    puts("        INTELLIGENT URBAN TRAFFIC SIGNAL & ROUTE COORDINATION SYSTEM           ");
    puts("================================================================================");
    puts(" 1. Understand Road Network     - Display topology, speeds, capacities & states");
    puts(" 2. Monitor Junction Traffic    - Real-time queue monitor & Level of Service (LOS)");
    puts(" 3. Intelligent Signal Control  - Queue-responsive adaptive green split optimizer");
    puts(" 4. Find Efficient Routes       - Congestion + signal delay aware Dijkstra routing");
    puts(" 5. Detect Congestion & Jams    - Automated scan for V/C > 1.0 & road blockages");
    puts(" 6. Dynamic Rerouting Engine    - Avoid road blockages and divert congested flows");
    puts(" 7. Green Wave Coordination     - Synchronize corridor signals to eliminate stops");
    puts(" 8. Measure System Improvement  - Quantified Impact Benchmark (Baseline vs System)");
    puts(" 9. Algorithmic Path Comparison - Dijkstra vs Bellman-Ford (DAA proof & timing)");
    puts("10. Sort & Search Traffic Loads - Quick Sort ranking & Binary Search lookup");
    puts("11. Run Demonstration Scenario  - Normal, Capacity Shock (C-D Block), or Peak Surge");
    puts("12. Export Report to CSV        - Save complete network state to traffic_report.csv");
    puts(" 0. Exit System");
    puts("================================================================================");
}

int main(void)
{
    int choice;
    reset_city();

    puts("********************************************************************************");
    puts("*     Welcome to the Urban Traffic Signal & Route Coordination System          *");
    puts("*          Autonomous Traffic Management & Performance Benchmark               *");
    puts("********************************************************************************");

    do
    {
        display_menu();
        if (!read_number("Select an option (0-12): ", &choice))
        {
            puts("Please enter a valid menu number.");
            continue;
        }

        switch (choice)
        {
        case 1:
            display_network();
            break;
        case 2:
            monitor_junctions();
            break;
        case 3:
            manage_signals_intelligently();
            break;
        case 4:
            find_efficient_route();
            break;
        case 5:
            detect_congestion_and_blockages();
            break;
        case 6:
            dynamic_rerouting_simulation();
            break;
        case 7:
            coordinate_green_wave();
            break;
        case 8:
            measure_system_improvements();
            break;
        case 9:
            compare_algorithms();
            break;
        case 10:
            sort_and_search_loads();
            break;
        case 11:
            run_demonstration_scenario();
            break;
        case 12:
            export_traffic_report();
            break;
        case 0:
            puts("Shutting down Urban Traffic Coordination System. Drive safely!");
            break;
        default:
            puts("Invalid selection. Please choose an option between 0 and 12.");
            break;
        }
    } while (choice != 0);

    return 0;
}
