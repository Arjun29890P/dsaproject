# MediRoute: Emergency Ambulance Dispatcher System

**MediRoute** is an advanced console-based C++ emergency logistics and intelligent ambulance dispatch platform. Engineered to eliminate latency in life-threatening situations, the system automates medical triage using data structures and optimizes emergency vehicle routing across a city road network via graph algorithms.

---

## Project Overview and Motivation

Traditional emergency dispatch systems often suffer from bottlenecks, manual sorting errors, and inefficient routing. **MediRoute** solves this by:
1. Instantly prioritizing critical patients over moderate cases using a heap-based triage model.
2. Modeling city maps as weighted graphs to compute the absolute shortest physical path from an ambulance station to the emergency site, and subsequently to the nearest hospital.
3. Streamlining fleet management with real-time tracking of ambulance availability.

---

## Data Structures and Algorithms (DSA) Architecture

| Component | Underlying Data Structure / Algorithm | Role and Justification |
| :--- | :--- | :--- |
| **Emergency Triage** | Max-Priority Heap (`priority_queue`) | Automatically bubbles life-threatening cases (Severity 1) to the top in $O(\log n)$ time, ensuring critical patients are never stuck behind routine calls. |
| **City Road Map** | Adjacency List (`vector<vector<Edge>>`) | Represents intersections and weighted road links efficiently, offering optimized memory usage for sparse city grids. |
| **Shortest Path Engine** | Dijkstra's Algorithm | Computes optimal point-to-point transit paths and cumulative distance weights from the closest available ambulance to the target coordinate. |
| **Fleet and Registries** | Standard Vectors (`vector`) | Manages dynamic collections of ambulances, hospitals, and location nodes for quick lookups and state modifications. |

---

## System Workflow and Operational Logic

1. **Emergency Registration (Option 1):** 
   * Captures patient credentials, exact location node from a pre-configured city list, condition description, and severity tier (1: Critical, 2: Serious, 3: Moderate).
   * Pushes the emergency packet directly into the priority queue.
2. **Automated Dispatch (Option 2):**
   * Pops the highest-priority emergency from the heap.
   * Scans active ambulances, filtering for available units.
   * Runs Dijkstra’s algorithm between each candidate ambulance location and the patient node to find the nearest asset.
   * Calculates estimated time of arrival (ETA) based on a distance-to-time factor and identifies the nearest hospital destination.
   * Marks the assigned ambulance as `BUSY`.
3. **Trip Completion (Option 5):**
   * Resets the ambulance status back to `AVAILABLE` once the patient is delivered, returning the vehicle to active rotation.

---

## Project Structure

```text
mediroute/
│
├── mediroute.cpp       # Consolidated core logic, graph engine, and interactive menu
└── README.md           # Comprehensive project documentation