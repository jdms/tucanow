#ifndef __TUCANO_WIDGET_IMPL__
#define __TUCANO_WIDGET_IMPL__


#include <GL/glew.h>

#include <tucano/utils/trackball.hpp>
#include <tucano/effects/phongshader.hpp>
#include <tucano/utils/plyimporter.hpp>
#include <tucano/utils/imageIO.hpp>
#include <tucano/gui/base.hpp>

#include "simple_widget.hpp"




struct SimpleWidget::TucanoWidgetImpl {
    /// Mesh to be rendered in this widget
    Tucano::Mesh mesh;

    /// Phong effect to render mesh
    Tucano::Effects::Phong phong;

    /// Trackball for manipulating the camera
    Tucano::Trackball camera;

    /// Trackball for manipulating light direction
    Tucano::DirectionalTrackball light;

    /// Path to shader's dir
    string shader_dir;

    /// OpenGL clear color
    Eigen::Vector4f clear_color = Eigen::Vector4f(1.f, 1.f, 1.f, 0.f);

    enum class MeshType { NONE, FROM_VECTORS, FROM_FILE } mesh_t = MeshType::NONE;
};

using MeshType = SimpleWidget::TucanoWidgetImpl::MeshType;

#endif
