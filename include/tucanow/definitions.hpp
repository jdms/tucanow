#ifndef TUCANOW_OBJECT_DEFINITIONS
#define TUCANOW_OBJECT_DEFINITIONS

/** @file object_definitions.hpp tucanow/object_definitions.hpp
 * */


namespace tucanow {


enum class ObjectType {
    PointCloud,
    CurveMesh,
    TriangleMesh,
    PLY,
    OBJ
};

enum class ObjectShader {
    // No shading -- select to prevent an object from being rendered
    None,
    // Work for all primitives
    DirectColor,
    // Require normals
    OnePassWireframe,
    Toon,
    // Require normals, and accept textures
    Phong
};

enum class SceneLight {
    SingleDirectional,
    Headlight
};

} // namespace tucanow


#endif 
