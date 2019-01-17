#ifndef __WIDGET_DATA__
#define __WIDGET_DATA__

#include <string>

struct WidgetData 
{
    std::string assets_dir_ = "./samples/assets/";
    std::string model_filename_ = {};
    bool mesh_is_initialized_ = false;
};

#endif
