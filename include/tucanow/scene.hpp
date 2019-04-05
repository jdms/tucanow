#ifndef __TUCANOW_SCENE__
#define __TUCANOW_SCENE__

/** @file scene.hpp tucanow/scene.hpp
 * */


#include<memory>
#include<string>
#include<vector>


namespace tucanow {


struct SceneImpl;
class Gui;


class Scene 
{
    public:
        /**
         * @brief Constructor implements the pimpl idiom
         */
        Scene();

        /**
         * @brief Default destructor
         */
        virtual ~Scene();

        /**
         * @brief Deleted copy constructor
         */
        Scene(const Scene &) = delete;

        /**
         * @brief Deleted copy assignment
         */
        Scene& operator=(const Scene &) = delete;

        /**
         * @brief Default move constructor
         */
        Scene(Scene &&);

        /**
         * @brief Default move assignment
         */
        Scene& operator=(Scene &&);

        // TODO: Change from initializer to factory method.
        std::unique_ptr<Scene> Get(int width, int height);

        /**
         * @brief Initializes the scene and shaders
         *
         * Must be called after Glew has been initialized
         *
         * @param width Viewport width 
         * @param height Viewport height 
         */
        virtual void initialize(int width, int height);

        /**
         * @brief Set scene viewport
         *
         * @param width Viewport width in pixels
         * @param height Viewport height in pixels 
         */
        virtual bool setViewport(int width, int height);

        /** @brief Set screen scale for HighDPI screens
         *
         * In HighDPI mode (e.g., macOS) the framebuffer size and the viewport
         * size may differ.  Set the screen scale to the ratio between both
         * sizes so that actions based on the screen position behave correctly
         *
         * @param scale_width Ratio (framebuffer width)/(logical window width) 
         * @param scale_height Ratio (framebuffer length)/(logical window length)
         */
        virtual bool setScreenScale(float scale_width, float scale_height);

        /** @brief Get screen scale for HighDPI screens
         *
         * In HighDPI mode (e.g., macOS) the framebuffer size and the viewport
         * size may differ.  Use the screen scale to fix actions that depend on
         * screen coordinates
         *
         * @param scale_width Ratio (framebuffer width)/(logical window width) 
         * @param scale_height Ratio (framebuffer length)/(logical window length)
         */
        virtual void getScreenScale(float& scale_width, float& scale_height);

        /**
         * @brief Set OpenGL clear color.
         *
         * @param r Intensity of red
         * @param g Intensity of green
         * @param b Intensity of blue
         * @param a Intensity of alpha (usually opacity)
         */
        virtual void setClearColor(float r, float g, float b, float a = 1.f);

        /**
         * @brief Set OpenGL clear color.
         *
         * Overloaded method
         *
         * @param r Intensity of red
         * @param g Intensity of green
         * @param b Intensity of blue
         * @param a Intensity of alpha (usually opacity)
         */
        virtual void setClearColor(int r, int g, int b, int a = 255);

        /**
         * @brief Repaints screen buffer.
         **/
        virtual void render();

        /**
         * @brief Render model with a single pass wireframe shader
         *
         * Triangles are shaded with flat colours.
         *
         * @param render_wireframe Set true to render wireframe, false for default shading
         */
        void renderWireframe(bool render_wireframe);

        /**
         * @brief Alternate between wireframe and default shading
         */
        void toggleRenderWireframe();

        /**
         * @brief Set the bounding box
         *
         * @param bbox_origin Bounding box's origin
         * @param bbox_size Bounding box's size
         */
        bool setBoundingBox( std::array<float, 3> bbox_origin, std::array<float, 3> bbox_size );

        /**
         * @brief Load a mesh to visualize
         *
         * @param object_id Object index (integer valued)
         * @param vertices Mesh vertices (must be non-empty)
         * @param indices Mesh triangles (vector of indices on the vertices' list)
         * @param vertex_normals Normals per vertex
         *
         * @return True if mesh was set correctly
         */
        bool loadMesh(
                int object_id,
                const std::vector<float> &vertices, 
                const std::vector<unsigned int> &indices = {}, 
                const std::vector<float> &vertex_normals = {}
                );

        /**
         * @brief Open a Ply mesh file
         *
         * @param object_id Object index (integer valued)
         * @param filename Name of file to open
         *
         * @return True if mesh was loaded successfully
         */
        bool loadPLY(int object_id, const std::string &filename);

