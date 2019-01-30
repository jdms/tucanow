#ifndef __SIMPLE_WIDGET__
#define __SIMPLE_WIDGET__

#include <GL/glew.h>

#include <tucano/utils/trackball.hpp>
#include <tucano/effects/phongshader.hpp>
#include <tucano/utils/plyimporter.hpp>
#include <tucano/utils/imageIO.hpp>
#include <tucano/gui/base.hpp>


class SimpleWidget 
{

private:
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

    enum class MeshType { NONE, FROM_VECTORS, FROM_FILE } mesh_t = MeshType::NONE;

public:
    SimpleWidget() = default;

    ~SimpleWidget() = default;

    /**
     * @brief Initializes the widget and shaders
     *
     * @param width Widget width in pixels
     * @param height Widget height in pixels 
     */
    /* void initialize(int width, int height, std::string assets_dir = "./samples/assets/"); */
    void initialize(int width, int height);

    /**
     * Repaints screen buffer.
     **/
    virtual void render();

    /**
    * @brief Returns a pointer to the camera instance
    *
    * @return pointer to trackball camera
    **/
    Tucano::Trackball* getCamera (void)
    {
        return &camera;
    }


    /**
     * @brief Returns pointer to GUI
     * @return pointer to GUI
     */
    /* Tucano::GUI::Base* getGUI (void) */
    /* { */
    /*     return &gui; */
    /* } */

    /**
    * @brief Returns a pointer to the light instance
    *
    * @return pointer to trackball light
    **/
    Tucano::DirectionalTrackball* getLight (void)
    {
        return &light;
    }

    bool setDefaultColor(float r, float g, float b);

    bool setDefaultColor(float r, float g, float b, float a);

    /* bool setVertices(std::vector<float> &vertices); */

    /* bool setIndices(std::vector<unsigned int> &indices); */

    /* bool setNormals(std::vector<float> &normals); */

    bool setMesh(
            const std::vector<float> &vertices, 
            const std::vector<unsigned int> &indices = {}, 
            const std::vector<float> &vertex_normals = {}
            );

    bool setMeshColorsRGB(std::vector<float> &colors);

    bool setMeshColorsRGBA(std::vector<float> &colors);

    bool setMeshTexCoords(std::vector<float> &texture);

    /**
     * @brief Open a Ply mesh file
     *
     * @param Name of file to open
     */
    bool openMeshFile(string filename);

    /**
     * @brief Set path for the shader's dir
     * @param Path to shader's dir
     */

    void setShaderDir(string dir);

    /**
     * @brief Sets a texture for the model
     *
     * Mesh must have tex coords to work properly
     * @param tex_file Texture filename
     */
    bool setModelTexture(std::string tex_file);

};

#endif // MODELWIDGET
