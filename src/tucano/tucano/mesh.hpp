/**
 * Tucano - A library for rapid prototying with Modern OpenGL and GLSL
 * Copyright (C) 2014
 * LCG - Laboratório de Computação Gráfica (Computer Graphics Lab) - COPPE
 * UFRJ - Federal University of Rio de Janeiro
 *
 * This file is part of Tucano Library.
 *
 * Tucano Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tucano Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tucano Library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MESH__
#define __MESH__

#include <tucano/model.hpp>
#include <tucano/shader.hpp>
#include <memory>


using ulong = unsigned long int;
using uint = unsigned int;


namespace Tucano
{

class Mesh;

/**
 * @brief A vertex attribute of a mesh.
 *
 * Some common attributes (coordinate, color, normal, texCoord) have custom loaders.
 * Otherwise, a generic attribute can be loaded.
 */
class VertexAttribute
{
    friend class Mesh;
private:
public:
    /// Attribute's name
    string name;
    /// size of attribute array (usually number of vertices)
    int size = 0;
    /// number of elements per attribute
    int element_size = 0;
    /// this is set by the shader using the attribute
    GLint location = -1;
    /// Vertex array Buffer ID
    GLuint bufferID = 0;
    /// Type of attribute element (ex. GL_FLOAT)
    GLenum type = GL_FLOAT;
    /// Type of attribute array (GL_ARRAY_BUFFER for most cases), indices are GL_ELEMENT_ARRAY_BUFFER
    GLenum array_type = GL_ARRAY_BUFFER;

    std::shared_ptr < GLuint > bufferID_sptr;

public:


    //VertexAttribute() = default;

    VertexAttribute(string in_name, int in_num_elements, int in_element_size, GLenum in_type, GLenum in_array_type = GL_ARRAY_BUFFER) :
        name(in_name), size(in_num_elements), element_size(in_element_size), type(in_type), array_type (in_array_type)
    {
        // create new buffer for attribute if it is not yet set (ex copy constructor)
        glGenBuffers(1, &bufferID);

       bufferID_sptr = std::shared_ptr < GLuint > ( 
                    new GLuint (bufferID),
                    [] (GLuint *p) {
                        glDeleteBuffers(1, p);
                        delete p;
                    }
                    );
        #ifdef TUCANODEBUG
        Tucano::Misc::errorCheckFunc(__FILE__, __LINE__, "mesh constructor");
        #endif
    }

   /**
     * @brief Returns the name of the attribute
     * @return Attribute's name
     */
    string getName (void) const {return name;}

    /**
     * @brief Returns the number of elements of this attribute (usually number of vertices in VAO)
     * @return Number of elements
     */
    int getSize (void) const {return size;}

    /**
     * @brief Returns the number of elements per attribute
     * For example, returns 3 if attribute is of type vec3
     * @return Size of one attribute
     */
    int getElementSize (void) const {return element_size;}

    /**
     * @brief Returns the type of the attribute (ex. GL_FLOAT)
     * @return Type of attribute
     */
    GLenum getType (void) const {return type;}

    /**
     * @brief Returns attribute's type size (ex. 4 for GL_FLOAT)
     * @return Attributes's type size in bytes.
     */
    uint getTypeSize() const
    {
		switch( type )
		{
			case GL_FLOAT:
			case GL_UNSIGNED_INT:
			case GL_INT: return 4;
			
			case GL_DOUBLE: return 8;
			
			case GL_BYTE:
			case GL_UNSIGNED_BYTE: return 1;
			
			case GL_SHORT:
			case GL_UNSIGNED_SHORT: return 2;
			
			default : throw logic_error( "Cannot calculate size of unexpected type." );
		}
	}
    
    /**
     * @brief Returns the type of array (ex. GL_ARRAY_BUFFER)
     * @return Type of array
     */
    GLenum getArrayType (void) const {return array_type;}

    /**
     * @brief Sets the type of array (default is GL_ARRAY_BUFFER)
     * @param at Array type
     */
    void setArrayType (GLenum at) {array_type = at;}

    /**
     * @brief Returns the location of the attribute.
     * The location is usually set by the shader, since it can be used by different
     * shaders in different situations, and we don't want to force the shader layout id to be
     * known beforehand
     * @return Attribute shader location
     */
    GLint getLocation(void) const {return location;}

    /**
     * @brief Sets the location of the attribute for a shader.
     * @param loc Attribute shader location
     */
    void setLocation(GLint loc) {location = loc;}

    /**
     * @brief Returns the id of the vertex array of this attribute
     * @return Buffer ID
     */
    GLuint getBufferID (void) {return *bufferID_sptr;}

    /// Bind the attribute
    void bind(void)
    {
        glBindBuffer(array_type, *bufferID_sptr);
    }

    /// Bind the attribute to a given location
    void enable(GLint loc)
    {
        setLocation(loc);
        enable();
    }

