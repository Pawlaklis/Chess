#ifndef SFML_GAME_PIECE_H
#define SFML_GAME_PIECE_H

#include <SFML/Graphics.hpp>


enum set{WHITE, BLACK};
enum pieceType{PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

class piece : public sf::RectangleShape{
public:
    int id;
    set color;
    pieceType type;
    bool isAlive;

    piece(set, pieceType, int, const sf::Texture *tx);
};



#endif //SFML_GAME_PIECE_H
