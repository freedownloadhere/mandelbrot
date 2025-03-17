#version 450 core

out vec4 FragColor;

const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 blue = vec4(0.2, 0.1, 0.9, 1.0);
const vec2 center = vec2(400, 300);
const float treshold = 2.5;
const int iterations = 50;

vec2 f(vec2 z, vec2 c) {
    float a = z.x;
    float b = z.y;
    return vec2(a * a - b * b, 2.0 * a * b) + c;
}

vec4 calc_stability_color(vec2 c) {
    vec2 z = vec2(0.0, 0.0);
    for (int i = 1; i <= iterations; i++)
        z = f(z, c);

    float dist = distance(c, z);

    if (dist <= treshold)
        return black;
    else
        return blue;
}

void main() {
    vec2 coord = (gl_FragCoord.xy - center) / 200;
    FragColor = calc_stability_color(coord);
}
