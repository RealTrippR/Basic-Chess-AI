#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

namespace pieces {
    class piece {
        public:
            int posX = 0;
            int posY = 0;
            int scope = 0; // getscope type
            char display = 'U'; // UNSET
            int type = 0; // 1 = King, 2 = Queen, 3 = Knight, 4 = Rook, 5 = Bishop, 6 = Pawn
    };
}

class pos2d {
    public:
        int X = 0;
        int Y = 0;
};

//pieces::piece iiiii;

//std::vector<pieces::piece> iiiiii = {iiiii};

class pos4d {
    public:
        // pos to move to to
        int X = 0;
        int Y = 0;
        // index of the piece to move
        int base_index = 0;
        //is_white;
        std::vector<pieces::piece> *user_reference_vec;
};

int getscope (int type) {

    switch(type) {
        case 1:
            return 1;
        case 2:
            return 8;
        case 3:
            return 0; // The Knight
        case 4:
            return 8;
        case 5:
            return 8;
        case 6:
            return 1;
    }
    return -1;
}

using namespace std;
using namespace pieces;

////////////////////////////////////////////////////////////////////////////////////////////////

vector<piece> white_pieces = {};
vector<piece> black_pieces = {};

bool whites_turn = true;

////////////////////////////////////////////////////////////////////////////////////////////////

void init_boards() {
    
    white_pieces = {};
    black_pieces = {};
    
    // -1 types means that piece is captured

    // White //
    ////////////////////////////////////////////////
    // creates white pawns

    for (int i = 0; i < 8; i++) {
        piece pawn;
        pawn.type = 6;
        pawn.display = 'P';
        pawn.scope = 0;
        pawn.posY = 6;
        pawn.posX = i;
        white_pieces.push_back(pawn);
    }

    piece rook;
    rook.type = 4;
    rook.display = 'R';
    rook.scope = 8;
    rook.posY = 7;
    rook.posX = 0;
    white_pieces.push_back(rook);

    rook.type = 4;
    rook.display = 'R';
    rook.scope = 8;
    rook.posY = 7;
    rook.posX = 7;
    white_pieces.push_back(rook);

    piece bishop;
    bishop.type = 5;
    bishop.display = 'B';
    bishop.scope = 8;
    bishop.posY = 7;
    bishop.posX = 2;
    white_pieces.push_back(bishop);

    bishop.type = 5;
    bishop.display = 'B';
    bishop.scope = 8;
    bishop.posY = 7;
    bishop.posX = 5;
    white_pieces.push_back(bishop);

    piece knight;
    knight.type = 3;
    knight.display = 'N';
    knight.scope = 8;
    knight.posY = 7;
    knight.posX = 1;
    white_pieces.push_back(knight);

    knight.type = 3;
    knight.display = 'N';
    knight.scope = 8;
    knight.posY = 7;
    knight.posX = 6;
    white_pieces.push_back(knight);

    piece queen;
    queen.type = 2;
    queen.display = 'Q';
    queen.scope = 8;
    queen.posY = 7;
    queen.posX = 3;
    white_pieces.push_back(queen);

    piece king;
    king.type = 1;
    king.display = 'K';
    king.scope = 8;
    king.posY = 7;
    king.posX = 4;
    white_pieces.push_back(king);

    // Black //
    ////////////////////////////////////////////////
    // creates black pawns

    for (int i = 0; i < 8; i++) {
        piece pawn;
        pawn.type = 6;
        pawn.display = 'P';
        pawn.scope = 0;
        pawn.posY = 1;
        pawn.posX = i;
        black_pieces.push_back(pawn);
    }

    rook.type = 4;
    rook.display = 'R';
    rook.scope = 8;
    rook.posY = 0;
    rook.posX = 0;
    black_pieces.push_back(rook);

    rook.type = 4;
    rook.display = 'R';
    rook.scope = 8;
    rook.posY = 0;
    rook.posX = 7;
    black_pieces.push_back(rook);

    bishop.type = 5;
    bishop.display = 'B';
    bishop.scope = 8;
    bishop.posY = 0;
    bishop.posX = 2;
    black_pieces.push_back(bishop);

    bishop.type = 5;
    bishop.display = 'B';
    bishop.scope = 8;
    bishop.posY = 0;
    bishop.posX = 5;
    black_pieces.push_back(bishop);

    knight.type = 3;
    knight.display = 'N';
    knight.scope = 8;
    knight.posY = 0;
    knight.posX = 1;
    black_pieces.push_back(knight);

    knight.type = 3;
    knight.display = 'N';
    knight.scope = 8;
    knight.posY = 0;
    knight.posX = 6;
    black_pieces.push_back(knight);

    queen.type = 2;
    queen.display = 'Q';
    queen.scope = 8;
    queen.posY = 0;
    queen.posX = 3;
    black_pieces.push_back(queen);

    king.type = 1;
    king.display = 'K';
    king.scope = 8;
    king.posY = 0;
    king.posX = 4;
    black_pieces.push_back(king);
}

//checks all of the pieces in the users array to see if the target pos is taken
bool is_pos_taken(vector<piece> vec1, vector<piece> vec2, int posX, int posY, bool is_white, bool both) {
/*
checks all of the pieces in the arrays to see if the target pos is taken
*/
    if (is_white || both) {
        for (int i = 0; i < vec1.size(); i++) {
            if (vec1[i].posX == posX && vec1[i].posY == posY) {
                return true;
            }
        }
    } 
    if (!is_white || both)  {
        for (int i = 0; i < vec2.size(); i++) {
            if (vec2[i].posX == posX && vec2[i].posY == posY) {
                return true;
            }
        }
    }

    return false;
}