    /// Binds the attribute to its location
    void enable()
    {
        if (location != -1)
        {
            glBindBuffer(array_type, *bufferID_sptr);
            glVertexAttribPointer(location, element_size, type, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(location);
        }
    }


    /// Unbind the attribute
    void unbind(void)
    {
        glBindBuffer(array_type, 0);
    }

    /// Disables associated location if attribute has one
    void disable(void)
    {
        if (location != -1)
        {
            glDisableVertexAttribArray(location);
        }
    }


    /** 
     * @brief Maps a range in the buffer so application can change its contents. The mapped range is write-only and the vertex
     * attribute must be bind() beforehand.
     * @param offset is the offset in the buffer where the mapped range starts (in element units).
     * @param length is the lenght of the mapped range (in element units).
     * @returns a pointer to the mapped buffer range. float* is used in order to unify return type.
     */
    float* map( unsigned int offset, unsigned int length )
    {
        int typeSize = getTypeSize();
        float* ptr = ( float * ) glMapBufferRange( array_type, offset * typeSize * element_size ,
                                                   length * typeSize * element_size,
                                             GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT );
        return ptr;
    }
    
    /**
     * @brief Unmaps the range associated with this vertex attribute. The vertex attribute must be bind() beforehand.
     */
    void unmap()
    {
        glUnmapBuffer( array_type );
    }


};

/**
 * @brief A common Mesh, usually containing triagles or points.
 *
 * The Mesh class handles the buffers generation and usage. such as VBO. IBO. TBO... It also contains methods for simple meshes generation. (e.g. a quadrilateral or a cube) and a wavefront .obj
 * loader. When a mesh is generated. geometrical computations are performed. detecting the center of the mesh through the axis-aligned bounding box and computing normalizating scale factors.
 * The attribute locations are predefined in this class. being the location 0 used for Vertex Buffer. 1 for Normals Buffer. 2 for Color Buffer and 3 for TexCoord Buffer.
 **/
class Mesh : public Tucano::Model {

public:

    enum PrimitiveType
    {
        POINT = GL_POINTS,
        CURVE = GL_LINES,
        TRIANGLE = GL_TRIANGLES,
        PATCH = GL_PATCHES
    };


protected:

    ///Array of generic attributes
    vector < Tucano::VertexAttribute > vertex_attributes;

    ///Number of vertices in vertices array.
    unsigned int numberOfVertices = 0;

    ///Number of normals in normals array.
    unsigned int numberOfNormals = 0;

    ///Number of indices in indices array
    unsigned int numberOfElements = 0;

    ///Number of texture coordinates in texCoords array.
    unsigned int numberOfTexCoords = 0;

    ///Number of colors in colors array.
    unsigned int numberOfColors = 0;

    /// Index Buffer
    GLuint index_buffer_id = 0;

    /// Vertex Array Object ID (VAO is just a descriptor, does not contain any data)
    GLuint vao_id = 0;

    /// Variable indicating if there will be tessellation
    bool willTessellate = false;

    /// Shared pointer for index buffer
    std::shared_ptr < GLuint > index_buffer_sptr = 0;

    /// Shared pointer for vertex array object
    std::shared_ptr < GLuint > vao_sptr = 0;

    PrimitiveType primitiveType = TRIANGLE;

public:

    /**
    * @brief Default Constructor.
    */
    Mesh (void)
    {

        glGenBuffers(1, &index_buffer_id);
        index_buffer_sptr = std::shared_ptr < GLuint > ( 
                    new GLuint (index_buffer_id),
                    [] (GLuint *p) {
                        glDeleteBuffers(1, p);
                        delete p;
                    }
                    );

        glGenVertexArrays(1, &vao_id);
        vao_sptr = std::shared_ptr < GLuint > ( 
                    new GLuint (vao_id),
                    [] (GLuint *p) {
                        glDeleteVertexArrays(1, p);
                        delete p;
                    }
                    );


        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
        #endif
    }

    /**
     * @brief Returns the default color of the model
     * @return RGBA default color
     */
    Eigen::Vector4f getColor (void)
    {
        return default_color;
    }

    /**
     * @brief Sets the default color of the model
     * @param color Given new default color
     */
    void setColor (const Eigen::Vector4f & color)
    {
        Model::setColor(color);
        default_color = color;

        size_t attrib_index;
        bool found = hasAttribute("in_Color", attrib_index);

        if ( found )
        {
            auto it = vertex_attributes.begin() + attrib_index;
            vertex_attributes.erase(it);
        }
    }

/*
    #if __cplusplus > 199711L
		/// Move constructor.
		Mesh( Mesh&& other )
		: vertex_attributes( std::move( other.vertex_attributes ) ),
		primitiveType( other.primitiveType ),
		numberOfVertices( other.numberOfVertices ),
		numberOfNormals( other.numberOfNormals ),
		numberOfElements( other.numberOfElements ),
		numberOfTexCoords( other.numberOfTexCoords ),
		numberOfColors( other.numberOfColors ),
		index_buffer_id( other.index_buffer_id ),
		vao_id( other.vao_id )
		{
			other.reset();
		}
		
		Mesh& operator=( Mesh&& other )
		{
			vertex_attributes = std::move( other.vertex_attributes );
			primitiveType = other.primitiveType;
			numberOfVertices = other.numberOfVertices;
			numberOfNormals = other.numberOfNormals;
			numberOfElements = other.numberOfElements;
			numberOfTexCoords = other.numberOfTexCoords;
			numberOfColors = other.numberOfColors;
			index_buffer_id = other.index_buffer_id;
			vao_id = other.vao_id;
		
			other.reset();
			
			return *this;
		}
	#endif
*/
    
    /**
     * @brief Returns the number of elements (primitives such as triangles) of the mesh.
     * @return Number of primitives.
     */
    int getNumberOfElements (void)
    {
        return numberOfElements;
    }

    /**
     * @brief Returns the number of vertices in the mesh.
     * @return Number of vertices.
     */
    int getNumberOfVertices (void)
    {
        return numberOfVertices;
    }

    /**
     * @brief Resets all vertex attributes locations to -1.
     */
    void resetLocations (void)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].setLocation(-1);
        }
    }

    void reset (void)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].disable();
        }
        vertex_attributes.clear();

        /// Shape matrix holds information about intrinsic scaling of other affine transformation of the object
        shape_matrix = Eigen::Affine3f::Identity();

        /// Model matrix, holds information about the models location and orientation.
        model_matrix = Eigen::Affine3f::Identity();

        /// Center of the mesh object.
        objectCenter = Eigen::Vector3f::Zero();

        /// Object's centroid (different from center of bounding box)
        centroid = Eigen::Vector3f::Zero();

        /// Radius of the mesh bounding sphere.
        radius = 1.0;

        /// The normalization scale factor, scales the model matrix to fit the model inside a unit cube.
        normalization_scale = 1.0;

        /// Default color
        default_color = Eigen::Vector4f (0.7, 0.7, 0.7, 1.0);
    }

    /**
	 * @brief Select the rendering primitive used when rendering this mesh.
	 */
    void selectPrimitive( const PrimitiveType primitive )
	{
		primitiveType = primitive;
	}
    
    ///Default Destructor. Deletes the bufferIDs. vertices. normals. colors. indices and texCoords arrays.
    virtual ~Mesh (void)
    {
        reset();
    }

