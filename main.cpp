#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "game.h"

using namespace std;

struct Candidate {
    char op;
    int reward;
    int total_bricks_hit;
    
    bool operator<(const Candidate& other) const {
        // Higher reward is better
        if (reward != other.reward) return reward < other.reward;
        // More bricks hit is better
        return total_bricks_hit < other.total_bricks_hit;
    }
};

int main() {
    Game *game = new Game(cin);
    
    vector<char> operations;
    int max_ops = game->m;
    
    // Multi-step lookahead with priority on reward (includes bonuses)
    while (game->bricksRemaining() > 0 && (int)operations.size() < max_ops) {
        priority_queue<Candidate> candidates;
        
        // For each operation, simulate with 3-step lookahead
        for (char op1 : {'A', 'B', 'C', 'D', 'E'}) {
            auto save1 = game->save();
            int reward1 = game->play(op1);
            int bricks1 = game->bricksHit();
            
            // 2-step lookahead
            int best_reward2 = 0;
            for (char op2 : {'A', 'B', 'C', 'D', 'E'}) {
                auto save2 = game->save();
                int reward2 = game->play(op2);
                
                // 3-step lookahead
                int best_reward3 = 0;
                for (char op3 : {'A', 'B', 'C', 'D', 'E'}) {
                    auto save3 = game->save();
                    int reward3 = game->play(op3);
                    best_reward3 = max(best_reward3, reward3);
                    game->load(save3);
                    game->erase(save3);
                }
                
                best_reward2 = max(best_reward2, reward2 + best_reward3 / 2);
                game->load(save2);
                game->erase(save2);
            }
            
            Candidate c;
            c.op = op1;
            c.reward = reward1 * 10 + best_reward2;
            c.total_bricks_hit = bricks1;
            candidates.push(c);
            
            game->load(save1);
            game->erase(save1);
        }
        
        // Execute the best operation
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
