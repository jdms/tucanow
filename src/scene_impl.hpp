#ifndef TUCANOW_SCENE_IMPL
#define TUCANOW_SCENE_IMPL


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
#include <tucano/effects/toon.hpp>
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
    ObjectType type;
    ObjectShader shader;
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

    /// Bounding box boundary mesh to be rendered in this scene
    ObjectDescriptor bbox_boundary;

    /// Objects to be rendered in this scene
    std::map<int, std::unique_ptr<ObjectDescriptor>> objects;

    /// DirectColor shader effect to render meshes
    Tucano::Effects::DirectColor directcolor;

    /// Toon shader effect to render meshes
    Tucano::Effects::Toon toon;

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
    bool render_bbox_boundary = false;

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

    void setBBox()
    {
        model_scale = 1.0f/std::max( bbox_size[0], std::max(bbox_size[1], bbox_size[2]) );

        Eigen::Vector3f dX = Eigen::Vector3f(bbox_size[0], 0.0f, 0.0f);
        Eigen::Vector3f dY = Eigen::Vector3f(0.0f, bbox_size[1], 0.0f);
        Eigen::Vector3f dZ = Eigen::Vector3f(0.0f, 0.0f, bbox_size[2]);

        Eigen::Vector3f p0 = Eigen::Vector3f(bbox_origin[0], bbox_origin[1], bbox_origin[2]);
        Eigen::Vector3f p1 = p0 + dX;
        Eigen::Vector3f p2 = p0 + dY;
        Eigen::Vector3f p3 = p0 + dX + dY;
        Eigen::Vector3f p4 = p0 + dZ;
        Eigen::Vector3f p5 = p4 + dX;
        Eigen::Vector3f p6 = p4 + dY;
        Eigen::Vector3f p7 = p4 + dX + dY;

        model_centroid = (p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7)/8.0f;

        std::vector<float> bbox_vertices = {{ 
            p0[0], p0[1], p0[2],
            p1[0], p1[1], p1[2],
            p2[0], p2[1], p2[2],
            p3[0], p3[1], p3[2],
            p4[0], p4[1], p4[2],
            p5[0], p5[1], p5[2],
            p6[0], p6[1], p6[2],
            p7[0], p7[1], p7[2]
        }};

        std::vector<unsigned int> bbox_indices = {
            0, 1,
            1, 3,
            3, 2,
            2, 0,
            4, 5,
            5, 7,
            7, 6,
            6, 4,
            0, 4,
            1, 5,
            2, 6,
            3, 7
        };

        bbox_boundary.mesh.reset();
        bbox_boundary.mesh.loadVertices(bbox_vertices);
        bbox_boundary.mesh.loadIndices(bbox_indices);
        bbox_boundary.mesh.selectPrimitive(Tucano::Mesh::CURVE);
        bbox_boundary.mesh.setColor(Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
        bbox_boundary.shader = ObjectShader::DirectColor;
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

        switch(ptr->shader)
        {
            case ObjectShader::OnePassWireframe:
                wireframe.render(ptr->mesh, camera, light);
                break;

            case ObjectShader::Phong:
                phong.render(ptr->mesh, camera, light, ptr->texture);
                break;

            case ObjectShader::Toon:
                toon.render(ptr->mesh, camera, light);

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


} // namespace tucanow


#endif
