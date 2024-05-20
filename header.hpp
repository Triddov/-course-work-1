#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


const int windowWith = 768, windowHeight = 768;
const int fields = 8;
const int tileSize = windowWith / fields;

class Chess {
private:
    sf::Texture t_board;
    sf::Sprite m_board;

    sf::Texture t_blackRook;
    sf::Texture t_blackKnight;
    sf::Texture t_blackBishop;
    sf::Texture t_blackQueen;
    sf::Texture t_blackKing;
    sf::Texture t_blackPawn;

    sf::Texture t_whiteRook;
    sf::Texture t_whiteKnight;
    sf::Texture t_whiteBishop;
    sf::Texture t_whiteQueen;
    sf::Texture t_whiteKing;
    sf::Texture t_whitePawn;

    std::vector<sf::Sprite> m_pieces;

    std::vector<sf::CircleShape> m_circles;

    std::vector<sf::Sprite> m_pawnPromotionQueensW;
    std::vector<sf::Sprite> m_pawnPromotionQueensB;

    sf::SoundBuffer b_pieceMove;
    sf::Sound s_pieceMove;
    sf::SoundBuffer b_capture;
    sf::Sound s_capture;

    std::pair<sf::RectangleShape, sf::RectangleShape> m_posSquares;

    int a_board[fields][fields] = {
        {-10,-20,-30,-40,-50,-31,-21,-11},
        {-60,-61,-62,-63,-64,-65,-66,-67},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {60,61,62,63,64,65,66,67},
        {10,20,30,40,50,31,21,11}
    };
    bool b_board[fields][fields] = { false };
    bool m_circleBoard[fields][fields] = { false };
    int m_canTakeBoard[fields][fields] = { 0 };

    int m_pawnPromotionCounterW;
    int m_pawnPromotionCounterB;

    bool m_whiteTurn;
    bool m_pieceClicked;

    bool m_whiteKingMoved;
    bool m_blackKingMoved;
    bool m_whiteLeftRookMoved;
    bool m_whiteRightRookMoved;
    bool m_blackRightRookMoved;
    bool m_blackLeftRookMoved;

    int m_currentPieceClicked;
    sf::Vector2i m_currentPiecePosition;

public:

    Chess();
    /*
    * Load Textures, Images and Sound Files
    */
    bool loadAssets();

    /*
    * Updates the board and pieces, whenever a piece changed it's position
    */
    void updateBoard();

    /*
    * Calculates all possibilities for the current piece clicked by the player
    */
    void move(sf::RenderWindow&); // !!!

    /*
    * Deletes the pieces from the board, whenever they are taken by the opposite side
    */
    void deletePieces(int, int); 

    /*
    * Calculates the positions of the circles
    */
    void setCircle(); // !!!

    /*
    * Draws the circles on the board
    */
    void drawCircle(sf::RenderWindow&);

    /*
    * Removes all circles from the board
    */
    void deleteCircle();

    /*
    * Set transparency of a piece when it can be taken by the opposite side
    */
    void setPieceTransparency();

    /*
    * Pawn being replaced by a queen, when it reaches the opposite side
    */
    void pawnPromotion();


    sf::Sprite getBoard() const { return m_board; }
    std::vector<sf::Sprite> getPieces() const { return m_pieces; }
    std::vector<sf::CircleShape> getCircle() const { return m_circles; }
    std::vector<sf::Sprite> getPawnPromotionQueensW() const { return m_pawnPromotionQueensW; }
    std::vector<sf::Sprite> getPawnPromotionQueensB() const { return m_pawnPromotionQueensB; }
    std::pair<sf::RectangleShape, sf::RectangleShape> getSquares() const { return m_posSquares; }

};

