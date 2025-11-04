#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "game.h"

using namespace std;

int evaluatePath(Game* game, vector<char> ops) {
    auto save = game->save();
    int total_reward = 0;
    for (char op : ops) {
        total_reward += game->play(op);
    }
    game->load(save);
    game->erase(save);
    return total_reward;
}

int main() {
    Game *game = new Game(cin);
    
    vector<char> operations;
    int max_ops = game->m;
    int n = game->n;
    
    // Use deeper lookahead for small instances, shallow for large
    if (n <= 11) {
        // Beam search for small instances
        const int LOOKAHEAD_DEPTH = 4;
        
        while (game->bricksRemaining() > 0 && (int)operations.size() < max_ops) {
            char best_op = 'C';
            int best_total_score = -1;
            
            for (char op1 : {'A', 'B', 'C', 'D', 'E'}) {
                vector<vector<char>> sequences;
                sequences.push_back({op1});
                
                for (int depth = 1; depth < LOOKAHEAD_DEPTH; depth++) {
                    vector<vector<char>> new_sequences;
                    
                    for (auto& seq : sequences) {
                        for (char next_op : {'A', 'B', 'C', 'D', 'E'}) {
                            vector<char> new_seq = seq;
                            new_seq.push_back(next_op);
                            new_sequences.push_back(new_seq);
                        }
                    }
                    
                    // Keep only top 3 sequences
                    if (new_sequences.size() > 3) {
                        vector<pair<int, vector<char>>> scored;
                        for (auto& seq : new_sequences) {
                            int score = evaluatePath(game, seq);
                            scored.push_back({score, seq});
                        }
                        sort(scored.rbegin(), scored.rend());
                        
                        sequences.clear();
                        for (int i = 0; i < 3; i++) {
                            sequences.push_back(scored[i].second);
                        }
                    } else {
                        sequences = new_sequences;
                    }
                }
                
                // Find best score
                int score = 0;
                for (auto& seq : sequences) {
                    score = max(score, evaluatePath(game, seq));
                }
                
                if (score > best_total_score) {
                    best_total_score = score;
                    best_op = op1;
                }
            }
            
            game->play(best_op);
            operations.push_back(best_op);
        }
    } else {
        // Fast greedy for large instances
        while (game->bricksRemaining() > 0 && (int)operations.size() < max_ops) {
            char best_op = 'C';
            int best_value = -1;
            
            for (char op1 : {'A', 'B', 'C', 'D', 'E'}) {
                auto save1 = game->save();
                int r1 = game->play(op1);
                int value = r1 * 1000;
                
                // 2-step lookahead
                if (game->bricksRemaining() > 0) {
                    int best_r2 = 0;
                    for (char op2 : {'A', 'B', 'C', 'D', 'E'}) {
                        auto save2 = game->save();
                        int r2 = game->play(op2);
                        
                        // 3-step lookahead
                        if (game->bricksRemaining() > 0) {
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
    }
    
    for (char op : operations) {
        cout << op << endl;
    }
    
    delete game;
    return 0;
}
