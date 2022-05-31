#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <unistd.h>
using namespace std;

typedef std::vector<sf::Vector2f> Rect;

const int maxDepth = 1;
mt19937 generator(time(nullptr));
uniform_real_distribution<> chance(0.0, 1.0);

sf::Color depthColor(int depth) {
    float depthf = depth * 1 / maxDepth;

    if(depthf < 0.5)
    {
        return sf::Color( (1 - depthf / 0.5) * 255, 0, depthf / 0.5 * 255);
    }
    else
    {
        depthf -= 0.5;
        return sf::Color(0, depthf / 0.5 * 255, (1 - depthf / 0.5) * 255);
    }

}

void draw(sf::RenderWindow& window, Rect shape, int depth = 0, float pa1 = 0, float pa2 = 0) {
    sf::ConvexShape rect;
    rect.setPointCount(4);
    for (int i = 0; i < 4; i++) {
        rect.setPoint(i, shape[i]);
    }
    rect.setFillColor(depthColor(depth));
    window.draw(rect);

    if (depth > maxDepth) return;

    float deltaX = shape[0].x - shape[1].x;
    float deltaY = shape[0].y - shape[1].y;
    float baselen = sqrtf(deltaX * deltaX + deltaY * deltaY);
    float nlbaselen = chance(generator) * baselen;
    float angle1 = chance(generator) * -45 - pa1;
    // rule of cosines
    float nrbaselen = sqrtf(nlbaselen * nlbaselen + baselen * baselen - 2 * nlbaselen * baselen * cos(angle1 * M_PI / 180));
    float angle2 = acosf((baselen*baselen + nrbaselen*nrbaselen - nlbaselen*nlbaselen)/(2 * baselen * nrbaselen)) * 180 / M_PI + pa2 ;

    float newX = cos(angle1 * M_PI / 180) * nlbaselen;
    float newY = sin(angle1 * M_PI / 180) * nlbaselen;
    sf::Vector2f nPoint = {shape[0].x + newX, shape[0].y + newY};


    Rect nlSquare = {
            {(float)(shape[0].x - cos((angle1 + 90) * M_PI / 180) * nlbaselen),
             (float)(shape[0].y - sin((angle1 + 90) * M_PI / 180) * nlbaselen)},
            {(float)(nPoint.x   - cos((angle1 + 90) * M_PI / 180) * nlbaselen),
             (float)(nPoint.y   - sin((angle1 + 90) * M_PI / 180) * nlbaselen)},
            nPoint,
            shape[0],
    };

    Rect nrSquare = {
            {(float)(nPoint.x   - cos((angle2 + 90) * M_PI / 180) * nrbaselen),
             (float)(nPoint.y   - sin((angle2 + 90) * M_PI / 180) * nrbaselen)},
            {(float)(shape[1].x - cos((angle2 + 90) * M_PI / 180) * nrbaselen),
             (float)(shape[1].y - sin((angle2 + 90) * M_PI / 180) * nrbaselen)},
            shape[1],
            nPoint,
    };

    cout << angle2 << "  " << angle2 + 90 << endl;
    /*for (auto i : nrSquare) {
        cout << i.x << " " << i.y << endl;
    }*/


    draw(window, nlSquare, depth+1, angle1, angle1);
    draw(window, nrSquare, depth+1, angle2, angle2);
}

int main() {
    // SFML Setup
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    sf::VideoMode mode = sf::VideoMode(1920, 1080, 32);
    sf::RenderWindow window(mode, "Fractal", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    /*sf::Font font;
    font.loadFromFile("../arial.ttf");
    sf::Text p1p;
    sf::Text p2p;
    p1p.setFont(font);
    p2p.setFont(font);
    p1p.setCharacterSize(20);
    p2p.setCharacterSize(20);
    //tFPS.setFillColor(Color::White);
    //tFPS.setStyle(Text::Bold);
    p1p.setPosition(200, 30);
    p2p.setPosition(200, 60);*/

     Rect sqaure ={
             {475, 775},
             {525, 775},
             {525, 825},
             {475, 825}
     };

    window.clear();
    draw(window, sqaure);
    window.display();
    // Main loop

    while (window.isOpen()) {
        sf::Event event{};
        //sf::Clock clock;

        // Closing
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        //clock.restart();
    }

    return 0;
}