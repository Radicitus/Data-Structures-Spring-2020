#include "OthelloAI.hpp"

namespace crsherry {
    class CCOthelloAI : public OthelloAI {
        public:
            std::pair<int, int> chooseMove(const OthelloGameState& state) override;

        private:
            int minimax(int depth, int max_depth, std::pair<int, int>& location, 
                        bool maximizingPlayer, bool isBlack, std::unique_ptr<OthelloGameState>& state, 
                        int alpha, int beta);
            
            int evaluate(std::pair<int, int>& location, std::unique_ptr<OthelloGameState>& sandbox, bool isBlack);
    
            std::vector<std::pair<int, int>> validMoves(const OthelloGameState& state);

            std::vector<std::pair<int, int>> validMoves(const std::unique_ptr<OthelloGameState>& sandbox);
    };
}