#include "AsteroidDash.h"


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
    
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here
    ifstream infile;
    infile.open(input_file);
    string line;
    while (getline(infile,line))
    {
        vector<int> r;
        
        for (char c : line)
        {
            if (c=='0' || c=='1')
            {
                int value = c - '0';
                r.push_back(value);
            }
            
        }
        
        space_grid.push_back(r);
    }
    infile.close();
}


// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here
    vector<vector<bool>> spacecraft_shape;
    ifstream infile;
    infile.open(player_file_name);
    string line;
    getline(infile,line);
    int start_row;
    int start_col;
    istringstream iss(line);
    iss >> start_row >> start_col;
    while (getline(infile,line))
    {
        vector<bool> r;
        bool value;
        for (char c : line)
        {
            if (c=='0' || c=='1')
            {
                value = (c == '1');
                r.push_back(value);
            }
            
        }
        spacecraft_shape.push_back(r);
    }
    infile.close();
    player=new Player(spacecraft_shape,start_row,start_col,player_name);
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here
    vector<vector<bool>> shape;
    ifstream infile;
    infile.open(input_file);
    string line;
    int start_row=0;
    int time_of_appearance=0;
    bool is_reading_shape = false;
    ObjectType object_type = ASTEROID;
    while (getline(infile,line))
    {
        if (line.find('[')==0)
        {
            shape.clear();
            object_type = ASTEROID;
            is_reading_shape = true;
        }
        if (line.find('{')==0)
        {
            shape.clear();
            object_type = LIFE_UP;
            is_reading_shape = true;
        }
        if (is_reading_shape)
        {
            if (line.find(']') == line.length()-1 || line.find('}') == line.length()-1) 
            {
                is_reading_shape = false;
            }
            
            vector<bool> r;
            for (char c : line)
            {
                if (c=='0' || c=='1')
                {
                    bool value=(c == '1');
                    r.push_back(value);
                }
            }
            shape.push_back(r);    
            

        }

        else if(line.find('s') == 0 || line.find('t') == 0 || line.find('e') == 0){
            if (line.find('s') == 0) {
                start_row = stoi(line.substr(2)); 
            } 
            else if (line.find('t') == 0) {
                time_of_appearance = stoi(line.substr(2)); 
            } 
            else if (line.find('e') == 0) {
                string effect = line.substr(2);
                if (effect == "life") {
                    object_type = LIFE_UP;
                } else if (effect == "ammo") {
                    object_type = AMMO;
                }
            }
        }
        else if (!is_reading_shape && !shape.empty()){
            CelestialObject* new_object = new CelestialObject(shape, object_type, start_row, time_of_appearance);
            int count=0;
        for (int i = 0; i < new_object->shape.size(); i++)
        {
            for (int j = 0; j < new_object->shape[0].size(); j++)
            {
                if (new_object->shape[i][j]==1)
                {
                    count++;
                }
                
            }
        }
        new_object->original_size=count;
            new_object->handleRotations();
            if (celestial_objects_list_head == nullptr) {
                celestial_objects_list_head = new_object;
            } else {
                CelestialObject* cur = celestial_objects_list_head;
                while (cur->next_celestial_object != nullptr) {
                    cur = cur->next_celestial_object;
                }
                cur->next_celestial_object = new_object;
            }
            shape.clear();
            
        }
    }

    if (!shape.empty())
    {
        CelestialObject* new_object = new CelestialObject(shape, object_type, start_row, time_of_appearance); 
        int count=0;
        for (int i = 0; i < new_object->shape.size(); i++)
        {
            for (int j = 0; j < new_object->shape[0].size(); j++)
            {
                if (new_object->shape[i][j]==1)
                {
                    count++;
                }
                
            }
        }
        new_object->original_size=count;
        




        new_object->handleRotations();  
        if (celestial_objects_list_head == nullptr) {
            celestial_objects_list_head = new_object;
        }
        else{
            CelestialObject* cur = celestial_objects_list_head;
        while (cur->next_celestial_object != nullptr) {
            cur = cur->next_celestial_object;
        }
        cur->next_celestial_object = new_object;
        }
        
        
        CelestialObject* c = celestial_objects_list_head;
        while (c->next_celestial_object != nullptr) {
            if (c->right_rotation == c) {
                
            }
            else{
                CelestialObject *p = c->right_rotation;

            while (p != c) {
                CelestialObject *q = p;
                p=p->right_rotation;
                q->next_celestial_object=c->next_celestial_object;
            }
            }
            c = c->next_celestial_object;
        }
    }
    
    
    

    

        
    infile.close();
}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
    if (!game_over)
    {
        cout << "Tick: " << game_time << endl;
        cout << "Lives: " << player->lives << endl;
        cout << "Ammo: " << player->current_ammo << endl;
        cout << "Score: " << current_score << endl;
        cout << "High Score: 465" << endl;
    }
    
    
    for (int i = 0; i < space_grid.size(); i++)
    {
        for (int j = 0; j < space_grid[0].size(); j++)
        {
            if (space_grid[i][j]==0)
            {
                cout<<unoccupiedCellChar;
            }
            else{
                cout<<occupiedCellChar;
            }
            
        }
        cout<<endl;
    }
    cout<<endl;
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    
    std::vector<std::vector<int>> new_grid(space_grid.size(), std::vector<int>(space_grid[0].size(), 0));

    // Mermileri sağa hareket ettir ve yeni pozisyonlarını güncelle
    for (int k = 0; k < bullet_positions.size();) {
        int i = bullet_positions[k].first;
        int j = bullet_positions[k].second;

        bool bullet_collided = false; // Merminin çarpışıp çarpmadığını izlemek için

        // Göksel objelerle çarpışmayı kontrol et
        CelestialObject* current_object = celestial_objects_list_head;
        CelestialObject* prev = nullptr;

        while (current_object) {
            int time_since_appearance = game_time - current_object->time_of_appearance;
            int start_col = space_grid[0].size() - 1 - time_since_appearance;

            int middle_row = current_object->shape.size() / 2;

            // Göksel objenin `shape` matrisinde çarpışmayı kontrol et
            for (int obj_i = 0; obj_i < current_object->shape.size(); obj_i++) {
                for (int obj_j = 0; obj_j < current_object->shape[0].size(); obj_j++) {
                    int grid_row = current_object->starting_row + obj_i;
                    int grid_col = start_col + obj_j;

                    // Eğer mermi, göksel objenin dolu hücresine (1) çarparsa
                    if ((grid_row == i && grid_col == j || grid_row == i && grid_col == j + 1) 
                        && current_object->shape[obj_i][obj_j] == 1) {
                        
                        if (current_object->object_type == ASTEROID) {
                            // Çarpışma gerçekleşti, göksel objenin hücresini 0 yap
                            
                            current_object->shape[obj_i][obj_j] = 0;
                            current_object->delete_rotations(current_object);
                            current_object->handleRotations();
                            bullet_collided = true;
                            current_score+=10;
                            bool destroyed=true;
                            for (int a = 0; a < current_object->shape.size(); a++)
                            {
                                for (int b = 0; b < current_object->shape[0].size(); b++)
                                {
                                    if (current_object->shape[a][b]==1)
                                    {
                                        destroyed=false;
                                        break;
                                    }
                                    
                                }
                                if (!destroyed) break;
                            }
                            if (destroyed)
                            {
                                current_score+=100 * current_object->original_size;
                            }
                            
                            

                            // Çarpışma sonucunda dönüşleri güncelle
                            if (current_object->shape.size() % 2 == 1) {
                                if (obj_i < middle_row) {
                                    if (prev == nullptr)
                                        celestial_objects_list_head = current_object->right_rotation;
                                    else
                                        prev->next_celestial_object = current_object->right_rotation;
                                } else if(obj_i > middle_row){
                                    if (prev == nullptr)
                                        celestial_objects_list_head = current_object->left_rotation;
                                    else
                                        prev->next_celestial_object = current_object->left_rotation;
                                }
                            } else {
                                if (obj_i <= middle_row) {
                                    if (prev == nullptr)
                                        celestial_objects_list_head = current_object->right_rotation;
                                    else
                                        prev->next_celestial_object = current_object->right_rotation;
                                } else if(obj_i > middle_row) {
                                    if (prev == nullptr)
                                        celestial_objects_list_head = current_object->left_rotation;
                                    else
                                        prev->next_celestial_object = current_object->left_rotation;
                                }
                            }
                            break;
                        }
                    }
                }
                if (bullet_collided) break;
            }

            if (bullet_collided) break;
            prev = current_object;
            current_object = current_object->next_celestial_object;
        }

        if (bullet_collided) {
            bullet_positions.erase(bullet_positions.begin() + k);
        } else {
            if (j + 1 < space_grid[0].size()) {
                new_grid[i][j + 1] = 1;
                bullet_positions[k].second = j + 1;
                k++;
            } else {
                bullet_positions.erase(bullet_positions.begin() + k);
            }
        }
    }


    // Oyuncunun gemisini griddeki yerine yerleştir
    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[0].size(); j++) {
            if (player->spacecraft_shape[i][j]) {
                new_grid[i + player->position_row][j + player->position_col] = 1; // Gemi hücresini 1 yap
            }
        }
    }

    // Göksel objeleri güncelle, new_grid'e yerleştir ve oyuncuya çarpışmayı kontrol et
    CelestialObject* current_object = celestial_objects_list_head;
    CelestialObject* prev_object = nullptr; // Önceki objeyi takip etmek için
    while (current_object) {
        
        int time_since_appearance = game_time - current_object->time_of_appearance;
        int start_col = space_grid[0].size() - 1 - time_since_appearance;

        bool collision = false; // Çarpışma kontrolü için bir bayrak
        
        // Göksel objenin şeklini new_grid'e kopyala ve çarpışmayı kontrol et
        for (int i = 0; i < current_object->shape.size(); i++) {
            for (int j = 0; j < current_object->shape[0].size(); j++) {
                int grid_row = current_object->starting_row + i;
                int grid_col = start_col + j;

                // Sadece göksel objenin dolu ('1') hücrelerinde çarpışmayı kontrol et
                if (current_object->shape[i][j] == 1 && // Göksel objenin dolu hücresi
                    grid_row < new_grid.size() && grid_col < new_grid[0].size() && grid_col >= 0) {
                    
                    // Çarpışmayı kontrol et
                    if (grid_row >= player->position_row && grid_row < player->position_row + player->spacecraft_shape.size() &&
                        grid_col >= player->position_col && grid_col < player->position_col + player->spacecraft_shape[0].size() &&
                        player->spacecraft_shape[grid_row - player->position_row][grid_col - player->position_col] == 1) { 
                        
                        // Çarpışma gerçekleşti, objenin türüne göre işlem yap
                        collision = true;
                        if (current_object->object_type == ASTEROID) {
                            // ASTEROID çarptığında oyuncunun canını azalt
                            player->lives--;
                            
                        } else if (current_object->object_type == LIFE_UP) {
                            // LIFE_UP çarptığında can artır
                            player->lives++;
                            
                        } else if (current_object->object_type == AMMO) {
                            // AMMO çarptığında cephane artır
                            player->current_ammo=player->max_ammo;
                            
                        }

                        break; // Çarpışma olduğunda döngüden çık
                    } else {
                        // Göksel objeyi grid'e yerleştir
                        new_grid[grid_row][grid_col] = 1;
                    }
                }
            }
            if (collision) break; // Çarpışma varsa dış döngüden de çık
        }

        if (collision) {
            // Eğer canımız varsa objeyi listeden çıkar
            if (player->lives > 0) {
                if (prev_object) {
                    prev_object->next_celestial_object = current_object->next_celestial_object;
                } else {
                    celestial_objects_list_head = current_object->next_celestial_object;
                }
                
                current_object = current_object->next_celestial_object;
                
                continue; // Döngünün sonraki objesine geç
            } else {
                game_over=true; // Oyunu sonlandır
                if (prev_object) {
                    prev_object->next_celestial_object = current_object->next_celestial_object;
                } else {
                    celestial_objects_list_head = current_object->next_celestial_object;
                }
            }
        }

        // Eğer çarpışma olmadıysa bir sonraki objeye geç
        prev_object = current_object;
        current_object = current_object->next_celestial_object;
    }

    // Sağ kenara ulaşan mermileri bullet_positions listesinden kaldır
    for (int k = 0; k < bullet_positions.size(); ) {
        if (bullet_positions[k].second >= space_grid[0].size() - 1) {
            bullet_positions.erase(bullet_positions.begin() + k);  // Kenara ulaşan mermiyi sil
        } else {
            k++;  // Sıradaki mermiye geç
        }
    }

    // Geçici grid'i asıl grid'e atayarak güncellemeyi tamamlıyoruz
    space_grid = new_grid;
    
    

}



// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // Check if player has enough ammo
    
if (player->current_ammo <= 0) {
        return; // Not enough current_ammo to shoot
    }

    // Identify the middle row of the spacecraft's rightmost column
    int middle_row = player->position_row + (player->spacecraft_shape.size() / 2);
    int shoot_col = player->position_col + player->spacecraft_shape[0].size()-1;

    // Ensure that the projectile's starting position is within the grid bounds
    if (middle_row >= 0 && middle_row < space_grid.size() && 
        shoot_col >= 0 && shoot_col < space_grid[0].size()) {
        
        // Place the projectile (assuming a projectile is represented by '1' on the grid)
        bullet_positions.push_back({middle_row, shoot_col});
        
        // Decrease the player's current_ammo
        player->current_ammo--;
    }
}


// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here
    delete celestial_objects_list_head;
    delete player;
}