vector<pos2d> get_moves_diag(vector<piece> vec1, vector<piece> vec2, int scope, piece _piece, bool is_white) {
    
    int posD_UpNLeft = -1;
    int posD_UpNRight = -1;
    int posD_DownNLeft = -1;
    int posD_DownNRight = -1;

    vector<pos2d> valid_moves = {};

    for (int i = 0; i < scope; i++) {
        if (is_pos_taken(vec1, vec2, _piece.posX - i, _piece.posY + i, is_white, true)) {
            posD_UpNLeft = i;
        } else if (posD_UpNLeft == -1) {
            pos2d move;
            move.X = _piece.posX - i;
            move.Y = _piece.posY + i;
            valid_moves.push_back(move);
        }
    }
    for (int i = 0; i < scope; i++) {
        if (is_pos_taken(vec1, vec2, _piece.posX + i, _piece.posY + i, is_white, true)) {
            posD_UpNRight = i;
        } else if (posD_UpNRight == -1) {
            pos2d move;
            move.X = _piece.posX + i;
            move.Y = _piece.posY + i;
            valid_moves.push_back(move);
        }
    }
    for (int i = 0; i < scope; i++) {
        if (is_pos_taken(vec1, vec2, _piece.posX - i, _piece.posY - i, is_white, true)) {
            posD_DownNLeft = -1;
        } else if (posD_DownNLeft == -1) {
            pos2d move;
            move.X = _piece.posX - i;
            move.Y = _piece.posY - 1;
            valid_moves.push_back(move);
        }
    }
    for (int i = 0; i < scope; i++) {
        if (is_pos_taken(vec1, vec2, _piece.posX + i, _piece.posY - i, is_white, true)) {
            posD_DownNRight = -1;
        } else if (posD_DownNRight == -1) {
            pos2d move;
            move.X = _piece.posX + i;
            move.Y = _piece.posY - i;
            valid_moves.push_back(move);
        }
    }
    
    return valid_moves;
}


/*
https://learn.chessbase.com/en/page/chess-rules-castling?setssocookie=eVSEJkg4HQR%2fdz5TCwmcTFg93m9K7%2bv31ta9afX9%2bMA%3d
The rules for castling
    -castling is only possible if neither the king nor the rook has moved
    -there must not be any pieces between the king and the rook
    -the king may not be in check
    -the square the king goes to and any intervening squares may not be under attack
    -however, there is nothing to prevent castling if the rook is under attack
*/

// returns 0 if not, 1 if in the king is check, 2 if the king is in checkmate

bool can_castle (vector<piece> vec1, vector<piece> vec2, piece start_piece, piece end_piece, bool is_white) {

    bool is_blocked;

    if (!(start_piece.type != 1 && end_piece.type != 4) || (start_piece.type != 4 && end_piece.type != 1) && start_piece.posY != end_piece.posY) {
        return false;
    }

    // diff in between the rook and the king
    int x_diff = abs(start_piece.posX - end_piece.posX);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < x_diff; j++) {
            if (white_pieces[i].posX == min(start_piece.posX, end_piece.posX) + j) {
                return false;
            }
            if (black_pieces[i].posX == min(start_piece.posX, end_piece.posX) + j) {
                return false;
            }
        }
    }

    if (x_diff > 4) {
        return false;
    }

    return true;
}

vector<pos2d> get_moves_straight(vector<piece> vec1, vector<piece> vec2, int scope, piece _piece, bool is_white) {
    
    int posX_left = -1;
    int posX_right = -1;
    int posY_up = -1;
    int posY_down = -1;

    vector<pos2d> valid_moves = {};

    for (int i = 0; i < scope; i++) {
        if (is_pos_taken(vec1, vec2, _piece.posX + i, _piece.posY, is_white, true) && posX_right == -1) {
            posX_right = scope;
        } else {
            pos2d move;
            move.X = _piece.posX + i;
            move.Y = _piece.posY;
            valid_moves.push_back(move);
        }
        if (is_pos_taken(vec1, vec2, _piece.posX - i, _piece.posY, is_white, true) && posX_left == -1) {
            posX_left = scope;
        } else {
            pos2d move;
            move.X = _piece.posX - i;
            move.Y = _piece.posY;
            valid_moves.push_back(move);
        }
        if (is_pos_taken(vec1, vec2, _piece.posX, _piece.posY + i, is_white, true) && posY_up == -1) {
            posY_up = scope;
        } else {
            pos2d move;
            move.X = _piece.posX;
            move.Y = _piece.posY + i;
            valid_moves.push_back(move);
        }
        if (is_pos_taken(vec1, vec2, _piece.posX, _piece.posY - i, is_white, true) && posY_up == -1) {
            posY_down = scope;
        } else {
            pos2d move;
            move.X = _piece.posX;
            move.Y = _piece.posY - i;
            valid_moves.push_back(move);
        }
    }

    return valid_moves;
}

vector<pos2d> get_moves_pawns(vector<piece> vec1, vector<piece> vec2, piece _piece, bool is_white) {
    
    vector<pos2d> valid_moves;

    int max = 1;

    if (is_white) {
        max = -1;
    }

    if (!is_white && _piece.posY == 1) {
        max = 2;
    }

    if (is_white && _piece.posY == 6) {
        max = -2;
    }

    for (int i = 1; i < abs(max) + 1; i++) {
        int a = i * (max / abs(max));
        if (is_pos_taken(vec1, vec2, _piece.posX + 1, _piece.posY + a, !is_white, true)) {
            pos2d move;
            move.X = _piece.posX + 1;
            move.Y = _piece.posY + a;
            valid_moves.push_back(move);
        }

        if (is_pos_taken(vec1, vec2, _piece.posX - 1, _piece.posY + a, !is_white, true)) {
            pos2d move;
            move.X = _piece.posX - 1;
            move.Y = _piece.posY + a;
            valid_moves.push_back(move);
        }

        if (!is_pos_taken(vec1, vec2, _piece.posX, _piece.posY + a, is_white, false)) { // set to both as it can't attack pieces directly in front of it
            pos2d move;
            move.X = _piece.posX;
            move.Y = _piece.posY + a;
            valid_moves.push_back(move);
        }
    }

    return valid_moves;
}

