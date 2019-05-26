#ifndef TUCANOW_PHONG_GUI
#define TUCANOW_PHONG_GUI

/** @file phong_gui.hpp tucanow/phong_gui.hpp
 * */


#include <memory>
#include <string>

#include "tucanow/gui.hpp"


namespace tucanow {


class Scene;


class PhongGui : public Gui
{
    public: 
        /**
         * @brief Factory method
         */
        static std::unique_ptr<tucanow::Gui> Get(Scene &s, std::string assets_dir = "./samples/assets/");

        /**
         * @brief Default destructor
         */
        virtual ~PhongGui();


    protected:
        /**
         * @brief Constructor implements the pimpl idiom
         */
        PhongGui(Scene&);

        struct PhongGuiImpl;
        std::unique_ptr<PhongGuiImpl> pimpl; ///<-- Tucano data

    private:
        /**
         * @brief Initialize the gui and shaders
         *
         * Must be called after Glew has been initialized
         *
         * @param width Viewport width 
         * @param height Viewport height 
         * @param assets_dir Path to dir where gui's assets can be found
         */
        void initialize(int width, int height, std::string assets_dir = "./samples/assets/");

};


} // namespace tucanow


#endif
