#include <ics46/factory/DynamicFactory.hpp>
#include "MyOthelloAI.hpp"

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, crsherry::CCOthelloAI, "CamCam's Othello AI (Required)");

std::pair<int, int> crsherry::CCOthelloAI::chooseMove(const OthelloGameState& state) {
    int index_to_return = 0;
    int max_score = 0;

    std::unique_ptr<OthelloGameState> sandbox = state.clone();

    //Store x,y indexes for all valid moves for this gamestate in std::pairs
    std::vector<std::pair<int, int>> moves = validMoves(state);

    bool isMyTurn = moves.size() != 0;
    bool isBlack;
    if (isMyTurn) {
        if (state.isBlackTurn()) {
            isBlack = true;
        } else {
            isBlack = false;
        }
    }

    if (!isMyTurn) {
        if (state.isBlackTurn()) {
            isBlack = false;
        } else {
            isBlack = true;
        }
    }

    // int max_depth = moves.size();
    int max_depth = 5;

    for (int i = 0; i < moves.size(); ++i) {
        int move_eval = minimax(0, max_depth, moves.at(i), isMyTurn, isBlack, sandbox, -1000, 1000);
        if (move_eval > max_score) {
            index_to_return = i;
            max_score = move_eval;
        }
    }
    
    return moves.at(index_to_return);
}

//minimax
int crsherry::CCOthelloAI::minimax(int depth, int max_depth, std::pair<int, int>& location, bool maximizingPlayer, bool isBlack, std::unique_ptr<OthelloGameState>& state, int alpha, int beta) {
    
    if (depth == max_depth) {
        return evaluate(location, state, isBlack);
    }

    if (maximizingPlayer) {
        int best_val = -1000;

        std::unique_ptr<OthelloGameState> sandbox = state->clone();
        sandbox->makeMove(location.first, location.second);

        std::vector<std::pair<int, int>> moves = validMoves(sandbox);

        for (int i = 0; i < moves.size(); ++i)
        {
            int val = minimax(depth + 1, max_depth, moves.at(i), false, isBlack, sandbox, alpha, beta);

            best_val = std::max(best_val, val);
            alpha = std::max(alpha, best_val);

            if (beta <= alpha) {
                break;
            }
        }
        
        return best_val;
    } else {
        int best_val = 1000;

        std::unique_ptr<OthelloGameState> sandbox = state->clone();
        sandbox->makeMove(location.first, location.second);

        std::vector<std::pair<int, int>> moves = validMoves(sandbox);

        for (int i = 0; i < moves.size(); ++i)
        {
            int val = minimax(depth + 1, max_depth, moves.at(i), true, isBlack, sandbox, alpha, beta);

            best_val = std::min(best_val, val);
            alpha = std::min(alpha, best_val);

            if (beta <= alpha) {
                break;
            }
        }
        
        return best_val;
    }
}