vector<pos2d> get_moves_knight(vector<piece> vec1, vector<piece> vec2, piece _piece, bool is_white) {
    
    vector<pos2d> valid_moves;

    for (int i = -2; i <= 3; i += 4) {
        if (!is_pos_taken(vec1, vec2, _piece.posX + i, _piece.posY + 1, is_white, false)) {
            pos2d move;
            move.X = _piece.posX + i;
            move.Y = _piece.posY + 1;
            valid_moves.push_back(move);
        }
        if (!is_pos_taken(vec1, vec2, _piece.posX + i, _piece.posY - 1, is_white, false)) {
            pos2d move;
            move.X = _piece.posX + i;
            move.Y = _piece.posY - 1;
            valid_moves.push_back(move);
        }
        if (!is_pos_taken(vec1, vec2, _piece.posX - i, _piece.posY - 1, is_white, false)) {
            pos2d move;
            move.X = _piece.posX - i;
            move.Y = _piece.posY - 1;
            valid_moves.push_back(move);
        }
        if (!is_pos_taken(vec1, vec2, _piece.posX - i, _piece.posY + 1, is_white, false)) {
            pos2d move;
            move.X = _piece.posX - i;
            move.Y = _piece.posY + 1;
            valid_moves.push_back(move);
        }
        if (!is_pos_taken(vec1, vec2, _piece.posX + 1, _piece.posY + i, is_white, false)) {
            pos2d move;
            move.X = _piece.posX + 1;
            move.Y = _piece.posY + i;
            valid_moves.push_back(move);
        }
        if (!is_pos_taken(vec1, vec2, _piece.posX - 1, _piece.posY + i, is_white, false)) {
            pos2d move;
            move.X = _piece.posX - 1;
            move.Y = _piece.posY + i;
            valid_moves.push_back(move);
        }
        if (!is_pos_taken(vec1, vec2, _piece.posX + 1, _piece.posY - i, is_white, false)) {
            pos2d move;
            move.X = _piece.posX + 1;
            move.Y = _piece.posY - i;
            valid_moves.push_back(move);
        }
        if (!is_pos_taken(vec1, vec2, _piece.posX - 1, _piece.posY - i, is_white, false)) {
            pos2d move;
            move.X = _piece.posX - 1;
            move.Y = _piece.posY - i;
            valid_moves.push_back(move);
        }
    }

    return valid_moves;
}
/*
searches for a piece in an array at a given position. If it finds it the func returns
the index of the piece. Returns -1 if it fails to find the piece.
*/
int search_for_piece(vector<piece> arr, pos2d pos) {
    //if (is_white) {
        //arr = white_pieces;
    //}
    for (int i = 0; i < arr.size(); i++) {
        //cout << "(" << "X:"<< pos.X << "Y:"<< pos.Y << ")";
        //cout << "(" << "X:"<< arr[i].posX << "Y:"<< arr[i].posY << ")";
        if (arr[i].posX == pos.X && arr[i].posY == pos.Y) {
            //cout << i <<"(" << "X:"<< arr[i].posX << "Y:"<< arr[i].posY << ")";
            return i;
        }
    }

    return -1;
}

// start posX & start posY can be retrieved from _piece

bool is_move_valid (vector<piece> vec1, vector<piece> vec2, piece _piece, pos2d endpos, bool is_white, bool AI = false) {

    vector<piece> users_vec = vec1;
    vector<piece> enemy_vec = vec1;
    
    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    if (!AI) {
        for (int i = 0; i < users_vec.size(); i++) {
            if (users_vec[i].posX == endpos.X && users_vec[i].posY == endpos.Y) {
                return false;
            }
        }
    }

    if (_piece.type == 1 || _piece.type == 2 || _piece.type == 4) {
        vector<pos2d> moves_straight = get_moves_straight(vec1, vec2, getscope(_piece.type), _piece, is_white);
        for (int i = 0; i < moves_straight.size(); i++) {
            if (moves_straight[i].X == endpos.X && moves_straight[i].Y == endpos.Y) {
                return true;
            }
        }
    }

    if (_piece.type == 1 || _piece.type == 2 || _piece.type == 5) { 
        // if (is_white) {
        //     cout << _piece.posY << "D,";
        // }
        vector<pos2d> moves_diag = get_moves_diag(vec1, vec2, getscope(_piece.type), _piece, is_white);
        for (int i = 0; i < moves_diag.size(); i++) {
            if (moves_diag[i].X == endpos.X && moves_diag[i].Y == endpos.Y) {
                return true;
            }
        }
    }
    // checks that the king has not moved more than 1 space
    if (_piece.type == 1) {
        if (abs(_piece.posX - endpos.X) > 1) {
            return false;
        }
        if (abs(_piece.posY -  endpos.Y) > 1) {
            return false;
        }
    }

    if (_piece.type == 3) {
        vector<pos2d> moves_knights = get_moves_knight(vec1, vec2, _piece, is_white);
        for (int i = 0; i < moves_knights.size(); i++) {
            if (moves_knights[i].X == endpos.X && moves_knights[i].Y == endpos.Y) {
                return true;
            }
        }
    }

    if (_piece.type == 6) {
        // if (is_white) {
        //     cout << _piece.posY << "P,";
        // }
        vector<pos2d> moves_pawns = get_moves_pawns(vec1, vec2, _piece, is_white);
        for (int i = 0; i < moves_pawns.size(); i++) {
            if (moves_pawns[i].X == endpos.X && moves_pawns[i].Y == endpos.Y) {
                //if (endpos.X == 5) {
                    //cout << "X:" << moves_pawns[i].X << ", Y:" << moves_pawns[i].Y << "|";
                //}
                //cout << "MP" << (moves_pawns[i].Y == endpos.Y) << "|";
                return true;
            }
        }
    }

    return false;
}

// checks to see if is_white can checkmate !is_white
// returns 0 if not, 1 if check, 2 if checkmate
// does it account for the fact that altough the king can be the initaitor, the rook may not be able to do the same?
int is_in_check_or_checkmate(vector<piece> vec1, vector<piece> vec2, bool is_white, pos2d ai_pos) {

    vector<piece> users_vec;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    pos2d kings_pos;
    kings_pos.X = users_vec[15].posX;
    kings_pos.Y = users_vec[15].posY;

    int borders_taken = 9;
    bool in_check = false;

    for (int i = 0; i < enemy_vec.size(); i++) {
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {

                pos2d end_pos;
                end_pos.X = kings_pos.X - 1 + x;
                end_pos.Y = kings_pos.Y - 1 + y;

                if (is_move_valid(white_pieces, black_pieces, enemy_vec[i], end_pos, !is_white)) {
                    borders_taken -= 1;
                }
            }
        }

        if (is_move_valid(white_pieces, black_pieces, enemy_vec[i], kings_pos, !is_white)) {
            in_check = true;
        }
    }

    if (borders_taken > 8) {
        return 2;
    }

    if (in_check) {
        return 1;
    }

    return 0;
}

