#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    ifstream file(filename); 

    if (!file.is_open()) {
        return;
    }

    
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        unsigned long score;
        time_t timestamp;
        string player_name;
        iss>>score>>timestamp>>player_name;
        

        // create and insert new entry
        LeaderboardEntry* new_entry = new LeaderboardEntry(score, timestamp, player_name);
        insert(new_entry);
    }

    file.close(); 

}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
    
    ofstream file(filename);     
    LeaderboardEntry* current = head_leaderboard_entry;

    while (current != nullptr) {
        file << current->score << " "<< current->last_played << " "<< current->player_name << "\n";
        current = current->next;
    }

    file.close(); 
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here
    cout<<"Leaderboard\n-----------"<<endl;

    LeaderboardEntry* current = head_leaderboard_entry;
    int order = 1;

    while (current != nullptr && order <= 10) {
        
        time_t time = current->last_played;
        tm* tm_ptr = localtime(&time);

        // formatting
        cout << order << ". " << current->player_name << " " << current->score << " " << put_time(tm_ptr, "%H:%M:%S/%d.%m.%Y") << endl;

        
        current = current->next;
        order++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // Eğer liste boşsa, yeni girdiyi başa ekleyin
    if (head_leaderboard_entry == nullptr || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        // Aksi halde, uygun konumu bul ve ekle
        LeaderboardEntry* current = head_leaderboard_entry;
        while (current->next != nullptr && current->next->score >= new_entry->score) {
            current = current->next;
        }
        new_entry->next = current->next;
        current->next = new_entry;
    }

    // Listenin boyutu 10'u geçiyorsa, en düşük skoru silin
    int count = 0;
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (temp != nullptr) {
        count++;
        if (count == 10 && temp->next != nullptr) {
            // Listenin 10. elemanına ulaştık, sıradaki elemanı sil
            LeaderboardEntry* to_delete = temp->next;
            temp->next = nullptr;
            delete to_delete;
            break;
        }
        temp = temp->next;
    }
}


// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
    LeaderboardEntry *temp=head_leaderboard_entry;
    while (temp!=nullptr)
    {
        LeaderboardEntry *q=temp;
        temp=temp->next;
        delete q;
    }

    
}
