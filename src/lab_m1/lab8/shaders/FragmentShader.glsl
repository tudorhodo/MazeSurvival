#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 material_kd;
uniform vec3 material_ks;
uniform vec3 material_ke;
uniform vec3 material_ka;
uniform int material_shininess;
uniform vec3 intensity;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform bool isSpot;

// Output
layout(location = 0) out vec4 out_color;

vec3 computeAmbient(vec3 ka, vec3 intensity) {
    vec3 result;
    result.x = ka.x * intensity.x;
    result.y = ka.y * intensity.y;
    result.z = ka.z * intensity.z;

    return result;
}

vec3 computeDifuse(vec3 kd, vec3 intensity, vec3 L, vec3 normal) {
    float max_value = max(dot(normal, L), 0);
    vec3 result;

    result.x = kd.x * intensity.x;
    result.y = kd.y * intensity.y;
    result.z = kd.z * intensity.z;
    result = result * max_value;

    return result;
}

vec3 computeSpecular(vec3 ks, vec3 intensity, vec3 V, vec3 R, int shiny) {
    vec3 result;
    float max_value = pow(max(dot(V, R), 0), shiny);

    result.x = ks.x * intensity.x;
    result.y = ks.y * intensity.y;
    result.z = ks.z * intensity.z;

    result = result * max_value;

    return result;
}

void main()
{
    // TODO(student): Define ambient, diffuse and specular light components

    vec3 emisiva = material_ke;

    vec3 new_normal = normalize(world_normal);
    
    vec3 L = normalize(light_position - world_position);

    vec3 V = normalize(eye_position - world_position);

    vec3 R = normalize(reflect(-L, world_normal));

    vec3 ambient_light = vec3(0.25, 0, 0.75);
    vec3 diffuse_light = computeDifuse(material_ka, intensity, L, new_normal);
    vec3 specular_light = vec3(0);
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (dot(L, new_normal) > 0)
    {
        specular_light = computeSpecular(material_ks, intensity, V, R, material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float cut_off = radians(30.0);
    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(cut_off);

    float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
    float light_att_factor = pow(linear_att, 2);
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    vec3 lumina = vec3(0);


    float atenuare = pow(distance(light_position, world_position), -2);

    
    if (isSpot) {
        if (spot_light > spot_light_limit) {
            lumina = ambient_light + (light_att_factor + atenuare) * (diffuse_light + specular_light);
        } else {
            lumina = ambient_light;
        }
    } else {
        lumina = ambient_light + atenuare * (diffuse_light + specular_light);
    }

    // TODO(student): Write pixel out color
    out_color = vec4(1);
    out_color *= vec4(object_color * lumina, 1);
}
