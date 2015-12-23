#version 330 core

layout (location = 0) out vec4 position_depth_buffer;
layout (location = 1) out vec3 normal_buffer;
layout (location = 2) out vec4 color_buffer;

in vec3 normal;
in vec3 fragment_position;
in vec2 tex_coord;

uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    position_depth_buffer.xyz = fragment_position;

    // And store linear depth into gPositionDepth's alpha component
    position_depth_buffer.a = LinearizeDepth(gl_FragCoord.z); // Divide by FAR if you need to store depth in range 0.0 - 1.0 (if not using floating point colorbuffer)
    
	// Also store the per-fragment normals into the gbuffer
    normal_buffer = normalize(normal);

    // And the diffuse per-fragment color
    color_buffer.rgba = vec4(1.0f); // TODO
}