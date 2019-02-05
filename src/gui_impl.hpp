#ifndef __TUCANOW_GUI_IMPL__
#define __TUCANOW_GUI_IMPL__


/* #include <GL/glew.h> */

/* #include <tucano/tucano.hpp> */
/* #include <tucano/utils/trackball.hpp> */
/* #include <tucano/effects/phongshader.hpp> */
/* #include <tucano/utils/plyimporter.hpp> */
/* #include <tucano/utils/imageIO.hpp> */
#include <tucano/gui/base.hpp>
#include "tucanow/scene.hpp"


namespace tucanow {


struct GuiImpl 
{
        /// GUI holder
        Tucano::GUI::Base gui;

        /// Box to group all gui elements
        Tucano::GUI::GroupBox groupbox;

        /// Reload button
        Tucano::GUI::Button reload_button;

        /// Menu show/hide button
        Tucano::GUI::SelectButton menu_button;

        /// Slider for diffuse coefficient
        Tucano::GUI::Slider kd_slider;
        Tucano::GUI::Slider ks_slider;
        Tucano::GUI::Slider ka_slider;
        Tucano::GUI::Slider shininess_slider;

        /// Label for diffuse text
        Tucano::GUI::Label diffuse_label;
        Tucano::GUI::Label specular_label;
        Tucano::GUI::Label ambient_label;
        Tucano::GUI::Label shininess_label;
};


} // namespace tucanow


#endif
