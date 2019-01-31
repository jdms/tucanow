#ifndef __SIMPLE_WIDGET__
#define __SIMPLE_WIDGET__

/* #include <GL/glew.h> */

/* #include <tucano/utils/trackball.hpp> */
/* #include <tucano/effects/phongshader.hpp> */
/* #include <tucano/utils/plyimporter.hpp> */
/* #include <tucano/utils/imageIO.hpp> */
/* #include <tucano/gui/base.hpp> */

#include<memory>
#include<string>
#include<vector>


struct TucanoWidgetImpl;
class Gui;

class SimpleWidget 
{
public:
    SimpleWidget();
    virtual ~SimpleWidget();

    SimpleWidget(const SimpleWidget &) = delete;
    SimpleWidget& operator=(const SimpleWidget &) = delete;

    SimpleWidget(SimpleWidget &&);
    SimpleWidget& operator=(SimpleWidget &&);

    /**
     * @brief Initializes the widget and shaders
     *
     * Must be called after Glew has been initialized
     *
     * @param viewport width in pixels
     * @param viewport height in pixels 
     */
    void initialize(int width, int height);
    /* void initialize(int width, int height, std::string assets_dir = "./samples/assets/"); */

    /**
     * Repaints screen buffer.
     **/
    virtual void render();

    /**
    * @brief Returns a pointer to the camera instance
    *
    * @return pointer to trackball camera
    **/
    /* Tucano::Trackball* getCamera (void) */
    /* { */
    /*     return &camera; */
    /* } */


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
    /* Tucano::DirectionalTrackball* getLight (void) */
    /* { */
    /*     return &light; */
    /* } */

    void setClearColor(float r, float g, float b, float a = 0.0f);

    /* bool setVertices(std::vector<float> &vertices); */

    /* bool setIndices(std::vector<unsigned int> &indices); */

    /* bool setNormals(std::vector<float> &normals); */

    bool setMesh(
            const std::vector<float> &vertices, 
            const std::vector<unsigned int> &indices = {}, 
            const std::vector<float> &vertex_normals = {}
            );

    bool setMeshColor(float r, float g, float b, float a = 0.0f);

    bool setMeshColorsRGB(std::vector<float> &colors);

    bool setMeshColorsRGBA(std::vector<float> &colors);

    bool setMeshTexCoords(std::vector<float> &texture);

    /**
     * @brief Open a Ply mesh file
     *
     * @param Name of file to open
     */
    bool loadPLY(std::string filename);

    /**
     * @brief Set path for the shader's dir
     * @param Path to shader's dir
     */

    void setShaderDir(std::string dir);

    /**
     * @brief Sets a texture for the model
     *
     * Mesh must have tex coords to work properly
     * @param tex_file Texture filename
     */
    bool setModelTexture(std::string tex_file);

    bool setViewport(int width, int height);

    void resetCamera();
    void increaseCameraZoom();
    void decreaseCameraZoom();
    void rotateCamera(float xpos, float ypos);
    void stopRotateCamera();
    void translateCamera(float xpos, float ypos);
    void stopTranslateCamera();
    void rotateLight(float xpos, float ypos);
    void stopRotateLight();

    bool setScreenScale(float scale_width, float scale_height);
    void getScreenScale(float& scale_width, float& scale_height);

private:
    std::unique_ptr<TucanoWidgetImpl> pimpl;
    friend class Gui;

    /* /// Mesh to be rendered in this widget */
    /* Tucano::Mesh mesh; */

    /* /// Phong effect to render mesh */
    /* Tucano::Effects::Phong phong; */

    /* /// Trackball for manipulating the camera */
    /* Tucano::Trackball camera; */

    /* /// Trackball for manipulating light direction */
    /* Tucano::DirectionalTrackball light; */

    /* /// Path to shader's dir */
    /* string shader_dir; */

    float scale_width = 1.0f; ///<-- Ratio size(framebuffer_width)/size(window_width)
    float scale_height = 1.0f; ///<-- Ratio size(framebuffer_height)/size(window_height)
};

#endif // MODELWIDGET
