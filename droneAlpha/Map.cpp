#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include<climits>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    for (auto &row : distanceMatrix) { 
        for (auto &cell : row) {       
            cell = 0;           
        }
    }
    // Initialize all provinces as unvisited
    for(bool &a: visited)
        a=false;
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    ifstream infile;
    infile.open(filename);
    string line;
    int i=0;
    
    while (getline(infile,line))
    {
        if (line.empty())
            continue;
        int j=0;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            distanceMatrix[i][j]=stoi(token);  // convert to int and add to the vector
            j++;
        }
        i++;
    }
    infile.close();
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    return (distanceMatrix[provinceA][provinceB]<=maxDistance); 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province]=true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for(bool &a: visited)
        a=false;
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int c=0;
    for(bool a : visited){
        if(a==true)
            c++;
    }
    return c;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB];
}