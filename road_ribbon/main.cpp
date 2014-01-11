#include <QCoreApplication>
#include <QString>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "gpsdataparser.h"
#include "plainroadbuilder.h"

int main(int argc, char *argv[])
{
    /*if (argc < 2)
    {
        printf("Not enough arguments\n");
        printf("Usage: %s [GPS data file]\n", argv[0]);

        return 1;
    }*/

    sf::RenderWindow window(sf::VideoMode(800, 600), "RoadRibbon", sf::Style::Default, sf::ContextSettings(32));

    window.setVerticalSyncEnabled(true);

    // GPSDataParser parser(argv[1]);
    GPSDataParser parser("gprmc.txt");

    // parser.toJS(argv[2]);

    sf::Text text;

    sf::Font font;

    font.loadFromFile("arial.ttf");

    // select the font
    text.setFont(font); // font is a sf::Font

    // set the string to display
    text.setString("Hello world");

    // set the character size
    text.setCharacterSize(24); // in pixels, not points!

    // set the color
    text.setColor(sf::Color::Red);

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    PlainRoadBuilder roadBuilder(parser.points);

    sf::ConvexShape convex;

    /*convex.setPointCount(roadBuilder.points.size());

    for (int i = 0; i < roadBuilder.points.size(); i++)
    {
        convex.setPoint(i, sf::Vector2f(roadBuilder.points[i].x, roadBuilder.points[i].y));
    }*/

    // resize it to 5 points
    convex.setPointCount(5);

    // define the points
    convex.setPoint(0, sf::Vector2f(0, 0));
    convex.setPoint(1, sf::Vector2f(150, 10));
    convex.setPoint(2, sf::Vector2f(120, 90));
    convex.setPoint(3, sf::Vector2f(30, 100));
    convex.setPoint(4, sf::Vector2f(0, 50));

    convex.setFillColor(sf::Color(155, 155, 50));

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            } else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // window.pushGLStates();

        window.draw(convex);

        // window.popGLStates();

        window.display();
    }

    return 0;
}
