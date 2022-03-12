#include <SFML/Graphics.hpp>

#include <triangberg_builder/Drawing.hpp>

int main() {
    using namespace com::saxbophone::triangberg;
    // create the Drawing object
    Drawing drawing({}, {}, {}, {}, {}, {});

    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // don't kill my CPU by running at stupid-fast framerate for no reason
    window.setFramerateLimit(60);

    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (not drawing.is_complete()) {
                    // dummy lambda --we don't care about it as it's not currently used
                    drawing.add_triangle([](std::size_t)->std::size_t {return 0;});
                } else {
                    window.close();
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        Drawing::Shapes shapes = drawing.get_shapes();
        // draw all the triangles first
        for (const Drawing::Triangle& t : shapes.triangles) {
            sf::VertexArray triangle(sf::Triangles, 3);

            // define the position of the triangle's points
            triangle[0].position = sf::Vector2f(t.a.x * 28, t.a.y * 28);
            triangle[1].position = sf::Vector2f(t.b.x * 28, t.b.y * 28);
            triangle[2].position = sf::Vector2f(t.c.x * 28, t.c.y * 28);

            // define the color of the triangle's points
            triangle[0].color = sf::Color::Red;
            triangle[1].color = sf::Color::Blue;
            triangle[2].color = sf::Color::Green;

            window.draw(triangle);
        }
        // draw the background silhouette last, over the top of the triangles
        sf::VertexArray silhouette(sf::LinesStrip, shapes.silhouette.size() + 1);
        for (std::size_t i = 0; i < shapes.silhouette.size() + 1; i++) {
            std::size_t j = i % shapes.silhouette.size();
            silhouette[i].position = sf::Vector2f(shapes.silhouette[j].x * 28, shapes.silhouette[j].y * 28);
            // default draw colour appears to be white anyway...
            // silhouette[i].color = sf::Color::White;
        }
        window.draw(silhouette);

        // end the current frame
        window.display();
    }

    return 0;
}
