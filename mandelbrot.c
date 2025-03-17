#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"
//#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
//#include "include/cimgui/cimgui.h"

int window_width = 800;
int window_height = 600;
const char* window_title = "mandelbrot";
GLFWwindow* window;
GLuint program, vao, vbo, ebo;
GLuint zoom_uni, treshold_uni, iterations_uni;

float zoom = 200.0;
float treshold = 2.5;
int iterations = 50;

const float vertices[12] = {
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
};

const int indices[6] = {
    0, 1, 2,
    2, 3, 0
};

char* readFile(const char* filePath) {
    FILE* fp = fopen(filePath, "r");
    assert(fp);
    assert(fseek(fp, 0, SEEK_END) == 0);
    size_t len = ftell(fp);
    assert(fseek(fp, 0, SEEK_SET) == 0);
    char* buf = (char*)malloc(len + 1);
    assert(buf);
    assert(memset(buf, 0, len + 1));
    size_t a = fread(buf, 1, len, fp);
    buf[len] = 0;
    assert(!ferror(fp));
    fclose(fp);
    return buf;
}

void create_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(
	window_width,
	window_height,
	window_title,
	NULL, NULL
    );
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    gladLoadGL();
    glViewport(0, 0, window_width, window_height);
}

void create_shader_program() {
    int status = 0;
    char info_log[1025] = { };
    GLuint vert_shader, frag_shader;
    const char* vert_path = "./vert.glsl";
    const char* frag_path = "./frag.glsl";
    const char* vert_source = readFile(vert_path);
    const char* frag_source = readFile(frag_path);
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_source, NULL);
    glCompileShader(vert_shader);
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if(!status) {
	glGetShaderInfoLog(vert_shader, 1024, NULL, info_log);
	printf("vert shader fail : %s", info_log);
	exit(1);
    }

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_source, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
    if(!status) {
	glGetShaderInfoLog(frag_shader, 1024, NULL, info_log);
	printf("frag shader fail : %s", info_log);
	exit(1);
    }

    program = glCreateProgram();
    glUseProgram(program);
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    free((void*)vert_source);
    free((void*)frag_source);

    glGetUniformLocation(program, "treshold");
    glGetUniformLocation(program, "iterations");
}

void scroll_cb() {
    GLuint zoom_uni = glGetUniformLocation(program, "zoom");

}

void key_cb() {

}

void create_buffers() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void do_rendering() {
    glUseProgram(program);
    glBindVertexArray(vao);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

int main() {
    create_window();
    create_shader_program();
    create_buffers();
    do_rendering();
    return 0;
}
