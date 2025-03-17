#version 330 core

out vec4 FragColor;

uniform vec2 position;
uniform vec2 screen_center;
uniform int iterations;
uniform float treshold;
uniform float zoom;

const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 blue = vec4(0.2, 0.1, 0.9, 1.0);

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
    vec2 coord = (gl_FragCoord.xy - screen_center - position) * zoom / 200;

    FragColor = calc_stability_color(coord);
}
