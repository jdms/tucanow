#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec4 vert_color[3];
in vec3 vert_normal[3];
in vec2 vert_texcoords[3];

out vec4 color;
out vec3 normal;
//out vec2 texcoords;
noperspective out vec3 edge_dist; //distance from three vertices
//out vec4 vert;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewportMatrix;


void main (void)
{

    vec3[3] dist;
    dist[0] = vec3(1.0, 0.0, 0.0);
    dist[1] = vec3(0.0, 1.0, 0.0);
    dist[2] = vec3(0.0, 0.0, 1.0);

    mat4 modelViewMatrix = viewMatrix * modelMatrix;
	mat4 normalMatrix = transpose(inverse(modelViewMatrix));

    // for (int i = 0; i < 3; ++i)
    // {
	//     vec4 vert = modelViewMatrix * gl_in[i].gl_Position; 
	//     gl_Position = projectionMatrix * vert;
    //     color = vert_color[i];
    //     normal = normalize(vec3(normalMatrix * vec4(vert_normal[i],0.0)).xyz);
	//      //texcoords = vert_texcoords[i];
    //     edge_dist = dist[i];
	//     EmitVertex();
    // }

    vec3 vert0 = vec3(viewportMatrix * modelViewMatrix * gl_in[0].gl_Position);
    vec3 vert1 = vec3(viewportMatrix * modelViewMatrix * gl_in[1].gl_Position);
    vec3 vert2 = vec3(viewportMatrix * modelViewMatrix * gl_in[2].gl_Position);

    float a = length(vert1 - vert2);
    float b = length(vert2 - vert0);
    float c = length(vert1 - vert0);
    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );
    float ha = abs( c * sin(beta) );
    float hb = abs( c * sin(alpha) );
    float hc = abs( b * sin(alpha) );

    edge_dist = vec3(ha, 0.0, 0.0);
    normal = normalize(vec3(normalMatrix * vec4(vert_normal[0],0.0)).xyz);
    gl_Position = projectionMatrix * modelViewMatrix * gl_in[0].gl_Position;
    color = vert_color[0];
    EmitVertex();
    
    edge_dist = vec3(0.0, hb, 0.0);
    normal = normalize(vec3(normalMatrix * vec4(vert_normal[1],0.0)).xyz);
    gl_Position = projectionMatrix * modelViewMatrix * gl_in[1].gl_Position;
    color = vert_color[1];
    EmitVertex();
    
    edge_dist = vec3(0.0, 0.0, hc);
    normal = normalize(vec3(normalMatrix * vec4(vert_normal[2],0.0)).xyz);
    gl_Position = projectionMatrix * modelViewMatrix * gl_in[2].gl_Position;
    color = vert_color[2];
    EmitVertex();
    
	EndPrimitive();
}
