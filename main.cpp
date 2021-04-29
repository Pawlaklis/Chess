#include <iostream>
#include <SFML/Graphics.hpp>
#include "src/Board.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "My window");
    Board board(window.getSize().x, window.getSize().y);

    sf::Vector2i from;
    sf::Vector2i to;
    set player = WHITE;
    int k = 0;
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    player = WHITE;
                    k = 0;
                    board.reset();
                }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (k == 0){
                        from = sf::Mouse::getPosition(window)/128;
                        if (board.isPlayer(player, from.x, from.y)) {
                            k++;
                            board.highlight(from.x, from.y);
                        }
                    }
                    else if(k == 1){
                        to = sf::Mouse::getPosition(window)/128;
                        std::cout << "Moved from " << int(from.x) << " " << int(from.y) << " : " << board.board[from.x][from.y] << " \n";
                        std::cout << "To " << to.x << " " << to.y << " : " << board.board[to.x][to.y] << std::endl;
                        int war = board.move(from.x, from.y, to.x, to.y, player);
                        std::cout << war << std::endl;
                        if(war < 0)
                        {
                            if (board.isPlayer(player, to.x, to.y)) {
                                from = to;
                                board.highlight(from.x, from.y);
                                k = 1;
                            }
                            else
                                k = 0;
                        }
                        else {
                            if (player == BLACK)
                                player = WHITE;
                            else
                                player = BLACK;
                            k = 0;
                        }
                    }
                }
            }


        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        board.draw(window);

        window.display();
    }

    return 0;
}