Chess::Chess() {
    m_whiteTurn = true;
    m_pieceClicked = false;
    m_pawnPromotionCounterW = 0;
    m_pawnPromotionCounterB = 0;

    m_posSquares.first.setSize(sf::Vector2f(tileSize, tileSize));
    m_posSquares.second.setFillColor(sf::Color(255, 60, 126, 128));
    m_posSquares.first.setPosition(-500, -500);
    m_posSquares.first.setFillColor(sf::Color(244, 247, 118, 196));
    m_posSquares.second.setSize(sf::Vector2f(tileSize, tileSize));
    m_posSquares.second.setPosition(-500, -500);

    bool m_whiteKingMoved = false;
    bool m_blackKingMoved = false;
    bool m_whiteLeftRookMoved = false;
    bool m_whiteRightRookMoved = false;
    bool m_blackRightRookMoved = false;
    bool m_blackLeftRookMoved = false;

    for (int i = 0; i < fields * fields; i++) {
        sf::CircleShape c;
        c.setFillColor(sf::Color(255, 255, 255, 255));
        c.setPosition(-100, -100);
        c.setRadius(0);
        m_circles.push_back(c);
    }
}
void Chess::setPieceTransparency() {
    for (int i = 0; i < fields; i++) {
        for (int j = 0; j < fields; j++) {

            for (int g = 8, f = -60; g <= 15 and f >= -67; g++, f--) {
                if (m_canTakeBoard[i][j] == f)
                    m_pieces[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 0, f = -10; g <= 4 and f >= -50; g++, f-=10) {
                if (m_canTakeBoard[i][j] == f)
                    m_pieces[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 5, f = -31; g <= 7 and f <= -11; g++, f += 10) {
                if (m_canTakeBoard[i][j] == f)
                    m_pieces[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 24, f = 60; g <= 31 and f <= 67; g++, f++) {
                if (m_canTakeBoard[i][j] == f)
                    m_pieces[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 16, f = 10; g <= 20 and f <= 50; g++, f+=10) {
                if (m_canTakeBoard[i][j] == f)
                    m_pieces[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 21, f = 31; g <= 23 and f >= 11; g++, f -= 10) {
                if (m_canTakeBoard[i][j] == f)
                    m_pieces[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 0, f = 41; g <= 7 and f >= 48; g++, f++) {
                if (m_canTakeBoard[i][j] == f)
                    m_pieces[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 0, f = 41; g <= 7 and f <= 48; g++, f++) {
                if (m_canTakeBoard[i][j] == f)
                    m_pawnPromotionQueensW[g].setColor(sf::Color(255, 255, 255, 128));
            }
            for (int g = 0, f = -41; g <= 7 and f >= -48; g++, f--) {
                if (m_canTakeBoard[i][j] == f)
                    m_pawnPromotionQueensB[g].setColor(sf::Color(255, 255, 255, 128));
            }
        }
    }
    if (m_whiteTurn) {
        for (int i = 24; i <= 31; i++) {
            m_pieces[i].setColor(sf::Color(255, 255, 255, 255));}
        for (int i = 16; i <= 23; i++) {
            m_pieces[i].setColor(sf::Color(255, 255, 255, 255));}
        for (int i = 0; i <= 7; i++) {
            m_pawnPromotionQueensW[i].setColor(sf::Color(255, 255, 255, 255));}
    }
    else {
        for (int i = 8; i <= 15; i++) {
            m_pieces[i].setColor(sf::Color(255, 255, 255, 255));}
        for (int i = 0; i <= 7; i++) {
            m_pieces[i].setColor(sf::Color(255, 255, 255, 255));}
        for (int i = 0; i <= 7; i++) {
            m_pawnPromotionQueensB[i].setColor(sf::Color(255, 255, 255, 255));}  
    }
}
void Chess::drawCircle(sf::RenderWindow& window) {
    int counter = 0;
    for (int i = 0; i < fields; i++) {
        for (int j = 0; j < fields; j++) {
            if (m_circleBoard[i][j] and m_canTakeBoard[i][j] == 0) {
                m_circles[counter].setPosition(j * tileSize + 40, i * tileSize + 40);
                m_circles[counter].setRadius(5);
                counter++;
            }
        }
    }
}
void Chess::deleteCircle() {
    for (int i = 0; i < fields; i++) {
        for (int j = 0; j < fields; j++) {
            m_circleBoard[i][j] = false;
        }
    }
    for (int i = 0; i < m_circles.size(); i++) {
        m_circles[i].setRadius(0);
    }
}
void Chess::setCircle() {
    if (m_pieceClicked) {
        if (m_whiteTurn and m_currentPieceClicked < 0) {
            return;
        }
        if (!m_whiteTurn and m_currentPieceClicked > 0) {
            return;
        }

        if (m_currentPieceClicked >= 60 and m_currentPieceClicked <= 67) {
            if (m_currentPiecePosition.x == 6) {
                if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                if (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y] == 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    m_circleBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] < 0) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                    m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y < 7 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] < 0) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;
                    m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1];
                }


            }
            else {
                if (m_currentPiecePosition.x > 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] < 0) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                    m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y < 7 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] < 0) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;
                    m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1];
                }

            }
        }
        else if (m_currentPieceClicked == 10 or m_currentPieceClicked == 11) {
            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }

        }
        else if (m_currentPieceClicked == 20 or m_currentPieceClicked == 21) {
            if (m_currentPiecePosition.x - 2 >= 0) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1];
                }
            }
            if (m_currentPiecePosition.x + 2 <= 7) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1];
                }
            }
            if (m_currentPiecePosition.y + 2 <= 7) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2];
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2];
                }
            }
            if (m_currentPiecePosition.y - 2 >= 0) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2];
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2];
                }
            }

        }
        else if (m_currentPieceClicked == 30 or m_currentPieceClicked == 31) {
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
        }
        else if (m_currentPieceClicked == 40 or m_currentPieceClicked == 41 or m_currentPieceClicked == 42 or m_currentPieceClicked == 43 or m_currentPieceClicked == 44
            or m_currentPieceClicked == 45 or m_currentPieceClicked == 46 or m_currentPieceClicked == 47 or m_currentPieceClicked == 48) {
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }


            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }
        }
        else if (m_currentPieceClicked == 50) {
            if (m_currentPiecePosition.x - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y];
                }

                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1];
                }
            }
            if (m_currentPiecePosition.x + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y];
                }

                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] < 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1];
                }
            }

            if (m_currentPiecePosition.y - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] == 0)
                    m_circleBoard[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] = true;
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] < 0)
                    m_canTakeBoard[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1];
            }
            if (m_currentPiecePosition.y + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] == 0)
                    m_circleBoard[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] = true;
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] < 0)
                    m_canTakeBoard[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1];
            }


            if (!m_whiteKingMoved and !m_whiteRightRookMoved) {
                if (a_board[7][6] == 0 and a_board[7][5] == 0) {
                    m_circleBoard[7][6] = true;

                }
            }
            if (!m_whiteKingMoved and !m_whiteLeftRookMoved) {
                if (a_board[7][1] == 0 and a_board[7][2] == 0 and a_board[7][3] == 0) {
                    m_circleBoard[7][2] = true;

                }
            }


        }
        if (m_currentPieceClicked <= -60 and m_currentPieceClicked >= -67) {
            if (m_currentPiecePosition.x == 1) {
                if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                if (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y] == 0 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    m_circleBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1];
                }

                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y < 8 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1];
                }


            }
            else {
                if (m_currentPiecePosition.x + 1 < 8 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1];
                }

                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y < 7 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1];
                }

            }
        }
        else if (m_currentPieceClicked == -10 or m_currentPieceClicked == -11) {
            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }


        }
        else if (m_currentPieceClicked == -20 or m_currentPieceClicked == -21) {
            if (m_currentPiecePosition.x - 2 >= 0) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1];
                }
            }
            if (m_currentPiecePosition.x + 2 <= 7) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1];
                }
            }
            if (m_currentPiecePosition.y + 2 <= 7) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2];
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2];
                }
            }
            if (m_currentPiecePosition.y - 2 >= 0) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2];
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] == 0)) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2];
                }
            }


        }
        else if (m_currentPieceClicked == -30 or m_currentPieceClicked == -31) {
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }

        }
        else if (m_currentPieceClicked == -40 or m_currentPieceClicked == -41 or m_currentPieceClicked == -42 or m_currentPieceClicked == -43 or m_currentPieceClicked == -44
            or m_currentPieceClicked == -45 or m_currentPieceClicked == -46 or m_currentPieceClicked == -47 or m_currentPieceClicked == -48) {
            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    m_circleBoard[i][m_currentPiecePosition.y] = true;
                    m_canTakeBoard[i][m_currentPiecePosition.y] = a_board[i][m_currentPiecePosition.y];
                    break;
                }
                m_circleBoard[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    m_circleBoard[m_currentPiecePosition.x][i] = true;
                    m_canTakeBoard[m_currentPiecePosition.x][i] = a_board[m_currentPiecePosition.x][i];
                    break;
                }
                m_circleBoard[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    m_circleBoard[i][j] = true;
                    m_canTakeBoard[i][j] = a_board[i][j];
                    break;
                }
                m_circleBoard[i][j] = true;
            }
        }
        else if (m_currentPieceClicked == -50) {
            if (m_currentPiecePosition.x - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0) {
                    m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y];
                }

                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1];
                }
            }
            if (m_currentPiecePosition.x + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0) {
                    m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y];
                }

                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1];
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] == 0)
                        m_circleBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0)
                        m_canTakeBoard[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1];
                }
            }

            if (m_currentPiecePosition.y - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] == 0)
                    m_circleBoard[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] = true;
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] > 0)
                    m_canTakeBoard[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] = a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1];
            }
            if (m_currentPiecePosition.y + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] == 0)
                    m_circleBoard[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] = true;
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] > 0)
                    m_canTakeBoard[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] = a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1];
            }


            if (!m_blackKingMoved and !m_blackRightRookMoved) {
                if (a_board[0][6] == 0 and a_board[0][5] == 0) {
                    m_circleBoard[0][6] = true;

                }
            }
            if (!m_blackKingMoved and !m_blackLeftRookMoved) {
                if (a_board[0][1] == 0 and a_board[0][2] == 0 and a_board[0][3] == 0) {
                    m_circleBoard[0][2] = true;

                }
            }
        }
    }
}
bool Chess::loadAssets() {
    // load board
    if (!t_board.loadFromFile("assets/_composite.png")) {
        std::cout << "Error opening board!" << std::endl;
        return false;
    }
    m_board.setTexture(t_board);

    // load pieces
    if (!t_blackRook.loadFromFile("assets/b_rook_png_128px.png") or !t_blackKnight.loadFromFile("assets/b_knight_png_128px.png") or !t_blackBishop.loadFromFile("assets/b_bishop_png_128px.png") or
        !t_blackQueen.loadFromFile("assets/b_queen_png_128px.png") or !t_blackKing.loadFromFile("assets/b_king_png_128px.png") or !t_blackPawn.loadFromFile("assets/b_pawn_png_128px.png") or
        !t_whiteRook.loadFromFile("assets/w_rook_png_128px.png") or !t_whiteKnight.loadFromFile("assets/w_knight_png_128px.png") or !t_whiteBishop.loadFromFile("assets/w_bishop_png_128px.png") or
        !t_whiteQueen.loadFromFile("assets/w_queen_png_128px.png") or !t_whiteKing.loadFromFile("assets/w_king_png_128px.png") or !t_whitePawn.loadFromFile("assets/w_pawn_png_128px.png")) {
        std::cout << "Error opening pieces!" << std::endl;
        return false;
    }

    if (!b_pieceMove.loadFromFile("assets/sounds/move-self.wav") or !b_capture.loadFromFile("assets/sounds/capture.wav")) {
        std::cout << "Error opening sound files!" << std::endl;
        return false;
    }
    s_pieceMove.setBuffer(b_pieceMove);
    s_capture.setBuffer(b_capture);

    sf::Sprite blackRook;
    blackRook.setTexture(t_blackRook);
    m_pieces.push_back(blackRook);

    sf::Sprite blackKnight;
    blackKnight.setTexture(t_blackKnight);
    m_pieces.push_back(blackKnight);

    sf::Sprite blackBishop;
    blackBishop.setTexture(t_blackBishop);
    m_pieces.push_back(blackBishop);

    sf::Sprite blackQueen;
    blackQueen.setTexture(t_blackQueen);
    m_pieces.push_back(blackQueen);

    sf::Sprite blackKing;
    blackKing.setTexture(t_blackKing);
    m_pieces.push_back(blackKing);

    sf::Sprite blackBishop2;
    blackBishop2.setTexture(t_blackBishop);
    m_pieces.push_back(blackBishop2);

    sf::Sprite blackKnight2;
    blackKnight2.setTexture(t_blackKnight);
    m_pieces.push_back(blackKnight2);

    sf::Sprite blackRook2;
    blackRook2.setTexture(t_blackRook);
    m_pieces.push_back(blackRook2);

    for (int i = 0; i < fields; i++) {
        sf::Sprite blackPawn;
        blackPawn.setTexture(t_blackPawn);
        m_pieces.push_back(blackPawn);
    }
    for (int i = 0; i < fields; i++) {
        sf::Sprite queen;
        queen.setTexture(t_whiteQueen);
        queen.setScale(0, 0);
        m_pawnPromotionQueensW.push_back(queen);
    }
    for (int i = 0; i < fields; i++) {
        sf::Sprite queen;
        queen.setTexture(t_blackQueen);
        queen.setScale(0, 0);
        m_pawnPromotionQueensB.push_back(queen);
    }

    sf::Sprite whiteRook;
    whiteRook.setTexture(t_whiteRook);
    m_pieces.push_back(whiteRook);

    sf::Sprite whiteKnight;
    whiteKnight.setTexture(t_whiteKnight);
    m_pieces.push_back(whiteKnight);

    sf::Sprite whiteBishop;
    whiteBishop.setTexture(t_whiteBishop);
    m_pieces.push_back(whiteBishop);

    sf::Sprite whiteQueen;
    whiteQueen.setTexture(t_whiteQueen);
    m_pieces.push_back(whiteQueen);

    sf::Sprite whiteKing;
    whiteKing.setTexture(t_whiteKing);
    m_pieces.push_back(whiteKing);

    sf::Sprite whiteBishop2;
    whiteBishop2.setTexture(t_whiteBishop);
    m_pieces.push_back(whiteBishop2);

    sf::Sprite whiteKnight2;
    whiteKnight2.setTexture(t_whiteKnight);
    m_pieces.push_back(whiteKnight2);

    sf::Sprite whiteRook2;
    whiteRook2.setTexture(t_whiteRook);
    m_pieces.push_back(whiteRook2);

    for (int i = 0; i < fields; i++) {
        sf::Sprite whitePawn;
        whitePawn.setTexture(t_whitePawn);
        m_pieces.push_back(whitePawn);
    }
    for (auto& p : m_pieces) {
        p.setScale(0.5, 0.5);
    }
    return true;
}
void Chess::updateBoard() {
    int offset = 16;

    for (int i = 0; i < fields; i++) {
        for (int j = 0; j < fields; j++) {

            for (int g = 0, f = -10; g <= 4 and f >= -50; g++, f -= 10) {
                if (a_board[i][j] == f)
                    m_pieces[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
            for (int g = 5, f = -31; g <= 7 and f <= -11; g++, f += 10) {
                if (a_board[i][j] == f)
                    m_pieces[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
            for (int g = 8, f = -60; g <= 15 and f >= -67; g++, f--) {
                if (a_board[i][j] == f)
                    m_pieces[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
            for (int g = 16, f = 10; g <= 20 and f <= 50; g++, f+=10) {
                if (a_board[i][j] == f)
                    m_pieces[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
            for (int g = 21, f = 31; g <= 23 and f >= 11; g++, f -= 10) {
                if (a_board[i][j] == f)
                    m_pieces[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
            for (int g = 24, f = 60; g <= 31 and f <= 67; g++, f++) {
                if (a_board[i][j] == f)
                    m_pieces[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
            for (int g = 0, f = 41; g <= 7 and f <= 48; g++, f++) {
                if (a_board[i][j] == f)
                    m_pawnPromotionQueensW[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
            for (int g = 0, f = -41; g <= 7 and f >= -48; g++, f--) {
                if (a_board[i][j] == f)
                    m_pawnPromotionQueensB[g].setPosition(tileSize * j + offset, tileSize * i + offset);
            }
        }
    }
}
void Chess::move(sf::RenderWindow& window) {

    int row = sf::Mouse::getPosition(window).y / tileSize;
    int col = sf::Mouse::getPosition(window).x / tileSize;

    if (!m_pieceClicked) {
        m_pieceClicked = true;
        m_currentPieceClicked = a_board[row][col];
        m_currentPiecePosition.x = row;
        m_currentPiecePosition.y = col;
        return;
    }
    if (m_whiteTurn and m_pieceClicked) {
        if (a_board[row][col] > 0) {

            m_currentPieceClicked = a_board[row][col];
            m_currentPiecePosition.x = row;
            m_currentPiecePosition.y = col;
            for (int i = 0; i < m_pieces.size(); i++) {
                m_pieces[i].setColor(sf::Color(255, 255, 255, 255));
            }
            for (int i = 0; i < m_pawnPromotionQueensW.size(); i++) {
                m_pawnPromotionQueensW[i].setColor(sf::Color(255, 255, 255, 255));
                m_pawnPromotionQueensB[i].setColor(sf::Color(255, 255, 255, 255));
            }
            for (int i = 0; i < fields; i++) {
                for (int j = 0; j < fields; j++) {
                    m_canTakeBoard[i][j] = 0;
                }

            }

            return;
        }

        m_pieceClicked = false;

        for (int i = 0; i < fields; i++) {
            for (int j = 0; j < fields; j++) {
                m_circleBoard[i][j] = false;
                m_canTakeBoard[i][j] = 0;
            }

        }
        for (int i = 0; i < m_pieces.size(); i++) {
            m_pieces[i].setColor(sf::Color(255, 255, 255, 255));
        }


        if (m_currentPieceClicked >= 60 and m_currentPieceClicked <= 67) {
            if (m_currentPiecePosition.x == 6) {
                if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                if (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y] == 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] < 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y < 7 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] < 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;

                if (b_board[row][col]) {
                    deletePieces(row, col);
                    a_board[row][col] = m_currentPieceClicked;
                    a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                    m_whiteTurn = false;
                    s_pieceMove.play();

                    m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                }

            }
            else {
                if (m_currentPiecePosition.x > 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] < 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                if (m_currentPiecePosition.x > 0 and m_currentPiecePosition.y < 7 and a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] < 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;

                if (b_board[row][col]) {
                    deletePieces(row, col);
                    a_board[row][col] = m_currentPieceClicked;
                    a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                    m_whiteTurn = false;
                    s_pieceMove.play();

                    m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                }
            }
        }
        else if (m_currentPieceClicked == 10 or m_currentPieceClicked == 11) {
            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }

            if (b_board[row][col]) {
                if (m_currentPieceClicked == 10)
                    m_whiteLeftRookMoved = true;
                if (m_currentPieceClicked == 11)
                    m_whiteRightRookMoved = true;

                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = false;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }

        }
        else if (m_currentPieceClicked == 20 or m_currentPieceClicked == 21) {
            if (m_currentPiecePosition.x - 2 >= 0) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] == 0)) {
                    b_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] == 0)) {
                    b_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] = true;
                }
            }
            if (m_currentPiecePosition.x + 2 <= 7) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] == 0)) {
                    b_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] == 0)) {
                    b_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] = true;
                }
            }
            if (m_currentPiecePosition.y + 2 <= 7) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] == 0)) {
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] = true;
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] == 0)) {
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] = true;
                }
            }
            if (m_currentPiecePosition.y - 2 >= 0) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] == 0)) {
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] = true;
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] == 0)) {
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] = true;
                }
            }


            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = false;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }
        }
        else if (m_currentPieceClicked == 30 or m_currentPieceClicked == 31) {
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }

            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = false;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }
        }
        else if (m_currentPieceClicked == 40 or m_currentPieceClicked == 41 or m_currentPieceClicked == 42 or m_currentPieceClicked == 43 or m_currentPieceClicked == 44
            or m_currentPieceClicked == 45 or m_currentPieceClicked == 46 or m_currentPieceClicked == 47 or m_currentPieceClicked == 48) {
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] > 0)
                    break;
                if (a_board[i][j] < 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] > 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] < 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] > 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] < 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }


            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = false;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }
        }
        else if (m_currentPieceClicked == 50) {
            if (m_currentPiecePosition.x - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] == 0)
                        b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] == 0)
                        b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;
                }
            }
            if (m_currentPiecePosition.x + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] == 0)
                        b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] == 0)
                        b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;
                }
            }

            if (m_currentPiecePosition.y - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] < 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] == 0)
                    b_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] = true;
            }
            if (m_currentPiecePosition.y + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] < 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] == 0)
                    b_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] = true;
            }

            bool castleWithRightRook = false;
            bool castleWithLeftRook = false;
            if (!m_whiteKingMoved and !m_whiteRightRookMoved) {
                if (a_board[7][6] == 0 and a_board[7][5] == 0) {
                    b_board[7][6] = true;
                    castleWithRightRook = true;
                }
            }
            if (!m_whiteKingMoved and !m_whiteLeftRookMoved) {
                if (a_board[7][1] == 0 and a_board[7][2] == 0 and a_board[7][3] == 0) {
                    b_board[7][2] = true;
                    castleWithLeftRook = true;
                }
            }

            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = false;

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);

                if (castleWithRightRook and row == 7 and col == 6) {
                    m_posSquares.first.setPosition(5 * tileSize, 7 * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                    a_board[7][7] = 0;
                    a_board[7][5] = 11;
                    m_whiteRightRookMoved = true;
                }
                if (castleWithLeftRook and row == 7 and col == 2) {
                    m_posSquares.first.setPosition(3 * tileSize, 7 * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                    a_board[7][0] = 0;
                    a_board[7][3] = 10;
                    m_whiteLeftRookMoved = true;
                }
                m_whiteKingMoved = true;
                s_pieceMove.play();
            }
        }
    }
    else if (!m_whiteTurn and m_pieceClicked) {
        if (a_board[row][col] < 0) {
            m_currentPieceClicked = a_board[row][col];
            m_currentPiecePosition.x = row;
            m_currentPiecePosition.y = col;
            for (int i = 0; i < m_pieces.size(); i++) {
                m_pieces[i].setColor(sf::Color(255, 255, 255, 255));
            }
            for (int i = 0; i < m_pawnPromotionQueensW.size(); i++) {
                m_pawnPromotionQueensW[i].setColor(sf::Color(255, 255, 255, 255));
                m_pawnPromotionQueensB[i].setColor(sf::Color(255, 255, 255, 255));
            }
            for (int i = 0; i < fields; i++) {
                for (int j = 0; j < fields; j++) {
                    m_canTakeBoard[i][j] = 0;
                }

            }
            return;
        }
        m_pieceClicked = false;

        for (int i = 0; i < m_pieces.size(); i++) {
            m_pieces[i].setColor(sf::Color(255, 255, 255, 255));
        }

        if (m_currentPieceClicked <= -60 and m_currentPieceClicked >= -67) {
            if (m_currentPiecePosition.x == 1) {
                if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                if (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y] == 0 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y < 8 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;

                if (b_board[row][col]) {
                    deletePieces(row, col);
                    a_board[row][col] = m_currentPieceClicked;
                    a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                    m_whiteTurn = true;
                    s_pieceMove.play();

                    m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                }

            }
            else {
                if (m_currentPiecePosition.x + 1 < 8 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y > 0 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                if (m_currentPiecePosition.x < 7 and m_currentPiecePosition.y < 7 and a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;

                if (b_board[row][col]) {
                    deletePieces(row, col);
                    a_board[row][col] = m_currentPieceClicked;
                    a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                    m_whiteTurn = true;
                    s_pieceMove.play();

                    m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                }
            }
        }
        else if (m_currentPieceClicked == -10 or m_currentPieceClicked == -11) {
            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }

            if (b_board[row][col]) {
                if (m_currentPieceClicked == -10)
                    m_blackLeftRookMoved = true;
                if (m_currentPieceClicked == -11)
                    m_blackRightRookMoved = true;

                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = true;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }

        }
        else if (m_currentPieceClicked == -20 or m_currentPieceClicked == -21) {
            if (m_currentPiecePosition.x - 2 >= 0) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] == 0)) {
                    b_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] == 0)) {
                    b_board[m_currentPiecePosition.x - 2][m_currentPiecePosition.y + 1] = true;
                }
            }
            if (m_currentPiecePosition.x + 2 <= 7) {
                if (m_currentPiecePosition.y - 1 >= 0 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] == 0)) {
                    b_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7 and (a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] == 0)) {
                    b_board[m_currentPiecePosition.x + 2][m_currentPiecePosition.y + 1] = true;
                }
            }
            if (m_currentPiecePosition.y + 2 <= 7) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] == 0)) {
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 2] = true;
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] == 0)) {
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 2] = true;
                }
            }
            if (m_currentPiecePosition.y - 2 >= 0) {
                if (m_currentPiecePosition.x - 1 >= 0 and (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] == 0)) {
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 2] = true;
                }
                if (m_currentPiecePosition.x + 1 <= 7 and (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] == 0)) {
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 2] = true;
                }
            }


            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = true;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }
        }
        else if (m_currentPieceClicked == -30 or m_currentPieceClicked == -31) {
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }

            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = true;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }
        }
        else if (m_currentPieceClicked == -40 or m_currentPieceClicked == -41 or m_currentPieceClicked == -42 or m_currentPieceClicked == -43 or m_currentPieceClicked == -44
            or m_currentPieceClicked == -45 or m_currentPieceClicked == -46 or m_currentPieceClicked == -47 or m_currentPieceClicked == -48) {
            for (int i = m_currentPiecePosition.x; i >= 0; i--) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.x; i < 8; i++) {
                if (i == m_currentPiecePosition.x)
                    continue;
                if (a_board[i][m_currentPiecePosition.y] < 0)
                    break;
                if (a_board[i][m_currentPiecePosition.y] > 0) {
                    b_board[i][m_currentPiecePosition.y] = true;
                    break;
                }
                b_board[i][m_currentPiecePosition.y] = true;
            }
            for (int i = m_currentPiecePosition.y; i < 8; i++) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.y; i >= 0; i--) {
                if (i == m_currentPiecePosition.y)
                    continue;
                if (a_board[m_currentPiecePosition.x][i] < 0)
                    break;
                if (a_board[m_currentPiecePosition.x][i] > 0) {
                    b_board[m_currentPiecePosition.x][i] = true;
                    break;
                }
                b_board[m_currentPiecePosition.x][i] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j >= 0; i--, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i >= 0 and j <= 7; i--, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j >= 0; i++, j--) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }
            for (int i = m_currentPiecePosition.x, j = m_currentPiecePosition.y; i <= 7 and j <= 7; i++, j++) {
                if (i == m_currentPiecePosition.x and j == m_currentPiecePosition.y)
                    continue;
                if (a_board[i][j] < 0)
                    break;
                if (a_board[i][j] > 0) {
                    b_board[i][j] = true;
                    break;
                }
                b_board[i][j] = true;
            }

            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = true;
                s_pieceMove.play();

                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
            }
        }
        else if (m_currentPieceClicked == -50) {
            if (m_currentPiecePosition.x - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] == 0)
                        b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] == 0)
                        b_board[m_currentPiecePosition.x - 1][m_currentPiecePosition.y + 1] = true;
                }
            }
            if (m_currentPiecePosition.x + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] == 0)
                    b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y] = true;
                if (m_currentPiecePosition.y - 1 >= 0) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] == 0)
                        b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y - 1] = true;
                }
                if (m_currentPiecePosition.y + 1 <= 7) {
                    if (a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] == 0)
                        b_board[m_currentPiecePosition.x + 1][m_currentPiecePosition.y + 1] = true;
                }
            }

            if (m_currentPiecePosition.y - 1 >= 0) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] > 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] == 0)
                    b_board[m_currentPiecePosition.x][m_currentPiecePosition.y - 1] = true;
            }
            if (m_currentPiecePosition.y + 1 <= 7) {
                if (a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] > 0 or a_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] == 0)
                    b_board[m_currentPiecePosition.x][m_currentPiecePosition.y + 1] = true;
            }

            bool castleWithRightRook = false;
            bool castleWithLeftRook = false;
            if (!m_blackKingMoved and !m_blackRightRookMoved) {
                if (a_board[0][6] == 0 and a_board[0][5] == 0) {
                    b_board[0][6] = true;
                    castleWithRightRook = true;
                }
            }
            if (!m_blackKingMoved and !m_blackLeftRookMoved) {
                if (a_board[0][1] == 0 and a_board[0][2] == 0 and a_board[0][3] == 0) {
                    b_board[0][2] = true;
                    castleWithLeftRook = true;
                }
            }

            if (b_board[row][col]) {
                deletePieces(row, col);
                a_board[row][col] = m_currentPieceClicked;
                a_board[m_currentPiecePosition.x][m_currentPiecePosition.y] = 0;
                m_whiteTurn = true;
                m_posSquares.first.setPosition(m_currentPiecePosition.y * tileSize, m_currentPiecePosition.x * tileSize);
                m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                if (castleWithRightRook and row == 0 and col == 6) {
                    m_posSquares.first.setPosition(5 * tileSize, 0 * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                    a_board[0][7] = 0;
                    a_board[0][5] = -11;
                    m_blackRightRookMoved = true;
                }
                if (castleWithLeftRook and row == 0 and col == 2) {
                    m_posSquares.first.setPosition(3 * tileSize, 0 * tileSize);
                    m_posSquares.second.setPosition(col * tileSize, row * tileSize);
                    a_board[0][0] = 0;
                    a_board[0][3] = -10;
                    m_blackLeftRookMoved = true;
                }
                m_blackKingMoved = true;
                s_pieceMove.play();
            }
        }
    }

    for (int i = 0; i < fields; i++) {
        for (int j = 0; j < fields; j++) {
            b_board[i][j] = false;
        }
    }
}
void Chess::deletePieces(int row, int col) {
    for (int g = 8, f = -60; g <= 15 and f >= -67; g++, f--) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
    for (int g = 0, f = -10; g <= 4 and f >= -50; g++, f-=10) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
    for (int g = 5, f = -31; g <= 7 and f <= -11; g++, f += 10) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
    for (int g = 16, f = 10; g <= 20 and f <= 50; g++, f += 10) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
    for (int g = 21, f = 31; g <= 23 and f >= 11; g++, f -= 10) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
    for (int g = 24, f = 60; g <= 31 and f <= 67; g++, f++) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
    for (int g = 0, f = 41; g <= 7 and f <= 48; g++, f++) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
    for (int g = 0, f = -41; g <= 7 and f >= -48; g++, f--) {
        if (a_board[row][col] == f) {
            m_pieces[g].setScale(0, 0);
            s_capture.play();
        }
    }
}

