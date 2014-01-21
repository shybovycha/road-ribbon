#include <QCoreApplication>
#include <QString>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "gpsdataparser.h"
#include "plainroadbuilder.h"
#include "camera.h"

int main(int argc, char *argv[])
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "RoadRibbon", sf::Style::Default, settings);

    window.setVerticalSyncEnabled(true);

    GPSDataParser parser("roadribbon_16012014_113134.txt");

    PlainRoadBuilder roadBuilder(parser.centers, 15.0);

    Camera camera;

    roadBuilder.setUpCamera(camera);

    glShadeModel(GL_SMOOTH);

    bool windowIsActive = true;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();

                gluPerspective(90.0f, (float) event.size.width / (float) event.size.height, 0.1f, 10000.0f);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
            }

            if (event.type == sf::Event::LostFocus)
            {
                windowIsActive = false;
            }

            if (event.type == sf::Event::GainedFocus)
            {
                windowIsActive = true;
            }
        }

        if (!windowIsActive)
        {
            continue;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            camera.rotate(Vector3d(0, -M_PI / 100.0, 0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            camera.rotate(Vector3d(0, M_PI / 100.0, 0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            camera.rotate(Vector3d(-M_PI / 100.0, 0, 0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            camera.rotate(Vector3d(M_PI / 100.0, 0, 0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            camera.move(Vector3d(0, 0, -1.0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.move(Vector3d(0, 0, 1.0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            camera.move(Vector3d(-1.0, 0, 0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            camera.move(Vector3d(1.0, 0, 0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            camera.move(Vector3d(0, 1, 0));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            camera.move(Vector3d(0, -1, 0));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.render();

        roadBuilder.render(PlainRoadBuilder::SURFACE, &window);

        glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(camera.lookAt.x, camera.lookAt.y, camera.lookAt.z);
            glVertex3f(camera.lookAt.x, camera.lookAt.y + 10, camera.lookAt.z);

            glVertex3f(camera.lookAt.x - 1, camera.lookAt.y + 9, camera.lookAt.z);
            glVertex3f(camera.lookAt.x + 0, camera.lookAt.y + 10, camera.lookAt.z);

            glVertex3f(camera.lookAt.x + 0, camera.lookAt.y + 10, camera.lookAt.z);
            glVertex3f(camera.lookAt.x + 1, camera.lookAt.y + 9, camera.lookAt.z);

            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(camera.lookAt.x, camera.lookAt.y, camera.lookAt.z);
            glVertex3f(camera.lookAt.x + 10, camera.lookAt.y, camera.lookAt.z);

            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(camera.lookAt.x, camera.lookAt.y, camera.lookAt.z);
            glVertex3f(camera.lookAt.x, camera.lookAt.y, camera.lookAt.z + 10);

            glColor3f(0.6, 0.6, 0.5);
            Vector3d v1 = camera.position;
            v1.y = camera.lookAt.y;
            Vector3d v = (camera.lookAt - v1).normalize() * 10.0;
            glVertex3f(camera.lookAt.x, camera.lookAt.y, camera.lookAt.z);
            glVertex3f(camera.lookAt.x + v.x, camera.lookAt.y, camera.lookAt.z + v.z);
        glEnd();

        window.display();
    }

    return 0;
}
