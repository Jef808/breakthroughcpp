#ifndef BOARD_H_
#define BOARD_H_

void create_graphical_board() {
    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {
            bool is_light_square = (file + rank) % 2 != 0;

            int square_color = is_light_square ? 1 : 0;
        }
    }
}

enum Piece {
    EMPTY = 0,
    WHITE = 2,
    BLACK = 4,
};

inline bool is_empty(Piece piece) {
    return not piece;
}

inline bool is_white(Piece piece) {
    return piece & 2;
}

inline bool is_black(Piece piece) {
    return piece & 4;
}


#endif // BOARD_H_
