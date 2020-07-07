#include <string>
#include <vector>
#include "Vec3.h"
#include "Sphere.h"
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
    std::vector<Sphere> spheres;
    // 3 values per pixel
    // a vec3<float> array the size of the image
    std::vector<Vec3<float>> image_vector;
    // unsigned char is 0-255
    // pixel_colors will be width*height*3 chars --> RGB for each pixel
    
     
public:
    std::vector<unsigned char> pixels;
    // method to add spheres
    void render();
    float mix_colors(float a, float b,float mix);
    Vec3<float> trace(const Vec3<float>& ray_origin,
                      const Vec3<float>& ray_direction,
                      const int& current_depth);
    void add_sphere(Vec3<float> position,float radius,Vec3<float> color,float reflectivity,float transparency,Vec3<float>  emission_color);
    Render(int max_depth,
           int width,
           int height,
           float field_of_view,
           std::string filename,
           Vec3<float> background_color);
    ~Render(){};
};
#endif

