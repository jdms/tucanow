#include "tucano_widget_impl.hpp"
#include "simple_widget.hpp"


SimpleWidget::SimpleWidget() : pimpl(new TucanoWidgetImpl)
{}

SimpleWidget::~SimpleWidget() = default;

SimpleWidget::SimpleWidget(SimpleWidget &&) = default;
SimpleWidget& SimpleWidget::operator=(SimpleWidget &&) = default;

/* void SimpleWidget::initialize(int width, int height, std::string assets_dir /1* = "../samples/assets/" *1/) */
void SimpleWidget::initialize(int width, int height)
{
    if ( width < 1 )
        width = 1;

    if ( height < 1 )
        height = 1;

    // initialize the shader effect (if TUCANOSHADERDIR is set, no need to set dir before init)
    pimpl->phong.initialize();

    pimpl->camera.setPerspectiveMatrix(60.0, (float)width/(float)height, 0.1f, 100.0f);
    pimpl->camera.setRenderFlag(false);
    pimpl->camera.setViewport(Eigen::Vector2f ((float)width, (float)height));

    pimpl->light.setRenderFlag(false);
    pimpl->light.setViewport(Eigen::Vector2f ((float)width, (float)height));

    // Manually tuned parameters
    pimpl->phong.setAmbientCoeff(0.525);
    pimpl->phong.setDiffuseCoeff(0.75);
    pimpl->phong.setSpecularCoeff(0.0875);
    pimpl->phong.setShininessCoeff(3.475);

    glEnable(GL_DEPTH_TEST);
}

void SimpleWidget::render(void)
{
    glClearColor(
            pimpl->clear_color[0], 
            pimpl->clear_color[1], 
            pimpl->clear_color[2],
            pimpl->clear_color[3]
        );
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    pimpl->phong.render(pimpl->mesh, pimpl->camera, pimpl->light);
    pimpl->camera.render();
}

void SimpleWidget::setShaderDir(std::string dir)
{
    pimpl->shader_dir = dir;
}

void SimpleWidget::setClearColor(float r, float g, float b, float a)
{
    pimpl->clear_color = Eigen::Vector4f(r, g, b, a);
}

bool SimpleWidget::setMesh(const std::vector<float> &vertices, const std::vector<GLuint> &indices, const std::vector<float> &vertex_normals)
{
    if ( !vertex_normals.empty() )
    if ( vertices.size() != vertex_normals.size() )
    {
        return false;
    }
    
    if ( vertices.empty() )
    {
        return false;
    }

    if ( pimpl->mesh_t != MeshType::NONE )
    {
        pimpl->mesh.reset();
    }

    bool success = pimpl->mesh.loadVertices(vertices);

    if ( success && !indices.empty() )
    {
        pimpl->mesh.loadIndices(indices);
    }

    if ( success && !vertex_normals.empty() )
    {
        pimpl->mesh.loadNormals(vertex_normals);
    }

    if ( success )
    {
        pimpl->mesh.normalizeModelMatrix();
        pimpl->mesh_t = MeshType::FROM_VECTORS;
    }

    return success;
}

/* bool SimpleWidget::setVertices(std::vector<float> &vertices) */
/* { */
/*     if ( pimpl->mesh_t != MeshType::NONE ) */
/*     { */
/*         pimpl->mesh.reset(); */
/*     } */

/*     if ( vertices.empty() ) */
/*     { */
/*         return false; */
/*     } */

/*     bool success = pimpl->mesh.loadVertices(vertices); */
/*     if ( success ) */
/*     { */
/*         pimpl->mesh.normalizeModelMatrix(); */
/*         mesh_t = MeshType::FROM_VECTORS; */
/*     } */

/*     return success; */
/* } */

/* bool SimpleWidget::setIndices(std::vector<unsigned int> &indices) */
/* { */
/*     if ( mesh_t != MeshType::FROM_VECTORS ) */
/*     { */
/*         return false; */
/*     } */

/*     if ( indices.empty() ) */
/*     { */
/*         return false; */
/*     } */

/*     pimpl->mesh.loadIndices(indices); */

/*     return true; */
/* } */

