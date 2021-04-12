#ifndef SFML_GAME_BOARD_H
#define SFML_GAME_BOARD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "piece.h"


class Board{
public:
    piece *board[8][8];
    Board(int x, int y);

    void draw(sf::RenderWindow& target);
    int move(int fromX, int fromY, int toX, int toY);
    bool isPlayer(set player, int x, int y);
    void highlight(int x, int y);
    void reset();

private:
    sf::Texture wTextures[6];
    sf::Texture bTextures[6];
    sf::RectangleShape whiteTile, blackTile, lastMoveTo, lastMoveFrom;
    std::vector<piece> whitePieces;
    std::vector<piece> blackPieces;

    void createTiles(int x, int y);
    int createPieces();
    bool loadTextures();
    bool isCheck();
    bool canMove(const piece& p, int fromX, int fromY, int toX, int toY);
};


#endif //SFML_GAME_BOARD_H
