#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include "Vec3.h"
#include "Render.h"

bool Render::export_image(std::string filename){
    std::ofstream output_stream(filename);
    output_stream << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned int i = 0; i < (this->width * this->height); ++i){
        unsigned char r = (unsigned char)(std::min(float(1), image_vector.at(i).x) * 255);
        unsigned char g = (unsigned char)(std::min(float(1), image_vector.at(i).y) * 255);
        unsigned char b = (unsigned char)(std::min(float(1), image_vector.at(i).z) * 255);
        output_stream << r << g << b;         
    } 
    output_stream.close(); 

};

void Render::add_random_sphere(){
    srand( (unsigned)time(NULL) );
    const float x = -10 + (rand() % (10-(-10) + 1));
    const float y = -10 + (rand() % (10-(-10) + 1));
    const float z = -100 + (rand() % (-200-(-100) + 1));
    const float radius = rand() % 1;
    // divide result my RAND_MAX to get number in range 0-1
    const float r = rand()/float(RAND_MAX);
    const float g = rand()/float(RAND_MAX);
    const float b = rand()/float(RAND_MAX);
    const float reflectivity = rand()/float(RAND_MAX);
    this->add_sphere(Vec3<float>(x,y,z),     4, Vec3<float>(r,g,b), 0, reflectivity, Vec3<float>(0.0f,0.0f,0.0f));
};

void Render::remove_sphere(unsigned int index){
    if (index <= this->spheres.size())
    {
        this->spheres.erase(this->spheres.begin() + index);
    }
    
};
void Render::remove_sphere(){
    this->spheres.pop_back();
};

