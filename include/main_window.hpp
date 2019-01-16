#ifndef __TUCANO_GLFW_WINDOW__
#define __TUCANO_GLFW_WINDOW__


#include <string>


class ModelWidget;
struct GLFWwindow;

class TucanoGLFWWindow
{
    public:
        int run(int width, int height, std::string title = "TUCANO Window");

    protected:
        // use widget
        static void initialize (int width, int height);

        // use widget
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        // use widget
        static void mouseButtonCallback (GLFWwindow* window, int button, int action, int mods);

        // use widget
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

        // use widget
        static void mouseWheelCallback (GLFWwindow* window, double xoffset, double yoffset);

        static ModelWidget* widget;
        GLFWwindow* main_window;
};

#endif
