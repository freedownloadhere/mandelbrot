#version 330 core

out vec4 FragColor;

uniform vec2 position;
uniform vec2 screen_center;
uniform int iterations;
uniform float treshold;
uniform float zoom;

const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 blue = vec4(0.2, 0.1, 0.7, 1.0);

vec2 f(vec2 z, vec2 c) {
    float a = z.x;
    float b = z.y;
    return vec2(a * a - b * b, 2.0 * a * b) + c;
}

vec4 calc_stability_color(vec2 c) {
    int i = 1;
    vec2 z = vec2(0.0, 0.0);

    while (i <= iterations) {
        z = f(z, c);
        if (distance(c, z) > treshold)
            break;
        i++;
    }

    if (i >= iterations)
        return black;

    float a = float(i) / iterations;
    vec4 col = 2.0 * a * blue;

    return col;
}

void main() {
    vec2 coord = (gl_FragCoord.xy - screen_center) / zoom - position;

    FragColor = calc_stability_color(coord);
}
