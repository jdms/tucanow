#ifndef __TUCANOW_GUI__
#define __TUCANOW_GUI__


#include <memory>
#include <string>


namespace Tucano {
namespace GUI {
    class Base;
}
}

namespace tucanow {


struct GuiImpl;
class Scene;


class Gui 
{
    public: 
        Gui(Scene&);
        virtual ~Gui();

        Gui(Gui &) = delete;
        Gui& operator=(Gui &) = delete;

        Gui(Gui &&) = delete;
        Gui& operator=(Gui&&) = delete;

        virtual void initialize(int width, int height, std::string assets_dir = "./samples/assets/");

        virtual bool setViewport(int width, int height);

        virtual void render();

        virtual bool leftButtonPressed(float xpos, float ypos);
        virtual bool leftButtonReleased(float xpos, float ypos);
        virtual bool cursorMove(float xpos, float ypos);

    protected:
        virtual Tucano::GUI::Base* getTucanoGui();

    private:
        Scene& scene;
        std::unique_ptr<GuiImpl> pimpl;
};


} // namespace tucanow


#endif
