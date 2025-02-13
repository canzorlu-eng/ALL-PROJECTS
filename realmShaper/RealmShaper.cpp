#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include<vector>
#include<string>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;

    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints
    ifstream infile;
    infile.open(filename);
    string line;
    
    while (getline(infile,line))
    {
        if (line.empty())
            continue;  
        string playerName;
        int honourPoints;
        string honourPointsStr;

        

        istringstream iss(line);


        //std::getline(iss, playerName, ' ');
        //std::getline(iss, honourPointsStr);
        iss>>playerName>>honourPointsStr;

        honourPoints = std::stoi(honourPointsStr);

        RealmShaper *player = new RealmShaper(playerName, honourPoints);
        players.push_back(player);
        
    }
    infile.close();

    return players;
}



bool RealmShaper::operator==(const RealmShaper &other) const
{
    // TODO: Compare by name, return true if same

    return (this->name==other.name) ? true : false;
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name<<std::endl); // << std::endl);
}
void RealmShaper::setLocateIsle(const std::string &isleName) {
    locateIsle = isleName;
}

const std::string &RealmShaper::getLocateIsle() const {
    return locateIsle;
}