#ifndef TUCANOW_OBJECT_DEFINITIONS
#define TUCANOW_OBJECT_DEFINITIONS

/** @file object_definitions.hpp tucanow/object_definitions.hpp
 * */


namespace tucanow {


enum class ObjectType {
    PointCloud,
    TriangleMesh,
    PLY,
    OBJ
};

enum class ObjectShader {
    None,
    DirectColor,
    OnePassWireframe,
    Phong
};

} // namespace tucanow


#endif 