protected:

	/**
	 * @brief Map a range of an attribute.
	 * @param name Name of the attribute.
	 * @param offset offset of the range start (in element units).
	 * @param length is the length of the range (in element units).
	 * @return A pointer to the mapped range.
	 */
	float* mapAttrib( const string name, const ulong offset, const ulong length )
	{
		// Not optimized for a great number of vertex atributes...
		for( unsigned int i = 0; i < vertex_attributes.size(); ++i )
        {
			VertexAttribute& attrib = vertex_attributes[ i ];
			if( !attrib.getName().compare( name ) )
			{
				attrib.bind();
				float* ptr = attrib.map( offset, length );
				return ptr;
			}
		}
		
		throw logic_error( "Trying to map vertex attribute before instancing it." );
	}
	
	/**
	 * @brief Unmap a range of an attribute.
	 * @param name Name of the attribute.
	 */
	void unmapAttrib( const string name )
	{
		// Not optimized for a great number of vertex atributes...
		for( unsigned int i = 0; i < vertex_attributes.size(); ++i )
        {
			VertexAttribute& attrib = vertex_attributes[ i ];
			if( !attrib.getName().compare( name ) )
			{
				attrib.bind();
				attrib.unmap();
				
				return;
			}
		}
		
		throw logic_error( "Trying to unmap vertex attribute before instancing it." );
	}

    /**
     * @brief Computes bounding box and centroid and normalization factors (normalization_scale).
     * @param vert Array of vertices.
     */
    void processVertices(const vector<Eigen::Vector4f> &vert)
    {
        float xMax = 0; float xMin = 0; float yMax = 0; float yMin = 0; float zMax = 0; float zMin = 0;

        int temp = 0;
        for(unsigned int i = 0; i < numberOfVertices*4; i+=4) {

            //X:
            if(vert[temp][0] > xMax) {
                xMax = vert[temp][0];
            }
            if(vert[temp][0] < xMin) {
                xMin = vert[temp][0];
            }

            //Y:
            if(vert[temp][1] > yMax) {
                yMax = vert[temp][1];
            }
            if(vert[temp][1] < yMin) {
                yMin = vert[temp][1];
            }

            //Z:
            if(vert[temp][2] > zMax) {
                zMax = vert[temp][2];
            }
            if(vert[temp][2] < zMin) {
                zMin = vert[temp][2];
            }

            //W:
            temp++;
        }

        normalization_scale = 1.0/max(max(xMax-xMin, yMax-yMin), zMax-zMin);

        float centerX = (xMax+xMin)/2.0;
        float centerY = (yMax+yMin)/2.0;
        float centerZ = (zMax+zMin)/2.0;
        objectCenter = Eigen::Vector3f(centerX, centerY, centerZ);

        // compute the centroid (different from the box center)
        centroid = Eigen::Vector3f::Zero();
        for(unsigned int i = 0; i < numberOfVertices; i++) {
            centroid = centroid + vert[i].head(3);//Eigen::Vector3f(vert[i][0], vert[i][1], vert[i][2]);
        }
        centroid = centroid / numberOfVertices;

        // compute the radius of the bounding sphere
        // most distance point from the centroid
        radius = 0.0;
        for(unsigned int i = 0; i < numberOfVertices; i++) {
            radius = max(radius, ( vert[i].head(3) - centroid ).norm());
        }

        normalization_scale = 1.0/radius;
    }

    /**
     * @brief Computes bounding box and centroid and normalization factors (normalization_scale).
     * @param vert Array of vertices.
     */
    void processVertices3(const vector<float> &vert)
    {
        float xMax = 0; float xMin = 0; float yMax = 0; float yMin = 0; float zMax = 0; float zMin = 0;
        centroid = Eigen::Vector3f::Zero();

        for(unsigned int i = 0; i < numberOfVertices; ++i) {

            //X:
            if(vert[3*i + 0] > xMax) {
                xMax = vert[3*i + 0];
            }
            if(vert[3*i + 0] < xMin) {
                xMin = vert[3*i + 0];
            }

            //Y:
            if(vert[3*i + 1] > yMax) {
                yMax = vert[3*i + 1];
            }
            if(vert[3*i + 1] < yMin) {
                yMin = vert[3*i + 1];
            }

            //Z:
            if(vert[3*i + 2] > zMax) {
                zMax = vert[3*i + 2];
            }
            if(vert[3*i + 2] < zMin) {
                zMin = vert[3*i + 2];
            }

            //centroid:
            centroid = centroid + Eigen::Vector3f(vert[3*i + 0], vert[3*i + 1], vert[3*i + 2]);

        }

        centroid = centroid / numberOfVertices;

        normalization_scale = 1.0/max(max(xMax-xMin, yMax-yMin), zMax-zMin);

        float centerX = (xMax+xMin)/2.0;
        float centerY = (yMax+yMin)/2.0;
        float centerZ = (zMax+zMin)/2.0;
        objectCenter = Eigen::Vector3f(centerX, centerY, centerZ);

        /* // compute the centroid (different from the box center) */
        /* centroid = Eigen::Vector3f::Zero(); */
        /* for(unsigned int i = 0; i < numberOfVertices; i++) { */
        /*     centroid = centroid + Eigen::Vector3f(vert[i + 0], vert[i + 1], vert[i + 2]); */
        /* } */
        /* centroid = centroid / numberOfVertices; */

        // compute the radius of the bounding sphere
        // farthest point from the centroid
        radius = 0.0;
        for(unsigned int i = 0; i < numberOfVertices; i++) {
            radius = max(radius, ( Eigen::Vector3f(vert[i + 0], vert[i + 1], vert[i + 2]) - centroid ).norm());
        }

        normalization_scale = 1.0/radius;
    }

