#include <string>

#include "main_window.hpp"


int main()
{
    MainWindow main_window;
    const int window_width = 600;
    const int window_height = 600;
    std::string window_title = "A Simple Phong Viewer";

    return main_window.run(window_width, window_height, window_title);
}
