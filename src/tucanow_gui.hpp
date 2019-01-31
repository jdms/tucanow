#ifndef __TUCANOW_GUI
#define __TUCANOW_GUI

#include<string>

class Gui {
    public: 
        virtual void initialize(int width, int height, std::string assets_dir) = 0;
        virtual void render() = 0;
        virtual bool leftButtonPressed(float xpos, float ypos) = 0;
        virtual bool leftButtonReleased(float xpos, float ypos) = 0;
        virtual bool cursorMove(float xpos, float ypos) = 0;
};

#endif
