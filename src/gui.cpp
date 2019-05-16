#include <iostream>

#include "scene_impl.hpp"
#include "tucanow/gui.hpp"


namespace tucanow {


/* struct Gui::GuiImpl */ 
/* { */
/*         /// GUI holder */
/*         Tucano::GUI::Base gui; */
/* }; */


Gui::Gui(Scene &s) : scene(s), pimpl( new Tucano::GUI::Base() ) {}

Gui::~Gui() = default;

Gui::Gui(Gui &&) = default;

Gui& Gui::operator=(Gui&&) = default;

void Gui::initialize(int width, int height, std::string assets_dir)
{
    setViewport(width, height);
}

bool Gui::setViewport(int width, int height)
{
    if ( ( width < 1 ) || ( height < 1 ) )
    {
        return false;
    }

    auto gui = getTucanoGui();
    gui->setViewportSize (width, height);

    scene.get().setViewport(width, height);

    return true;
}

void Gui::getViewport(int &width, int &height)
{
    scene.get().getViewport(width, height);
}

void Gui::render()
{
    auto gui = getTucanoGui();
    gui->render();
}

bool Gui::leftButtonPressed(float xpos, float ypos)
{
    float scale_width, scale_height;
    scene.get().getScreenScale(scale_width, scale_height);

    double scaled_xpos = scale_width * xpos;
    double scaled_ypos = scale_height * ypos;

    auto gui = getTucanoGui();
    return gui->leftButtonPressed (scaled_xpos, scaled_ypos);
}

bool Gui::leftButtonReleased(float xpos, float ypos)
{
    float scale_width, scale_height;
    scene.get().getScreenScale(scale_width, scale_height);

    double scaled_xpos = scale_width * xpos;
    double scaled_ypos = scale_height * ypos;

    auto gui = getTucanoGui();
    return gui->leftButtonReleased (scaled_xpos, scaled_ypos);
}

bool Gui::cursorMove(float xpos, float ypos)
{
    float scale_width, scale_height;
    scene.get().getScreenScale(scale_width, scale_height);

    double scaled_xpos = scale_width * xpos;
    double scaled_ypos = scale_height * ypos;

    auto gui = getTucanoGui();
    return gui->cursorMove (scaled_xpos, scaled_ypos);
}

Tucano::GUI::Base* Gui::getTucanoGui()
{
    return pimpl.get();
}

SceneImpl* Gui::getSceneImpl()
{
    return scene.get().pimpl.get();
}

} //namespace tucanow
