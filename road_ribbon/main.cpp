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

    PlainRoadBuilder roadBuilder(parser.points, 15.0, PlainRoadBuilder::LINES_ONLY);

    sf::View view = window.getDefaultView();

    view.move(-50, -100);

    window.setView(view);

    for (int i = 0; i < roadBuilder.points.size() - 2; i += 2)
    {
        /*sf::ConvexShape convex;

        convex.setFillColor(sf::Color(0, 0, 0, 0));
        convex.setOutlineColor(sf::Color(100, 100, 150));
        convex.setOutlineThickness(1);

        convex.setPointCount(2);
        convex.setPoint(0, sf::Vector2f(roadBuilder.points[i + 0].x, roadBuilder.points[i + 0].y));
        convex.setPoint(1, sf::Vector2f(roadBuilder.points[i + 1].x, roadBuilder.points[i + 1].y));*/

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(roadBuilder.points[i + 0].x, roadBuilder.points[i + 0].y)),
            sf::Vertex(sf::Vector2f(roadBuilder.points[i + 1].x, roadBuilder.points[i + 1].y))
        };

        window.draw(line, 2, sf::Lines);

        /*convex.setPointCount(4);

        convex.setPoint(0, sf::Vector2f(roadBuilder.points[i + 0].x, roadBuilder.points[i + 0].y));
        convex.setPoint(1, sf::Vector2f(roadBuilder.points[i + 1].x, roadBuilder.points[i + 1].y));
        convex.setPoint(2, sf::Vector2f(roadBuilder.points[i + 2].x, roadBuilder.points[i + 2].y));
        convex.setPoint(3, sf::Vector2f(roadBuilder.points[i + 3].x, roadBuilder.points[i + 3].y));*/

        // window.draw(convex);

        window.display();
    }

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            view.move(0, -10);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            view.move(0, 10);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            view.move(-10, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            view.move(10, 0);

        window.setView(view);

        window.display();
    }

    return 0;
}
