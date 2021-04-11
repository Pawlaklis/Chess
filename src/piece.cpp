//
// Created by Pawla on 08.04.2021.
//

#include "piece.h"

piece::piece(set c, pieceType p, int k, const sf::Texture *tx) : sf::RectangleShape(sf::Vector2f(128.f, 128.f)) {
    color = c;
    isAlive = true;
    id = k;
    type = p;
    this->setTexture(tx, true);
}
