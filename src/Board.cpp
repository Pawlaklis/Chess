#include "Board.h"
#include <iostream>

//auxiliary functions

bool isInBoard(int x, int y){
    if (x < 0 || y < 0 || x >= 8 || y >= 8)
        return false;
    return true;
}

int abs(int a){
    if(a < 0)
        return -a;
    return a;
}

void copy(piece *to[8][8], piece* from[8][8]){
    for (int i = 0;i< 8;i++){
        for (int j = 0; j < 8; ++j) {
            to[i][j] = from[i][j];
        }
    }
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

//Public methods

Board::Board(int x, int y) {
    for (auto & i : board) {
        for (auto & j : i) {
            j = nullptr;
        }
    }
    createTiles(x, y);
    if(!loadTextures())
        return;
    createPieces();
}

void Board::draw(sf::RenderWindow &target) {
    for (int i = 0; i < 8; ++i) {
        whiteTile.setPosition(i*whiteTile.getSize().x, 0.f );
        blackTile.setPosition(i*blackTile.getSize().x, 0.f );
        for (int j = 0; j < 8; ++j) {
            if(i%2) {
                target.draw(blackTile);
                blackTile.move(0, 2 * blackTile.getSize().y);
                whiteTile.move(0, whiteTile.getSize().y);
                target.draw(whiteTile);
                whiteTile.move(0, whiteTile.getSize().y);
            }
            else{
                target.draw(whiteTile);
                whiteTile.move(0, 2 * whiteTile.getSize().y);
                blackTile.move(0, blackTile.getSize().y);
                target.draw(blackTile);
                blackTile.move(0, blackTile.getSize().y);

            }
        }
        if (lastMoveFrom.getPosition() != lastMoveTo.getPosition()) {
            target.draw(lastMoveTo);
            target.draw(lastMoveFrom);
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if(board[i][j] != nullptr){
                board[i][j]->setPosition((i + 0.5) * blackTile.getSize().x - 0.5 * board[i][j]->getSize().x, (j + 0.5) * blackTile.getSize().y -  0.5 * board[i][j]->getSize().y);
                target.draw(*board[i][j]);
            }
        }
    }
}

int Board::move(int fromX, int fromY, int toX, int toY, set player) {

    piece *copyBoard[8][8];
    copy(copyBoard, board);


    if(fromX == toX && fromY == toY)
        return -1;
    if(board[fromX][fromY] != nullptr && canMove( *board[fromX][fromY], fromX, fromY, toX, toY)){



        if (board[toX][toY] != nullptr) {
            board[toX][toY]->isAlive = false;
        }
        board[toX][toY] = board[fromX][fromY];

        board[fromX][fromY] = nullptr;

        std::pair<int, int> whiteKing, blackKing;
        whiteKing = findWhiteKing();
        blackKing = findBlackKing();

        if (player == WHITE){
            if (isWhiteCheck(whiteKing.first, whiteKing.second)){
                copy(board, copyBoard);
                return -1;
            }
        } else{
            if (isBlackCheck(blackKing.first, blackKing.second)){
                copy(board, copyBoard);
                return -1;
            }
        }

        lastMoveFrom.setPosition(whiteTile.getSize().x*fromX + 5, whiteTile.getSize().y*fromY + 5);
        lastMoveTo.setPosition(whiteTile.getSize().x*toX + 5, whiteTile.getSize().y*toY + 5);
        lastMoveTo.setOutlineThickness(5.f);


        return 1;
    }

    return -1;
}

bool Board::isPlayer(set player, int x, int y) {
    if (board[x][y] != nullptr)
        return board[x][y]->color == player;
    return false;
}

void Board::highlight(int x, int y) {
    lastMoveFrom.setPosition(whiteTile.getSize().x*x + 5, whiteTile.getSize().y*y + 5);
    lastMoveTo.setOutlineThickness(0);
}

void Board::reset() { //flushes the board and allocates the new pieces
    for (auto & i : board) {
        for (auto & j : i) {
            j = nullptr;
        }
    }
    whitePieces.clear();
    blackPieces.clear();

    createPieces();
}



//Private methods

void Board::createTiles(int x, int y) { // sets properties of tiles
    whiteTile = sf::RectangleShape(sf::Vector2f(x/8, y/8));
    whiteTile.setFillColor(sf::Color::White);
    blackTile = sf::RectangleShape(sf::Vector2f(x/8, y/8));
    blackTile.setFillColor(sf::Color(64, 64, 64));

    lastMoveTo = sf::RectangleShape(sf::Vector2f(x/8 - 10, y/8 - 10));
    lastMoveTo.setOutlineThickness(5.f);
    lastMoveTo.setFillColor(sf::Color::Transparent);
    lastMoveTo.setOutlineColor(sf::Color(255, 102, 0));

    lastMoveFrom = sf::RectangleShape(sf::Vector2f(x/8 - 10, y/8 - 10));
    lastMoveFrom.setOutlineThickness(5.f);
    lastMoveFrom.setFillColor(sf::Color::Transparent);
    lastMoveFrom.setOutlineColor(sf::Color::Yellow);
}

int Board::createPieces() { //allocates pieces and puts them in vectors (idk why, maybe I'll need that) and on board
    for (int i = 0; i < 8; ++i) {
        piece *t = new piece(WHITE, PAWN, i, &wTextures[0]);
        whitePieces.push_back(*t);
        board[i][6] = t;
    }
    for (int i = 0; i < 8; ++i) {
        piece *t = new piece(BLACK, PAWN, i, &bTextures[0]);
        blackPieces.push_back(*t);
        board[i][1] = t;
    }
    for (int i = 0; i < 2; ++i) {
        piece *t = new piece(WHITE, KNIGHT, i, &wTextures[1]);
        whitePieces.push_back(*t);
        board[1 + 5 * i][7] = t;
    }
    for (int i = 0; i < 2; ++i) {
        piece *t = new piece(BLACK, KNIGHT, i, &bTextures[1]);
        blackPieces.push_back(*t);
        board[1 + 5 * i][0] = t;
    }
    for (int i = 0; i < 2; ++i) {
        piece *t = new piece(WHITE, BISHOP, i, &wTextures[2]);
        whitePieces.push_back(*t);
        board[2 + 3 * i][7] = t;
    }
    for (int i = 0; i < 2; ++i) {
        piece *t = new piece(BLACK, BISHOP, i, &bTextures[2]);
        blackPieces.push_back(*t);
        board[2 + 3 * i][0] = t;
    }
    for (int i = 0; i < 2; ++i) {
        piece *t = new piece(WHITE, ROOK, i, &wTextures[3]);
        whitePieces.push_back(*t);
        board[7 * i][7] = t;
    }
    for (int i = 0; i < 2; ++i) {
        piece *t = new piece(BLACK, ROOK, i, &bTextures[3]);
        blackPieces.push_back(*t);
        board[7 * i][0] = t;
    }
    piece *t = new piece(WHITE, QUEEN, 1, &wTextures[4]);
    whitePieces.push_back(*t);
    board[3][7] = t;
    t = new piece(WHITE, KING, 1, &wTextures[5]);
    whitePieces.push_back(*t);
    board[4][7] = t;
    t = new piece(BLACK, QUEEN, 1, &bTextures[4]);
    blackPieces.push_back(*t);
    board[3][0] = t;
    t = new piece(BLACK, KING, 1, &bTextures[5]);
    blackPieces.push_back(*t);
    board[4][0] = t;

    return 0;
}

bool Board::loadTextures() {
    if (!wTextures[0].loadFromFile("..\\icons\\w_pawn.png") || !wTextures[1].loadFromFile("..\\icons\\w_knight.png")|| !wTextures[2].loadFromFile("..\\icons\\w_bishop.png"))
    {
        std::cout << "ERROR! Texture not loaded.";
        return false;
    }
    if (!bTextures[0].loadFromFile("..\\icons\\b_pawn.png") || !bTextures[1].loadFromFile("..\\icons\\b_knight.png") || !bTextures[2].loadFromFile("..\\icons\\b_bishop.png"))
    {
        std::cout << "ERROR! Texture not loaded.";
        return false;
    }
    if (!wTextures[3].loadFromFile("..\\icons\\w_rook.png") || !bTextures[3].loadFromFile("..\\icons\\b_rook.png"))
    {
        std::cout << "ERROR! Texture not loaded.";
        return false;
    }
    if (!wTextures[4].loadFromFile("..\\icons\\w_queen.png") || !bTextures[4].loadFromFile("..\\icons\\b_queen.png"))
    {
        std::cout << "ERROR! Texture not loaded.";
        return false;
    }
    if (!wTextures[5].loadFromFile("..\\icons\\w_king.png") || !bTextures[5].loadFromFile("..\\icons\\b_king.png"))
    {
        std::cout << "ERROR! Texture not loaded.";
        return false;
    }

    for (auto i: wTextures) {
        i.setSmooth(true);
    }
    for (auto i: bTextures) {
        i.setSmooth(true);
    }
    return true;
}

bool Board::isWhiteCheck(int x, int y) { // returns true if there is a check on white king on (x, y) coordinates
    bool blackChecks[8][8] = {false};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->color == BLACK){
                switch (board[i][j]->type) {
                    case PAWN:
                        if (isInBoard(i + 1,j + 1))
                            blackChecks[i + 1][j + 1] = true;
                        if (isInBoard(i - 1,j + 1))
                            blackChecks[i - 1][j + 1] = true;
                        break;
                    case KNIGHT:
                        if (isInBoard(i + 2, j + 1))
                            blackChecks[i + 2][j + 1] = true;
                        if (isInBoard(i - 2, j + 1))
                            blackChecks[i - 2][j + 1] = true;
                        if (isInBoard(i + 2, j - 1))
                            blackChecks[i + 2][j - 1] = true;
                        if (isInBoard(i - 2, j - 1))
                            blackChecks[i - 2][j - 1] = true;

                        if (isInBoard(i + 1, j + 2))
                            blackChecks[i + 1][j + 2] = true;
                        if (isInBoard(i - 1, j + 2))
                            blackChecks[i - 1][j + 2] = true;
                        if (isInBoard(i + 1, j - 2))
                            blackChecks[i + 1][j - 2] = true;
                        if (isInBoard(i - 1, j - 2))
                            blackChecks[i - 1][j - 2] = true;
                        break;
                    case BISHOP:
                        for (int k = 1; isInBoard(i + k ,j + k); ++k) {
                            blackChecks[i + k][j + k] = true;

                            if (board[i + k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i + k ,j - k); ++k) {
                            blackChecks[i + k][j - k] = true;

                            if (board[i + k][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j + k); ++k) {
                            blackChecks[i - k][j + k] = true;

                            if (board[i - k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j - k); ++k) {
                            blackChecks[i - k][j - k] = true;

                            if (board[i - k][j - k] != nullptr)
                                break;
                        }
                        break;
                    case ROOK:
                        for (int k = 1; isInBoard(i + k ,j); ++k) {
                            blackChecks[i + k][j] = true;
                            if (board[i + k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j - k); ++k) {
                            blackChecks[i][j - k] = true;

                            if (board[i][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j); ++k) {
                            blackChecks[i - k][j] = true;
                            if (board[i - k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j + k); ++k) {
                            blackChecks[i][j + k] = true;

                            if (board[i][j + k] != nullptr)
                                break;
                        }
                        break;
                    case QUEEN:
                        for (int k = 1; isInBoard(i + k ,j); ++k) {
                            blackChecks[i + k][j] = true;

                            if (board[i + k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j - k); ++k) {
                            blackChecks[i][j - k] = true;

                            if (board[i][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j); ++k) {
                            blackChecks[i - k][j] = true;

                            if (board[i - k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j - k); ++k) {
                            blackChecks[i][j - k] = true;
                            if (board[i][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i + k ,j + k); ++k) {
                            blackChecks[i + k][j + k] = true;
                            if (board[i + k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i + k ,j - k); ++k) {
                            blackChecks[i + k][j - k] = true;
                            if (board[i + k][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j + k); ++k) {
                            blackChecks[i - k][j + k] = true;
                            if (board[i - k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j - k); ++k) {
                            blackChecks[i - k][j - k] = true;
                            if (board[i - k][j - k] != nullptr)
                                break;
                        }
                        break;
                    case KING:
                        if (isInBoard(i + 1, j + 1)){
                            blackChecks[i + 1][j + 1] = true;
                        }
                        if (isInBoard(i + 1, j)){
                            blackChecks[i + 1][j] = true;
                        }
                        if (isInBoard(i + 1, j - 1)){
                            blackChecks[i + 1][j - 1] = true;
                        }
                        if (isInBoard(i, j - 1)){
                            blackChecks[i][j - 1] = true;
                        }
                        if (isInBoard(i, j + 1)){
                            blackChecks[i][j + 1] = true;
                        }
                        if (isInBoard(i - 1, j - 1)){
                            blackChecks[i - 1][j - 1] = true;
                        }
                        if (isInBoard(i - 1, j)){
                            blackChecks[i + 1][j] = true;
                        }
                        if (isInBoard(i - 1, j + 1)){
                            blackChecks[i - 1][j + 1] = true;
                        }
                        break;
                }
            }
        }
    }
    
    if (blackChecks[x][y])
        return true;

    return false;
}

bool Board::isBlackCheck(int x, int y) { //same but for black
    bool whiteChecks[8][8] = {false};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->color == WHITE){
                switch (board[i][j]->type) {
                    case PAWN:
                        if (isInBoard(i - 1,j - 1))
                            whiteChecks[i + 1][j - 1] = true;
                        if (isInBoard(i - 1,j - 1))
                            whiteChecks[i - 1][j - 1] = true;
                        break;
                    case KNIGHT:
                        if (isInBoard(i + 2, j + 1))
                            whiteChecks[i + 2][j + 1] = true;
                        if (isInBoard(i - 2, j + 1))
                            whiteChecks[i - 2][j + 1] = true;
                        if (isInBoard(i + 2, j - 1))
                            whiteChecks[i + 2][j - 1] = true;
                        if (isInBoard(i - 2, j - 1))
                            whiteChecks[i - 2][j - 1] = true;

                        if (isInBoard(i + 1, j + 2))
                            whiteChecks[i + 1][j + 2] = true;
                        if (isInBoard(i - 1, j + 2))
                            whiteChecks[i - 1][j + 2] = true;
                        if (isInBoard(i + 1, j - 2))
                            whiteChecks[i + 1][j - 2] = true;
                        if (isInBoard(i - 1, j - 2))
                            whiteChecks[i - 1][j - 2] = true;
                        break;
                    case BISHOP:
                        for (int k = 1; isInBoard(i + k ,j + k); ++k) {
                            whiteChecks[i + k][j + k] = true;

                            if (board[i + k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i + k ,j - k); ++k) {
                            whiteChecks[i + k][j - k] = true;

                            if (board[i + k][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j + k); ++k) {
                            whiteChecks[i - k][j + k] = true;

                            if (board[i - k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j - k); ++k) {
                            whiteChecks[i - k][j - k] = true;

                            if (board[i - k][j - k] != nullptr)
                                break;
                        }
                        break;
                    case ROOK:
                        for (int k = 1; isInBoard(i + k ,j); ++k) {
                            whiteChecks[i + k][j] = true;
                            if (board[i + k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j - k); ++k) {
                            whiteChecks[i][j - k] = true;

                            if (board[i][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j); ++k) {
                            whiteChecks[i - k][j] = true;
                            if (board[i - k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j + k); ++k) {
                            whiteChecks[i][j + k] = true;

                            if (board[i][j + k] != nullptr)
                                break;
                        }
                        break;
                    case QUEEN:
                        for (int k = 1; isInBoard(i + k ,j); ++k) {
                            whiteChecks[i + k][j] = true;

                            if (board[i + k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j - k); ++k) {
                            whiteChecks[i][j - k] = true;

                            if (board[i][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j); ++k) {
                            whiteChecks[i - k][j] = true;

                            if (board[i - k][j] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i ,j - k); ++k) {
                            whiteChecks[i][j - k] = true;
                            if (board[i][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i + k ,j + k); ++k) {
                            whiteChecks[i + k][j + k] = true;
                            if (board[i + k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i + k ,j - k); ++k) {
                            whiteChecks[i + k][j - k] = true;
                            if (board[i + k][j - k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j + k); ++k) {
                            whiteChecks[i - k][j + k] = true;
                            if (board[i - k][j + k] != nullptr)
                                break;
                        }
                        for (int k = 1; isInBoard(i - k ,j - k); ++k) {
                            whiteChecks[i - k][j - k] = true;
                            if (board[i - k][j - k] != nullptr)
                                break;
                        }
                        break;
                    case KING:
                        if (isInBoard(i + 1, j + 1)){
                            whiteChecks[i + 1][j + 1] = true;
                        }
                        if (isInBoard(i + 1, j)){
                            whiteChecks[i + 1][j] = true;
                        }
                        if (isInBoard(i + 1, j - 1)){
                            whiteChecks[i + 1][j - 1] = true;
                        }
                        if (isInBoard(i, j - 1)){
                            whiteChecks[i][j - 1] = true;
                        }
                        if (isInBoard(i, j + 1)){
                            whiteChecks[i][j + 1] = true;
                        }
                        if (isInBoard(i - 1, j - 1)){
                            whiteChecks[i - 1][j - 1] = true;
                        }
                        if (isInBoard(i - 1, j)){
                            whiteChecks[i + 1][j] = true;
                        }
                        if (isInBoard(i - 1, j + 1)){
                            whiteChecks[i - 1][j + 1] = true;
                        }
                        break;
                }
            }
        }
    }

    if (whiteChecks[x][y])
        return true;

    return false;
}

std::pair<int, int> Board::findWhiteKing(){
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->color == WHITE && board[i][j]->type == KING){
                return {i, j};
            }

        }
    }
}
std::pair<int, int> Board::findBlackKing() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->color == BLACK && board[i][j]->type == KING){
                return {i, j};
            }
        }
    }
}

bool Board::canMove(const piece& p, int fromX, int fromY, int toX, int toY){

    if (board[toX][toY] != nullptr && board[toX][toY]->color == p.color)
        return false;
    if(p.type == PAWN){
        if(p.color == WHITE){
            if(fromX - toX == 0){
                if (fromY == 6 && fromY - toY == 2 && board[toX][toY + 1] == nullptr && board[toX][toY] == nullptr){
                    return true;
                }
                if(fromY - toY != 1) {
                    return false;
                }
                if(board[toX][toY] != nullptr)
                    return false;
                return true;
            }
            else if ((fromX - toX == 1 || fromX - toX == -1) && toY - fromY == -1 && board[toX][toY] != nullptr && board[toX][toY]->color != board[fromX][fromY]->color){
                return true;
            }
            else
                return false;
        }
        if(p.color == BLACK){
            if(fromX - toX == 0){
                if (fromY == 1 && fromY - toY == -2 && board[toX][toY - 1] == nullptr && board[toX][toY] == nullptr){
                    return true;
                }
                if(fromY - toY != -1) {
                    return false;
                }
                if(board[toX][toY] != nullptr)
                    return false;
                return true;
            }
            else if ((fromX - toX == 1 || fromX - toX == -1) && toY - fromY == 1 && board[toX][toY] != nullptr && board[toX][toY]->color != board[fromX][fromY]->color){
                return true;
            }
            else
                return false;
        }
    }
    if(p.type == KNIGHT){
        if (abs(fromX - toX) == 1 &&  abs(fromY - toY) == 2 || abs(fromX - toX) == 2 &&  abs(fromY - toY) == 1)
            return true;
        return false;
    }
    if(p.type == BISHOP){
        if (abs(fromX - toX) == abs(fromY - toY)) {
            int deltaX = fromX - toX;
            int deltaY = fromY - toY;
            if (deltaX < 0 && deltaY < 0){
                for (int i = 1; i < abs(deltaX); ++i){
                    if (board[fromX + i][fromY + i] != nullptr)
                        return false;
                }
                return true;
            }
            if (deltaX > 0 && deltaY > 0){
                for (int i = 1; i < abs(deltaX); ++i) {
                    if (board[fromX - i][fromY - i] != nullptr)
                        return false;
                }
                return true;
            }
            if (deltaX > 0 && deltaY < 0){
                for (int i = 1; i < abs(deltaX); ++i) {
                    if (board[fromX - i][fromY + i] != nullptr)
                        return false;
                }
                return true;
            }
            if (deltaX < 0 && deltaY > 0){
                for (int i = 1; i < abs(deltaX); ++i) {
                    if (board[fromX + i][fromY - i] != nullptr)
                        return false;
                }
                return true;
            }
        }
        return false;
    }
    if (p.type == ROOK){
        if (fromX != toX && fromY != toY)
            return false;
        for (int i = 1; i < abs(fromX - toX); ++i) {
            if (board[fromX + i*sgn(toX - fromX)][fromY] != nullptr)
                return false;
        }
        for (int i = 1; i < abs(fromY - toY); ++i) {
            if (board[fromX][fromY + i*sgn(toY - fromY)] != nullptr)
                return false;
        }
        return true;
    }
    if (p.type == QUEEN){
        if (fromX == toX || fromY == toY) {
            for (int i = 1; i < abs(fromX - toX); ++i) {
                if (board[fromX + i * sgn(toX - fromX)][fromY] != nullptr)
                    return false;
            }
            for (int i = 1; i < abs(fromY - toY); ++i) {
                if (board[fromX][fromY + i * sgn(toY - fromY)] != nullptr)
                    return false;
            }
            return true;
        }
        else{
            if (abs(fromX - toX) == abs(fromY - toY)) {
                int deltaX = fromX - toX;
                int deltaY = fromY - toY;
                if (deltaX < 0 && deltaY < 0){
                    for (int i = 1; i < abs(deltaX); ++i){
                        if (board[fromX + i][fromY + i] != nullptr)
                            return false;
                    }
                    return true;
                }
                if (deltaX > 0 && deltaY > 0){
                    for (int i = 1; i < abs(deltaX); ++i) {
                        if (board[fromX - i][fromY - i] != nullptr)
                            return false;
                    }
                    return true;
                }
                if (deltaX > 0 && deltaY < 0){
                    for (int i = 1; i < abs(deltaX); ++i) {
                        if (board[fromX - i][fromY + i] != nullptr)
                            return false;
                    }
                    return true;
                }
                if (deltaX < 0 && deltaY > 0){
                    for (int i = 1; i < abs(deltaX); ++i) {
                        if (board[fromX + i][fromY - i] != nullptr)
                            return false;
                    }
                    return true;
                }
            }
            return false;
        }
    }
    if (p.type == KING){
        if (abs(fromX - toX) <= 1 && abs(fromY- toY) <= 1)
            return true;
        return false;
    }
    return false;
}




