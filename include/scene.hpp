#ifndef __TUCANOW_SCENE__
#define __TUCANOW_SCENE__


#include<memory>
#include<string>
#include<vector>


namespace tucanow {


struct SceneImpl;
class Gui;


class Scene 
{
    public:
        Scene();
        virtual ~Scene();

        Scene(const Scene &) = delete;
        Scene& operator=(const Scene &) = delete;

        Scene(Scene &&);
        Scene& operator=(Scene &&);

        /**
         * @brief Initializes the widget and shaders
         *
         * Must be called after Glew has been initialized
         *
         * @param viewport width in pixels
         * @param viewport height in pixels 
         */
        virtual void initialize(int width, int height);
        /* void initialize(int width, int height, std::string assets_dir = "./samples/assets/"); */

        virtual bool setViewport(int width, int height);

        virtual bool setScreenScale(float scale_width, float scale_height);
        virtual void getScreenScale(float& scale_width, float& scale_height);

        virtual void setClearColor(float r, float g, float b, float a = 0.0f);

        /**
         * Repaints screen buffer.
         **/
        virtual void render();

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
        bool loadMeshFromPLY(std::string filename);

        /**
         * @brief Set path for the shader's dir
         * @param Path to shader's dir
         */

        /* void setShaderDir(std::string dir); */

        /**
         * @brief Sets a texture for the model
         *
         * Mesh must have tex coords to work properly
         * @param tex_file Texture filename
         */
        bool setModelTexture(std::string tex_file);

        void resetCamera();
        void increaseCameraZoom();
        void decreaseCameraZoom();
        void rotateCamera(float xpos, float ypos);
        void stopRotateCamera();
        void translateCamera(float xpos, float ypos);
        void stopTranslateCamera();

        void rotateLight(float xpos, float ypos);
        void stopRotateLight();

    private:
        std::unique_ptr<SceneImpl> pimpl;
        friend class Gui;

        float scale_width = 1.0f; ///<-- Ratio size(framebuffer_width)/size(window_width)
        float scale_height = 1.0f; ///<-- Ratio size(framebuffer_height)/size(window_height)
};


} // namespace tucanow


#endif // MODELWIDGET
