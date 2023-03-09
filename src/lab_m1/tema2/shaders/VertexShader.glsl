#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal; //am modificat locatia in 3 pentru exercitiul 6
layout(location = 2) in vec2 v_coordinate;
layout(location = 3) in vec3 v_color; //am modificat locatia in 1 pentru exercitiul 6

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform vec3 MyColor;
uniform bool Explode;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    // TODO(student): Send output to fragment shader
    vec3 new_pos;

    if (Explode) {
        new_pos = v_position * rand(vec2(v_position.x, v_position.y) * Time) + v_position * pow(Time, 2);

    } else {
        new_pos = v_position;
    }

    frag_color = MyColor;

    gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