// moves a piece from one pos to another. Returns 1 if succesful, 0 if not.
bool move_pos(vector<piece> vec1, vector<piece> vec2, pos2d start_pos, pos2d end_pos, bool is_white) {

    // see: https://stackoverflow.com/questions/7713266/how-can-i-change-the-variable-to-which-a-c-reference-refers
    vector<piece> Users_vec;
    vector<piece> Enemy_vec;
    
    if (is_white) {
        Users_vec = vec1;
        Enemy_vec = vec2;
    } else {
        Users_vec = vec2;
        Enemy_vec = vec1;
    }

    // for (int z = 0; z <  16; z++) {
    //     cout << "(" << Users_vec[z].display << ", X: " << Users_vec[z].posX << ", Y:" << Users_vec[z].posY << ")";
    // }

    // check if piece exists
    int a = search_for_piece(Users_vec, start_pos);
    int b = search_for_piece(Users_vec, end_pos);

    piece _piece;
    piece end_piece;

    if (a != -1) {
        _piece = Users_vec[a];
    } else {
        printf("a = %d | Start position not found.\n", a);
        return 0;
    }

    if (b != -1) {
        end_piece = Users_vec[b];
        cout << "Position is taken.";
        return 0;
    }

    if (is_move_valid(white_pieces, black_pieces, _piece, end_pos, is_white)) {
        //cout << "VALID, A: " << a << "," << Users_vec[a].posX << "," << Users_vec[a].posY << "," << Users_vec[a].type << "\n";
        //cout << "ENPOS: " << end_pos.X << "," << end_pos.Y << "\n";
        //cout << "SIZE:" <<  Users_vec.size() << "\n";

        if (is_white) {
            white_pieces[a].posX = end_pos.X;
            white_pieces[a].posY = end_pos.Y;
        } else {
            black_pieces[a].posX = end_pos.X; // didn't seg fault when I changed it from x to y... why?
            black_pieces[a].posY = end_pos.Y;
        }


        //for (int j = 0; j < 16; j++) {
            /*piece i = (enemy_vec)[j];
            if (i.posX == (users_vec)[a].posX && i.posY == (users_vec)[a].posY) {
                i.type = -1;
                i.display = ' ';
                cout << "Piece captured. (Line 707)";
            }*/
        //}
        // does fim.
        cout << "FIN\n";

    } else {
        //cout << "INVALID MOVE\n";

        //return 0;
    }

    if (is_white) {
        //white_pieces = Users_vec;
        //black_pieces = Enemy_vec;
    } else {
        //white_pieces = Enemy_vec;
        //black_pieces = Users_vec;
    }

    return 1;
}


// type (1 = row, 2 = column, 3 = diagonal)

bool is_piece_at_risk(vector<piece> vec1, vector<piece> vec2, bool is_white, piece _piece, pos2d pos) {

    vector<piece> users_vec;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    pos2d temp_pos;
    temp_pos.X = _piece.posX;
    temp_pos.Y = _piece.posY;
    int index = search_for_piece(users_vec, temp_pos);

    for (int i = 0; i < 16; i++) {
        if (is_move_valid(vec1, vec2, enemy_vec[i], pos, !is_white)) {
            return true;
        }
    }

    return false;
}

bool is_piece_protected(vector<piece> vec1, vector<piece> vec2, piece _piece, bool is_white) {

    vector<piece> users_vec = vec1;
    vector<piece> enemy_vec = vec2;

    // I must set enemy_vec to an empty vec with the piece in it because of the way the is_move_valid() function works, and due to the fact that the position of the piece in the users vec is not updated to temporary position

    if (is_white) {
        // for (int i = 0; i < 16; i++) {
        //     if (vec1[i].posX == _piece.posX && vec1[i].posY == _piece.posY) {
        //         //vec1[i].posX = -1;
        //         break;
        //     }
        // }   
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
        // vec1 = enemy_vec;
        // for (int i = 0; i < 16; i++) {
        //     if (vec2[i].posX == _piece.posX && vec2[i].posY == _piece.posY) {
        //         //vec2[i].posX = -1;
        //         break;
        //     }
        // }   
    }

    /*int index = 0;
    for (int i = 0; i < users_vec.size(); i++) {
        if (users_vec[i].posX == _piece.posX && users_vec[i].posY == _piece.posY) {
            index = i;
            break;
        }
    }*/
    
    pos2d end_pos;
    end_pos.X = _piece.posX;
    end_pos.Y = _piece.posY;

    for (int i = 0; i < 16; i++) {
        /*if (end_pos.X == 5 && end_pos.Y == 2 && users_vec[i].posX == 5 && users_vec[i].posY == 1) {
            cout << "ENDPOS: " << end_pos.X << ", " << end_pos.Y << " | ";
            cout << "USERVEC[I]: " << users_vec[i].posX << ", " << users_vec[i].posY << " | ";
        }
        if (is_move_valid(vec1, vec2, users_vec[i], end_pos, is_white, true)) {
            cout << "PROTECTED!";
        }*/

        if (is_move_valid(vec1, vec2, users_vec[i], end_pos, is_white, false) && users_vec[i].posX != _piece.posX && users_vec[i].posY != _piece.posY) {
            return true;
        }
    }
    
    return false;
}

// returns the total value of the pieces it is protecting (7 - piece.type) to give accurate results
// error is here

