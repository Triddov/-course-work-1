#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "header.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowWith, windowHeight), "MyChess", sf::Style::Close);
    window.setFramerateLimit(60);

    Chess game{};
    if (!game.loadAssets()) {
        return -1;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {

                game.move(window);
            }
        }
        game.setCircle();
        game.setPieceTransparency();
        game.updateBoard();
        game.drawCircle(window);
        game.pawnPromotion();

        window.clear();
        window.draw(game.getBoard());
        window.draw(game.getSquares().first);
        window.draw(game.getSquares().second);

        for (auto& c : game.getCircle())
            window.draw(c);
        for (auto& p : game.getPieces())
            window.draw(p);
        for (auto& p : game.getPawnPromotionQueensB())
            window.draw(p);
        for (auto& p : game.getPawnPromotionQueensW())
            window.draw(p);
        window.display();

        game.deleteCircle();
    }

    return 0;
}