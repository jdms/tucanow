#ifndef __TUCANOW_WIDGET_DATA__
#define __TUCANOW_WIDGET_DATA__

#include <string>
#include <vector>


struct WidgetData 
{
    std::vector<float> vertices = {};
    std::vector<unsigned int> triangles = {};
    std::vector<float> normals = {};
    std::vector<float> colors_rgb = {};

    std::string assets_dir_ = "./samples/assets/";
    std::string model_filename_ = {};
    bool mesh_is_initialized_ = false;
};


#endif
