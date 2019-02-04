#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__


#include <memory>
#include <string>
#include <vector>


struct GLFWwindow;
struct WidgetData;

namespace tucanow {
    class Scene;
    class Gui;
}


/// Create and manage aplication window
class MainWindow
{
    public:
        /// MainWindow is a singleton
        static MainWindow& Get();

        /// Default dtor
        ~MainWindow() = default;

        /// Deleted default copy ctor
        MainWindow(MainWindow &) = delete;

        /// Deleted default copy assigment
        MainWindow& operator=(const MainWindow &) = delete;

        /// Deleted default move ctor
        MainWindow(MainWindow &&) = delete;

        /// Deleted default move assigment
        MainWindow& operator=(MainWindow &&) = delete;

        /// Set mesh to be visualized (if PLY file is set, the PLY file will be visualized instead)
        bool setMesh(
                const std::vector<float> &vertices,
                const std::vector<unsigned int> &triangles = {}, 
                const std::vector<float> &normals = {},
                const std::vector<float> &colors_rgb = {}
            );

        /// Set name of PLY mesh file to be visualized
        bool openPLY(std::string name);

        /// Path to dir with widget's assets (textures, icons, etc.)
        bool setAssetsDir(std::string name);

        /// Create glfw3 window, track events, render OpenGL widget
        int run(int width, int height, std::string title = "Tucano Window");

    protected:
        /// Initialize widget (call glew)
        static void initialize (int width, int height, WidgetData &data);

        /// glfw3 callback
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        /// glfw3 callback
        static void mouseButtonCallback (GLFWwindow* window, int button, int action, int mods);

        /// glfw3 callback
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

        /// glfw3 callback
        static void mouseWheelCallback (GLFWwindow* window, double xoffset, double yoffset);

        /// glfw3 callback
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        static GLFWwindow* main_window; ///< Points to memory managed by glfw3
        static std::unique_ptr<tucanow::Scene> pscene; ///< Must be initialized after glew
        static std::unique_ptr<tucanow::Gui> pgui;

    private: 
        std::unique_ptr<WidgetData> pdata_;

        /// Private ctor for singleton pattern
        MainWindow();
};

#endif
