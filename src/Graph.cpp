#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

void Graph::addStation(int id, const std::string& name) {
    stationNames[id] = name;
    if (adjList.find(id) == adjList.end()) {
        adjList[id] = std::vector<Edge>();
    }
}

void Graph::addTrack(int source_id, int dest_id, double base_time) {
    adjList[source_id].push_back({dest_id, base_time, 0.0});
}

void Graph::printNetwork() {
    // Hidden for Web API
}

void Graph::updateTrackPenalty(int source_id, int dest_id, double penalty) {
    for (auto& track : adjList[source_id]) {
        if (track.destination_id == dest_id) {
            track.dynamic_penalty = penalty;
            return;
        }
    }
}

double Graph::getNextAvailableTime(int source_id, int dest_id, double current_time, double travel_time) {
    auto track_key = std::make_pair(source_id, dest_id);
    if (reservationTable.find(track_key) == reservationTable.end()) return current_time;

    double test_time = current_time;
    bool found = false;
    while (!found) {
        found = true;
        for (const auto& slot : reservationTable[track_key]) {
            if (test_time < slot.end_time && (test_time + travel_time) > slot.start_time) {
                test_time = slot.end_time; 
                found = false;
                break; 
            }
        }
    }
    return test_time;
}

void Graph::bookTrack(int source_id, int dest_id, double start_time, double end_time, int train_id) {
    auto forward_key = std::make_pair(source_id, dest_id);
    reservationTable[forward_key].push_back({start_time, end_time, train_id});
    auto backward_key = std::make_pair(dest_id, source_id);
    reservationTable[backward_key].push_back({start_time, end_time, train_id});
}

void Graph::findShortestPathForTrain(int train_id, const std::string& train_name, int start_id, int end_id, double start_time) {
    // Handled dynamically by the tick engine now
}

double Graph::getTrackTime(int source_id, int dest_id) {
    for(const auto& track : adjList[source_id]) {
        if(track.destination_id == dest_id) return track.getTotalCost();
    }
    return 0.0;
}

// =======================================================================
// THE NEW SENSOR: MOVING BLOCK (HEADWAY) TELEMETRY
// =======================================================================
bool Graph::isTrackClear(int station_A, int station_B) {
    // The "Safety Bubble": Trains must be at least 15 minutes apart!
    double SAFE_HEADWAY_MINUTES = 15.0; 
    
    for (const auto& train : activeTrains) {
        // Only check trains that are actually moving on the tracks
        if (!train.is_active || train.time_until_arrival <= 0) continue;

        // 1. HEAD-ON CRASH CHECK (Absolute Halt)
        // If a train is coming towards us (B to A), we CANNOT enter the track.
        if (train.current_station_id == station_B && train.next_station_id == station_A) {
            return false; 
        }

        // 2. REAR-END CRASH CHECK (Headway Platoon)
        // If a train is going the exact same way (A to B), check how far ahead it is!
        if (train.current_station_id == station_A && train.next_station_id == station_B) {
            
            double total_track_time = getTrackTime(station_A, station_B);
            double time_already_traveled = total_track_time - train.time_until_arrival;
            
            // If the train ahead left less than 15 minutes ago, it's too close. We must wait!
            if (time_already_traveled < SAFE_HEADWAY_MINUTES) {
                return false; 
            }
        }
    }
    return true; // Track is safe! Proceed!
}
// =======================================================================

void Graph::spawnTrain(int id, const std::string& name, int start_station, int destination) {
    
    std::vector<int> path = dijkstra(start_station, destination);

    if (path.empty()) {
        std::cout << "Train " << name << " cannot find route!\n";
        return;
    }

    // Remove starting station
    path.erase(path.begin());

    Train newTrain = {
        id,
        name,
        start_station,
        start_station,
        0.0,
        true,
        path
    };

    activeTrains.push_back(newTrain);
}

void Graph::tick(double current_clock) {
    for (auto& train : activeTrains) {
        if (!train.is_active) continue;

        if (train.time_until_arrival > 0) {
            train.time_until_arrival -= 1.0; 
        } 
        else {
            train.current_station_id = train.next_station_id;

            if (!train.itinerary.empty()) {
                int next_stop = train.itinerary.front(); 
                
                // Call our highly advanced Moving Block sensor
                if (!isTrackClear(train.current_station_id, next_stop)) {
                    continue; // Stay delayed at the station
                }

                train.itinerary.erase(train.itinerary.begin()); 
                train.next_station_id = next_stop;
                train.time_until_arrival = getTrackTime(train.current_station_id, next_stop);
                
            }
        }
    }
}

// Format the data for the Web Dashboard
std::string Graph::getSystemStateJSON(double current_clock) {
    std::string json = "{\n";
    json += "  \"system_time\": " + std::to_string((int)current_clock) + ",\n";
    json += "  \"active_trains\": [\n";

    int count = 0;
    int total_trains = 0;
    for (const auto& t : activeTrains) if (t.is_active) total_trains++;

    for (const auto& train : activeTrains) {
        if (!train.is_active) continue;

        json += "    {\n";
        json += "      \"id\": " + std::to_string(train.id) + ",\n";
        json += "      \"name\": \"" + train.name + "\",\n";
        json += "      \"current_location\": \"" + stationNames[train.current_station_id] + "\",\n";
        
        if (train.itinerary.empty() && train.time_until_arrival <= 0) {
            json += "      \"status\": \"RESTING_AT_DESTINATION\"\n";
        } else if (train.time_until_arrival > 0) {
            json += "      \"status\": \"IN_TRANSIT\",\n";
            json += "      \"heading_to\": \"" + stationNames[train.next_station_id] + "\",\n";
            json += "      \"minutes_remaining\": " + std::to_string((int)train.time_until_arrival) + "\n";
        } else {
            json += "      \"status\": \"DELAYED_WAITING_FOR_TRACK\"\n";
        }

        json += "    }";
        
        count++;
        if (count < total_trains) json += ","; 
        json += "\n";
    }

    json += "  ]\n";
    json += "}";
    
    return json;
}

std::vector<int> Graph::dijkstra(int start, int end) {
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;

    std::priority_queue<
        std::pair<double, int>,
        std::vector<std::pair<double, int>>,
        std::greater<>
    > pq;

    // Initialize distances
    for (auto& node : adjList) {
        dist[node.first] = std::numeric_limits<double>::infinity();
    }

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        std::pair<double, int> topElement = pq.top();
        pq.pop();

        double currentDist = topElement.first;
        int currentNode = topElement.second;

        for (auto& edge : adjList[currentNode]) {
            double weight = edge.getTotalCost();
            double newDist = currentDist + weight;

            if (newDist < dist[edge.destination_id]) {
                dist[edge.destination_id] = newDist;
                parent[edge.destination_id] = currentNode;
                pq.push({newDist, edge.destination_id});
            }
        }
    }

    // No path case
    if (parent.find(end) == parent.end() && start != end) {
        std::cout << "No path found!\n";
        return {};
    }

    // Build path
    std::vector<int> path;
    int current = end;

    while (current != start) {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}