#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h> 

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Graph railway;

    // 1. REGISTER STATIONS
    railway.addStation(1, "Mumbai");
    railway.addStation(2, "Lonavala");
    railway.addStation(3, "Pune");
    railway.addStation(4, "Nashik"); 
    railway.addStation(5, "Kalyan"); 
    railway.addStation(6, "Surat");
    railway.addStation(7, "Nagpur");
    railway.addStation(8, "Solapur");
    railway.addStation(9, "Bhusawal");

    // 2. REGISTER TRACKS
    railway.addTrack(6, 4, 120.0); railway.addTrack(4, 6, 120.0); 
    railway.addTrack(4, 9, 130.0); railway.addTrack(9, 4, 130.0); 
    railway.addTrack(4, 5, 80.0);  railway.addTrack(5, 4, 80.0);  
    railway.addTrack(5, 1, 40.0);  railway.addTrack(1, 5, 40.0);  
    railway.addTrack(5, 2, 60.0);  railway.addTrack(2, 5, 60.0);  
    railway.addTrack(1, 2, 90.0);  railway.addTrack(2, 1, 90.0);  
    railway.addTrack(2, 3, 45.0);  railway.addTrack(3, 2, 45.0);  
    railway.addTrack(3, 8, 120.0); railway.addTrack(8, 3, 120.0); 
    railway.addTrack(9, 7, 200.0); railway.addTrack(7, 9, 200.0); 
    railway.addTrack(3, 7, 300.0); railway.addTrack(7, 3, 300.0); 

    // 3. THE FLEET
    railway.spawnTrain(101, "Rajdhani Exp", 6, 1);
    railway.spawnTrain(102, "Vande Bharat", 3, 7);
    railway.spawnTrain(103, "Panchavati Exp", 4, 1);
    railway.spawnTrain(104, "Deccan Queen", 3, 1);
    railway.spawnTrain(105, "Vidarbha Exp", 7, 4);
    railway.spawnTrain(106, "Godavari Exp", 9, 3);

    std::cout << "--- ENGINE STARTING (T=0) ---" << std::endl;
    
    double global_clock = 0.0;
    while (global_clock <= 500.0) {
        railway.tick(global_clock);
        std::string jsonResult = railway.getSystemStateJSON(global_clock);

        // std::ios::trunc wipes the file clean every single second
        std::ofstream outFile("train_state.json", std::ios::trunc); 
        if (outFile.is_open()) {
            outFile << jsonResult;
            outFile.close();
            std::cout << "System Clock: T=" << (int)global_clock << std::endl;
        }

        Sleep(1000); 
        global_clock += 1.0;
    }

    std::cout << "\n--- ENGINE OFFLINE ---" << std::endl;
    return 0;
}