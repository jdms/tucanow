#ifndef TUCANOW_GUI
#define TUCANOW_GUI

/** @file gui.hpp tucanow/gui.hpp
 * */


#include <functional>
#include <memory>
#include <string>


namespace Tucano {
namespace GUI {
    class Base;
}
}

namespace tucanow {


class Scene;
struct SceneImpl;


class Gui 
{
    public: 
        /**
         * @brief Constructor implements the pimpl idiom
         */
        Gui(Scene&);

        /**
         * @brief Default destructor
         */
        virtual ~Gui();


        /**
         * @brief Deleted copy constructor
         */
        Gui(Gui &) = delete;

        /**
         * @brief Deleted copy assignment
         */
        Gui& operator=(Gui &) = delete;

        /**
         * @brief Default move constructor
         */
        Gui(Gui &&);

        /**
         * @brief Default move assignment
         */
        Gui& operator=(Gui&&);

        /**
         * @brief Initialize the gui and shaders
         *
         * Must be called after Glew has been initialized
         *
         * @param width Viewport width 
         * @param height Viewport height 
         * @param assets_dir Path to dir where gui's assets can be found
         */
        virtual void initialize(int width, int height, std::string assets_dir = "./samples/assets/");

        /**
         * @brief Set gui's scene viewport
         *
         * @param width Viewport width
         * @param height Viewport height
         */
        virtual bool setViewport(int width, int height);

        /**
         * @brief Get gui's scene viewport
         *
         * @param width Viewport width
         * @param height Viewport height
         */

        virtual void getViewport(int &width, int &height);

        /**
         * @brief Render gui.
         **/
        virtual void render();

        /**
         * @brief Handles mouse left click callback for all elements
         *
         * @param xpos Mouse x position
         * @param ypos Mouse y position
         *
         * @return True if any gui element was clicked, false otherwise
         */
        virtual bool leftButtonPressed(float xpos, float ypos);

        /**
         * @brief Handles mouse left release callback for last clicked element
         *
         * @param xpos Mouse x position
         * @param ypos Mouse y position
         *
         * @return True if any element is selected, false otherwise
         */
        virtual bool leftButtonReleased(float xpos, float ypos);

        /**
         * @brief Handles mouse movement
         *
         * @param xpos Mouse x position
         * @param ypos Mouse y position
         *
         * @return True if any element is selected, false otherwise
         */
        virtual bool cursorMove(float xpos, float ypos);

    protected:
        /**
         * @brief Get a pointer to Tucano's gui base class
         *
         * @return Pointer to Tucano::GUI::Base
         */
        Tucano::GUI::Base* getTucanoGui();

        /**
         * @brief Get a pointer to tucanow::Scene's implementation
         *
         * @return Pointer to Tucano::GUI::Base
         */
        SceneImpl* getSceneImpl();

        std::reference_wrapper<Scene> scene; ///<-- Reference to Scene object
        std::unique_ptr<Tucano::GUI::Base> pimpl; ///<-- Tucano data
};


} // namespace tucanow


#endif
