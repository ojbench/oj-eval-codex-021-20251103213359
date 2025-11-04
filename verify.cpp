#include <iostream>
#include <fstream>
#include <vector>
#include "game.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: verify <input_file> <output_file>" << endl;
        return 1;
    }
    
    ifstream infile(argv[1]);
    ifstream opfile(argv[2]);
    
    Game *game = new Game(infile);
    
    int total_reward = 0;
    string line;
    int op_count = 0;
    
    while (getline(opfile, line)) {
        if (line.empty()) continue;
        char op = line[0];
        int reward = game->play(op);
        total_reward += reward;
        op_count++;
    }
    
    cout << "Operations: " << op_count << endl;
    cout << "Total reward: " << total_reward << endl;
    cout << "Bricks hit: " << game->bricksHit() << endl;
    cout << "Bricks remaining: " << game->bricksRemaining() << endl;
    
    delete game;
    return 0;
}
