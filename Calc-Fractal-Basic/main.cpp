#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
using namespace std;

// List of coordinates for the squares
typedef std::vector<sf::Vector2f> Rect;

// How many repititions of the fractal
const int maxDepth = 10;

// Color for the squares as they get closer to the final one
sf::Color getColor(int deep)
{
    float deepf = deep * 1.0 / maxDepth;

    if (deepf < 0.5) {
        return sf::Color( (1 - deepf / 0.5) * 255, 0, deepf / 0.5 * 255);
    } else {
        deepf -= 0.5;
        return sf::Color(0, deepf / 0.5 * 255, (1 - deepf / 0.5) * 255);
    }
}

void draw(sf::RenderWindow& window, Rect shape, int depth = 0, float pa = 0) {

    // The actual SFML object to draw and operations on it
    sf::ConvexShape rect;
    rect.setPointCount(4);
    for (int i = 0; i < 4; i++) {
        rect.setPoint(i, shape[i]);
    }
    rect.setFillColor(getColor(depth));
    window.draw(rect);
    window.display();
    sf::sleep(sf::seconds(.001));

    // Stop if we've gone far enough
    if (depth > maxDepth) return;

    // Calculating sizes and positions of new shapes
    float deltaX = shape[0].x - shape[1].x;
    float deltaY = shape[0].y - shape[1].y;
    // Origin square new length
    float baselen = sqrtf(deltaX * deltaX + deltaY * deltaY);
    // New left base length
    float nlbaselen = cos(45 * M_PI / 180) * baselen;
    // New right base length
    float nrbaselen = nlbaselen;

    // Angle of left and right square from the previous angle
    // Effectively relative from previous square
    float angle1 = -45 + pa;
    float angle2 = 45 + pa ;

    // The point where the new squares meet.
    float newX = cos(angle1 * M_PI / 180) * nlbaselen;
    float newY = sin(angle1 * M_PI / 180) * nlbaselen;
    sf::Vector2f nPoint = {shape[0].x + newX, shape[0].y + newY};

    // The new left squares coordinates
    Rect nlSquare = {
            {(float)(shape[0].x - cos((angle1 + 90) * M_PI / 180) * nlbaselen),
             (float)(shape[0].y - sin((angle1 + 90) * M_PI / 180) * nlbaselen)},
            {(float)(nPoint.x   - cos((angle1 + 90) * M_PI / 180) * nlbaselen),
             (float)(nPoint.y   - sin((angle1 + 90) * M_PI / 180) * nlbaselen)},

            nPoint,

            shape[0],
    };

    // The new right squares coordinates
    Rect nrSquare = {
            {(float)(nPoint.x   - cos((angle2 + 90) * M_PI / 180) * nrbaselen),
             (float)(nPoint.y   - sin((angle2 + 90) * M_PI / 180) * nrbaselen)},

            {(float)(shape[1].x - cos((angle2 + 90) * M_PI / 180) * nrbaselen),
             (float)(shape[1].y - sin((angle2 + 90) * M_PI / 180) * nrbaselen)},

            shape[1],

            nPoint,
    };

    // Recursive!
    draw(window, nlSquare, depth+1, angle1);
    draw(window, nrSquare, depth+1, angle2);
}

int main() {
    // SFML Setup
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    sf::VideoMode mode = sf::VideoMode(1000, 1000, 16);
    sf::RenderWindow window(mode, "Fractal", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // The original sqaure
     Rect sqaure ={
             {450, 750},
             {550, 750},
             {550, 850},
             {450, 850}
     };

    window.clear(sf::Color::Black);
    window.display();
     while (true) {
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
             break;
         }
     }
    // DO THE STUFF
    draw(window, sqaure);
    window.display();

    // Main loop to handle closing
    while (window.isOpen()) {
        sf::Event event{};

        // Closing
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::sleep(sf::seconds(10));
    }
    return 0;
}