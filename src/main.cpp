#include "main_window.hpp"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600


int main()
{
    TucanoGLFWWindow main_window;

    return main_window.run(WINDOW_WIDTH, WINDOW_HEIGHT);
}
