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
    
    // Adaptive strategy based on problem size
    int lookahead = (n <= 15) ? 3 : 2;
    
    while (game->bricksRemaining() > 0 && (int)operations.size() < max_ops) {
        char best_op = 'C';
        int best_value = -1000000;
        
        // Try all 5 operations
        for (char op1 : {'A', 'B', 'C', 'D', 'E'}) {
            auto save1 = game->save();
            int r1 = game->play(op1);
            
            int value = r1 * 100; // Weight immediate reward heavily
            
            // Lookahead level 2
            if (lookahead >= 2 && game->bricksRemaining() > 0) {
                int best_r2 = 0;
                for (char op2 : {'A', 'B', 'C', 'D', 'E'}) {
                    auto save2 = game->save();
                    int r2 = game->play(op2);
                    
                    // Lookahead level 3 only for small problems
                    if (lookahead >= 3 && game->bricksRemaining() > 0) {
                        int best_r3 = 0;
                        for (char op3 : {'A', 'B', 'C', 'D', 'E'}) {
                            auto save3 = game->save();
                            int r3 = game->play(op3);
                            best_r3 = max(best_r3, r3);
                            game->load(save3);
                            game->erase(save3);
                        }
                        r2 += best_r3 / 3;
                    }
                    
                    best_r2 = max(best_r2, r2);
                    game->load(save2);
                    game->erase(save2);
                }
                value += best_r2 * 10;
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
