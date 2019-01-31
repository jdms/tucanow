#ifndef __TUCANOW_WIDGET_DATA__
#define __TUCANOW_WIDGET_DATA__

#include <string>


struct WidgetData 
{
    std::string assets_dir_ = "./samples/assets/";
    std::string model_filename_ = {};
    bool mesh_is_initialized_ = false;
};


#endif
