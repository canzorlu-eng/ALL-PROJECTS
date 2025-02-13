#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
    clear(root);
    
    
}
void Map::clear(MapNode* node) {
    if (!node) return;
    
    clear(node->left);
    clear(node->right);
    if (node->isle) {
        delete node->isle;
    }
    delete node;
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items
    for (Isle *isle : isles){
        if (isle)
        {
            root = insert(root, isle);
            rebalanceNumber=0;
        }
        
        
    }

    populateWithItems();
}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (!current || !current->left)
    {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    // Update heights
    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}


MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    if (!current || !current->right)
    {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    // Update heights
    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}


int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    return node ? node->height : 0;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    MapNode *newNode = nullptr;

    // TODO: Recursively insert isle to the tree
    if (!node)
        return new MapNode(isle);
    newNode=node;
    // Insert recursively based on isle's name
    if (isle->getName() < newNode->isle->getName())
        newNode->left = insert(newNode->left, isle);
    else if (isle->getName() > newNode->isle->getName())
        newNode->right = insert(newNode->right, isle);
    else
        return newNode; // If the isle already exists, do not insert duplicates

    // Update height of the current newNode
    newNode->height = 1 + std::max(height(newNode->left), height(newNode->right));

    // Calculate the balance factor
    int balance = height(newNode->left) - height(newNode->right);
    
    

    // Perform rotations if the newNode is unbalanced
   
    if (balance > 1 || balance < -1)
        rebalanceNumber++;    

    // Left-Left case
    if (balance > 1 && isle->getName() < newNode->left->isle->getName()){
        return rotateRight(newNode);
    }

    // Right-Right case
    if (balance < -1 && isle->getName() > newNode->right->isle->getName()){
        return rotateLeft(newNode);
    }

    // Left-Right case
    if (balance > 1 && isle->getName() > newNode->left->isle->getName())
    {
        newNode->left = rotateLeft(newNode->left);
        return rotateRight(newNode);
    }

    // Right-Left case
    if (balance < -1 && isle->getName() < newNode->right->isle->getName())
    {
        newNode->right = rotateRight(newNode->right);
        return rotateLeft(newNode);
    }

    // returns inserted node

    return newNode;
}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);
    if (rebalanceNumber==3)
    {
        populateWithItems();
        dropItemBFS();
        rebalanceNumber=0;
    }
    

    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;

    if (!node) {
        std::cout << "[Remove] " << "Tree is Empty" << std::endl;
        return nullptr;
    }

    // Perform standard BST delete operation
    if (isle->getName() < node->isle->getName()) {
        node->left = remove(node->left, isle);
    } else if (isle->getName() > node->isle->getName()) {
        node->right = remove(node->right, isle);
    } else {
        // Node to be deleted found

        // Case 1: Node with only one child or no child
        if (!node->left || !node->right) {
            MapNode* temp = node->left ? node->left : node->right;

            if (!temp) {
                // No child case
                temp = node;
                
                node = nullptr;
            } else {
                // One child case
                *node = *temp;
            }
            
            delete temp;
            
        } else {
            // Case 2: Node with two children
            // Find the inorder predecessor (largest in the left subtree)
            MapNode* temp = node->left;
            while (temp && temp->right) {
                temp = temp->right;
            }

            // Copy the inorder predecessor's data to this node
            if (temp) {
                node->isle = temp->isle;
                // Delete the inorder predecessor
                node->left = remove(node->left, temp->isle);
            }
        }
    }

    // If the tree had only one node, return it
    if (!node) return node;

    // Update height of the current node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // Get the balance factor
    int balance = height(node->left) - height(node->right);
    if (balance > 1 || balance < -1)
       rebalanceNumber++;

    // Perform AVL rotations to balance the tree

    // Left-Left case
    if (balance > 1 && height(node->left->left) >= height(node->left->right)) {
        return rotateRight(node);
    }

    // Left-Right case
    if (balance > 1 && height(node->left->left) < height(node->left->right)) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Right case
    if (balance < -1 && height(node->right->right) >= height(node->right->left)) {
        return rotateLeft(node);
    }

    // Right-Left case
    if (balance < -1 && height(node->right->right) < height(node->right->left)) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    delete isle;
    if (rebalanceNumber==3)
    {
        populateWithItems();
        dropItemBFS();
        rebalanceNumber=0;
    }
    // you might need to insert some checks / functions here depending on your implementation
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (!current)
        return;

    // Drop EINSTEINIUM on every fifth Isle
    if (count == 1)
    {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
        count = 5; // Reset the count after a drop
    }
    else
        count--;

    // Traverse left subtree
    preOrderItemDrop(current->left, count);

    // Traverse right subtree
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (!current)
        return;

    // Traverse left subtree
    postOrderItemDrop(current->left, count);

    // Traverse right subtree
    postOrderItemDrop(current->right, count);

    // Drop GOLDIUM on every third Isle
    if (count == 1)
    {
        current->isle->setItem(GOLDIUM);
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
        count = 3; 
    }
    else
        count--;
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item
    std::queue<MapNode *> nodeQueue;
    nodeQueue.push(node);

    while (!nodeQueue.empty())
    {
        MapNode *current = nodeQueue.front();
        nodeQueue.pop();

        // Check if the current Isle has no items
        if (current->isle->getItem()==EMPTY)
        {
            return current;
        }

        // Add left and right children to the queue if they exist
        if (current->left)
        {
            nodeQueue.push(current->left);
        }
        if (current->right)
        {
            nodeQueue.push(current->right);
        }
    }

    // No empty Isle found
    return nullptr;
}
void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    MapNode *targetNode = findFirstEmptyIsle(root);

    if (targetNode)
    {
        targetNode->isle->setItem(AMAZONITE);
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    }
    else
    {
        std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    }
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM
    int goldiumCount = 3;
    postOrderItemDrop(root, goldiumCount);

    int einsteiniumCount = 5;
    preOrderItemDrop(root, einsteiniumCount);

}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else
    if (!root || !node)
        return -1;

    std::queue<std::pair<MapNode *, int>> q;
    q.push({root, 0});

    while (!q.empty())
    {
        MapNode *current = q.front().first;
        int depth = q.front().second;
        q.pop();

        if (current == node)
            return depth;

        if (current->left)
            q.push({current->left, depth + 1});
        if (current->right)
            q.push({current->right, depth + 1});
    }
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    if (!root || !isle)
        return -1;

    std::queue<std::pair<MapNode *, int>> q;
    q.push({root, 0});

    while (!q.empty())
    {
        MapNode *current = q.front().first;
        int depth = q.front().second;
        q.pop();

        if (current->isle == isle)
            return depth;

        if (current->left)
            q.push({current->left, depth + 1});
        if (current->right)
            q.push({current->right, depth + 1});
    }

    return -1; // Isle not found
    
}
int maxDepth(MapNode *node)
{
    if (!node)
        return 0;

    int leftDepth = maxDepth(node->left);
    int rightDepth = maxDepth(node->right);

    return 1 + std::max(leftDepth, rightDepth);
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    return maxDepth(root)-1;
}




Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    if (!root)
        return nullptr;

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        if (*(current->isle) == isle) 
            return current->isle;

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }


    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    if (!root)
        return nullptr;

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        if (current->isle->getName() == name) 
            return current->isle;

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    if (!root)
        return nullptr;

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        if (*(current->isle) == isle) 
            return current;

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    if (!root)
        return nullptr;

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        if (current->isle->getName() == name) 
            return current;

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}



