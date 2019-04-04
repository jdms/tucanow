#ifndef __TUCANOW_SCENE_IMPL__
#define __TUCANOW_SCENE_IMPL__


/** @file scene_impl.hpp src/scene_impl.hpp
 * */


/* #include <GL/glew.h> */

#include <memory>
#include <map>
#include <string>
#include <array>

#include <Eigen/Dense>

#include <tucano/texture.hpp>
#include <tucano/effects/directcolor.hpp>
#include <tucano/effects/phongshader.hpp>
#include <tucano/effects/wireframe.hpp>
#include <tucano/gui/base.hpp>
#include <tucano/utils/trackball.hpp>
#include <tucano/utils/plyimporter.hpp>
#include <tucano/utils/imageIO.hpp>

#include "tucanow/definitions.hpp"
#include "tucanow/scene.hpp"


namespace tucanow {


struct ObjectDescriptor {
    Tucano::Mesh mesh;
    Tucano::Texture texture;
    ObjectType object_type;
    ObjectShader object_shader;
};

struct SceneImpl 
{
    /// Bounding box origin
    std::array<float, 3> bbox_origin = {-1.0f, -1.0f, -1.0f};

    /// Bounding box size
    std::array<float, 3> bbox_size ={2.0f, 2.0f, 2.0f};

    /// Bounding box normalization scale (1/max{bbox_size}) -- used to normalize Tucano::Model::ModelMatrix
    float model_scale = 1.0f;

    /// Bounding box centroid -- used to normalize Tucano::Model::ModelMatrix
    Eigen::Vector3f model_centroid = Eigen::Vector3f::Zero();

    /// DEPRECATED: mesh to be rendered in this scene
    Tucano::Mesh mesh;

    /// Objects to be rendered in this scene
    std::map<int, std::unique_ptr<ObjectDescriptor>> objects;

    /// DirectColor shader effect to render meshes
    Tucano::Effects::DirectColor directcolor;

    /// Phong shader effect to render meshes
    Tucano::Effects::Phong phong;

    /// Single pass wireframe shader effect to render meshes
    Tucano::Effects::Wireframe wireframe;

    /// Trackball for manipulating the camera
    Tucano::Trackball camera;

    /// Trackball for manipulating light direction
    Tucano::DirectionalTrackball light;

    /// Path to shader's dir
    std::string shader_dir;

    /// OpenGL clear color
    Eigen::Vector4f clear_color = Eigen::Vector4f(1.f, 1.f, 1.f, 0.f);

    /// Source of current mesh
    enum class MeshType { NONE, FROM_VECTORS, FROM_FILE } mesh_t = MeshType::NONE;

    ObjectDescriptor* Object( int object_id ) 
    {
        auto it = objects.find( object_id );
        if ( it == objects.end() )
        {
            return nullptr;
        }

        return it->second.get();
    };

    ObjectDescriptor* createObject( int object_id )
    {
        objects[object_id] = std::make_unique<ObjectDescriptor>();

        return Object(object_id);
    }

    void setBBoxCentroidAndScale()
    {
        model_scale = 1.0f/std::max( bbox_size[0], std::max(bbox_size[1], bbox_size[2]) );

        Eigen::Vector3f dX = Eigen::Vector3f(bbox_origin[0], 0.0f, 0.0f);
        Eigen::Vector3f dY = Eigen::Vector3f(0.0f, bbox_origin[1], 0.0f);
        Eigen::Vector3f dZ = Eigen::Vector3f(0.0f, 0.0f, bbox_origin[2]);

        auto p0 = Eigen::Vector3f(bbox_origin[0], bbox_origin[1], bbox_origin[2]);
        auto p1 = p0 + dX;
        auto p2 = p0 + dY;
        auto p3 = p0 + dX + dY;
        auto p4 = p0 + dZ;
        auto p5 = p4 + dX;
        auto p6 = p4 + dY;
        auto p7 = p4 + dX + dY;

        model_centroid = (p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7)/8.0f;
    }

    bool normalizeObjectModelMatrix(ObjectDescriptor* ptr)
    {
        if ( ptr == nullptr )
        {
            return false;
        }

        ptr->mesh.normalizeModelMatrix(model_centroid, model_scale);

        return true;
    }

    bool denormalizeObjectModelMatrix(ObjectDescriptor* ptr)
    {
        if ( ptr == nullptr )
        {
            return false;
        }

        ptr->mesh.desnormalizeModelMatrix(model_centroid, model_scale);

        return true;
    }

    bool render(ObjectDescriptor *ptr)
    {
        if ( ptr == nullptr )
        {
            return false;
        }

        normalizeObjectModelMatrix(ptr);

        switch(ptr->object_shader)
        {
            case ObjectShader::OnePassWireframe:
                wireframe.render(ptr->mesh, camera, light);
                break;

            case ObjectShader::Phong:
                phong.render(ptr->mesh, camera, light, ptr->texture);
                break;

            case ObjectShader::DirectColor:
                directcolor.render(ptr->mesh, camera);
                break;

            case ObjectShader::None:
                break;

            default:
                break;
        }

        denormalizeObjectModelMatrix(ptr);

        return true;
    }
};

using MeshType = SceneImpl::MeshType; ///<-- Rename SceneImpl::MeshType in tucanow namespace


} // namespace tucanow


#endif
