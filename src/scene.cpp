#include "scene_impl.hpp"
#include "tucanow/scene.hpp"
#include "tucanow/misc.hpp"


namespace tucanow {


Scene::Scene() //: pimpl(new SceneImpl)
{
    // Glew must be initialized before any Tucano object is created
    misc::initGlew();

    clear();
}

Scene::~Scene() = default;

Scene::Scene(Scene &&) = default;
Scene& Scene::operator=(Scene &&) = default;

SceneImpl& Scene::Impl()
{
    return *pimpl;
}

const SceneImpl& Scene::Impl() const
{
    return *pimpl;
}

/* void Scene::initialize(int width, int height, std::string assets_dir /1* = "../samples/assets/" *1/) */
void Scene::initialize(int width, int height)
{
    if ( width < 1 )
        width = 1;

    if ( height < 1 )
        height = 1;

    // initialize the shader effect (if TUCANOSHADERDIR is set, no need to set dir before init)
    pimpl->directcolor.initialize();
    pimpl->toon.initialize();
    pimpl->phong.initialize();
    pimpl->wireframe.initialize();

    pimpl->camera.setPerspectiveMatrix(60.0, (float)width/(float)height, 0.1f, 100.0f);
    pimpl->camera.setRenderFlag(false);
    pimpl->camera.setViewport(Eigen::Vector2f ((float)width, (float)height));

    pimpl->light.setRenderFlag(false);
    pimpl->light.setViewport(Eigen::Vector2f ((float)width, (float)height));
    setHeadlight(headlight_camera);

    // Manually tuned parameters
    pimpl->phong.setAmbientCoeff(0.525);
    pimpl->phong.setDiffuseCoeff(0.75);
    pimpl->phong.setSpecularCoeff(0.0875);
    pimpl->phong.setShininessCoeff(3.475);

    glEnable(GL_DEPTH_TEST);
}

void Scene::render()
{
    glClearColor(
            Impl().clear_color[0],
            Impl().clear_color[1],
            Impl().clear_color[2],
            Impl().clear_color[3]
        );
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if ( Impl().render_bbox_boundary )
    {
        Impl().render( &Impl().bbox_boundary );
    }

    for ( auto &entry : Impl().objects )
    {
        Impl().render(entry.second.get());

    }

    Impl().camera.render();
}

void Scene::renderWireframe(bool wireframe)
{
    render_wireframe = wireframe;
}

void Scene::toggleRenderWireframe()
{
    render_wireframe = !render_wireframe;
}

// /**
//  * @brief Set path for the shader's dir
//  *
//  * @param dir Path to shader's dir
//  */

// /* void setShaderDir(std::string dir); */

/* void Scene::setShaderDir(std::string dir) */
/* { */
/*     pimpl->shader_dir = dir; */
/* } */

void Scene::setClearColor(float r, float g, float b, float a)
{
    Impl().clear_color = Eigen::Vector4f(r, g, b, a);
}

void Scene::setClearColor(int r, int g, int b, int a)
{
    setClearColor((float)(r)/255.0f, (float)(g)/255.0f, (float)(b)/255.0f, (float)(a)/255.0f);
}

bool Scene::setBoundingBox( std::array<float, 3> bbox_origin, std::array<float, 3> bbox_size )
{
    bool success = true;

    success &= bbox_size[0] > 0.0f;
    success &= bbox_size[1] > 0.0f;
    success &= bbox_size[2] > 0.0f;

    if ( !success )
    {
        return false;
    }

    Impl().bbox_origin = bbox_origin;
    Impl().bbox_size = bbox_size;

    Impl().setBBox();

    return success;
}

bool Scene::loadPointCloud(
        int object_id,
        const std::vector<float> &vertices 
        )
{
    if ( vertices.empty() )
    {
        /* std::cout << "\nError: vertices.empty() != true\n"; */
        return false;
    }

    auto object = Impl().createObject(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    // TODO: destroy object in case of failure
    bool success = object->mesh.loadVertices(vertices);
    if ( !success )
    {
        /* object->mesh.normalizeModelMatrix(); */
        /* std::cout << "\nGot mesh\n"; */
        return false;
    }
    object->shader = ObjectShader::DirectColor;
    /*     std::cout << "\nGot mesh\n"; */

    return success;
}

bool Scene::loadCurveMesh( int object_id,
        const std::vector<float> &vertices,
        const std::vector<unsigned int> &indices
        )
{
    if ( vertices.empty() )
    {
        /* std::cout << "\nError: vertices.empty() != true\n"; */
        return false;
    }

    if ( indices.empty() || (indices.size() % 3 != 0) )
    {
        return false;
    }

    auto object = Impl().createObject(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    // TODO: destroy object in case of failure
    bool success = object->mesh.loadVertices(vertices);
    if ( !success )
    {
        /* object->mesh.normalizeModelMatrix(); */
        /* std::cout << "\nGot mesh\n"; */
        return false;
    }
    object->mesh.loadIndices(indices);
    object->mesh.selectPrimitive(Tucano::Mesh::CURVE);
    object->shader = ObjectShader::DirectColor;
    object->type = ObjectType::CurveMesh;
    /*     std::cout << "\nGot mesh\n"; */

    return success;
}

bool Scene::loadTriangleMesh( int object_id,
        const std::vector<float> &vertices, 
        const std::vector<unsigned int> &indices, 
        const std::vector<float> &vertex_normals
        )
{
    if ( vertices.empty() )
    {
        /* std::cout << "\nError: vertices.empty() != true\n"; */
        return false;
    }

    if ( indices.empty() || (indices.size() % 3 != 0) )
    {
        return false;
    }

    if ( !vertex_normals.empty() )
    if ( vertices.size() != vertex_normals.size() )
    {
        /* std::cout << "\nError: vertices.size() != normals.size()\n"; */
        return false;
    }
    
    auto object = Impl().createObject(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    // TODO: destroy object in case of failure
    bool success = object->mesh.loadVertices(vertices);
    if ( !success )
    {
        /* object->mesh.normalizeModelMatrix(); */
        /* std::cout << "\nGot mesh\n"; */
        return false;
    }
    object->mesh.loadIndices(indices);
    object->shader = ObjectShader::DirectColor;
    object->type = ObjectType::TriangleMesh;
    /*     std::cout << "\nGot mesh\n"; */

    if ( success && !vertex_normals.empty() )
    {
        object->mesh.loadNormals(vertex_normals);
        object->shader = ObjectShader::Phong;
        /* std::cout << "\nGot normals\n"; */
    }

    return success;
}

bool Scene::loadPLY(int object_id, const std::string &filename)
{
    auto object = Impl().createObject(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    bool success = Tucano::MeshImporter::loadPlyFile(&object->mesh, filename);
    if (success)
    {
        object->shader = ObjectShader::Phong;
        object->type = ObjectType::PLY;

        /* object->mesh.normalizeModelMatrix(); */

        std::string tex_file = Tucano::MeshImporter::getPlyTextureFile(filename);
        if (!tex_file.empty())
        {
            size_t found = filename.find_last_of("/\\");
            string dir = filename.substr(0, found);
            string tex_file_with_dir = dir + "/" + tex_file;
 
            setMeshTexture(object_id, tex_file_with_dir);
        }
    }

    return success;
}

bool Scene::eraseObject( int object_id )
{
    return Impl().eraseObject(object_id);
}

void Scene::clear()
{
    pimpl.reset(new SceneImpl);

    /* // Set light cyan as the background colour */
    /* setClearColor(224, 255, 255, 255); */
    setClearColor(255, 255, 255, 255);

    Impl().setBBox();

    glEnable(GL_LINE_SMOOTH);
    glPointSize(2.0f);
}

bool Scene::setObjectShader(int object_id, const ObjectShader& shader)
{
    auto object = Impl().Object(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    object->shader = shader;

    return true;
}

bool Scene::setObjectColor(int object_id, float r, float g, float b, float a)
{
    auto object = Impl().Object(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    r = (r >= 0.0) ? r : 0.0;
    r = (r <= 1.0) ? r : 1.0;
    g = (g >= 0.0) ? g : 0.0;
    g = (g <= 1.0) ? g : 1.0;
    b = (b >= 0.0) ? b : 0.0;
    b = (b <= 1.0) ? b : 1.0;
    a = (a >= 0.0) ? a : 0.0;
    a = (a <= 1.0) ? a : 1.0;
    object->mesh.setColor(Eigen::Vector4f(r, g, b, a));

    if (std::fabs(a - 1.0) < 0.001)
    {
        object->opaque = true;
    }
    else
    {
        object->opaque = false;
    }

    return true;
}

bool Scene::setObjectColor(int object_id, int r, int g, int b, int a)
{
    return setObjectColor(object_id, (float)(r)/255.0f, (float)(g)/255.0f, (float)(b)/255.0f, (float)(a)/255.0f);
}

bool Scene::setObjectColorsRGB(int object_id, const std::vector<float> &colors)
{
    auto object = Impl().Object(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    bool success = object->mesh.loadColorsRGB(colors);
    if (success)
    {
        object->opaque = true;
    }

    return success;
}

bool Scene::setObjectColorsRGBA(int object_id, const std::vector<float> &colors)
{
    auto object = Impl().Object(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    bool success = object->mesh.loadColorsRGBA(colors);
    if (success)
    {
        object->opaque = false;
    }

    return success;
}

bool Scene::setMeshTextureCoordinates(int object_id, const std::vector<float> &texture)
{
    if ( texture.empty() )
    {
        return false;
    }

    auto object = Impl().Object(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    return object->mesh.loadTexCoords(texture);
}

bool Scene::setMeshTexture(int object_id, const std::string &tex_file)
{
    auto object = Impl().Object(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    Tucano::Texture texture;
    bool success = Tucano::ImageImporter::loadImage(tex_file, &texture);
    if (success)
    {        
        object->texture = texture;
        object->texture.setTexParameters( GL_CLAMP, GL_CLAMP, GL_LINEAR, GL_LINEAR );
    }

    return success;
}

bool Scene::setViewport(int width, int height)
{
    if ( ( width < 1 ) || ( height < 1 ) )
    {
        return false;
    }

    pimpl->camera.setViewport(Eigen::Vector2f((float)width, (float)height));
    pimpl->light.setViewport (Eigen::Vector2f((float)width, (float)height));

    return true;
}

void Scene::getViewport(int &width, int &height)
{
    Eigen::Vector4f viewport = pimpl->camera.getViewport();

    width = static_cast<int>(viewport[2] - viewport[0]);
    height = static_cast<int>(viewport[3] - viewport[1]);
}

void Scene::setHeadlight(bool headligh)
{
    headlight_camera = headligh;

    if (headlight_camera)
    {
        *(pimpl->light.viewMatrix()) = *(pimpl->camera.viewMatrix());
    }
}

void Scene::toggleHeadlight()
{
    setHeadlight(!headlight_camera);
}

void Scene::resetCamera()
{
    pimpl->camera.reset();
    pimpl->light.reset();
}

void Scene::increaseCameraZoom(float zoom_factor)
{
    if ( zoom_factor <= 1.0f )
    {
        zoom_factor = 1.03f;
    }

    pimpl->camera.increaseZoom(zoom_factor);
}

void Scene::decreaseCameraZoom(float zoom_factor)
{
    if ( zoom_factor <= 1.0f )
    {
        zoom_factor = 1.03f;
    }

    pimpl->camera.increaseZoom(1.0f/zoom_factor);
}

void Scene::rotateCamera(float xpos, float ypos)
{
    float scaled_xpos = scale_width * xpos;
    float scaled_ypos = scale_height * ypos;

    /* std::cout << "\nxpos = " << xpos << "; ypos = " << ypos << "\n"; */
    /* std::cout << "\nscaled_xpos = " << scaled_xpos << "; scaled_ypos = " << scaled_ypos << "\n"; */

    pimpl->camera.rotateCamera( Eigen::Vector2f (scaled_xpos, scaled_ypos) );

    if( headlight_camera )
    {
        *(pimpl->light.viewMatrix()) = *(pimpl->camera.viewMatrix());
    }
}

void Scene::stopRotateCamera()
{
    pimpl->camera.endRotation();

    if( headlight_camera )
    {
        *(pimpl->light.viewMatrix()) = *(pimpl->camera.viewMatrix());
    }
}

void Scene::translateCamera(float xpos, float ypos)
{
    float scaled_xpos = scale_width * xpos;
    float scaled_ypos = scale_height * ypos;

    pimpl->camera.translateCamera( Eigen::Vector2f(scaled_xpos, scaled_ypos) );
}

void Scene::stopTranslateCamera()
{
    pimpl->camera.endTranslation();
}

void Scene::focusCameraOnBoundingBox()
{
    Impl().setBBox();
    Impl().normalizeAllModelMatrices();
}

bool Scene::focusCameraOnObject(int object_id)
{
    auto object = Impl().Object(object_id);
    if ( object == nullptr )
    {
        return false;
    }

    Impl().model_centroid = object->mesh.getCentroid();
    Impl().model_scale = object->mesh.getNormalizationScale();
    Impl().normalizeAllModelMatrices();

    return true;
}

void Scene::rotateLight(float xpos, float ypos)
{
    float scaled_xpos = scale_width * xpos;
    float scaled_ypos = scale_height * ypos;

    if (!headlight_camera)
    {
        pimpl->light.rotateCamera( Eigen::Vector2f (scaled_xpos, scaled_ypos) );
    }
}

void Scene::stopRotateLight()
{
    if (!headlight_camera)
    {
        pimpl->light.endRotation();
    }
}


bool Scene::setScreenScale( float swidth, float sheight )
{
    bool success = true;
    success &= swidth > 0;
    success &= sheight > 0;

    if ( success )
    {
        scale_width = swidth;
        scale_height = sheight;
    }

    return success;
}

void Scene::getScreenScale( float& swidth, float& sheight )
{
    swidth = scale_width;
    sheight = scale_height;
}

} // namespace tucanow