void Map::writeToFile(const std::string &filename)
{
    if (!root)
    {
        return;
    }

    ofstream outfile(filename);
    queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        int levelSize = q.size(); 
        bool hasNonNull = false;  // check if leve has at least one non null

        std::vector<std::string> levelNodes; 

        for (int i = 0; i < levelSize; ++i)
        {
            MapNode *current = q.front();
            q.pop();

            if (current)
            {
                levelNodes.push_back(current->isle->getName());
                hasNonNull = true; 
                q.push(current->left);
                q.push(current->right);
            }
            else
            {
                levelNodes.push_back("NULL");
                q.push(nullptr);
                q.push(nullptr);
            }
        }

        // if this level has at least one non null value print
        if (hasNonNull)
        {
            for (const auto &node : levelNodes)
            {
                outfile << node << " ";
            }
            outfile << std::endl;
        }
        else
        {
            // if all are null stop
            break;
        }
    }

    outfile.close();
}



void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    
    if (!root)
    {
        return;
    }

    // Gather all Isles into a vector
    std::vector<Isle *> isles;
    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        isles.push_back(current->isle);

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }

    // Sort Isles alphabetically by their names
    std::sort(isles.begin(), isles.end(), [](Isle *a, Isle *b) {
        return a->getName() < b->getName(); 
    });

    // Write sorted Isles to file
    std::ofstream outfile(filename);
    for (Isle *isle : isles)
    {
        outfile << isle->getName() << std::endl; // Write the isle's name
    }

    outfile.close();
    std::cout << "[Output] " << "Isles have been written to " << filename << " in alphabetical order." << std::endl;
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}