        /**
         * @brief Set object's single colour -- does not affect mesh loaded from a ply file
         *
         * @param object_id Object index (integer valued)
         * @param r Intensity of red
         * @param g Intensity of green
         * @param b Intensity of blue
         * @param a Intensity of alpha (usually opacity)
         */
        bool setObjectColor(int object_id, float r, float g, float b, float a = 1.f);

        /**
         * @brief Set object's single colour -- does not affect mesh loaded from a ply file
         *
         * Overloaded method
         *
         * @param object_id Object index (integer valued)
         * @param r Intensity of red
         * @param g Intensity of green
         * @param b Intensity of blue
         * @param a Intensity of alpha (usually opacity)
         */
        bool setObjectColor(int object_id, int r, int g, int b, int a = 255);

        /**
         * @brief Set mesh's RGB colours per vertex -- does not affect mesh loaded from a ply file
         *
         * @param object_id Object index (integer valued)
         * @param colors Vector of RGB colours per vertex
         *
         * @return True if colours vector size is a multiple of 3
         */
        bool setObjectColorsRGB(int object_id, const std::vector<float> &colors);

        /**
         * @brief Set mesh's RGB colours per vertex -- does not affect mesh loaded from a ply file
         *
         * @param object_id Object index (integer valued)
         * @param colors Vector of RGBA colours per vertex
         *
         * @return True if colours vector size is a multiple of 4
         */
        bool setObjectColorsRGBA(int object_id, const std::vector<float> &colors);

        /**
         * @brief Set texture coordinates (u,v) as a vertex attribute.
         *
         * @param object_id Object index (integer valued)
         * @param texture_coords Texture coordinates array.
         *
         * @return True if texture vector is multiple of 2, false otherwise.
         */
        bool setMeshTextureCoordinates(int object_id, const std::vector<float> &texture_coords);

        /**
         * @brief Sets a texture for the model
         *
         * Mesh must have tex coords to work properly
         *
         * @param object_id Object index (integer valued)
         * @param tex_file Texture filename
         *
         * @return True if texture file was loaded correctly
         */
        bool setMeshTexture(int object_id, const std::string &tex_file);

        /**
         * @brief Place light source direction at the trackball camera direction
         *
         * @param headlight Set true for headlight, false for fixed light
         */
        void setHeadlight(bool headlight);

        /**
         * @brief Alternate between a headlight and a fixed light
         */
        void toggleHeadlight();
        // toggle

        /**
         * @brief Reset camera
         */
        void resetCamera();

        /**
         * @brief Increase zoom level
         */
        void increaseCameraZoom();

        /**
         * @brief Decrease zoom level
         */
        void decreaseCameraZoom();

        /**
         * @brief Accumulate sequence of increments to rotate camera 
         *
         * @param xpos Mouse x position
         * @param ypos Mouse y position
         */
        void rotateCamera(float xpos, float ypos);

        /**
         * @brief Rotate camera and stop rotateCamera() mode
         */
        void stopRotateCamera();

        /**
         * @brief Accumulate sequence of increments to translate camera 
         *
         * @param xpos Mouse x position
         * @param ypos Mouse y position
         */
        void translateCamera(float xpos, float ypos);

        /**
         * @brief Translate camera and stop translateCamera() mode
         */
        void stopTranslateCamera();

        /**
         * @brief Change camera focus to track the bounding box
         */
        void focusCameraOnBoundingBox();

        /**
         * @brief Change camera focus to track a particular object
         *
         * @param object_id Object index (integer valued)
         *
         * @return True if object exists
         */
        bool focusCameraOnObject(int object_id);

        /**
         * @brief Accumulate sequence of increments to rotate light 
         *
         * @param xpos Mouse x position
         * @param ypos Mouse y position
         */
        void rotateLight(float xpos, float ypos);

        /**
         * @brief Rotate light and stop rotateLight() mode
         */
        void stopRotateLight();

    protected:
        std::unique_ptr<SceneImpl> pimpl; ///<-- Tucano data

        SceneImpl& Impl();
        const SceneImpl& Impl() const;

        /** @relates tucanow::Gui
         * */
        friend class Gui;

    private:
        std::array<float, 3> bbox_origin, bbox_size;
        bool render_wireframe = false; ///<-- Controls whether wireframe or default rendering is used

        bool headlight_camera = true; ///<-- Controls whether a headlight or a fixed light is used

        float scale_width = 1.0f; ///<-- Ratio framebuffer_width/window_width
        float scale_height = 1.0f; ///<-- Ratio framebuffer_height/window_height
};


} // namespace tucanow


#endif // MODELWIDGET
