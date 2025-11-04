#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "game.h"

using namespace std;

struct Candidate {
    char op;
    int total_score;
    int immediate_reward;
    
    bool operator<(const Candidate& other) const {
        return total_score < other.total_score;
    }
};

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
    
    const int LOOKAHEAD_DEPTH = 4;
    
    while (game->bricksRemaining() > 0 && (int)operations.size() < max_ops) {
        priority_queue<Candidate> candidates;
        
        // Evaluate all possible sequences of length LOOKAHEAD_DEPTH
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
                
                // Keep only top 3 sequences to avoid explosion
                if (new_sequences.size() > 3) {
                    vector<pair<int, vector<char>>> scored;
                    for (auto& seq : new_sequences) {
                        int score = evaluatePath(game, seq);
                        scored.push_back({score, seq});
                    }
                    sort(scored.rbegin(), scored.rend());
                    
                    sequences.clear();
                    for (int i = 0; i < 3 && i < (int)scored.size(); i++) {
                        sequences.push_back(scored[i].second);
                    }
                } else {
                    sequences = new_sequences;
                }
            }
            
            // Evaluate the best sequence for this first op
            int best_score = 0;
            for (auto& seq : sequences) {
                int score = evaluatePath(game, seq);
                best_score = max(best_score, score);
            }
            
            Candidate c;
            c.op = op1;
            c.total_score = best_score;
            c.immediate_reward = evaluatePath(game, {op1});
            candidates.push(c);
        }
        
        // Execute the best first operation
        char best_op = candidates.top().op;
        game->play(best_op);
        operations.push_back(best_op);
    }
    
    for (char op : operations) {
        cout << op << endl;
    }
    
    delete game;
    return 0;
}
