#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Time.hpp"

#include <cstdlib>
#include <iostream>

const unsigned int width = 800;
const unsigned int height = 800;

int main(void)
{
    glfw::Window window;

    window.Create(width, height, "Simple example");

    window.MakeContextCurrent();

    while (!window.ShouldClose())
    {
        window.PollEvents();

        glfw::Event event;
        while (window.GetEvents(event))
        {
            std::cout << "Got event from queue\tEvent type is: " << static_cast<int>(event.type) << "\n";
            switch (event.type)
            {
            case glfw::Event::Type::Key:
                if (event.key.action == glfw::KeyAction::Press)
                    if (event.key.key == glfw::Key::Escape)
                        window.SetShouldClose(GL_TRUE);
            }
        }

        window.SwapBuffers();
    }

    return 0;
}
