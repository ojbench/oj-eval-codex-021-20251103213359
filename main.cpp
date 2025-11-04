#include <iostream>
#include <vector>
#include <algorithm>
#include "game.h"

using namespace std;

int main() {
    Game *game = new Game(cin);
    
    vector<char> operations;
    int max_ops = game->m;
    int n = game->n;
    
    // Adaptive lookahead
    int lookahead = (n <= 15) ? 3 : 2;
    
    while (game->bricksRemaining() > 0 && (int)operations.size() < max_ops) {
        char best_op = 'C';
        int best_value = -1000000;
        
        for (char op1 : {'A', 'B', 'C', 'D', 'E'}) {
            auto save1 = game->save();
            int r1 = game->play(op1);
            
            // Score heavily weights reward (which includes combo bonuses)
            int value = r1 * 1000;
            
            // Lookahead
            if (lookahead >= 2 && game->bricksRemaining() > 0) {
                int best_r2 = 0;
                for (char op2 : {'A', 'B', 'C', 'D', 'E'}) {
                    auto save2 = game->save();
                    int r2 = game->play(op2);
                    
                    if (lookahead >= 3 && game->bricksRemaining() > 0) {
                        int best_r3 = 0;
                        for (char op3 : {'A', 'B', 'C', 'D', 'E'}) {
                            auto save3 = game->save();
                            int r3 = game->play(op3);
                            best_r3 = max(best_r3, r3);
                            game->load(save3);
                            game->erase(save3);
                        }
                        r2 += best_r3 / 2;
                    }
                    
                    best_r2 = max(best_r2, r2);
                    game->load(save2);
                    game->erase(save2);
                }
                value += best_r2 * 100;
            }
            
            if (value > best_value) {
                best_value = value;
                best_op = op1;
            }
            
            game->load(save1);
            game->erase(save1);
        }
        
        game->play(best_op);
        operations.push_back(best_op);
    }
    
    for (char op : operations) {
        cout << op << endl;
    }
    
    delete game;
    return 0;
}
