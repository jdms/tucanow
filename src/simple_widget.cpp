#include "simple_widget.hpp"

/* void SimpleWidget::initialize(int width, int height, std::string assets_dir /1* = "../samples/assets/" *1/) */
void SimpleWidget::initialize(int width, int height)
{
    // initialize the shader effect (if TUCANOSHADERDIR is set, no need to set dir before init)
    phong.initialize();

    camera.setPerspectiveMatrix(60.0, (float)width/(float)height, 0.1f, 100.0f);
    camera.setRenderFlag(false);
    camera.setViewport(Eigen::Vector2f ((float)width, (float)height));

    light.setRenderFlag(false);
    light.setViewport(Eigen::Vector2f ((float)width, (float)height));

    glEnable(GL_DEPTH_TEST);
}

void SimpleWidget::render(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    phong.render(mesh, camera, light);
    camera.render();
}

void SimpleWidget::setShaderDir(std::string dir)
{
    shader_dir = dir;
}

bool SimpleWidget::setDefaultColor(float r, float g, float b)
{
    phong.setDefaultColor(Eigen::Vector4f(r, g, b, 1.0f));

    return true;
}

bool SimpleWidget::setDefaultColor(float r, float g, float b, float a)
{
    phong.setDefaultColor(Eigen::Vector4f(r, g, b, a));

    return true;
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

    if ( mesh_t != MeshType::NONE )
    {
        mesh.reset();
    }

    bool success = mesh.loadVertices(vertices);

    if ( success && !indices.empty() )
    {
        mesh.loadIndices(indices);
    }

    if ( success && !vertex_normals.empty() )
    {
        mesh.loadNormals(vertex_normals);
    }

    if ( success )
    {
        mesh.normalizeModelMatrix();
        mesh_t = MeshType::FROM_VECTORS;
    }

    return success;
}

/* bool SimpleWidget::setVertices(std::vector<float> &vertices) */
/* { */
/*     if ( mesh_t != MeshType::NONE ) */
/*     { */
/*         mesh.reset(); */
/*     } */

/*     if ( vertices.empty() ) */
/*     { */
/*         return false; */
/*     } */

/*     bool success = mesh.loadVertices(vertices); */
/*     if ( success ) */
/*     { */
/*         mesh.normalizeModelMatrix(); */
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

/*     mesh.loadIndices(indices); */

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

/*     return mesh.loadNormals(normals); */
/* } */


bool SimpleWidget::setMeshColorsRGB(std::vector<float> &colors)
{
    if ( mesh_t != MeshType::FROM_VECTORS )
    {
        return false;
    }

    return mesh.loadColorsRGB(colors);
}

bool SimpleWidget::setMeshColorsRGBA(std::vector<float> &colors)
{
    if ( mesh_t != MeshType::FROM_VECTORS )
    {
        return false;
    }

    return mesh.loadColorsRGBA(colors);
}

bool SimpleWidget::setMeshTexCoords(std::vector<float> &texture)
{
    if ( mesh_t != MeshType::FROM_VECTORS )
    {
        return false;
    }

    if ( texture.empty() )
    {
        return false;
    }

    return mesh.loadTexCoords(texture);
}

bool SimpleWidget::openMeshFile(std::string filename)
{
    if ( mesh_t != MeshType::NONE )
    {
        mesh.reset();
    }

    bool success = Tucano::MeshImporter::loadPlyFile(&mesh, filename);
    if (success)
    {
        mesh.normalizeModelMatrix();
        std::string tex_file = Tucano::MeshImporter::getPlyTextureFile(filename);
        if (!tex_file.empty())
        {
            size_t found = filename.find_last_of("/\\");
            string dir = filename.substr(0, found);
            string tex_file_with_dir = dir + "/" + tex_file;
 
            setModelTexture(tex_file_with_dir);
        }

        mesh_t = MeshType::FROM_FILE;
    }

    return success;
}

bool SimpleWidget::setModelTexture(std::string tex_file)
{
    Tucano::Texture texture;
    bool success = Tucano::ImageImporter::loadImage(tex_file, &texture);
    if (success)
    {        
        phong.setTexture(texture);
        phong.getTexture()->setTexParameters( GL_CLAMP, GL_CLAMP, GL_LINEAR, GL_LINEAR );
    }

    return success;
}

bool SimpleWidget::setViewport(int width, int height)
{
    if ( ( width < 1 ) || ( height < 1 ) )
    {
        return false;
    }

    camera.setViewport(Eigen::Vector2f((float)width, (float)height));
    light.setViewport (Eigen::Vector2f((float)width, (float)height));

    /* gui.setViewportSize (width, height); */

    return true;
}

void SimpleWidget::resetCamera()
{
    camera.reset();
    light.reset();
}

void SimpleWidget::increaseCameraZoom()
{
        camera.increaseZoom(1.05);
}

void SimpleWidget::decreaseCameraZoom()
{
        camera.increaseZoom(1.0/1.05);
}

void SimpleWidget::rotateCamera(float xpos, float ypos)
{
    camera.rotateCamera( Eigen::Vector2f (xpos, ypos) );
}

void SimpleWidget::stopRotateCamera()
{
    camera.endRotation();
}

void SimpleWidget::translateCamera(float xpos, float ypos)
{
    camera.translateCamera( Eigen::Vector2f(xpos, ypos) );
}

void SimpleWidget::stopTranslateCamera()
{
    camera.endTranslation();
}

void SimpleWidget::rotateLight(float xpos, float ypos)
{
    light.rotateCamera( Eigen::Vector2f (xpos, ypos) );
}

void SimpleWidget::stopRotateLight()
{
    light.endRotation();
}