int is_piece_protecting(vector<piece> vec1, vector<piece> vec2, piece _piece, pos2d end_pos, bool is_white) {

    vector<piece> users_vec = vec1;
    vector<piece> enemy_vec = vec2;

    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    pos2d pos;
    pos.X = _piece.posX;
    pos.X = _piece.posY;

    int index = search_for_piece(users_vec, pos);
    //cout << "X: " << end_pos.X << ", Y: " << end_pos.Y << " | ";
    //cout << "Index: " << index << " | ";
    (vec2)[index].posX = end_pos.X;
    (vec2)[index].posY = end_pos.Y;

    int total = 0;

    for (int i = 0; i < 16; i++) {

        int index = search_for_piece(users_vec, end_pos);

        if (_piece.type == 6) { // if pawn
            vector<piece> tmp_vec(16);
            tmp_vec[0] = (users_vec)[i];
            if (is_white) {
                vec2 = tmp_vec;
            } else {
                vec1 = tmp_vec;
            }
        }

        if (is_move_valid(vec1, vec2, _piece, end_pos, is_white)) {
            total += 7 - (users_vec)[i].type;
        }
    }

    return total;
}

// valid moves for pawns need to both vectors, that is the issue
bool is_blocked(vector<piece> vec1, vector<piece> vec2, piece _piece, pos2d end_pos, int type, bool is_white) {
    if (type == 1) {
        int min = std::min(_piece.posX, end_pos.X);
        int max = std::max(_piece.posX, end_pos.X);
        for (int i = min; i < max; i++) {
            if (vec1[i].posY == _piece.posY && vec1[i].posX == i) {
                return true;
            }

            if (vec2[i].posY == _piece.posY && vec2[i].posX == i) {
                return true;
            }
        }
    }

    if (type == 2) {
        int min = std::min(_piece.posY, end_pos.Y);
        int max = std::max(_piece.posY, end_pos.Y);
        for (int i = min; i < max; i++) {
            if (vec1[i].posX == _piece.posX && vec1[i].posY == i) {
                return true;
            }

            if (vec2[i].posX == _piece.posX && vec2[i].posY == i) {
                return true;
            }
        }
    }

    return false;
}

int is_rook_strong(vector<piece> vec1, vector<piece> vec2, piece _piece, bool is_white) {

    bool doubled_on_X = false;
    bool doubled_on_Y = false;
    bool rook_on_7th = false;
    
    if (_piece.type != 4) { // checks if its a rook or not
        return false;
    }
    

    vector<piece> users_vec;
    //vector<piece>* users_vec_reference;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1;
        //users_vec_reference = &vec1;
        enemy_vec = vec2;
    } else {
        enemy_vec = vec1;
        users_vec = vec2;
    }

    // checks if rook is doubled (next to or above each other)
    for (int i = 0; i < users_vec.size(); i++) {
        // is row or column blocked?
        if (users_vec[i].type == _piece.type) {

            pos2d end_pos;
            end_pos.X = users_vec[i].posX;
            end_pos.Y = users_vec[i].posY;

            if (!is_blocked(vec1, vec2, _piece, end_pos, 1, is_white) && users_vec[i].posX == _piece.posX) {
                doubled_on_X = true;
            }

            if (!is_blocked(vec1, vec2, _piece, end_pos, 2, is_white) && users_vec[i].posY == _piece.posY) {
                doubled_on_Y = true;
            }
        }
    }

    // checks if rook is on 7th
    if ((is_white && _piece.posY == 1) || ( !is_white && _piece.posY == 6)) { // counting starts at 0, keep that in mind
        rook_on_7th = true;
    }

    // checks if the file that the rook is on is open (returns an int from 1 - 8; 8 being completely full)
    int open_pieces_x = 0;


    for (int i = 0; i < 8; i++) {
        int max_upper = _piece.posY;
        int max_lower = _piece.posY;
        if (enemy_vec[i].posX == _piece.posX) {
            if (enemy_vec[i].posY > max_upper && max_upper != _piece.posY) {
                max_upper =  enemy_vec[i].posY;
            }
            if (enemy_vec[i].posY < max_lower && max_lower != _piece.posY) {
                max_upper =  enemy_vec[i].posY;
            }
        }
        if (users_vec[i].posX == _piece.posX) {
            if (users_vec[i].posY > max_upper && max_upper != _piece.posY) {
                max_upper =  users_vec[i].posY;
            }
            if (users_vec[i].posY < max_lower && max_lower != _piece.posY) {
                max_upper =  users_vec[i].posY;
            }
        }
    }

    return rook_on_7th + doubled_on_X + doubled_on_Y + open_pieces_x;
}

/*"(a)Advanced knights (at e5, d5, c5, f5, e6, d6, c6, f6), especially if protected by
pawn and free from pawn attack." */
bool is_knight_strong(vector<piece> vec1, vector<piece> vec2, piece _piece, bool is_white) {

    if (_piece.type != 3) {
        return false;
    }

    //bool protected_ = is_piece_protected(vec1, vec2, is_white, _piece);
    bool good_space = false;

    for (int i = 0; i < 2; i++) {
        int a;
        if (is_white) {
            a = 5 + i;
        } else {
            a = 3 + i;
        }
        if ((_piece.posX == 'E' - 65 && _piece.posY == a) || (_piece.posX == 'D' - 65 && _piece.posY == a) || (_piece.posX == 'C' - 65 && _piece.posY == a) || (_piece.posX == 'F' - 65 && _piece.posY == a)) {
            good_space = true;
        }
    }

    return good_space;
}

