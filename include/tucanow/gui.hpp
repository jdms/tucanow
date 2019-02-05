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

        /**
         * @brief Initialize the gui and shaders
         *
         * Must be called after Glew has been initialized
         *
         * @param Viewport width 
         * @param Viewport height 
         * @param Path to dir where gui's assets can be found
         */
        virtual void initialize(int width, int height, std::string assets_dir = "./samples/assets/");

        /**
         * @brief Set gui's scene viewport
         *
         * @param Viewport width
         * @param Viewport height
         */
        virtual bool setViewport(int width, int height);

        /**
         * Render gui.
         **/
        virtual void render();

        /**
         * @brief Treats mouse left click callback for all elements
         *
         * @param Mouse x position
         * @param Mouse y position
         *
         * @return True if any gui element was clicked, false otherwise
         */
        virtual bool leftButtonPressed(float xpos, float ypos);

        /**
         * @brief Treats mouse left release callback for last clicked element
         *
         * @param Mouse x position
         * @param Mouse y position
         *
         * @return True if any element is selected, false otherwise
         */
        virtual bool leftButtonReleased(float xpos, float ypos);

        /**
         * @brief Treats mouse mouvement
         *
         * @param Mouse x position
         * @param Mouse y position
         *
         * @return True if any element is selected, false otherwise
         */
        virtual bool cursorMove(float xpos, float ypos);

    protected:
        virtual Tucano::GUI::Base* getTucanoGui();

        Scene& scene;
        std::unique_ptr<GuiImpl> pimpl;
};


} // namespace tucanow


#endif
