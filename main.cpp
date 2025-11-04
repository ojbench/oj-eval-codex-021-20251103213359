#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include "game.h"

using namespace std;

int main() {
    Game *game = new Game(cin);
    
    vector<char> operations;
    int max_ops = game->m;
    int n = game->n;
    
    // Greedy strategy with lookahead
    while (game->bricksRemaining() > 0 && (int)operations.size() < max_ops) {
        char best_op = 'C';
        int best_reward = -1;
        
        // Try all 5 operations
        for (char op : {'A', 'B', 'C', 'D', 'E'}) {
            auto save_state = game->save();
            int reward = game->play(op);
            
            // Choose operation with highest immediate reward
            if (reward > best_reward) {
                best_reward = reward;
                best_op = op;
            }
            
            game->load(save_state);
            game->erase(save_state);
        }
        
        // Execute the best operation
        game->play(best_op);
        operations.push_back(best_op);
    }
    
    // Output all operations
    for (char op : operations) {
        cout << op << endl;
    }
    
    delete game;
    return 0;
}