/*(2)Pawn formation:
(a)Backward, isolated and doubled pawns.
(b)Relative control of centre (pawns at e4, d4, c4).
(c)Weakness of pawns near king (e.g. advanced g pawn).
(d)Pawns on opposite colour squares from bishop.
(e)Passed pawns. 
*/
int is_pawn_strong(vector<piece> vec1, vector<piece> vec2, piece _piece, bool is_white)
{

    if (_piece.type != 6) {
        return false;
    }

    vector<piece> users_vec;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    bool isolated = true;
    bool passed = false;
    bool backwards = false;
    bool opposite_of_bishop = false;

    // checks if its isolated (avoid this)

    for (int i = 0; i < users_vec.size(); i++) {
        if (users_vec[i].type == 6 && (users_vec[i].posX == _piece.posX - 1) || (users_vec[i].posX == _piece.posX + 1)) {
            isolated = false;
        }
    }

    // checks if pawn is backwards (avoid this)

    for (int i = 0; i < users_vec.size(); i++) {
        if (users_vec[i].posY > _piece.posY) {
            backwards = true;
            break;
        }
    }


    // checks if pawn is on an oppisite color sqaure from the enemies bishop 
    // first to make sure that there is only one bishop
    bool one_bishop = false;
    int bishop_index = -1;

    for (int i = 0; i < enemy_vec.size(); i++) {
        if (enemy_vec[i].type == 5) {
            one_bishop = true;
            bishop_index = i;
            break;
        }
    }

    if (bishop_index != -1) {
        if ((_piece.posX + _piece.posY) % 2 == !(enemy_vec[bishop_index].posX + enemy_vec[bishop_index].posY) % 2)
        {
            opposite_of_bishop = true;
        }
    }

    // checks if pawn is passed (passed means that it has no pieces in front of it)
    for (int i = 0; i < 8; i++) {
        pos2d check_pos;
        check_pos.X = _piece.posX;
        check_pos.Y = _piece.posY;
        if ((vec1[i].posX == check_pos.X && vec1[i].posY == check_pos.Y + 1) || (vec2[i].posX == check_pos.X && vec2[i].posY == check_pos.Y + 1))
        {
            passed = true;
        }
    }

    // calculates distance from center
    float dist_from_center = sqrt(pow(_piece.posX - 4, 2) + pow(_piece.posY - 4, 2));

    // calculates distances of pawn from king
    pos2d king_pos;
    king_pos.X = users_vec[7].posX;
    king_pos.Y = users_vec[7].posY;

    float dist_from_king = sqrt(pow(_piece.posX - king_pos.X, 2) + pow(_piece.posY - king_pos.Y, 2));

    return !isolated + !backwards + passed + opposite_of_bishop + dist_from_center + dist_from_king;
}

// calculates how many pieces it could take 
// take into account protected and unprotected pieces
// the _pieces pos is set to pos
int pieces_in_sight(vector<piece> vec1, vector<piece> vec2, piece _piece, pos2d pos, bool is_white) {

    vector<piece> users_vec;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    int total = 0;

    for (int i = 0; i < enemy_vec.size(); i++) {

        piece temp_piece = _piece;
        temp_piece.posX = pos.X;
        temp_piece.posY = pos.Y;

        pos2d end_pos;
        end_pos.X = enemy_vec[i].posX;
        end_pos.Y = enemy_vec[i].posY;

        if (is_move_valid(vec1, vec2, temp_piece, end_pos, is_white)) {
            total += 7 - enemy_vec[i].type;

        }
    }

    return total;
}

int users_pieces_blocked(vector<piece> vec1, vector<piece> vec2, piece _piece, bool is_white) {

    int num_moves_before_invis = 0;
    int num_moves_after_invis = 0;

    vector<piece> users_vec;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    for (int i = 0; i < users_vec.size(); i++) {
        for (int j = 0; j < enemy_vec.size(); j++) {
            pos2d end_pos;
            end_pos.X = enemy_vec[j].posX;
            end_pos.Y = enemy_vec[j].posY;
            if (is_move_valid(vec1, vec2, users_vec[i], end_pos, is_white)) {
                num_moves_after_invis += (7 - enemy_vec[j].type);
            }
        }
    }


    pos2d tmp_pos;
    tmp_pos.X = _piece.posX;
    tmp_pos.Y = _piece.posY;

    int index = search_for_piece(users_vec, tmp_pos);

    if (is_white) {
        vec1[index].posX = -1;
    } else {
        vec2[index].posX = -1;
    }

    for (int i = 0; i < vec1.size(); i++) {
        for (int j = 0; j < enemy_vec.size(); j++) {
            pos2d end_pos;
            end_pos.X = enemy_vec[j].posX;
            end_pos.Y = enemy_vec[j].posY;
            if (is_move_valid(vec1, vec2, users_vec[i], end_pos, is_white)) {
                num_moves_before_invis += (7 - enemy_vec[j].type);
            }
        }
    }

    return num_moves_after_invis - num_moves_before_invis;
}

bool is_piece_blocking_castle(pos2d pos, bool is_white) {
    if (is_white && (pos.X == 6 || pos.Y == 7  || pos.Y == 7 || pos.Y == 7)) {
        return true;
    }

    if (!is_white && (pos.X == 6 || pos.X == 7  || pos.Y == 0 || pos.Y == 0)) {
        return true;
    }

    return false;
}

/* returns 0 if not, returns value of enemy piece if so
not does check if move is valid, should be done elsewhere in the code */
int does_move_capture_piece(vector<piece> vec1, vector<piece> vec2, piece, pos2d end_pos, bool is_white) {
    
    vector<piece> users_vec;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1;
        enemy_vec = vec2;
    } else {
        users_vec = vec2;
        enemy_vec = vec1;
    }

    int index = search_for_piece(enemy_vec, end_pos);

    if (index) {
        return users_vec[index].type;
    }

    return 0;
}

int ai_moves_made = 0;
// see: https://vision.unipv.it/IA1/ProgrammingaComputerforPlayingChess.pdf, p. 17
// Could use improvement. A should also calculate the risk faced by potential enemy moves.
// Some stuff has been commented out to improve performance - Idea: Reduce load as o increases
// As accuracy decreases, I should decrease the weight of variable a
// I should add pos to piece class and replace vec1 and vec2 with users_vec and enemy_vec globally

