
typedef struct {
    int row;
    int col;
    int points_gain;
    int points_stolen;
    int free_spaces;
    int hori_open;
    int verti_open;
    int degrees_space;
    char gain_scoring_axis;
    char steal_scoring_axis;
} Position;

Position create_position(int row, int col, int points_gain, int points_stolen, int free_spaces) {
    Position p;
    p.row = row;
    p.col = col;
    p.points_gain = points_gain;
    p.points_stolen = points_stolen;
    p.free_spaces = free_spaces;
    p.hori_open = 0;
    p.verti_open = 0;
    p.degrees_space = 0;
    p.gain_scoring_axis = ' ';
    p.steal_scoring_axis = ' ';
    return p;
}


void playMove(char board[SIZE][SIZE], char player, int *x, int *y) {
    srand(time(NULL)); // Seed the random number generator
    Position moves[SIZE * SIZE];
    int move_count = 0;
    char opponent = (player == 'X') ? 'O' : 'X';

    // Center move as a good starting point
    if (board[3][3] == ' ') {
        *x = 3;
        *y = 3;
        return;
    }

    // Random move 5% of the time
    if ((rand() % 100) < 5) {
        int empty_positions[SIZE * SIZE];
        int empty_count = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == ' ') {
                    empty_positions[empty_count++] = i * SIZE + j;
                }
            }
        }
        if (empty_count > 0) {
            int chosen_position = empty_positions[rand() % empty_count];
            *x = chosen_position / SIZE;
            *y = chosen_position % SIZE;
            return;
        }
    }

    int best_gain = -1;
    Position best_move = {0, 0, 0, 0, 0};

    // Use scoreBoard to simulate the score impact of each move
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                // Simulate placing a piece on the board
                board[i][j] = player;

                // Evaluate the score after this move
                int simulated_countX[5], simulated_countO[5];
                scoreBoard(board, simulated_countX, simulated_countO);

                // Calculate the gain in score by this move
                int gain = 0;
                for (int k = 0; k < 5; k++) {
                    if (player == 'X') {
                        gain += simulated_countX[k] * (k + 3);  // Weight by the sequence length
                    } else {
                        gain += simulated_countO[k] * (k + 3);  // Weight by the sequence length
                    }
                }

                // Determine if this is the best move so far
                if (gain > best_gain) {
                    best_gain = gain;
                    best_move = create_position(i, j, gain, 0, 0);
                }

                // Reset the board position
                board[i][j] = ' ';
            }
        }
    }

    // Choose the best move found
    *x = best_move.row;
    *y = best_move.col;
}

