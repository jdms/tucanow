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
         * @brief Initializes the scene and shaders
         *
         * Must be called after Glew has been initialized
         *
         * @param Viewport width 
         * @param Viewport height 
         */
        virtual void initialize(int width, int height);

        /**
         * @brief Set scene viewport
         *
         * @param Viewport width in pixels
         * @param Viewport height in pixels 
         */
        virtual bool setViewport(int width, int height);

        /** @brief Set screen scale for HighDPI screens
         *
         * In HighDPI mode (e.g., macOS) the framebuffer size and the viewport
         * size may differ.  Set the screen scale to the ratio between both
         * sizes so that actions based on the screen position behave correctly
         *
         * @param Ratio (framebuffer width)/(logical window width) 
         * @param Ratio (framebuffer length)/(logical window length)
         */
        virtual bool setScreenScale(float scale_width, float scale_height);

        /** @brief Get screen scale for HighDPI screens
         *
         * In HighDPI mode (e.g., macOS) the framebuffer size and the viewport
         * size may differ.  Use the screen scale to fix actions that depend on
         * screen coordinates
         *
         * @param Ratio (framebuffer width)/(logical window width) 
         * @param Ratio (framebuffer length)/(logical window length)
         */
        virtual void getScreenScale(float& scale_width, float& scale_height);

        /**
         * @brief Set OpenGL clear color.
         *
         * @param Intensity of red
         * @param Intensity of green
         * @param Intensity of blue
         * @param Intensity of alpha (usually opacity)
         */
        virtual void setClearColor(float r, float g, float b, float a = 1.f);

        /**
         * @brief Set OpenGL clear color.
         *
         * Overloaded method
         *
         * @param Intensity of red
         * @param Intensity of green
         * @param Intensity of blue
         * @param Intensity of alpha (usually opacity)
         */
        virtual void setClearColor(int r, int g, int b, int a = 255);

        /**
         * Repaints screen buffer.
         **/
        virtual void render();

        /**
         * @brief Render model with a single pass wireframe shader
         *
         * Triangles are shaded with flat colours.
         *
         * @param Set true to render wireframe, false for default shading
         */
        void renderWireframe(bool render_wireframe);

        /**
         * Alternate between wireframe and default shading
         */
        void toggleRenderWireframe();
        /**
         * @brief Load a mesh to visualize
         *
         * @param Mesh vertices (must be non-empty)
         * @param Mesh triangles (vector of indices on the vertices' list)
         * @param Normals per vertex
         *
         * @return True if mesh was set correctly
         */
        bool setMesh(
                const std::vector<float> &vertices, 
                const std::vector<unsigned int> &indices = {}, 
                const std::vector<float> &vertex_normals = {}
                );

        /**
         * @brief Set mesh's single colour -- does not affect mesh loaded from a ply file
         *
         * @param Intensity of red
         * @param Intensity of green
         * @param Intensity of blue
         * @param Intensity of alpha (usually opacity)
         */
        void setMeshColor(float r, float g, float b, float a = 1.f);

        /**
         * @brief Set mesh's single colour -- does not affect mesh loaded from a ply file
         *
         * Overloaded method
         *
         * @param Intensity of red
         * @param Intensity of green
         * @param Intensity of blue
         * @param Intensity of alpha (usually opacity)
         */
        void setMeshColor(int r, int g, int b, int a = 255);

        /**
         * @brief Set mesh's RGB colours per vertex -- does not affect mesh loaded from a ply file
         *
         * @param Vector of RGB colours per vertex
         *
         * @return True if colours vector size is a multiple of 3
         */
        bool setMeshColorsRGB(std::vector<float> &colors);

        /**
         * @brief Set mesh's RGB colours per vertex -- does not affect mesh loaded from a ply file
         *
         * @param Vector of RGBA colours per vertex
         *
         * @return True if colours vector size is a multiple of 4
         */
        bool setMeshColorsRGBA(std::vector<float> &colors);

        /**
         * @brief Set texture coordinates (u,v) as a vertex attribute.
         *
         * @param Texture coordinates array.
         *
         * @return True if texture vector is multiple of 2, false otherwise.
         */
        bool setMeshTexCoords(std::vector<float> &texture);

        /**
         * @brief Open a Ply mesh file
         *
         * @param Name of file to open
         *
         * @return True if mesh was loaded successfully
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
         *
         * @return True if texture file was loaded correctly
         */
        bool setModelTexture(std::string tex_file);

        /**
         * @brief Place light source direction at the trackball camera direction
         *
         * @param Set true for headlight, false for fixed light
         */
        void setHeadlight(bool headlight);

        /**
         * Alternate between a headlight and a fixed light
         */
        void toggleHeadlight();
        // toggle

        /**
         * Reset camera
         */
        void resetCamera();

        /**
         * Increase zoom level
         */
        void increaseCameraZoom();

        /**
         * Decrease zoom level
         */
        void decreaseCameraZoom();

        /**
         * @brief Accumulate sequence of increments to rotate camera 
         *
         * @param Mouse x position
         * @param Mouse y position
         */
        void rotateCamera(float xpos, float ypos);

        /**
         * Rotate camera and stop rotateCamera() mode
         */
        void stopRotateCamera();

        /**
         * @brief Accumulate sequence of increments to translate camera 
         *
         * @param Mouse x position
         * @param Mouse y position
         */
        void translateCamera(float xpos, float ypos);

        /**
         * Translate camera and stop translateCamera() mode
         */
        void stopTranslateCamera();

        /**
         * @brief Accumulate sequence of increments to rotate light 
         *
         * @param Mouse x position
         * @param Mouse y position
         */
        void rotateLight(float xpos, float ypos);

        /**
         * Rotate light and stop rotateLight() mode
         *
         * @param Mouse x position
         * @param Mouse y position
         */
        void stopRotateLight();

    private:
        std::unique_ptr<SceneImpl> pimpl;
        friend class Gui;

        bool render_wireframe = false;

        bool headlight_camera = true;

        float scale_width = 1.0f; ///<-- Ratio (framebuffer_width)/(window_width)
        float scale_height = 1.0f; ///<-- Ratio (framebuffer_height)/(window_height)
};


} // namespace tucanow


#endif // MODELWIDGET