pos4d calc_AI_pos(vector<piece> vec1, vector<piece> vec2, bool is_white) {

    // I still need to make it so that pawns defend castled king
    // I also need to make castling a thing

    //cout << "START";

    vector<piece> vec1_copy = vec1;
    vector<piece> vec2_copy = vec2;

    vector<piece> users_vec;
    vector<piece>* users_vec_reference;
    vector<piece> enemy_vec;

    if (is_white) {
        users_vec = vec1_copy;
        users_vec_reference = &vec1_copy;
        enemy_vec = vec2_copy;
    } else {
        users_vec = vec2_copy;
        users_vec_reference = &vec2_copy;
        enemy_vec = vec1_copy;
    }

    vector<pos4d> best_pos(2);
    vector<float> best(2,0);
    float highest = 0;
    vector<pos4d> highest_cords(2);

    int reduce_point = 1; // when o reaches this number, decrease load to decrease computation time

    for (int o = 0; o < 1; o++) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 64; j++) {

                //cout << i << "\n";

                piece end_piece = users_vec[i];
                end_piece.posX = int(j % 8);
                end_piece.posY = floor(j / 8);

                pos2d end_pos;
                end_pos.X = int(j % 8);
                end_pos.Y = floor(j / 8);

                if (is_move_valid(vec1, vec2, users_vec[i], end_pos, is_white)) {
                    //cout << "A";
                    float protected_ = is_piece_protected(vec1, vec2, end_piece, is_white);
                    //cout << "B";
                    int protecting = is_piece_protecting(vec1, vec2, users_vec[i], end_pos, is_white);
                    //cout << "C";
                    float pawn = is_pawn_strong(vec1, vec2, end_piece, is_white) / 6;
                    float knight = is_knight_strong(vec1, vec2, end_piece, is_white);
                    float rook = is_rook_strong(vec1, vec2, end_piece, is_white) / 4;
                    float _pieces_in_sight = pieces_in_sight(vec1, vec2, users_vec[i], end_pos, is_white); // enemy pieces in sight
                    //float _pieces_in_sight = 0;
                    bool at_risk = is_piece_at_risk(vec1, vec2, is_white, users_vec[i], end_pos);
                    float dist_from_center = abs(pow(float(j % 8 - 4), 2.0) + pow(float(floor(j / 8) - 4), 2.0));
                    //bool is_blocking_castle = is_piece_blocking_castle(end_pos, is_white);
                    int pieces_blocked = users_pieces_blocked(vec1, vec2, users_vec[i], is_white); // returns the number of pieces is it blocking from making a move on teh enemy, considering importance of enemy piece
                    int is_blocking_castle = 0;
                    //int pieces_blocked = 0;
                    int move_captures_piece = 7 - does_move_capture_piece(vec1, vec2, users_vec[i], end_pos, is_white); // does the move result in the capture of an enemy piece?

                    pos2d tmp_pos;
                    tmp_pos.X = users_vec[i].posX;
                    tmp_pos.Y = users_vec[i].posY;

                    int index = search_for_piece(users_vec, tmp_pos);
                    
                    vector<piece> vec1_copy2 = vec1;
                    vector<piece> vec2_copy2 = vec2;

                    if (is_white) {
                        vec1_copy2[index].posX = end_pos.X;
                        vec1_copy2[index].posY = end_pos.Y;
                    } else {
                        vec2_copy2[index].posX = end_pos.X;
                        vec2_copy2[index].posY = end_pos.Y;
                    }

                    int check_or_checkmate = is_in_check_or_checkmate(vec1_copy2, vec2_copy2, is_white, end_pos);
                    int should_AI_castle = can_castle(vec1, vec2, users_vec[i], users_vec[9], is_white);
                    //returns 0 if nothing, 1 if check, 2 if checkmate.

                    
                    float a = (exp(check_or_checkmate * 5), 2) + (protected_ * 15) + (pawn * 1) + (knight * 4) + (rook * 5) + (_pieces_in_sight * 1) + (protecting * 5) + ((7 - users_vec[i].type) * 3) - ((at_risk * (7 - users_vec[i].type)) * 5) + (9 - dist_from_center) + (move_captures_piece * 25) - (at_risk * 15) - pieces_blocked - (is_blocking_castle * 3);

                    if (a < 1) { // a must be at least one to avoid seg fault
                        a = 1;
                    }

                    //cout << " - A: " << a << ",";

                    if (a > best[o]) {
                        best[o] = a;

                        best_pos[o].X = end_pos.X;
                        best_pos[o].Y = end_pos.Y;
                        best_pos[o].base_index = i;
                        if (is_white) {
                            //best_pos[o].user_reference_vec = &white_pieces;
                        } else {
                            //best_pos[o].user_reference_vec = &black_pieces;
                        }
                    }
                }

                if (best[0] + best[1] > highest) {
                    highest = best[0] + best[1];
                    highest_cords[o] = best_pos[o];
                }
            }

            //(*users_vec_reference)[i].posX = best_pos[o].X;
            //(*users_vec_reference)[i].posY = best_pos[o].Y;
            
        }
    }

    // default opening moves (black centric)
    // see: https://www.masterclass.com/articles/chess-101-what-are-the-best-opening-moves-in-chess-learn-5-tips-for-improving-your-chess-opening
    // aswell as: https://www.chess.com/article/view/the-best-chess-openings-for-beginners

    // for white & black

    // for white only


    // for black only
    // Sicilian Defense, Najdorf Variation
    //if () {

    //}

    // Petrov’s Defense

    // regardless of color
    // The Italian Game

    //1. e4 e5 2.Nf3 Nc6 3.Bc4

    vector<pos2d> starting_moves;
    pos2d pos;
    if (is_white) {
        pos.X = 'E' - 65;
        pos.Y = 6;
        starting_moves.push_back(pos);

        pos.X = 'E' - 65;
        pos.Y = 4;
        starting_moves.push_back(pos);
    } else {
        pos.X = 'E' - 65;
        pos.Y = 1;
        starting_moves.push_back(pos);

        pos.X = 'E' - 65;
        pos.Y = 3;
        starting_moves.push_back(pos);
    }

    if (is_white) {
        pos.X = 'G' - 65;
        pos.Y = 7;
        starting_moves.push_back(pos);

        pos.X = 'F' - 65;
        pos.Y = 5;
        starting_moves.push_back(pos);
    } else {
        pos.X = 'B' - 65;
        pos.Y = 0;
        starting_moves.push_back(pos);

        pos.X = 'C' - 65;
        pos.Y = 2;
        starting_moves.push_back(pos);
    }

    if (ai_moves_made < (starting_moves.size() / 2)) {
        cout << ai_moves_made * 2 << "-AI,";
        pos4d return_pos;
        return_pos.X = starting_moves[(ai_moves_made * 2) + 1].X;
        return_pos.Y = starting_moves[(ai_moves_made * 2) + 1].Y;
        cout << return_pos.Y << "-END";
        pos2d start_pos;
        start_pos.X = starting_moves[(ai_moves_made * 2)].X;
        start_pos.Y = starting_moves[(ai_moves_made * 2)].Y;
        cout << start_pos.Y << "-START,";
        return_pos.base_index = search_for_piece(users_vec, start_pos);

        if (is_white) {
            return_pos.user_reference_vec = &white_pieces;
        } else {
            return_pos.user_reference_vec = &black_pieces;
        }

        // this is slashed out because normal AI is still fucked up
        ai_moves_made++;

        return return_pos;
    }

    ai_moves_made++;
    return highest_cords[0];
}

