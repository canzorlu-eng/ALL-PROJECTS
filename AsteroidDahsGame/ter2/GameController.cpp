#include "GameController.h"

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
    
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here
    ifstream infile;
    infile.open(commands_file);
    string line;
    //getline(infile,line);
    
    while (getline(infile,line))
    {

        if (game->game_over)
        {
            for (int i = 0; i <game->player->spacecraft_shape.size() ; i++)
            {
                for (int j = 0; j <game->player->spacecraft_shape[0].size() ; j++)
                {
                    game->player->spacecraft_shape[i][j]=false;
                }
            }
            game->update_space_grid();
            
            break;
        }
        
        else if (line == "MOVE_DOWN") {
            game->player->move_down(game->space_grid[0].size());
        } else if (line == "MOVE_UP") {
            game->player->move_up();
        } else if (line == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid.size());
        } else if (line == "MOVE_LEFT") {
            game->player->move_left();
        }else if (line == "SHOOT") {
            game->shoot();
        }
        else if (line == "NOP") {}
        else if(line=="PRINT_GRID"){
            game->update_space_grid();
            
            
            game->print_space_grid();game->game_time++;if (!(game->c))
                game->current_score++;}
        else{cout<<"Unknown command"<<endl;}

        if (line!="PRINT_GRID")
        {
            game->update_space_grid();
            game->game_time++;
            if (!(game->c))
                game->current_score++;
        }
        
    }

    LeaderboardEntry *new_entry=new LeaderboardEntry(game->current_score,0,game->player->player_name);
    game->leaderboard.insert(new_entry);
    if (game->game_over==true)
    {
        cout<<"GAME OVER!"<<endl;
        cout << "Tick: " << game->game_time -1<< endl;
        cout << "Lives: " << game->player->lives << endl;
        cout << "Ammo: " << game->player->current_ammo << endl;
        cout << "Score: " << game->current_score << endl;
        cout << "High Score: " <<game->leaderboard.head_leaderboard_entry->score<< endl;
        cout << "Player: " << game->player->player_name << endl;
        game->print_space_grid();
        game->leaderboard.print_leaderboard();
        //cout<<"Leaderboard\n-----------";
    }else{
        game->game_over=true;
        game->update_space_grid();
        cout << "GAME FINISHED! No more commands!" << endl;
        cout << "Tick: " << game->game_time -1<< endl;
        cout << "Lives: " << game->player->lives << endl;
        cout << "Ammo: " << game->player->current_ammo << endl;
        cout << "Score: " << game->current_score<< endl;
        cout << "High Score: " <<game->leaderboard.head_leaderboard_entry->score<< endl;
        cout << "Player: " << game->player->player_name << endl;
        
        game->print_space_grid();
         game->leaderboard.print_leaderboard();
        //cout<<"Leaderboard\n-----------";
   
    }
    infile.close();
    
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
    delete game;
}
