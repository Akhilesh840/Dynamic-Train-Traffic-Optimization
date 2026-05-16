#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <utility>

// Represents a physical track
struct Edge {
    int destination_id;
    double base_weight;      
    double dynamic_penalty;  

    double getTotalCost() const {
        return base_weight + dynamic_penalty;
    }
};

// Represents a booked time slot on a track
struct TimeSlot {
    double start_time;
    double end_time;
    int train_id;
};

// Represents a moving train in the simulation
struct Train {
    int id;
    std::string name;
    int current_station_id;
    int next_station_id;
    double time_until_arrival; 
    bool is_active;
    std::vector<int> itinerary; 
};



class Graph {
private:
    std::unordered_map<int, std::vector<Edge>> adjList;
    std::unordered_map<int, std::string> stationNames;
    std::map<std::pair<int, int>, std::vector<TimeSlot>> reservationTable;
    
    std::vector<Train> activeTrains;
   

public:
    Graph() = default;

    void addStation(int id, const std::string& name);
    void addTrack(int source_id, int dest_id, double base_time);
    void printNetwork();
    void updateTrackPenalty(int source_id, int dest_id, double penalty);

    double getNextAvailableTime(int source_id, int dest_id, double current_time, double travel_time);
    void bookTrack(int source_id, int dest_id, double start_time, double end_time, int train_id);
    void findShortestPathForTrain(int train_id, const std::string& train_name, int start_id, int end_id, double start_time);

    double getTrackTime(int source_id, int dest_id);
    bool isTrackClear(int station_A, int station_B); 
    void spawnTrain(int id, const std::string& name, int start_station, int destination);
    void tick(double current_clock);

    // --- PHASE 3: THE WEB API BRIDGE ---
    // Generates a JSON string representing the entire physical state of the simulation
    std::string getSystemStateJSON(double current_clock); 
    std::vector<int> dijkstra(int start, int end);
};
    

#endif