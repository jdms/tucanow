#include <iostream>

/* Must include Tucano before GLFW or else gets 'gl.h' included before 'glew.h' error */
/* #include <tucano/tucano.hpp> */
#include <GL/glew.h>

#include "GLFW/glfw3.h"

#include "main_window.hpp"
#include "widget_data.hpp"
#include "scene.hpp"
#include "gui.hpp"
#include "misc.hpp"


GLFWwindow* MainWindow::main_window = nullptr;
std::unique_ptr<tucanow::Scene> MainWindow::pscene = nullptr;
std::unique_ptr<tucanow::Gui> MainWindow::pgui = nullptr;
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

bool MainWindow::setMesh(
        const std::vector<float> &vertices,
        const std::vector<unsigned int> &triangles,
        const std::vector<float> &normals,
        const std::vector<float> &colors_rgb
        )
{
    if ( vertices.empty() || (vertices.size() % 3 != 0) )
    {
        return false;
    }

    if ( !triangles.empty() )
    {
        if ( triangles.size() % 3 != 0 )
        {
            return false;
        }
    }

    if ( !normals.empty() )
    {
        if ( normals.size() != vertices.size() )
        {
            return false;
        }
    }

    if ( !colors_rgb.empty() )
    {
        if ( colors_rgb.size() % 3 != 0 )
        {
            return false;
        }

        if ( colors_rgb.size() != vertices.size() )
        {
            return false;
        }
    }

    pdata_->vertices = vertices;
    pdata_->triangles = triangles;
    pdata_->normals = normals;
    pdata_->colors_rgb = colors_rgb;
    pdata_->mesh_is_initialized_ = true;

    return true;
}

bool MainWindow::openPLY(std::string filename)
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

/* void initGlew() */
/* { */

/*     glewExperimental = true; */
/*     GLenum glewInitResult = glewInit(); */
/*     if (GLEW_OK != glewInitResult) */
/*     { */
/*         std::cerr << "Error: " << glewGetErrorString(glewInitResult) << std::endl; */
/*         exit(EXIT_FAILURE); */
/*     } */
/* } */

void MainWindow::initialize (int width, int height, WidgetData &data)
{
    /* Tucano::Misc::initGlew(); */
    tucanow::misc::initGlew();
    
    pscene.reset( new tucanow::Scene() );
    pgui.reset( new tucanow::Gui( *pscene ) );

    pscene->initialize(width, height);
    pgui->initialize(width, height, data.assets_dir_);

    if ( !data.model_filename_.empty() )
    {
        pscene->loadMeshFromPLY(data.model_filename_);
    }
    else
    {
        pscene->setMesh(data.vertices, data.triangles, data.normals);
        pscene->setMeshColorsRGB(data.colors_rgb);
    }

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

    if ( pscene == nullptr )
        return;

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        pscene->resetCamera();
    }
}

void MainWindow::mouseButtonCallback (GLFWwindow* window, int button, int action, int mods)
{
    if( pscene == nullptr )
        return;

/* <<<<<<< HEAD */
    /* if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) */
    /* { */
    /*     if (pscene->getGUI()->leftButtonPressed (xpos, ypos)) */
    /*         return; */
    /* } */

    /* if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) */
    /* { */
        /* pscene->getGUI()->leftButtonReleased (xpos, ypos); */
    /* } */
/* ======= */
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if ( pgui->leftButtonPressed(xpos, ypos) )
            return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        pgui->leftButtonReleased(xpos, ypos);
    }
/* >>>>>>> master */

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        pscene->rotateCamera(xpos, ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        pscene->stopRotateCamera();
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        pscene->translateCamera(xpos, ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        pscene->stopTranslateCamera();
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            pscene->rotateLight(xpos, ypos);
        }
        else if (action == GLFW_RELEASE)
        {
            pscene->stopRotateLight();
        }
    }
}

void MainWindow::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if( pscene == nullptr )
        return;

/* <<<<<<< HEAD */
    /* if ( pscene->getGUI()->cursorMove (xpos, ypos) ) */
    /* { */
    /*     return; */
    /* } */
/* ======= */
    if ( pgui->cursorMove(xpos, ypos) )
    {
        return;
    }
/* >>>>>>> master */

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        pscene->rotateCamera(xpos, ypos);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        pscene->translateCamera(xpos, ypos);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
    {
        pscene->rotateLight(xpos, ypos);
    }

}

void MainWindow::mouseWheelCallback (GLFWwindow* window, double xoffset, double yoffset)
{
    if( pscene == nullptr )
        return;

    if (yoffset > 0)
    {
        pscene->increaseCameraZoom();
    }
    else if (yoffset < 0)
    {
        pscene->decreaseCameraZoom();
    }
}

void MainWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    if( pscene == nullptr )
        return;

    pscene->setViewport(width, height);
    pgui->setViewport(width, height);
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
    std::cout << "OpenGL version in use: " << openGLVersion << std::endl;

    // macOS highdpi mode creates a framebuffer that differs from the window size
    int fb_width = 0;
    int fb_height = 0;
    glfwGetFramebufferSize( main_window, &fb_width, &fb_height );

    initialize(fb_width, fb_height, *pdata_);
    if ( pscene != nullptr )
        if ( (width > 0) && (height > 0) )
        {
            pscene->setScreenScale( 
                    static_cast<float>(fb_width)/static_cast<float>(width), 
                    static_cast<float>(fb_height)/static_cast<float>(height) 
                    );
        }

    glfwSetKeyCallback(main_window, keyCallback); 

    glfwSetMouseButtonCallback(main_window, mouseButtonCallback); 

    glfwSetCursorPosCallback(main_window, cursorPosCallback); 

    glfwSetScrollCallback(main_window, mouseWheelCallback); 

    glfwSetInputMode(main_window, GLFW_STICKY_KEYS, true);

    glfwSetFramebufferSizeCallback(main_window, framebufferResizeCallback);

    if( pscene != nullptr )
        pscene->render();
    glfwSwapBuffers( main_window );

    while (!glfwWindowShouldClose(main_window))
    {
        glfwMakeContextCurrent(main_window);
        if( pscene != nullptr )
        {
            pscene->render();
            pgui->render();
        }
        glfwSwapBuffers( main_window );

        glfwPollEvents();
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}

