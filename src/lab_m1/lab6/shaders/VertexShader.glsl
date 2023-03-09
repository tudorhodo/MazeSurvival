#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal; //am modificat locatia in 3 pentru exercitiul 6
layout(location = 2) in vec2 v_coordinate;
layout(location = 1) in vec3 v_color; //am modificat locatia in 1 pentru exercitiul 6

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_coordinate;
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader

    frag_position = v_position;
    frag_normal = sin(Time) * v_normal;
    frag_coordinate = v_coordinate;
    frag_color = v_color;


    // TODO(student): Compute gl_Position
   vec3 new_pos;
   new_pos.x = v_position.x;
   new_pos.y = v_position.y + cos(Time);
   new_pos.z = v_position.z + sin(Time);

    gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
