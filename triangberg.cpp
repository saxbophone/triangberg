#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <triangberg_builder/Drawing.hpp>

// fudge factor, for scaling up graphics. Will do for demos for now.
// TODO: replace with automatic scaling of image to-screen size based on drawing
// size.
const std::size_t SCALE = 28;

int main() {
    using namespace com::saxbophone::triangberg;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Triangberg", sf::Style::Default, settings);

    // don't kill my CPU by running at stupid-fast framerate for no reason
    window.setFramerateLimit(60);

    Unit angle = 1;
    Percentage p = 0.1;

    sf::Event event;
    // click when ready to start
    do {
        window.pollEvent(event);
    } while (event.type != sf::Event::MouseButtonPressed);

    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop

        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // create the Drawing object
        Drawing drawing({200, 200}, 10, 0, 1, p, angle);
        std::size_t give_up = 0;
        while (not drawing.is_complete() and give_up < 50) {
            // dummy lambda --we don't care about it as it's not currently used
            drawing.add_triangle([](std::size_t)->std::size_t {return 0;});
            give_up++;
        }
        std::cout << "p: " << p << " angle: " << angle << std::endl;

        angle += 1;
        if (angle >= 119.9) {
            angle = 1;
            p += 0.1;
            if (p >= 0.9) {
                p = 0.1;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        Drawing::Shapes shapes = drawing.get_shapes();
        // draw all the triangles first
        // don't ever draw more than 20 objects
        std::size_t drawn = 0;
        for (const Drawing::Shape& t : shapes.triangles) {
            if (drawn >= 50) {
                break;
            }
            sf::VertexArray triangle(sf::Triangles, 3);
            // colours to use for each vertex
            sf::Color colours[] = {
                sf::Color::Red, sf::Color::Green, sf::Color::Blue,
            };
            for (std::size_t i = 0; i < 3; i++) {
                // define the position of the triangle's points
                triangle[i].position = sf::Vector2f(t[i].x, t[i].y);
                // define the color of the triangle's points
                triangle[i].color = colours[i];
            }

            window.draw(triangle);
            drawn++;
        }
        // draw the background silhouette last, over the top of the triangles
        // sf::VertexArray silhouette(sf::LinesStrip, shapes.silhouette.size() + 1);
        // for (std::size_t i = 0; i < shapes.silhouette.size() + 1; i++) {
        //     std::size_t j = i % shapes.silhouette.size();
        //     silhouette[i].position = sf::Vector2f(shapes.silhouette[j].x * SCALE, shapes.silhouette[j].y * SCALE);
        //     // default draw colour appears to be white anyway...
        //     // silhouette[i].color = sf::Color::White;
        // }
        // window.draw(silhouette);

        // end the current frame
        window.display();
    }

    return 0;
}
