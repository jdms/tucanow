#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__


#include <memory>
#include <string>


class SimpleWidget;
struct GLFWwindow;

class MainWindow
{
    public:
        int run(int width, int height, std::string title = "TUCANO Window");

    protected:
        static void initialize (int width, int height);

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void mouseButtonCallback (GLFWwindow* window, int button, int action, int mods);

        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

        static void mouseWheelCallback (GLFWwindow* window, double xoffset, double yoffset);

        static std::unique_ptr<SimpleWidget> widget;
        GLFWwindow* main_window;
};

#endif
