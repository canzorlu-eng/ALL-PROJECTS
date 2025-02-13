#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}


void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;

    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    //int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth) {return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);}
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
    if (realmShaper == nullptr) {
        cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }
    int playerDepth = shaperTree.getDepth(realmShaper);

    if (playerDepth == -1)
    {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }

    int isleDepth = mapTree.getIsleDepth(isle);
    if (isleDepth == -1) {
        return false;
    }
    
    int totalShaperTreeHeight = shaperTree.getDepth();
    if (totalShaperTreeHeight>0)
    {
        int totalMapDepth = mapTree.getDepth();
    int minAccessDepth = mapTree.calculateMinMapDepthAccess(playerDepth, totalShaperTreeHeight, totalMapDepth);

    hasAccess = isleDepth >= minAccessDepth;
    }
    

    

    return hasAccess;
}


void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    if (!hasAccess(realmShaper,isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() 
                  << " does not have access to explore area " << *isle << std::endl;
        return;
    }
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    std::string previousIsleName = realmShaper->getLocateIsle();
    if (previousIsleName != isle->getName() ) {//previousIsleName != isle->getName() && !previousIsleName.empty() 
        Isle *previousIsle = mapTree.findIsle(previousIsleName); // find previous isle on map
        if (previousIsle) {
            previousIsle->decreaseShaperCount();
        }
    }
  
    

    // update
    realmShaper->setLocateIsle(isle->getName());
    realmShaper->collectItem(isle->getItem());
    // check overcrowding for Isle, 
    
    bool overcrowding=isle->increaseShaperCount();
    // delete Isle if necessary
    
    
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    if (overcrowding)
    {
        
        std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;
        mapTree.remove(isle);
    }


    

    

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;
    

    // Check if the shaper has enough energy
    if (shaper->hasEnoughEnergy()) {
        // Deduct energy for crafting
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        shaper->loseEnergy();
        // Create the new isle and add it to the map
        Isle* newIsle = new Isle(isleName);
        mapTree.insert(newIsle);
        // Print the success messages
        
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    } else {
        // Print failure message if not enough energy
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }
}

void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();
    
    std::ifstream accessFile(accessLogs);
    

    // Open duel log file
    std::ifstream duelFile(duelLogs);
    
    

    std::string accessLine;
    std::string duelLine;
    int accessCount = 0;
    getline(accessFile,accessLine);
    getline(duelFile,duelLine);

    // Parse access logs
    while (std::getline(accessFile, accessLine))
    {
        std::istringstream lineStream(accessLine);
        std::string playerName, isleName;

        lineStream >> playerName >> isleName;

        RealmShaper *realmShaper = shaperTree.findPlayer(playerName);
        Isle *isle = mapTree.findIsle(isleName);
        if (realmShaper)
        {
            if (isle)
                exploreArea(realmShaper, isle);
            else
                craft(realmShaper, isleName);
        }
        
        

        accessCount++;

        // For every 5 access events, trigger a duel event if available
        if (accessCount % 5 == 0 && std::getline(duelFile, duelLine))
        {
            std::istringstream duelLineStream(duelLine);
            std::string duelPlayerName;
            string duelStr;
            bool duelResult;

            duelLineStream >> duelPlayerName >> duelStr;
            duelResult = static_cast<bool>(std::stoi(duelStr));

            RealmShaper *duelShaper = shaperTree.findPlayer(duelPlayerName);

            if (duelShaper)
            {
                shaperTree.duel(duelShaper, duelResult);
            }
            
        }
    }

    // Process remaining duels after all access events
    while (std::getline(duelFile, duelLine))
    {
        std::istringstream duelLineStream(duelLine);
        std::string duelPlayerName;
        string duelStr;
        bool duelResult;

        duelLineStream >> duelPlayerName >> duelStr;
        duelResult = static_cast<bool>(std::stoi(duelStr));
        RealmShaper *duelShaper = shaperTree.findPlayer(duelPlayerName);

        if (duelShaper)
        {
            shaperTree.duel(duelShaper, duelResult);
        }
        
    }

    // Close the files
    accessFile.close();
    duelFile.close();

    // Display the final game state
    displayGameState();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);

}