//evaluate
int crsherry::CCOthelloAI::evaluate(std::pair<int, int>& location, std::unique_ptr<OthelloGameState>& sandbox, bool isBlack) {
    int y = sandbox->board().height();
    int x = sandbox->board().width();

    if (isBlack) {
        int base_score = sandbox->blackScore() - sandbox->whiteScore();
        int total_count = 0;
        //bottom row
        if (location.second < y - 1) {
            //top row
            if (location.second > 0) {
                //right col
                if (location.first < x - 1) {
                    //left col
                    if (location.first > 0) {
                        //middle top
                        if (sandbox->board().cellAt(location.first, location.second - 1) == OthelloCell::white) {
                            int count = 0;
                            for (int i = location.second - 1; i > 0; i--)
                            {
                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                        //middle top left
                        if (sandbox->board().cellAt(location.first - 1, location.second - 1) == OthelloCell::white) {
                            int count = 0;
                            int i = location.second - 1;
                            int j = location.first - 1;

                            while (i > 0 && j > 0)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i--;
                                j--;
                            }
                        }
                        //middle top right
                        if (sandbox->board().cellAt(location.first + 1, location.second - 1) == OthelloCell::white) {
                            int count = 0;
                            int i = location.second - 1;
                            int j = location.first + 1;

                            while (i > 0 && j < x)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i--;
                                j++;
                            }
                        }
                        //middle bottom
                        if (sandbox->board().cellAt(location.first, location.second + 1) == OthelloCell::white) {
                            int count = 0;
                            for (int i = location.second + 1; i < y; i++)
                            {
                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                        //middle bottom left
                        if (sandbox->board().cellAt(location.first - 1, location.second + 1) == OthelloCell::white) {
                            int count = 0;
                            int i = location.second + 1;
                            int j = location.first - 1;

                            while (i < y && j > 0)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i++;
                                j--;
                            }
                        }
                        //middle bottom right
                        if (sandbox->board().cellAt(location.first + 1, location.second + 1) == OthelloCell::white) {
                            int count = 0;
                            int i = location.second + 1;
                            int j = location.first + 1;

                            while (i < y && j < x)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i++;
                                j++;
                            }
                        }
                        //middle left
                        if (sandbox->board().cellAt(location.first - 1, location.second) == OthelloCell::white) {
                            int count = 0;
                            for (int i = location.first - 1; i > 0; i--)
                            {
                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                        //middle right
                        if (sandbox->board().cellAt(location.first + 1, location.second) == OthelloCell::white) {
                            int count = 0;
                            for (int i = location.first + 1; i < x; i++)
                            {
                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::black)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                        return base_score + total_count;
                    }                   
                    //middle top
                    if (sandbox->board().cellAt(location.first, location.second - 1) == OthelloCell::white) {
                        int count = 0;
                        for (int i = location.second - 1; i > 0; i--)
                        {
                            if (sandbox->board().cellAt(location.first, i) == OthelloCell::black)
                            {
                                total_count += count;
                                break;
                            }

                            if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                            {
                                break;
                            }
                            count++;
                        }
                    }
                    //middle top right
                    if (sandbox->board().cellAt(location.first + 1, location.second - 1) == OthelloCell::white) {
                        int count = 0;
                        int i = location.second - 1;
                        int j = location.first + 1;

                        while (i > 0 && j < x)
                        {
                            if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                            {
                                total_count += count;
                                break;
                            }

                            if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                            {
                                break;
                            }
                            count++;
                            i--;
                            j++;
                        }
                    }
                    //middle bottom
                    if (sandbox->board().cellAt(location.first, location.second + 1) == OthelloCell::white) {
                        int count = 0;
                        for (int i = location.second + 1; i < y; i++)
                        {
                            if (sandbox->board().cellAt(location.first, i) == OthelloCell::black)
                            {
                                total_count += count;
                                break;
                            }

                            if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                            {
                                break;
                            }
                            count++;
                        }
                    }
                    //middle bottom right
                    if (sandbox->board().cellAt(location.first + 1, location.second + 1) == OthelloCell::white) {
                        int count = 0;
                        int i = location.second + 1;
                        int j = location.first + 1;

                        while (i < y && j < x)
                        {
                            if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                            {
                                total_count += count;
                                break;
                            }

                            if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                            {
                                break;
                            }
                            count++;
                            i++;
                            j++;
                        }
                    }
                    //middle right
                    if (sandbox->board().cellAt(location.first + 1, location.second) == OthelloCell::white) {
                        int count = 0;
                        for (int i = location.first + 1; i < x; i++)
                        {
                            if (sandbox->board().cellAt(i, location.second) == OthelloCell::black)
                            {
                                total_count += count;
                                break;
                            }

                            if (sandbox->board().cellAt(i, location.second) == OthelloCell::empty)
                            {
                                break;
                            }
                            count++;
                        }
                    }
                    return base_score + total_count;
                }
                //middle top
                if (sandbox->board().cellAt(location.first, location.second - 1) == OthelloCell::white) {
                    int count = 0;
                    for (int i = location.second - 1; i > 0; i--)
                    {
                        if (sandbox->board().cellAt(location.first, i) == OthelloCell::black)
                        {
                            total_count += count;
                            break;
                        }

                        if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                        {
                            break;
                        }
                        count++;
                    }
                }   
                //middle bottom
                if (sandbox->board().cellAt(location.first, location.second + 1) == OthelloCell::white) {
                    int count = 0;
                    for (int i = location.second + 1; i < y; i++)
                    {
                        if (sandbox->board().cellAt(location.first, i) == OthelloCell::black)
                        {
                            total_count += count;
                            break;
                        }

                        if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                        {
                            break;
                        }
                        count++;
                    }
                }
                return base_score + total_count;    
            }
            //middle bottom left
            if (sandbox->board().cellAt(location.first - 1, location.second + 1) == OthelloCell::white) {
                int count = 0;
                int i = location.second + 1;
                int j = location.first - 1;

                while (i < y && j > 0)
                {
                    if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                    {
                        total_count += count;
                        break;
                    }

                    if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                    {
                        break;
                    }
                    count++;
                    i++;
                    j--;
                }
            }
            //middle bottom right
            if (sandbox->board().cellAt(location.first + 1, location.second + 1) == OthelloCell::white) {
                int count = 0;
                int i = location.second + 1;
                int j = location.first + 1;

                while (i < y && j < x)
                {
                    if (sandbox->board().cellAt(j, i) == OthelloCell::black)
                    {
                        total_count += count;
                        break;
                    }

                    if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                    {
                        break;
                    }
                    count++;
                    i++;
                    j++;
                }
            }
            //middle bottom
            if (sandbox->board().cellAt(location.first, location.second + 1) == OthelloCell::white) {
                int count = 0;
                for (int i = location.second + 1; i < y; i++)
                {
                    if (sandbox->board().cellAt(location.first, i) == OthelloCell::black)
                    {
                        total_count += count;
                        break;
                    }

                    if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                    {
                        break;
                    }
                    count++;
                }
            }
            return base_score + total_count;
        }
        return base_score + total_count;
    } else {
        int base_score = sandbox->whiteScore() - sandbox->blackScore();
        int total_count = 0;
        if (location.second < y - 1) {
            if (location.second > 0) {
                if (location.first < x - 1) {
                    if (location.first > 0) {
                        //middle top
                        if (sandbox->board().cellAt(location.first, location.second - 1) == OthelloCell::black) {
                            int count = 0;
                            for (int i = location.second - 1; i > 0; i--)
                            {
                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                        //middle top left
                        if (sandbox->board().cellAt(location.first - 1, location.second - 1) == OthelloCell::black) {
                            int count = 0;
                            int i = location.second - 1;
                            int j = location.first - 1;

                            while (i > 0 && j > 0)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i--;
                                j--;
                            }
                        }
                        //middle top right
                        if (sandbox->board().cellAt(location.first + 1, location.second - 1) == OthelloCell::black) {
                            int count = 0;
                            int i = location.second - 1;
                            int j = location.first + 1;

                            while (i > 0 && j < x)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i--;
                                j++;
                            }
                        }
                        //middle bottom
                        if (sandbox->board().cellAt(location.first, location.second + 1) == OthelloCell::black) {
                            int count = 0;
                            for (int i = location.second + 1; i < y; i++)
                            {
                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(location.first, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                        //middle bottom left
                        if (sandbox->board().cellAt(location.first - 1, location.second + 1) == OthelloCell::black) {
                            int count = 0;
                            int i = location.second + 1;
                            int j = location.first - 1;

                            while (i < y && j > 0)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i++;
                                j--;
                            }
                        }
                        //middle bottom right
                        if (sandbox->board().cellAt(location.first + 1, location.second + 1) == OthelloCell::black) {
                            int count = 0;
                            int i = location.second + 1;
                            int j = location.first + 1;

                            while (i < y && j < x)
                            {
                                if (sandbox->board().cellAt(j, i) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(j, i) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                                i++;
                                j++;
                            }
                        }
                        //middle left
                        if (sandbox->board().cellAt(location.first - 1, location.second) == OthelloCell::black) {
                            int count = 0;
                            for (int i = location.first - 1; i > 0; i--)
                            {
                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                        //middle right
                        if (sandbox->board().cellAt(location.first + 1, location.second) == OthelloCell::black) {
                            int count = 0;
                            for (int i = location.first + 1; i < x; i++)
                            {
                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::white)
                                {
                                    total_count += count;
                                    break;
                                }

                                if (sandbox->board().cellAt(i, location.second) == OthelloCell::empty)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                    }                   
                }
            }       
        }
        return base_score + total_count;
    }
}



//validMoves with GameState reference
std::vector<std::pair<int, int>> crsherry::CCOthelloAI::validMoves(const OthelloGameState& state) {
    std::vector<std::pair<int, int>> validMoves;
    for(int r = 0; r < state.board().height(); ++r) {
        for(int c = 0; c < state.board().width(); ++c) {
            if(state.isValidMove(c, r)) {
                validMoves.push_back(std::pair(c, r));
            }   
        }
    }

    return validMoves;
}


//validMoves with unique_pointer
std::vector<std::pair<int, int>> crsherry::CCOthelloAI::validMoves(const std::unique_ptr<OthelloGameState>& sandbox) {
    std::vector<std::pair<int, int>> validMoves;
    for(int r = 0; r < sandbox->board().height(); ++r) {
        for(int c = 0; c < sandbox->board().width(); ++c) {
            if(sandbox->isValidMove(c, r)) {
                validMoves.push_back(std::pair(c, r));
            }   
        }
    }

    return validMoves;
}