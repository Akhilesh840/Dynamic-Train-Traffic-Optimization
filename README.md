# 🚂 Dynamic Train Traffic Optimization Engine

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Algorithms](https://img.shields.io/badge/Algorithms-Graph_Theory-FF6F00?style=for-the-badge)
![Data Structures](https://img.shields.io/badge/Data_Structures-STL-008080?style=for-the-badge)

## 📌 Overview
The Dynamic Train Traffic Optimization Engine is a C++ based simulation designed to solve real-world routing bottlenecks in railway networks. By modeling train stations as vertices and railway tracks as weighted edges, this system calculates the most efficient routes, minimizes transit time, and resolves traffic conflicts.

This project was built to apply abstract **Data Structures and Algorithms (DSA)** to a practical, system-level architecture.

## 🧠 Core Algorithms & Data Structures

This engine relies heavily on Graph Theory to compute shortest paths and manage network state.

* **Graph Representation:** The railway network is modeled using an **Adjacency List** (`std::unordered_map` / `std::vector`), ensuring efficient memory usage compared to an adjacency matrix.
* **Shortest Path Calculation:** Implemented **Dijkstra's Algorithm** to find the minimum-distance path between any two stations in the network.
* **Optimization:** Utilizes a **Min-Heap / Priority Queue** (`std::priority_queue`) to greedily select the next closest unvisited station, significantly reducing search time.

### ⏱️ Complexity Analysis
* **Time Complexity:** `O(E log V)` — where *E* is the number of tracks (edges) and *V* is the number of stations (vertices). The Priority Queue ensures we aren't scanning the entire graph unnecessarily.
* **Space Complexity:** `O(V + E)` — to store the graph in an Adjacency List.

## 🚀 Key Features
* **Pathfinding:** Accurately calculates the shortest distance and exact route from a source station to a destination station.
* **Dynamic Node Management:** Capable of handling a dynamic graph structure (adding/removing tracks or stations).
* **Weight Integration:** Accounts for distance/time as "weights" on each edge to provide realistic routing outputs.

## 💻 Tech Stack
* **Language:** C++11 / C++14
* **Libraries:** Standard Template Library (STL) - `vector`, `queue`, `unordered_map`
* **Environment:** Terminal / Command Line Interface

## ⚙️ How to Run Locally

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/Akhilesh840/Train-Traffic-Optimization.git](https://github.com/Akhilesh840/Train-Traffic-Optimization.git)
   cd Train-Traffic-Optimization
