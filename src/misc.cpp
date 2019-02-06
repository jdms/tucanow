#include <iostream>

#include <GL/glew.h>

#include "tucanow/misc.hpp"


namespace tucanow {
namespace misc {


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

}
}
