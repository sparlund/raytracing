#include <string>
#include <vector>
#include <memory>
#include "Vec3.h"
#include "Sphere.h"
#include "Triangle.h"
#ifndef RENDER_H
#define RENDER_H
class Render
{
private:
    int max_depth,width,height;
    int number_of_pixels;
    float field_of_view,aspect_ratio,angle;
    Vec3<float> background_color;
    std::string filename;
    // let the render class remember how many spheres we have
    // 3 values per pixel
    // a vec3<float> array the size of the image
    std::vector<Vec3<float>> image_vector;
    // unsigned char is 0-255
    // pixel_colors will be width*height*3 chars --> RGB for each pixel
    
     
public:
    bool export_image(std::string filename);
    // vector of unique pointers to all the objects in our scene (spheres, triangles..)
    // TODO: implement some kind of kd-tree to speed up rendering
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<unsigned char> pixels;
    // init to 0, will be updated and displayed every time we render a new scene
    float render_time=0;
    // method to add spheres
    void render();
    float mix_colors(float a, float b,float mix);
    Vec3<float> cast_ray_and_trace_color(const Vec3<float>& ray_origin,
                                         const Vec3<float>& ray_direction,
                                         const int& current_depth); 
    void add_sphere(Vec3<float> position,float radius,Vec3<float> color,float reflectivity,float transparency,Vec3<float>  emission_color);
    void add_random_sphere();
    void add_random_triangle();
    void add_triangle(Vec3<float> triangle_p1,Vec3<float> triangle_p2,Vec3<float> triangle_p3,Vec3<float> color,Vec3<float> emission_color);
    void remove_sphere(unsigned int index);
    void remove_sphere();

    Render(int max_depth,
           int width,
           int height,
           float field_of_view,
           std::string filename,
           Vec3<float> background_color);
    ~Render(){};
};
#endif

