#include <iostream>
#include <vector>
#include "game.h"

using namespace std;

int main() {
    Game *game = new Game(cin);
    
    cout << "Total bricks: " << game->bricksTotal() << endl;
    cout << "Initial bricks remaining: " << game->bricksRemaining() << endl;
    
    vector<char> ops = {'A', 'E', 'C', 'B', 'A'};
    
    for (int i = 0; i < 5; i++) {
        int reward = game->play(ops[i]);
        cout << "Op " << ops[i] << ": reward=" << reward 
             << " hit=" << game->bricksHit() 
             << " remaining=" << game->bricksRemaining() << endl;
    }
    
    delete game;
    return 0;
}
