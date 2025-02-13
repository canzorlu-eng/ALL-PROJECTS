#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include<sstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    // Load map data from file

    map.loadDistanceData(distance_data);
    map.resetVisited();


    // Mark all provinces as unvisited initially

    // Load priority provinces
    loadPriorityProvinces(priority_data);
    // Load restricted provinces
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    ifstream infile;
    infile.open(filename);
    string line;
    int i=0;
    
    while (getline(infile,line))
    {
        if (line.empty())
            continue;


        // finding indices of paranthesis
        int start = line.find('(');
        int end = line.find(')');
        
        string numberStr = line.substr(start + 1, end - start - 1); //(startIndex,Length)
        int number = stoi(numberStr); 
        priorityProvinces[i]=number;
        
        i++;
    }
    if (i<MAX_PRIORITY_PROVINCES)
    {
        for (int j = i;  j< MAX_PRIORITY_PROVINCES; j++)
        {
            priorityProvinces[j]=-1;
        }
    }
    numPriorityProvinces=i;
    
    
    infile.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    ifstream infile;
    infile.open(filename);
    string line;
    int i=0;
    
    while (getline(infile,line))
    {
        if (line.empty())
            continue;


        // finding indices of paranthesis
        int start = line.find('(');
        int end = line.find(')');
        
        string numberStr = line.substr(start + 1, end - start - 1); //(startIndex,Length)
        int number = stoi(numberStr); 
        weatherRestrictedProvinces[i]=number;
        
        i++;
    }
    if(i<MAX_WEATHER_RESTRICTED_PROVINCES){
        for (int j = i;  j< MAX_WEATHER_RESTRICTED_PROVINCES; j++)
        {
            weatherRestrictedProvinces[j]=-1;
        }
    }
    numWeatherRestrictedProvinces=i;
    
    
    infile.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int a : priorityProvinces)
    {
        if(a==province)
            return true;
    }
    
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int a : weatherRestrictedProvinces)
    {
        if(a==province)
            return true;
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    map.markAsVisited(startingCity);
    route.push_back(startingCity);
    stack.push(startingCity);
    exploreFromProvince(startingCity);
}


void RoutePlanner::exploreFromProvince(int province) {
    
    vector<int> currentRoute;       // Geçici rota
    vector<int> bestRoute;          // En iyi rota
    int bestRouteDistance = 0;      // En iyi rota mesafesi

    stack.push(province);  // Başlangıç ilini yığına ekle
    map.markAsVisited(province);  // Başlangıç ilini ziyaret edildi olarak işaretle
    currentRoute.push_back(province); // Başlangıç ilini geçici rotaya ekle
enqueueNeighbors(province);
    if(!foundNextProvince) {
        backtrack();
        if(isExplorationComplete()) {
            displayResults();
            exit(0);
        }
    }
    while (!isExplorationComplete())
    {

        while (!stack.isEmpty()) {
            int currentProvince = stack.peek(); // take the top of the stack

            // enqueue all neighbors
            enqueueNeighbors(currentProvince);

            

            if (foundNextProvince)
            {
                while (!queue.isEmpty()) {
                int nextProvince = queue.dequeue();
                if (isWeatherRestricted(nextProvince)) {
                    // if it is a weather restricted province just send a message and continue
                    cout << "Province " << cities[nextProvince] << " is weather-restricted. Skipping.\n";
                    stack.pop();
                    continue;
                }

                if (!map.isVisited(nextProvince) && !isWeatherRestricted(nextProvince) &&
                    map.isWithinRange(currentProvince, nextProvince, maxDistance)) {
                    map.markAsVisited(nextProvince);            //mark it
                    currentRoute.push_back(nextProvince);       // add into current route
                    totalDistanceCovered += map.getDistance(currentProvince, nextProvince); //calculate total distance
                    stack.push(nextProvince);           
                    foundNextProvince = true;                  
                    break; // break to get next
                }
            }
        }
        
        

        // if it is dead-end, backtrack
        if (!foundNextProvince) {
            backtrack();//stack.pop();
            if (!currentRoute.empty()) {
                currentRoute.pop_back(); // while backtracking update route
            }
        }

        // find the longest route(which we visit the most province)
        if (currentRoute.size() > bestRoute.size()) {
            bestRoute = currentRoute;
            bestRouteDistance = totalDistanceCovered;
        }
    }

    // finally take best as route
    route = bestRoute;
    totalDistanceCovered = bestRouteDistance;
    }
    //display results
    displayResults();
}


void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    foundNextProvince = false;
    map.markAsVisited(province);
    for (int neighbor = 0; neighbor < 81; ++neighbor) {
        if (!map.isVisited(neighbor) && neighbor != province) {
            if (map.isWithinRange(province, neighbor, maxDistance)) {
                if (isPriorityProvince(neighbor)) 
                    queue.enqueuePriority(neighbor);
                else 
                    queue.enqueue(neighbor);

                foundNextProvince=true;
            }
        }
    }

}

void RoutePlanner::backtrack() {
    if (!stack.isEmpty()) {
        stack.pop();
    }
}


bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    return stack.isEmpty() && queue.isEmpty();
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    cout << "----------------------------\n";
    cout << "Journey Completed!\n";
    cout << "----------------------------\n";
    cout << "Total Number of Provinces Visited: " << route.size() << "\n";
    cout << "Total Distance Covered: " << totalDistanceCovered << " km\n";
    cout << "Route Taken:\n";
    for (int i = 0; i < route.size(); ++i) {
        cout << cities[route[i]];
        if (i < route.size() - 1) cout << " -> ";
    }
    cout << " -> End";
    cout << "\n\nPriority Provinces Status:\n";
    int visitedCount = 0;
    for (int province : priorityProvinces) {
        if (province != -1) {
            cout << "- " << cities[province] << (map.isVisited(province) ? " (Visited)" : " (Not Visited)") << "\n";
            if (map.isVisited(province)) {
                visitedCount++;
            }
        }
    }
    cout << "\nTotal Priority Provinces Visited: " << visitedCount << " out of " << numPriorityProvinces << "\n";
    cout << (visitedCount == numPriorityProvinces ? "Success: All priority provinces were visited.\n" : "Warning: Not all priority provinces were visited.\n");
}


