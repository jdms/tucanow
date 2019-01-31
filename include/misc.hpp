#ifndef __TUCANOW_MISC__
#define __TUCANOW_MISC__


#include<iostream>
#include<GL/glew.h>

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

#endif
