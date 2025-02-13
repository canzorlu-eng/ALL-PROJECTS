#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <utility>

ShaperTree::ShaperTree()
{
    
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (RealmShaper* shaper : realmShapers) {
        delete shaper;
    }
   realmShapers.clear();
   
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    realmShapers.clear();
    for (size_t i = 0; i < shapers.size(); i++)
    {
        insert(shapers[i]);
    }
    
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if(index< static_cast<int>(realmShapers.size()) && index>=0)
        isValid=true;

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    if (shaper){
        int a=findIndex(shaper);
        if (a==-1)
            realmShapers.push_back(shaper);
    }
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    int a=findIndex(shaper);
    if (a!=-1)
    {
        
        
        RealmShaper* toDelete = realmShapers[a];
        

        if (a == static_cast<int>(realmShapers.size()) - 1)
        {
            realmShapers.pop_back();
        }
        else
        {

            for (size_t j = a; j < realmShapers.size()-1; j++)
            {
                realmShapers[j]=realmShapers[j+1];
            }   
            realmShapers.pop_back();
        }
        delete toDelete;
        return a;
        
    }
    
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    for (size_t i = 0; i < realmShapers.size(); i++)
    {
        if (realmShapers[i]==shaper)
        {
            return static_cast<int>(i);
        }
        
    }
    
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    for (size_t i = 0; i < realmShapers.size(); i++)
    {
        if (realmShapers[i] == shaper)
        {
            //h=floor(log(n+1))
            return static_cast<int>(floor(log2(static_cast<int>(i) + 1)));
        }
    }
    return -1;
}

int ShaperTree::getDepth()
{
    if (realmShapers.empty())
        return 0;
    
    // return total|max depth|height of the tree
    return static_cast<int>(floor(log2(realmShapers.size())));
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    
    
    RealmShaper *opponent=getParent(challenger);
    if (opponent)
    {
        if(!result){
        opponent->gainHonour();
        challenger->loseHonour();
        cout << "[Duel] " << challenger->getName() << " lost the duel" << endl;
        cout << "[Honour] New honour points: "
                  << challenger->getName() << " - " << challenger->getHonour() << " "
                  << opponent->getName() << " - " << opponent->getHonour() << endl;
        if (challenger->getHonour()<=0)
        {
            cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << endl;
            remove(challenger);
        }

        return *opponent;
        
        }
        else{
            challenger->gainHonour();
            opponent->loseHonour();
            replace(challenger,opponent);

            cout << "[Duel] " << challenger->getName() << " won the duel" << endl;
            cout << "[Honour] New honour points: "
                    << challenger->getName() << "-" << challenger->getHonour() << " "
                    << opponent->getName() << "-" << opponent->getHonour() << endl;
            if (opponent->getHonour()<=0)
            {
                cout << "[Duel] " << opponent->getName() << " lost all honour, delete" << endl;
                remove(opponent);
            }

            return *challenger;
        }
        
    }
    
    return *challenger;
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper
    int a=findIndex(shaper);
    if (a <= 0)
    {
        return shaper;
    }

    parent=realmShapers[(a-1)/2];

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int index_low = findIndex(player_low);
    int index_high = findIndex(player_high);

    // Check if both players exist in the tree
    if (index_low != -1 && index_high != -1)
        swap(realmShapers[index_low], realmShapers[index_high]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (size_t i = 0; i < realmShapers.size(); i++)
    {
        if (realmShapers[i])
        {
            if (*realmShapers[i]==shaper)
            {
                foundShaper=realmShapers[i];
                return foundShaper;
            }
        }
        
        
        
    }

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (size_t i = 0; i < realmShapers.size(); i++)
    {
        if (realmShapers[i])
        {
            if (realmShapers[i]->getName()==name)
            {
                foundShaper=realmShapers[i];
                return foundShaper;
            }
        }
        
        
        
    }
    
    return foundShaper;
}

void inOrderHelper(const vector<RealmShaper *> &tree, int index, vector<std::string> &result)
{
    if (index >= static_cast<int>(tree.size()) || tree[index] == nullptr)
        return;

    // Traverse the left subtree
    inOrderHelper(tree, 2 * index + 1, result);
    // Visit the current node and add the name to the result vector
    result.push_back(tree[index]->getName());
    // Traverse the right subtree
    inOrderHelper(tree, 2 * index + 2, result);
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    inOrderHelper(realmShapers, index, result);

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well
    return result;
}






void preOrderHelper(const vector<RealmShaper *> &tree, int index, vector<std::string> &result)
{
    if (index >= static_cast<int>(tree.size()) || tree[index] == nullptr)
        return;

    // Visit the current node and add the name to the result vector
    result.push_back(tree[index]->getName());
    // Traverse the left subtree
    preOrderHelper(tree, 2 * index + 1, result);
    // Traverse the right subtree
    preOrderHelper(tree, 2 * index + 2, result);
}
std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector
    preOrderHelper(realmShapers, index, result);
    // Define and implement as many helper functions as necessary for recursive implementation
    return result;
}



void postOrderHelper(const vector<RealmShaper *> &tree, int index, vector<std::string> &result)
{
    if (index >= static_cast<int>(tree.size()) || tree[index] == nullptr)
        return;


    // Traverse the left subtree
    postOrderHelper(tree, 2 * index + 1, result);
    // Traverse the right subtree
    postOrderHelper(tree, 2 * index + 2, result);
    // Visit the current node and add the name to the result vector
    result.push_back(tree[index]->getName());
    
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector
    postOrderHelper(realmShapers, index, result);
    // Define and implement as many helper functions as necessary for recursive implementation
    return result;
}






void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file
    // Base case: if index is out of bounds, return
    if (index >= static_cast<int>(realmShapers.size()) || !realmShapers[index])
        return;

    // Write the current node to the file
    outFile << realmShapers[index]->getName() << std::endl;

    // Recurse for the left child: 2 * index + 1
    preOrderTraversal(2 * index + 1, outFile);

    // Recurse for the right child: 2 * index + 2
    preOrderTraversal(2 * index + 2, outFile);
    // Define and implement as many helper functions as necessary for recursive implementation
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file
    for (size_t i = 0; i < realmShapers.size(); i++)
    {
        if (realmShapers[i])
        {
            outFile << realmShapers[i]->getName() << std::endl;
        }
    }
    // Define and implement as many helper functions as necessary
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2*index+1;  // TODO: Calculate left index
    int right = 2*index+2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    ofstream outFile(filename);
    breadthFirstTraversal(outFile);

    outFile.close();
    cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    ofstream outFile(filename);
    preOrderTraversal(0,outFile);

    outFile.close();
    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}