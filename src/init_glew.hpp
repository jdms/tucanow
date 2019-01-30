#ifndef __INIT_GLEW_TUCANO_WIDGET__
#define __INIT_GLEW_TUCANO_WIDGET__


#include<iostream>
#include<GL/glew.h>

namespace twidget {
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
