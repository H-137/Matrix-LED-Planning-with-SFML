/* 
 * LED Matrix Simulator
 * Author: Matthew Leopold - leopolmb@bc.edu
 * C++ program to simulate an LED matrix I am planning to build later with aurdiuno
*/


#include <iostream>
#include <stdlib.h> 
#include <SFML/Graphics.hpp>
#include <list>

class Dot {
    public:
        Dot() {
            dot.setRadius(10.f);
            dot.setPosition(0,0);
            dot.setOutlineColor(sf::Color(255,255,255));
            dot.setOutlineThickness(2);
            dot.setFillColor(sf::Color(255,0,0));
        }
        void draw(sf::RenderWindow& window) {
            window.draw(dot);
        }
        void setColor(sf::Color color) {
            dot.setFillColor(color);
        }
        void setPos(int x, int y) {
            dot.setPosition(x*50 + 12, y*50 + 12);
        }
        bool lerpColor(sf::Color new_color) {
            sf::Color current_color = dot.getFillColor();
            if (current_color.r < new_color.r) {
                current_color.r += 1;
            } else if (current_color.r > new_color.r) {
                current_color.r -= 1;
            }
            if (current_color.g < new_color.g) {
                current_color.g += 1;
            } else if (current_color.g > new_color.g) {
                current_color.g -= 1;
            }
            if (current_color.b < new_color.b) {
                current_color.b += 1;
            } else if (current_color.b > new_color.b) {
                current_color.b -= 1;
            }
            dot.setFillColor(current_color);
            if (current_color == new_color) {
                return true;
            }
            return false;
        }
        void printColor() {
            sf::Color color = dot.getFillColor();
            std::cout << "R: " << color.r << " G: " << color.g << " B: " << color.b << std::endl;
        }
        
    private:
        sf::CircleShape dot;
};

class ButtonBox {
    public:
        ButtonBox(int x) {
            button.setSize(sf::Vector2f(50, 50));
            button.setPosition(x, 512);
            button.setFillColor(sf::Color(100, 200,200));
            button.setOutlineColor(sf::Color(255,255,255));
            button.setOutlineThickness(2);
        }
        void draw(sf::RenderWindow& window) {
            window.draw(button);
        }
        bool isClicked(sf::RenderWindow& window) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                return true;
            }
            return false;
        }
    private:
        sf::RectangleShape button;
};

void setPositions(Dot dot[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            dot[i][j].setPos(i, j);
        }
    }
}

void drawCircles(Dot dot[10][10], sf::RenderWindow& window) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            dot[i][j].draw(window);
        }
    }
}

/*
 * Wave Pattern going from top left to bottom right
 * KNOWN ISSUES: Unable to add other effeccts while wave is running
*/
void wavePattern(Dot dot[10][10], sf::RenderWindow& window, int speed) {
    sf::Clock clock;
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    sf::Color color(r, g, b);

    int row = 1;
    int done = 0;
    while (done < 100) {
        clock.restart();
        while (clock.getElapsedTime().asMilliseconds() < speed * 20) {
            done = 0;
            for (int i = 0; i < row; i++) {
                for (int j = i; j >= 0; j--) {
                    if (j < 10 && i-j < 10) {
                        if (dot[j][i-j].lerpColor(color)) {
                            done++;
                        }
                    }
                }
            }
            drawCircles(dot, window);
            window.display();
            sf::sleep(sf::milliseconds(speed));
        }
        if (row < 20) {
            row++;
        }
    }
}

/*
 * Random Pattern changing all colors individually
*/
void randomPattern(Dot dot[10][10], sf::RenderWindow& window) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int r = rand() % 255;
            int g = rand() % 255;
            int b = rand() % 255;
            sf::Color color(r, g, b);
            dot[i][j].setColor(color);
        }
    }
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(500, 600), "LED Matrix", sf::Style::Default, settings);

    Dot dot[10][10];
    setPositions(dot);

    ButtonBox waveButton(12);
    ButtonBox randomButton(77);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color(0,0,0));

        drawCircles(dot, window);

        randomButton.draw(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (randomButton.isClicked(window)) {
                randomPattern(dot, window);
            }
        }

        waveButton.draw(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (waveButton.isClicked(window)) {
                wavePattern(dot, window, 5);
            }
        }

        window.display();
    }
    return 0;
}