// I.E: A,1
pos2d str_to_pos(string str_pos) {

    int posX = str_pos[2] - 48;
    int posY = tolower(str_pos[0]) - 97;

    pos2d pos;
    pos.X = posX - 1;
    pos.Y = posY;

    if (str_pos[1] != ',') {
        pos.X = -1;
    }

    if (posX > 8 || posX < 0 || posY > 8 || posY < 0) {
        pos.X = -1;
    }
    return pos;
}

// seg faults on the row H
void render_board(vector<piece> vec1, vector<piece> vec2) {

    //printf("\033c"); // clears board

    //printf("   |   |   |   |   |   |   |   |   \n");
    printf("\n   ");
    for (int i = 0; i < 8; i++) {
        //cout << i + 49;
        printf("| %c ", i + 49);
    }
    //printf("\n   |   |   |   |   |   |   |   |   \n");
    printf("\n-----------------------------------\n");

    for (int i = 0; i < 8; i++) {
        //printf("   |   |   |   |   |   |   |   |   \n");
        printf(" %c |", i + 65);
        for (int j = 0; j < 8; j++) {
            bool printed_char = false;
            for (int k = 0; k < 16; k++) {
                //cout << "Size: " << vec1.size() << ",";
                //cout << "K:" << k << ",";
                //cout << "pos.X:" << vec1[k].posX << ",";
                //cout << "pos.Y:" << vec1[k].posY << ",";
                if (vec1[k].posX == j && vec1[k].posY == i) {
                    printf("W.%c", vec1[k].display);
                    printed_char = true;
                    break;
                } else if (vec2[k].posX == j && vec2[k].posY == i) {
                    printf("B.%c", vec2[k].display);
                    printed_char = true;
                    break;
                }
            }

            if (!printed_char) {
                printf("   ");
            }

            if (j < 7) {
                printf("|");
            }
        }
        //printf("\n   |   |   |   |   |   |   |   |   \n");
        if (i < 7) {
            printf("\n-----------------------------------\n");
        }
    }
    cout << ("Test");
}

// reproduce error via:
// g,1 -> f,2
// g,2 -> f,2
// f,1 -> e,1
// commenting out AI section fixes seg fault... but why?
int main() {

    init_boards();

    render_board(white_pieces, black_pieces);

    bool white_won = false;
    bool black_won = false;

    while (!(white_won || black_won)) {
        
        if (whites_turn) {

            render_board(white_pieces, black_pieces);

            bool invalid_move = true;
            
            while (invalid_move) {
                REDO:
                printf("\n\nIt is whites turn. Enter the position of a piece that you would like to move: ");
                string answer;
                std::getline (std::cin, answer);
                pos2d start_pos = str_to_pos(answer);
                
                if (start_pos.X == -1 || search_for_piece(white_pieces, start_pos) == -1){
                    printf("Invalid Pos.");
                    invalid_move = true;
                    goto REDO;
                }
                
                printf("\nEnter a position on the board to move it to: ");
                std::getline (std::cin, answer);
                pos2d end_pos = str_to_pos(answer);
                if (end_pos.X == -1 && search_for_piece(white_pieces, end_pos) != -1){
                    printf("Invalid Pos.");
                    invalid_move = true;
                    goto REDO;
                }

                cout << "B";
                
                for (int i = 0; i < 16; i++) {
                    cout << "i: " << i << "," << white_pieces[i].display << "," << black_pieces[i].display  << ",";
                }
                // seg fault here, on third turn?
                if (move_pos(white_pieces, black_pieces, start_pos, end_pos, whites_turn) == 1) {
                    cout << "\nH\n";
                    invalid_move = false;
                    whites_turn = false;
                } else {
                    invalid_move = true;
                    goto REDO;
                }
            }
        } else {
            /*
            printf("\n\nIt is blacks turn. Enter the position of a piece that you would like to move: ");
            string answer;
            std::getline (std::cin, answer);
            pos2d start_pos = str_to_pos(answer);
            if (start_pos.X == -1){
                printf("Invalid Pos.");
            }
            printf("\nEnter a position on the board to move it to: ");
            std::getline (std::cin, answer);
            pos2d end_pos = str_to_pos(answer);
            if (end_pos.X == -1){
                printf("Invalid Pos.");
            }
            */

            //AI:
            cout << "AI1\n";
            pos4d pos = calc_AI_pos(white_pieces, black_pieces, whites_turn);
            cout << "AI2\n";
            pos2d start_pos;
            start_pos.X = black_pieces[pos.base_index].posX;
            start_pos.Y = black_pieces[pos.base_index].posY;
            pos2d end_pos;
            end_pos.X = pos.X;
            end_pos.Y = pos.Y;
            cout << "\n STARTPOS: " << start_pos.X << "X, " << start_pos.Y << "Y";
            cout << "\n ENDPOS: " << end_pos.X << "X, " << end_pos.Y << "Y\n";

            // cout << "O";

            move_pos(white_pieces, black_pieces, start_pos, end_pos, false); // commenting this line out fixes seg fault... but why?

            render_board(white_pieces, black_pieces);
            
            whites_turn = true;
        }
    }

    return 0;
}