public:

    /**
     * @brief Load vertices (x,y,z,w) and creates appropriate vertex attribute.
     * The default attribute name is "in_Position".
     * Computes bounding box and centroid and normalization factors (normalization_scale).
     * @param vert Array of vertices.
     */
    void loadVertices (vector<Eigen::Vector4f> &vert)
    {
        std::vector<float> vertices( 3*vert.size() );
        for (size_t i = 0; i < vert.size(); ++i)
        {
            vertices[3*i + 0] = vert[i][0];
            vertices[3*i + 1] = vert[i][1];
            vertices[3*i + 2] = vert[i][2];
        }

        loadVertices(vertices);

        /* numberOfVertices = vert.size(); */

        /* // creates new attribute and load vertex coordinates */
        /* createAttribute("in_Position", vert); */

        /* // from now on we are just computing some information about the model such as bounding box, centroid ... */
        /* processVertices(vert); */
    }

    /**
     * @brief Load vertices (x,y,z) and creates appropriate vertex attribute.
     * The default attribute name is "in_Position".
     * Computes bounding box and centroid and normalization factors (normalization_scale).
     * @param vert Array of vertices.
     * @return True if vertices vector is multiple of 3, false otherwise.
     */
    bool loadVertices(const vector<float> &vert)
    {
        if ( vert.empty() )
        {
            return false;
        }

        if ( vert.size() % 3 != 0 )
        {
            return false;
        }

        numberOfVertices = vert.size()/3;

        // creates new attribute and load vertex coordinates
        createAttribute3("in_Position", vert);

        // from now on we are just computing some information about the model such as bounding box, centroid ...
        processVertices3(vert);

        return true;
    }

    /**
     * @brief Load normals (x,y,z) as a vertex attribute.
     * @param norm Normals list.
     */
    void loadNormals (vector<Eigen::Vector3f> &norm)
    {
        numberOfNormals = norm.size();

        createAttribute("in_Normal", norm);
    }

    /**
     * @brief Load normals (x,y,z) as a vertex attribute.
     * @param norm Normals list.
     * @return True if normals vector is multiple of 3, false otherwise.
     */
    bool loadNormals (const vector<float> &norm)
    {
        if ( norm.empty() )
        {
            return false;
        }

        if ( norm.size() % 3 != 0 )
        {
            return false;
        }

        numberOfNormals = norm.size()/3;

        createAttribute3("in_Normal", norm);

        return true;
    }

    /**
     * @brief Load tex coords (u,v) as a vertex attribute.
     * Optionally normalizes coords in range [0,1]
     * @param tex Texture coordinates array.
     * @param normalize If true normalizes the texcoords in range [0,1], otherwise does not normalize.
     */
    void loadTexCoords (vector<Eigen::Vector2f> &tex, bool normalize = false)
    {
        numberOfTexCoords = tex.size();

        if (normalize)
        {
            vector<Eigen::Vector2f> tex_normalized;

            float texXmax = tex[0][0];
            float texXmin = tex[0][0];
            float texYmax = tex[0][1];
            float texYmin = tex[0][1];

            // compute min and max values for x and y
            for(unsigned int i = 0; i < numberOfTexCoords; i++)
            {
                if (tex[i][0] < texXmin) texXmin = tex[i][0];
                if (tex[i][0] > texXmax) texXmax = tex[i][0];
                if (tex[i][1] < texYmin) texYmin = tex[i][1];
                if (tex[i][1] > texYmax) texYmax = tex[i][1];

            }
            for(unsigned int i = 0; i < numberOfTexCoords; i++)
            {
                tex_normalized.push_back ( Eigen::Vector2f(
                                               (tex[i][0] - texXmin) / (texXmax - texXmin),
                                           (tex[i][1] - texYmin) / (texYmax - texYmin) ) );
            }
            createAttribute("in_TexCoords", tex_normalized);
        }
        else
        {
            createAttribute("in_TexCoords", tex);
        }
    }

    /**
     * @brief Load tex coords (u,v) as a vertex attribute.
     * Optionally normalizes coords in range [0,1]
     * @param tex Texture coordinates array.
     * @param normalize If true normalizes the texcoords in range [0,1], otherwise does not normalize.
     * @return True if texture vector is multiple of 2, false otherwise.
     */
    bool loadTexCoords (const vector<float> &tex, bool normalize = false)
    {
        if ( tex.empty() )
        {
            return false;
        }

        if ( tex.size() % 2 != 0 )
        {
            return false;
        }

        numberOfTexCoords = tex.size()/2;

        if (normalize)
        {
            /* vector<Eigen::Vector2f> tex_normalized; */
            vector<float> tex_normalized;

            float texXmax = tex[0 + 0];
            float texXmin = tex[0 + 0];
            float texYmax = tex[0 + 1];
            float texYmin = tex[0 + 1];

            // compute min and max values for x and y
            for(unsigned int i = 0; i < numberOfTexCoords; ++i)
            {
                if (tex[2*i + 0] < texXmin) texXmin = tex[2*i + 0];
                if (tex[2*i + 0] > texXmax) texXmax = tex[2*i + 0];
                if (tex[2*i + 1] < texYmin) texYmin = tex[2*i + 1];
                if (tex[2*i + 1] > texYmax) texYmax = tex[2*i + 1];

            }
            for(unsigned int i = 0; i < numberOfTexCoords; ++i)
            {
                /* tex_normalized.push_back ( Eigen::Vector2f( */
                                               /* (tex[i + 0] - texXmin) / (texXmax - texXmin), */
                                           /* (tex[i + 1] - texYmin) / (texYmax - texYmin) ) ); */
                tex_normalized[2*i + 0] = (tex[2*i + 0] - texXmin) / (texXmax - texXmin);
                tex_normalized[2*i + 0] = (tex[2*i + 0] - texXmin) / (texXmax - texXmin);
            }
            createAttribute2("in_TexCoords", tex_normalized);
        }
        else
        {
            createAttribute2("in_TexCoords", tex);
        }

        return true;
    }


    /**
     * @brief Load colors (r,g,b,a) as a vertex attribute.
     * @param clrs Colors array.
     */
    void loadColors (vector<Eigen::Vector4f> &clrs)
    {
        createAttribute("in_Color", clrs);
    }

    /**
     * @brief Load colors (r,g,b,a) as a vertex attribute.
     * @param clrs Colors array.
     * @return True if colors vector is multiple of 4, false otherwise.
     */
    bool loadColorsRGBA (const vector<float> &clrs)
    {
        if ( clrs.empty() )
        {
            return false;
        }

        if ( clrs.size() % 4 != 0 )
        {
            return false;
        }

        createAttribute4("in_Color", clrs);

        return true;
    }

    /**
     * @brief Load colors (r,g,b) as a vertex attribute.
     * @param clrs Colors array.
     * @return True if colors vector is multiple of 3, false otherwise.
     */
    bool loadColorsRGB (const vector<float> &clrs)
    {
        if ( clrs.empty() )
        {
            return false;
        }

        if ( clrs.size() % 3 != 0 )
        {
            return false;
        }

        createAttribute3("in_Color", clrs);

        return true;
    }


    /**
     * @brief Load indices into indices array
     * @param ind Indices array.
     */
    template<typename Integer>
    bool loadIndices(const vector<Integer> &ind)
    {
        if ( ind.empty() )
        {
            return false;
        }

        numberOfElements = ind.size();
        std::vector<GLuint> indices(numberOfElements);

        for( size_t i = 0; i < numberOfElements; ++i )
        {
            indices[i] = static_cast<GLuint>(ind[i]);
        }

        loadIndices(indices);

        return true;
    }


    /**
     * @brief Load indices into indices array
     * @param ind Indices array.
     */
    void loadIndices(const vector<GLuint> &ind)
    {
        numberOfElements = ind.size();

        /* GLuint *indices = new GLuint[ind.size()]; */

        /* //Indices: */
        /* for(unsigned int i = 0; i < ind.size(); i++) */
        /* { */
        /*     indices[i] = ind[i]; */
        /* } */

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_sptr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size()*sizeof(GLuint), ind.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        /* delete [] indices; */
    }


	/**
	 * @brief Reserve vertex attribute memory in order to fill latter on, with mapVertices().
	 * @param element_size number of elements in an attribute value (number of coordinates in a vertex, for instance).
	 * @param size Number of attribute values (size of the vertex attribute array).
	 */
	void reserveVertices( const int element_size, const ulong size )
	{
		numberOfVertices = size;
		createAttribute( "in_Position", element_size, size );
	}
    
	/**
	 * @brief Reserve normal attribute memory in order to fill latter on, with mapNormals().
	 * @param element_size number of elements in an attribute value (number of coordinates in a vertex, for instance).
	 * @param size Number of attribute values (size of the vertex attribute array).
	 */
    void reserveNormals( const ulong size )
	{
		numberOfNormals = size;
		createAttribute( "in_Normal", 3, size );
	}
	
	/**
	 * @brief Reserve color attribute memory in order to fill latter on, with mapColors().
	 * @param element_size number of elements in an attribute value (number of coordinates in a vertex, for instance).
	 * @param size Number of attribute values (size of the vertex attribute array).
	 */
    void reserveColors( const int element_size, const ulong size )
	{
		numberOfNormals = size;
		createAttribute( "in_Color", element_size, size );
	}
	
	/**
	 * @brief Reserve texture coordinate attribute memory in order to fill latter on, with mapTexCoords().
	 * @param size Number of attribute values (size of the vertex attribute array).
	 */
    void reserveTexCoords( const ulong size )
	{
		numberOfTexCoords = size;
		createAttribute( "in_TexCoords", 2, size );
	}
	
	/**
	 * @brief Reserve index buffer memory in order to fill it latter on, with mapIndices().
	 * @param size Number of attribute values (number of indices).
	 */
	void reserveIndices( const ulong size )
	{
		numberOfElements = size;
		glGenBuffers( 1, &index_buffer_id );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer_id );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, size * sizeof( uint ), NULL, GL_DYNAMIC_DRAW );
	}
    
    /**
	 * @brief Map a range of the vertices attribute.
	 * @param offset offset of the range start (in element units).
	 * @param length is the length of the range (in element units).
	 * @return A pointer to the mapped range.
	 */
    float* mapVertices( const ulong offset, const ulong length )
	{
		return mapAttrib( "in_Position", offset, length );
	}
	
	/**
	 * @brief Map a range of the normals attribute.
	 * @param offset offset of the range start (in element units).
	 * @param length is the length of the range (in element units).
	 * @return A pointer to the mapped range.
	 */
	float* mapNormals( const ulong offset, const ulong length )
	{
		return mapAttrib( "in_Normal", offset, length );
	}
	
	/**
	 * @brief Map a range of the colors attribute.
	 * @param offset offset of the range start (in element units).
	 * @param length is the length of the range (in element units).
	 * @return A pointer to the mapped range.
	 */
	float* mapColors( const ulong offset, const ulong length )
	{
		return mapAttrib( "in_Color", offset, length );
	}
	
	/**
	 * @brief Map a range of the texture coordinates attribute.
	 * @param offset offset of the range start (in element units).
	 * @param length is the length of the range (in element units).
	 * @return A pointer to the mapped range.
	 */
	float* mapTexCoords( const ulong offset, const ulong length )
	{
		return mapAttrib( "in_TexCoords", offset, length );
	}
	
	/**
	 * @brief Map a range of the indices buffer.
	 * @param offset offset of the range start (in element units).
	 * @param length is the length of the range (in element units). The index buffer used in rendering will have the same
	 * lenght.
	 * @return A pointer to the mapped range.
	 */
	uint* mapIndices( const ulong offset, const ulong length )
	{
		numberOfElements = length;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer_id );
		uint* ptr = ( uint* ) glMapBufferRange( GL_ELEMENT_ARRAY_BUFFER, offset * sizeof( uint ), length * sizeof( uint ),
												GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT );
		return ptr;
	}
    
    /**
	 * @brief Unmap a range of the vertices vertex attribute.
	 */
    void unmapVertices()
	{
		unmapAttrib( "in_Position" );
	}
	
	/**
	 * @brief Unmap a range of the normals vertex attribute.
	 */
	void unmapNormals()
	{
		unmapAttrib( "in_Normal" );
	}
	
	/**
	 * @brief Unmap a range of the colors vertex attribute.
	 */
	void unmapColors()
	{
		unmapAttrib( "in_Color" );
	}
	
	/**
	 * @brief Unmap a range of the texture coordinates vertex attribute.
	 */
	void unmapTexCoords()
	{
		unmapAttrib( "in_TexCoords" );
	}
	
	/**
	 * @brief Unmap a range of the index buffer.
	 */
	void unmapIndices()
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer_id );
		glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );
	}
    
    /**
     * @brief Sets default attribute locations.
     * vertex coords -> location 0
     * normals -> location 1
     * colors -> location 2
     * texCoords -> location 3
     */
    void setDefaultAttribLocations (void)
    {
		resetLocations();
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if (!vertex_attributes[i].getName().compare("in_Position"))
            {
                vertex_attributes[i].setLocation(0);
            }
            else if (!vertex_attributes[i].getName().compare("in_Normal"))
            {
                vertex_attributes[i].setLocation(1);
            }
            else if (!vertex_attributes[i].getName().compare("in_Color"))
            {
                vertex_attributes[i].setLocation(2);
            }
            else if (!vertex_attributes[i].getName().compare("in_TexCoords"))
            {
                vertex_attributes[i].setLocation(3);
            }
        }
    }

    /**
     * @brief Returns whether an attribute exists or not.
     * @param name Name of attribute to be queried.
     * @return True if attribute exists, false otherwise.
     */
    bool hasAttribute (const string& name) const
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if (!vertex_attributes[i].getName().compare(name))
            {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns whether an attribute exists or not.
     * @param name Name of attribute to be queried.
     * @param attrib_index Index of queried attribute, if found.
     * @return True if attribute exists, false otherwise.
     */
    bool hasAttribute (const string& name, size_t &attrib_index ) const
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if ( vertex_attributes[i].getName().compare(name) == 0 )
            {
                attrib_index = static_cast<size_t>(i);
                return true;
            }
        }
        return false;
    }


    /**
    * @brief Returns a pointer to an attribute
    * Given an attribute name, searches to see if it exists, if so, returns a pointer to it
    * @param name Attribute name
    * @return Pointer to attribute, or NULL if it does not exist
    */
    VertexAttribute* getAttribute(const string& name)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if (!vertex_attributes[i].getName().compare(name))
            {
                return &vertex_attributes[i];
            }
        }
        return NULL;
    }

    /**
     * @brief Automatically sets the attribute locations for a given Shader.
     *
     * For every mesh attribute, the Shader will be queried for an attribute with the same name,
     * if one is found they will linked.
     * @param shader A pointer to the given shader.
     */
    void setAttributeLocation (Shader *shader)
    {       
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            GLint loc = shader->getAttributeLocation(vertex_attributes[i].getName().c_str());
            vertex_attributes[i].setLocation(loc);
        }
        if (shader->getTessellationEvaluationShader() != 0)
		{
			primitiveType = PATCH;
		}
        
    }

	void setAttributeLocation (const Shader& shader)
	{
		setAttributeLocation((Shader*)(&shader));
	}


    /**
     * @brief Sets the location of a generic vertex attribute.
     * @param name Name of the given attribute
     * @param loc Location of the attribute.
     */
    void setAttributeLocation (string name, GLint loc)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if (!name.compare(vertex_attributes[i].getName()))
            {
                vertex_attributes[i].setLocation(loc);
            }
        }
    }


    /**
     * @brief Creates and loads a new mesh attribute of 4 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute(string name, vector<Eigen::Vector4f> &attrib)
    {
        /* // create new vertex attribute */
        /* VertexAttribute va (name, attrib.size(), 4, GL_FLOAT); */

        /* float *attrib_array = new float[ va.getSize()*va.getElementSize() ]; */
        std::vector<float> attrib_array( 4*attrib.size() );

        int temp = 0;
        for(size_t i = 0; i < 4*attrib.size(); i+=4)
        {
            attrib_array[i] = attrib[temp][0];
            attrib_array[i+1] = attrib[temp][1];
            attrib_array[i+2] = attrib[temp][2];
            attrib_array[i+3] = attrib[temp][3];
            temp++;
        }

        /* // fill buffer with attribute data */
        /* va.bind(); */
        /* glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*va.getTypeSize(), attrib_array, GL_STATIC_DRAW); */
        /* va.unbind(); */

        /* vertex_attributes.push_back(va); */
        /* delete [] attrib_array; */
        /* return &vertex_attributes[vertex_attributes.size()-1]; */

        return createAttribute4(std::move(name), attrib_array);
    }
    
    /**
     * @brief Creates and loads a new mesh attribute of 4 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute4(string name, const vector<float> &attrib)
    {
        // create new vertex attribute
        VertexAttribute va (name, attrib.size()/4, 4, GL_FLOAT);

        // fill buffer with attribute data
        /* va.bind(); */
        /* glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*va.getTypeSize(), attrib.data(), GL_STATIC_DRAW); */
        /* va.unbind(); */
        fillBufferWithAttribute(va, attrib);

        /* vertex_attributes.push_back(va); */
        /* return &vertex_attributes[vertex_attributes.size()-1]; */
        return pushAttribute(va);
    }
    
    /**
     * @brief Fill buffer with attribute data
     *
     * @param va Vertex attribute whose buffer is meant to be filled.
     * @param attrib Array with new attribute data.
     */
    void fillBufferWithAttribute( VertexAttribute &va, const std::vector<float> &attrib )
    {
        // fill buffer with attribute data
        va.bind();
        glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*va.getTypeSize(), attrib.data(), GL_STATIC_DRAW);
        va.unbind();
    }

    /**
     * @brief Loads a new vertex attribute, substituting an older attribute if of the same name
     *
     * @param va Vertex attribute to store.
     * @return Pointer to created attribute
     */
    VertexAttribute* pushAttribute( VertexAttribute &va )
    {
        size_t attrib_index;
        bool found = hasAttribute(va.getName(), attrib_index);

        if ( found )
        {
            vertex_attributes[attrib_index] = va;
        }
        else
        {
            attrib_index = vertex_attributes.size();
            vertex_attributes.push_back(va);
        }

        return &vertex_attributes[attrib_index];
    }

    /**
     * @brief Creates and loads a new mesh attribute of 3 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute(string name, vector<Eigen::Vector3f> &attrib)
    {
        /* // create new vertex attribute */
        /* VertexAttribute va (name, attrib.size(), 3, GL_FLOAT); */

        /* float *attrib_array = new float[ va.getSize()*va.getElementSize() ]; */
        std::vector<float> attrib_array( 3*attrib.size() );

        size_t temp = 0;
        for(size_t i = 0; i < 3*attrib.size(); i+=3) {
            attrib_array[i] = attrib[temp][0];
            attrib_array[i+1] = attrib[temp][1];
            attrib_array[i+2] = attrib[temp][2];
            temp++;
        }

        /* // fill buffer with attribute data */
        /* va.bind(); */
        /* glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*va.getTypeSize(), attrib_array, GL_STATIC_DRAW); */
        /* va.unbind(); */

        /* vertex_attributes.push_back(va); */
        /* delete [] attrib_array; */
        /* return &vertex_attributes[vertex_attributes.size()-1]; */

        return createAttribute3(std::move(name), attrib_array);
    }

    /**
     * @brief Creates and loads a new mesh attribute of 3 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute3(string name, const vector<float> &attrib)
    {
        // create new vertex attribute
        VertexAttribute va (name, attrib.size()/3, 3, GL_FLOAT);

        // fill buffer with attribute data
        /* va.bind(); */
        /* glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*va.getTypeSize(), attrib.data(), GL_STATIC_DRAW); */
        /* va.unbind(); */
        fillBufferWithAttribute(va, attrib);

        /* vertex_attributes.push_back(va); */
        /* return &vertex_attributes[vertex_attributes.size()-1]; */
        return pushAttribute(va);
    }

    /**
     * @brief Creates and loads a new mesh attribute of 2 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute(string name, vector<Eigen::Vector2f> &attrib)
    {
        /* // create new vertex attribute */
        /* VertexAttribute va (name, attrib.size(), 2, GL_FLOAT); */

        /* float * attrib_array = new float[va.getSize()*va.getElementSize()]; */
        std::vector<float> attrib_array( 2*attrib.size() );

        size_t temp = 0;
        for(size_t i = 0; i < 2*attrib.size(); i+=2) {
            attrib_array[i] = attrib[temp][0];
            attrib_array[i+1] = attrib[temp][1];
            temp++;
        }

        /* // fill buffer with attribute data */
        /* va.bind(); */
        /* glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*va.getTypeSize(), attrib_array, GL_STATIC_DRAW); */
        /* va.unbind(); */

        /* vertex_attributes.push_back(va); */
        /* delete [] attrib_array; */
        /* return &vertex_attributes[vertex_attributes.size()-1]; */

        return createAttribute2(std::move(name), attrib_array);
    }

    /**
     * @brief Creates and loads a new mesh attribute of 2 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute2(string name, const vector<float> &attrib)
    {
        // create new vertex attribute
        VertexAttribute va (name, attrib.size()/2, 2, GL_FLOAT);

        // fill buffer with attribute data
        /* va.bind(); */
        /* glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*va.getTypeSize(), attrib.data(), GL_STATIC_DRAW); */
        /* va.unbind(); */
        fillBufferWithAttribute(va, attrib);

        /* vertex_attributes.push_back(va); */
        /* return &vertex_attributes[vertex_attributes.size()-1]; */
        return pushAttribute(va);
    }

	/**
	 * @brief Creates a new mesh attribute, not loading contents into it.
	 * @param name Name of the attribute.
	 * @param element_size number of elements in an attribute value (number of coordinates in a vertex, for instance).
	 * @param size Number of attribute values (size of the vertex attribute array).
	 * @return Pointer to created attribute.
	 */
    VertexAttribute* createAttribute( const string name, const int element_size, const ulong size )
	{
		VertexAttribute va( name, size, element_size, GL_FLOAT );
		
		va.bind();
		glBufferData( va.getArrayType(), va.getSize() * va.getElementSize() * va.getTypeSize(), NULL, GL_DYNAMIC_DRAW );
        // JD: why not unbind?
		
		/* vertex_attributes.push_back(va); */
		/* return &vertex_attributes.back(); */
        return pushAttribute(va);
	}
    


    /**
     * @brief Binds all buffers.
     *
     * If there is a index buffer it will also be binded.
     * Then, binds one buffer for each vertex attribute.
     */
    virtual void bindBuffers (void) 
    {
        assert (vao_sptr != 0);

//        std::cout << *vao_sptr << std::endl;
        glBindVertexArray(*vao_sptr); //Vertex Array Object

        if (*index_buffer_sptr)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_sptr);
        }

        // bind generic attributes
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].enable();
        }

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
        #endif
    }

    /**
     * @brief Unbinds all buffers.
     */
    virtual void unbindBuffers (void) 
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].disable();
        }
    }

    /**
     * @brief Render only points. Uses index buffer if it is defined.
     */
    virtual void renderPoints (void)
    {
		if( numberOfElements > 0 )
		{
			glDrawElements( GL_POINTS, numberOfElements, GL_UNSIGNED_INT, (GLvoid*)0 );
		}
		else
		{
			glDrawArrays(GL_POINTS, 0, numberOfVertices);
		}
    }

    /**
     * @brief Call the draw method for rendering triangles.
     * This method requires that a index buffer has been created.
     */
    virtual void renderLines (void) 
    {
		if( numberOfElements == 0 )
		{
			renderPoints();
		}
		else
		{
			glDrawElements(GL_LINES, numberOfElements, GL_UNSIGNED_INT, (GLvoid*)0);
		}
    }

    /**
     * @brief Call the draw method for rendering triangles.
     * This method requires that a index buffer has been created.
     */
    virtual void renderElements (void) 
    {
		if( numberOfElements == 0 )
		{
			renderPoints();
		}
		else
		{
			glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, (GLvoid*)0);
		}
    }

    /**
     * @brief Render all vertices as a continous line loop
     */
    virtual void renderLineLoop (void)
    {
        glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);
    }


    /**
     * @brief Call the draw method for rendering patches.
     * This method requires that a index buffer has been created.
     */
    virtual void renderPatches(void)
    {
		if( numberOfElements == 0 )
		{
			renderPoints();
		}
		else
		{
			glPatchParameteri(GL_PATCH_VERTICES, 3);
			glDrawElements(GL_PATCHES, numberOfElements,GL_UNSIGNED_INT, 0);
		}
    }

    /**
     * @brief Render the mesh triangles.
     * The method binds the buffers, calls the method to render triangles, and then unbinds all buffers.
     * Note that a index buffer is necessary.
     */
    virtual void render (void)
    {
        bindBuffers();

        switch( primitiveType )
        {
            case POINT: renderPoints(); break;
            case CURVE: renderLines(); break;
            case TRIANGLE: renderElements(); break;
            case PATCH: renderPatches(); break;
        }

        unbindBuffers();
    }

    /**
     * @brief Sets the mesh as a Parallelpiped with given dimensions, scales so larger side is equal to 1.
     * @param x Width
     * @param y Height
     * @param z Depth
     */
    void createParallelepiped(float x, float y, float z)
    {

        numberOfVertices = 32;
        numberOfElements = 36;
        //numberOfColors = 32;

        vector<Eigen::Vector4f> vert;
        vector<GLuint> ind;

        //Normalizing the cube coordinates:
        float scale = x;
        if (y > x) scale = y;
        if (z > scale) scale = z;

        vert.push_back( Eigen::Vector4f (-.5f*x/scale, -.5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (-.5f*x/scale,  .5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale,  .5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale, -.5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (-.5f*x/scale, -.5f*y/scale, -.5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (-.5f*x/scale,  .5f*y/scale, -.5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale,  .5f*y/scale, -.5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale, -.5f*y/scale, -.5f*z/scale, 1) );

        GLuint tempIndices[36] = {
            0,2,1,  0,3,2,
            4,3,0,  4,7,3,
            4,1,5,  4,0,1,
            3,6,2,  3,7,6,
            1,6,5,  1,2,6,
            7,5,6,  7,4,5
        };

        for(unsigned int i = 0; i < numberOfElements;i++) {
            ind.push_back( tempIndices[i] );
        }

        /*float colors[32] =
    {
        0, 0, 1, 1,
        1, 0, 0, 1,
        0, 1, 0, 1,
        1, 1, 0, 1,
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 0, 1, 1,
        0, 0, 1, 1
    };*/

        loadVertices(vert);
        loadIndices(ind);

        setDefaultAttribLocations();
    }

    /**
     * @brief Sets the mesh as Unit Quad.
     */
    void createQuad (void)
    {
        vector<Eigen::Vector4f> vert;
        vector<Eigen::Vector2f> texCoord;
        vector<GLuint> elementsVertices;

        vert.push_back ( Eigen::Vector4f(-1.0, -1.0, 0.0, 1.0) );
        vert.push_back ( Eigen::Vector4f( 1.0, -1.0, 0.0, 1.0) );
        vert.push_back ( Eigen::Vector4f( 1.0,  1.0, 0.0, 1.0) );
        vert.push_back ( Eigen::Vector4f(-1.0,  1.0, 0.0, 1.0) );

        elementsVertices.push_back(0);
        elementsVertices.push_back(1);
        elementsVertices.push_back(2);
        elementsVertices.push_back(2);
        elementsVertices.push_back(3);
        elementsVertices.push_back(0);

        texCoord.push_back ( Eigen::Vector2f(0.0, 0.0) );
        texCoord.push_back ( Eigen::Vector2f(1.0, 0.0) );
        texCoord.push_back ( Eigen::Vector2f(1.0, 1.0) );
        texCoord.push_back ( Eigen::Vector2f(0.0, 1.0) );

        loadVertices(vert);
        loadTexCoords(texCoord);
        loadIndices(elementsVertices);

        setDefaultAttribLocations();

    }
};

}
#endif