void Chess::pawnPromotion() {
    int offset = 16;
    for (int i = 0; i < fields; i++) {
        if (a_board[0][i] >= 60 and a_board[0][i] <= 67) {

            for (int g = 24, f = 60; g <= 31 and f <= 67; g++, f++){
                if (a_board[0][i] == f)
                    m_pieces[g].setScale(0, 0);
            }
            a_board[0][i] = 41 + m_pawnPromotionCounterW;
            m_pawnPromotionQueensW[m_pawnPromotionCounterW].setScale(0.5, 0.5);
            m_pawnPromotionQueensW[m_pawnPromotionCounterW].setPosition(i * tileSize + offset, offset);
            m_pawnPromotionCounterW++;
        }
        else if (a_board[7][i] <= -60 and a_board[7][i] >= -67) {
            for (int g = 8, f = -60; g <= 15 and f <= 67; g++, f--) {
                if (a_board[7][i] == f)
                    m_pieces[g].setScale(0, 0);
            }
            a_board[7][i] = -41 - m_pawnPromotionCounterB;
            m_pawnPromotionQueensB[m_pawnPromotionCounterB].setScale(0.5, 0.5);
            m_pawnPromotionQueensB[m_pawnPromotionCounterB].setPosition(i * tileSize + offset, 7 * tileSize + offset);
            m_pawnPromotionCounterB++;
        }
    }
}
