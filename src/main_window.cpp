#include <iostream>

/* Must include Tucano before GLFW or else gets 'gl.h' included before 'glew.h' error */
/* #include <tucano/tucano.hpp> */
#include <GL/glew.h>

#include "GLFW/glfw3.h"

#include "main_window.hpp"
#include "widget_data.hpp"
#include "simple_widget.hpp"


GLFWwindow* MainWindow::main_window = nullptr;
std::unique_ptr<SimpleWidget> MainWindow::widget = nullptr;
/* std::unique_ptr<WidgetData> MainWindow::pdata_ = nullptr; */


MainWindow::MainWindow() : pdata_( new WidgetData() )
{
    /* pdata_.reset( new WidgetData() ); */
}

MainWindow& MainWindow::Get()
{
    // This is based on Scott Meyers' singleton pattern
    static MainWindow single_instance;
    return single_instance;
}

bool MainWindow::openMeshFile(std::string filename)
{
    if ( filename.empty() )
    {
        return false;
    }

    pdata_->model_filename_ = filename;
    pdata_->mesh_is_initialized_ = true;

    return true;
}

bool MainWindow::setAssetsDir(std::string dirname)
{
    if ( dirname.empty() )
    {
        return false;
    }

    pdata_->assets_dir_ = dirname;

    return true;
}

void initGlew()
{

    glewExperimental = true;
    GLenum glewInitResult = glewInit();
    if (GLEW_OK != glewInitResult)
    {
        std::cerr << "Error: " << glewGetErrorString(glewInitResult) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void MainWindow::initialize (int width, int height, WidgetData &data)
{
    /* Tucano::Misc::initGlew(); */
    initGlew();
    
    widget.reset( new SimpleWidget() );
    /* widget->initialize(width, height, data.assets_dir_); */
    widget->initialize(width, height);

    widget->openMeshFile(data.model_filename_);

    std::cout << std::endl << std::endl;
    std::cout << " *********************************************** " << std::endl;
    std::cout << " *** Usage:"                                       << std::endl;
    std::cout << " *********************************************** " << std::endl;
    std::cout << std::endl;
    std::cout << " Left mouse button   : rotate trackball"           << std::endl;
    std::cout << " Right mouse button  : translate trackball"        << std::endl;
    std::cout << " Middle mouse button : change light direction"     << std::endl;
    std::cout << std::endl;
    std::cout << " R : reset trackball and light direction"          << std::endl;
    std::cout << " *********************************************** " << std::endl;
}

void MainWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);    

    if ( widget == nullptr )
        return;

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        widget->resetCamera();
    }
}

void MainWindow::mouseButtonCallback (GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if( widget == nullptr )
        return;

    /* if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) */
    /* { */
    /*     if (widget->getGUI()->leftButtonPressed (xpos, ypos)) */
    /*         return; */
    /* } */

    /* if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) */
    /* { */
        /* widget->getGUI()->leftButtonReleased (xpos, ypos); */
    /* } */

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        widget->rotateCamera(xpos, ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        widget->stopRotateCamera();
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        widget->translateCamera(xpos, ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        widget->stopTranslateCamera();
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            widget->rotateLight(xpos, ypos);
        }
        else if (action == GLFW_RELEASE)
        {
            widget->stopRotateLight();
        }
    }
}

void MainWindow::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if( widget == nullptr )
        return;

    /* if ( widget->getGUI()->cursorMove (xpos, ypos) ) */
    /* { */
    /*     return; */
    /* } */

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        widget->rotateCamera(xpos, ypos);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        widget->translateCamera(xpos, ypos);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
    {
        widget->rotateLight(xpos, ypos);
    }

}

void MainWindow::mouseWheelCallback (GLFWwindow* window, double xoffset, double yoffset)
{
    if( widget == nullptr )
        return;

    if (yoffset > 0)
    {
        widget->increaseCameraZoom();
    }
    else if (yoffset < 0)
    {
        widget->decreaseCameraZoom();
    }
}

void MainWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    if( widget == nullptr )
        return;

    widget->setViewport(width, height);
}

int MainWindow::run(int width, int height, std::string title)
{
    if ( main_window )
    {
        std::cerr << "MainWindow::run() has already been run" << std::endl;
        return 3;
    }

    if ( !pdata_->mesh_is_initialized_ )
    {
        std::cerr << "Failed to get a mesh" << std::endl;
        return 2;
    }

    if (!glfwInit()) 
    {
        std::cerr << "Failed to init glfw" << std::endl;
        return 1;
    }

    // double x dimension for splitview and add margin
    /* main_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TUCANO :: Flythrough Camera", NULL, NULL); */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    main_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (main_window == nullptr)
    {
        std::cerr << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(main_window);

    const GLubyte* openGLRenderer = glGetString(GL_RENDERER);
    const GLubyte* openGLVersion = glGetString(GL_VERSION);
    std::cout << std::endl;
    std::cout << "Renderer: " << openGLRenderer << std::endl;
    std::cout << "OpenGL version supported: " << openGLVersion << std::endl;

    // macOS highdpi mode creates a framebuffer that differs from the window size
    int fb_width = 0;
    int fb_height = 0;
    glfwGetFramebufferSize( main_window, &fb_width, &fb_height );

    initialize(fb_width, fb_height, *pdata_);

    glfwSetKeyCallback(main_window, keyCallback); 

    glfwSetMouseButtonCallback(main_window, mouseButtonCallback); 

    glfwSetCursorPosCallback(main_window, cursorPosCallback); 

    glfwSetScrollCallback(main_window, mouseWheelCallback); 

    glfwSetInputMode(main_window, GLFW_STICKY_KEYS, true);

    glfwSetFramebufferSizeCallback(main_window, framebufferResizeCallback);

    if( widget != nullptr )
        widget->render();
    glfwSwapBuffers( main_window );

    while (!glfwWindowShouldClose(main_window))
    {
        glfwMakeContextCurrent(main_window);
        if( widget != nullptr )
            widget->render();
        glfwSwapBuffers( main_window );

        glfwPollEvents();
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}