void Render::render(){

    unsigned int current_pixel = 0;
    float invWidth = 1 / float(width);
    float invHeight = 1 / float(height); 
    field_of_view = 30;
    aspect_ratio = (float)width / (float)height;
    angle = tan(M_PI * 0.5 * field_of_view / 180.);
    const Vec3<float> zero_vector = Vec3<float>(0.0,0.0,0.0);
    // empty image_vector as we are making a new image!
    this->image_vector.clear();
    std::clock_t timer;
    timer = std::clock();
    for (unsigned int y = 0; y < this->height; ++y)
    {
        for (unsigned int x = 0; x < this->width; ++x)
        {
            // ray direction in camera space
            float x_ray_dir = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspect_ratio; 
            float y_ray_dir = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
            // z-dir of ray is always -1
            Vec3<float> ray_direction(x_ray_dir, y_ray_dir, -1); 
            // normalize vector
            ray_direction.normalize(); 
            Vec3<float> pixel = trace(zero_vector, ray_direction,0);
            this->image_vector.push_back(pixel); 
            // move on to next pixel...
            current_pixel++;
        }
    }
    render_time = ( std::clock() - timer ) / (float) CLOCKS_PER_SEC;
    // Clear pixel vector and re-allocate it's size
    this->pixels.clear();
    this->pixels.reserve(width*height*3);
    for (unsigned int i = 0; i < (width * height); ++i){
        unsigned char r = (unsigned char)(std::min(float(1), image_vector.at(i).x) * 255);
        unsigned char g = (unsigned char)(std::min(float(1), image_vector.at(i).y) * 255);
        unsigned char b = (unsigned char)(std::min(float(1), image_vector.at(i).z) * 255);
        this->pixels.push_back(r);
        this->pixels.push_back(g);
        this->pixels.push_back(b);
    } 
    return;
};
float Render::mix_colors(float a, float b,float mix){
    return b * mix + a * (1.0 - mix); 
};
Vec3<float> Render::trace(const Vec3<float>& ray_origin,
                          const Vec3<float>& ray_direction,
                          const int& current_depth){
    float distance_nearest_sphere = INFINITY;
    Sphere* hit_sphere = nullptr; 
    // check current ray against every object in the scene and see if the hits any
    for (int current_sphere = 0; current_sphere < this->spheres.size(); ++current_sphere)
    {
        // t0 is entry point on sphere, t1 exit point 
        float t0=INFINITY, t1=INFINITY;
        if (spheres.at(current_sphere).compute_intersection(ray_origin,ray_direction,t0,t1))
        {

            if (t0 < 0)
            {
                t0 = t1;
            }
            // intersected shere nearer than previous intersected sphere
            if (t0 < distance_nearest_sphere)
            {
                distance_nearest_sphere = t0;   
                hit_sphere = &spheres.at(current_sphere);
            }
        }
    }
    // not on sphere -> return background color
    if (!hit_sphere)
    {
        return background_color;
    }
    
    // We have hit an object, need to find how to shade pixel now
    // set init color of the ray/surfaceof the object intersected by the ray 
    Vec3<float> color = Vec3<float>(0,0,0); 
    Vec3<float> point_of_intersection = ray_origin + ray_direction * distance_nearest_sphere; 
    Vec3<float> normal_at_point_of_intersection = point_of_intersection - hit_sphere->center; 
    // normalize
    normal_at_point_of_intersection.normalize();
    float bias = 1e-4;
    bool inside_sphere = false; 
    if (ray_direction.dot(normal_at_point_of_intersection) > 0){
        normal_at_point_of_intersection = -normal_at_point_of_intersection;
        inside_sphere = true; 
    }

    // check if object is transparent or reflective
    if ((hit_sphere->transparency > 0 || hit_sphere->reflection > 0) && current_depth < max_depth) {  
        float facing_ratio = -ray_direction.dot(normal_at_point_of_intersection);
        float fresnel = mix_colors(pow(1 - facing_ratio, 3), 1, 0.1);  
        Vec3<float> reflection_direction = ray_direction - normal_at_point_of_intersection * 2 * ray_direction.dot(normal_at_point_of_intersection); 
        reflection_direction.normalize(); 
        // recursive, watch out!       origin, direction, current depth
        
        Vec3<float> reflection = trace(point_of_intersection + normal_at_point_of_intersection*bias,
                                        reflection_direction,
                                        current_depth + 1); 
        Vec3<float> refraction = Vec3<float>(0.0,0.0,0.0);
        // check if the hit sphere has transparency bigger than zero
        if (hit_sphere->transparency) { 
            float ior = 1.1;
            // check if inside or outside the surface 
            float eta = (inside_sphere) ? ior : 1 / ior; 
            float cosi = -normal_at_point_of_intersection.dot(ray_direction); 
            float k = 1 - eta * eta * (1 - cosi * cosi); 
            Vec3<float> reflection_direction = ray_direction * eta + normal_at_point_of_intersection * (eta *  cosi - std::sqrt(k)); 
            reflection_direction.normalize(); 
            // recursive!
            refraction = trace(point_of_intersection - normal_at_point_of_intersection*bias,
                               reflection_direction,
                               current_depth + 1); 
        }   
            color = (reflection * fresnel + refraction * (1 - fresnel) * hit_sphere->transparency) * hit_sphere->color;           
    }
    else{
        // check if each sphere is a light source or not
        for (unsigned int i = 0; i < this->spheres.size(); ++i) { 
            if (this->spheres.at(i).emission_color.x > 0) { 
                Vec3<float> transmission = Vec3<float>(1.0,1.0,1.0); 
                Vec3<float> light_direction = this->spheres.at(i).center - point_of_intersection; 
                light_direction.normalize(); 
                for (unsigned int j = 0; j < this->spheres.size(); ++j) { 
                    if (i != j) { 
                        float t0, t1; 
                        if (spheres.at(j).compute_intersection(point_of_intersection + normal_at_point_of_intersection, light_direction, t0, t1)) { 
                            transmission = Vec3<float>(0.0,0.0,0.0); 
                            break; 
                        } 
                    } 
                } 
                color = color + (hit_sphere->color * transmission * std::max(float(0), normal_at_point_of_intersection.dot(light_direction)) * spheres.at(i).emission_color); 
            } 
        }
    }
    return color + hit_sphere->emission_color; 
};
void Render::add_sphere(Vec3<float> position,
                        float radius,
                        Vec3<float> sphere_color,
                        float reflectivity,
                        float transparency,
                        Vec3<float>  sphere_emission_color){
    // add sphere object to a vector of objects to render
    Sphere sphere(position,radius,sphere_color,sphere_emission_color,transparency,reflectivity);
    this->spheres.push_back(sphere);
    return;
};
Render::Render(int max_depth,
               int width,
               int height,
               float field_of_view,
               std::string filename,
               Vec3<float> background_color):
       max_depth(max_depth),
       width(width),
       height(height),
       field_of_view(field_of_view),
       filename(filename),
       background_color(background_color){
    number_of_pixels = width*height;
    angle = std::tan(M_PI * 0.5 * field_of_view / 180.);
    aspect_ratio = (float)width / (float)height; 
};