#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include "src/Vec3.h"
#include "src/Sphere.h"
#include "src/Render.h"
#include "src/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "src/stb_image.h"

void callback_window_size(GLFWwindow *window, int width, int height);

int main(int argc, char const *argv[])
{
    int window_width    = 640;
    int window_height   = 480;
    // Create window and input handler
    if (!glfwInit())
    {
        std::cout << "Initialization failed\n";
    }
    // openGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "ray_tracer", NULL, NULL);
    if (window == NULL)
        {
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
    // Need to init glad before we call openGL
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // failed glad :(
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, callback_window_size);
    // Shader object
    Shader shader_rendered_image("src/shaders/shader_rendered_image.vs","src/shaders/shader_rendered_image.fs");
    // generate ray traced image
    unsigned int rendered_image_width   = 640;
    unsigned int rendered_image_height  = 480;
    int max_depth = 6;
    float field_of_view = 30;
    Vec3<float> background_color(0.69f,0.69f,0.69f);
    std::string filename = "test.ppm";
    Render render(max_depth,
             rendered_image_width,
             rendered_image_height,
             field_of_view,
             filename,
             background_color);
    // Vec3<float> position,float radius,Vec3<float> color,float reflectivity,float transparency,Vec3<float>  emission_color
    // add sphere to image
    const unsigned int number_of_sphere = 5;
    srand( (unsigned)time(NULL) );
    for (int i = 0; i < number_of_sphere; ++i)
    {
        const float x = -10 + (rand() % (10-(-10) + 1));
        const float y = -10 + (rand() % (10-(-10) + 1));
        const float z = -100 + (rand() % (-200-(-100) + 1));
        const float radius = rand() % 1;
        const float r = rand()/float(RAND_MAX);
        const float g = rand()/float(RAND_MAX);
        const float b = rand()/float(RAND_MAX);
        render.add_sphere(Vec3<float>(x,y,z),     4, Vec3<float>(r,g,b), 0, 0.5, Vec3<float>(0.0f,0.0f,0.0f));
        // exit(1);
    }
    // r.add_sphere(Vec3<float>(-5.5,      0, -15-20),     3, Vec3<float>(0.90, 0.90, 0.90), 0, 0.0, Vec3<float>(0.0f,0.0f,0.0f));
    // add light
    render.add_sphere(Vec3<float>( 0.0,     20, -30-20),     3, Vec3<float>(0.00, 0.00, 0.00), 0, 0.0, Vec3<float>(3.0f,3.0f,3.0f));
    render.render(); 
    // vertices for our texture will always be the same as we always want a "fullscreen" view of the rendered image
                        // positions
    float vertices[] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures( 1, &texture );
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Read the texture data from file and upload it to the GPU
    

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, rendered_image_width, rendered_image_height, 0,GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)&render.pixels.front());
    // glGenerateMipmap(GL_TEXTURE_2D);
    glViewport(0,0,window_width,window_height);

    shader_rendered_image.use();
    shader_rendered_image.setInt("texture", 0);
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    // std::clock_t timer;
    // float duration;
    // timer = std::clock();
    // duration = ( std::clock() - timer ) / (float) CLOCKS_PER_SEC;
    // std::cout<< duration <<'\n';
    return 0;
}

void callback_window_size(GLFWwindow *window, int width, int height)
{
    // Change the viewport according to window size
    glViewport(0, 0, width, height);
}