/* bool SimpleWidget::setNormals(std::vector<float> &normals) */
/* { */
/*     if ( mesh_t != MeshType::FROM_VECTORS ) */
/*     { */
/*         return false; */
/*     } */

/*     if ( normals.empty() ) */
/*     { */
/*         return false; */
/*     } */

/*     return pimpl->mesh.loadNormals(normals); */
/* } */

bool SimpleWidget::setMeshColor(float r, float g, float b, float a)
{
    /* pimpl->phong.setMeshColor(Eigen::Vector4f(r, g, b, a)); */
    pimpl->mesh.setColor(Eigen::Vector4f(r, g, b, a));

    return true;
}


bool SimpleWidget::setMeshColorsRGB(std::vector<float> &colors)
{
    if ( pimpl->mesh_t != MeshType::FROM_VECTORS )
    {
        return false;
    }

    return pimpl->mesh.loadColorsRGB(colors);
}

bool SimpleWidget::setMeshColorsRGBA(std::vector<float> &colors)
{
    if ( pimpl->mesh_t != MeshType::FROM_VECTORS )
    {
        return false;
    }

    return pimpl->mesh.loadColorsRGBA(colors);
}

bool SimpleWidget::setMeshTexCoords(std::vector<float> &texture)
{
    if ( pimpl->mesh_t != MeshType::FROM_VECTORS )
    {
        return false;
    }

    if ( texture.empty() )
    {
        return false;
    }

    return pimpl->mesh.loadTexCoords(texture);
}

bool SimpleWidget::loadPLY(std::string filename)
{
    if ( pimpl->mesh_t != MeshType::NONE )
    {
        pimpl->mesh.reset();
    }

    bool success = Tucano::MeshImporter::loadPlyFile(&pimpl->mesh, filename);
    if (success)
    {
        pimpl->mesh.normalizeModelMatrix();
        std::string tex_file = Tucano::MeshImporter::getPlyTextureFile(filename);
        if (!tex_file.empty())
        {
            size_t found = filename.find_last_of("/\\");
            string dir = filename.substr(0, found);
            string tex_file_with_dir = dir + "/" + tex_file;
 
            setModelTexture(tex_file_with_dir);
        }

        pimpl->mesh_t = MeshType::FROM_FILE;
    }

    return success;
}

bool SimpleWidget::setModelTexture(std::string tex_file)
{
    Tucano::Texture texture;
    bool success = Tucano::ImageImporter::loadImage(tex_file, &texture);
    if (success)
    {        
        pimpl->phong.setTexture(texture);
        pimpl->phong.getTexture()->setTexParameters( GL_CLAMP, GL_CLAMP, GL_LINEAR, GL_LINEAR );
    }

    return success;
}

bool SimpleWidget::setViewport(int width, int height)
{
    if ( ( width < 1 ) || ( height < 1 ) )
    {
        return false;
    }

    pimpl->camera.setViewport(Eigen::Vector2f((float)width, (float)height));
    pimpl->light.setViewport (Eigen::Vector2f((float)width, (float)height));

    /* gui.setViewportSize (width, height); */

    return true;
}

void SimpleWidget::resetCamera()
{
    pimpl->camera.reset();
    pimpl->light.reset();
}

void SimpleWidget::increaseCameraZoom()
{
        pimpl->camera.increaseZoom(1.05);
}

void SimpleWidget::decreaseCameraZoom()
{
        pimpl->camera.increaseZoom(1.0/1.05);
}

void SimpleWidget::rotateCamera(float xpos, float ypos)
{
    pimpl->camera.rotateCamera( Eigen::Vector2f (xpos, ypos) );
}

void SimpleWidget::stopRotateCamera()
{
    pimpl->camera.endRotation();
}

void SimpleWidget::translateCamera(float xpos, float ypos)
{
    pimpl->camera.translateCamera( Eigen::Vector2f(xpos, ypos) );
}

void SimpleWidget::stopTranslateCamera()
{
    pimpl->camera.endTranslation();
}

void SimpleWidget::rotateLight(float xpos, float ypos)
{
    pimpl->light.rotateCamera( Eigen::Vector2f (xpos, ypos) );
}

void SimpleWidget::stopRotateLight()
{
    pimpl->light.endRotation();
}
