#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include "Vec3.h"
#include "Render.h"
#include "Object.h"
#include "Triangle.h"
#include "Sphere.h"

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

void Render::add_triangle(Vec3<float> triangle_p1,Vec3<float> triangle_p2,Vec3<float> triangle_p3,Vec3<float> color,Vec3<float> emission_color){
    // unique pointer to new triangle
    this->objects.push_back(std::unique_ptr<Object> (new Triangle(triangle_p1,triangle_p2,triangle_p3,color,emission_color)));
};

void Render::add_random_triangle(){
    srand( (unsigned)time(NULL) );
    const float x1 = -10 + (rand() % (10-(-10) + 1));
    const float y1 = -10 + (rand() % (10-(-10) + 1));
    const float z1 = -100 + (rand() % (-200-(-100) + 1));
    const float x2 = -10 + (rand() % (10-(-10) + 1));
    const float y2 = -10 + (rand() % (10-(-10) + 1));
    const float z2 = -100 + (rand() % (-200-(-100) + 1));
    const float x3 = -10 + (rand() % (10-(-10) + 1));
    const float y3 = -10 + (rand() % (10-(-10) + 1));
    const float z3 = -100 + (rand() % (-200-(-100) + 1));
    const float radius = rand() % 1;
    // divide result my RAND_MAX to get number in range 0-1
    const float r = rand()/float(RAND_MAX);
    const float g = rand()/float(RAND_MAX);
    const float b = rand()/float(RAND_MAX);
    const float reflectivity = rand()/float(RAND_MAX);
    this->add_triangle(Vec3<float>(x1,y1,z1),
                       Vec3<float>(x2,y2,z2),
                       Vec3<float>(x3,y3,z3),
                       Vec3<float>(r,g,b),
                       Vec3<float>(0.0f,0.0f,0.0f)); 

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
    if (index <= this->objects.size())
    {
        this->objects.erase(this->objects.begin() + index);
    }
    
};
void Render::remove_sphere(){
    this->objects.pop_back();
};

void Render::render(){

    unsigned int current_pixel = 0;
    float invWidth = 1 / float(width);
    float invHeight = 1 / float(height); 
    field_of_view = 30;
    aspect_ratio = (float)width / (float)height;
    angle = tan(M_PI * 0.5 * field_of_view / 180.);
    const Vec3<float> origin(0.0f,0.0f,0.0f);
    // empty image_vector as we are making a new image!
    this->image_vector.clear();
    std::clock_t timer;
    timer = std::clock();
    for (unsigned int y = 0; y < this->height; ++y)
    {
        for (unsigned int x = 0; x < this->width; ++x)
        {
            // ray direction in screen space
            float x_ray_dir = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspect_ratio; 
            float y_ray_dir = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
            // z-dir of ray is always -1
            // vector of current ray in screen space
            Vec3<float> ray_direction(x_ray_dir, y_ray_dir, -1); 
            // normalize vector
            ray_direction.normalize(); 
            Vec3<float> pixel = cast_ray_and_trace_color(origin, ray_direction,0);
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
Vec3<float> Render::cast_ray_and_trace_color(const Vec3<float>& ray_origin,
                                             const Vec3<float>& ray_direction,
                                             const int& current_depth){
    float distance_nearest_object = INFINITY;
    // init color of pixel to zero vector
    Vec3<float> color = Vec3<float>(0,0,0); 
    // pointer to an object that we've hit, init it to no hit 
    // using -1 as indicator as not hit..
    int index_hit_object=-1; 
    // check current ray against every object in the scene and see if the hits any
        float t=0;
    for (int current_object = 0; current_object < this->objects.size(); ++current_object)
    {
        // init distance from ray origin to hit object
        if (objects.at(current_object)->compute_intersection(ray_origin,ray_direction,t))
        {
            if (t < distance_nearest_object)
            {
                distance_nearest_object = t;   
                index_hit_object = current_object;
            }
        }
    }
    // not on object -> return background color
    if (index_hit_object==-1)
    {
        return background_color;
    }
    
    // We have hit an object, need to find how to shade pixel now
    // set init color of the ray/surfaceof the object intersected by the ray 
    Vec3<float> point_of_intersection = ray_origin + ray_direction * distance_nearest_object; 
    Vec3<float> normal_at_point_of_intersection = point_of_intersection - objects.at(index_hit_object)->center; 
    // normalize
    normal_at_point_of_intersection.normalize();
    float bias = 1e-4;
    bool inside_sphere = false; 
    if (ray_direction.dot(normal_at_point_of_intersection) > 0){
        normal_at_point_of_intersection = -normal_at_point_of_intersection;
        inside_sphere = true; 
    }

    // check if object is transparent or reflective
    if ((objects.at(index_hit_object)->transparency > 0 || objects.at(index_hit_object)->reflection > 0) && current_depth < max_depth) {  
        float facing_ratio = -ray_direction.dot(normal_at_point_of_intersection);
        float fresnel = mix_colors(pow(1 - facing_ratio, 3), 1, 0.1);  
        Vec3<float> reflection_direction = ray_direction - normal_at_point_of_intersection * 2 * ray_direction.dot(normal_at_point_of_intersection); 
        reflection_direction.normalize(); 
        // recursive, watch out!
        // parameters: origin, direction, current depth
        Vec3<float> reflection = cast_ray_and_trace_color(point_of_intersection + normal_at_point_of_intersection*bias,
                                                          reflection_direction,
                                                          current_depth + 1); 
        Vec3<float> refraction = Vec3<float>(0.0,0.0,0.0);
        // check if the hit sphere has transparency bigger than zero
        if (objects.at(index_hit_object)->transparency) { 
            float ior = 1.1;
            // check if inside or outside the surface 
            float eta = (inside_sphere) ? ior : 1 / ior; 
            float cosi = -normal_at_point_of_intersection.dot(ray_direction); 
            float k = 1 - eta * eta * (1 - cosi * cosi); 
            Vec3<float> reflection_direction = ray_direction * eta + normal_at_point_of_intersection * (eta *  cosi - std::sqrt(k)); 
            reflection_direction.normalize(); 
            // recursive!
            refraction = cast_ray_and_trace_color(point_of_intersection - normal_at_point_of_intersection*bias,
                                                  reflection_direction,
                                                  current_depth + 1); 
        }   
            color = (reflection * fresnel + refraction * (1 - fresnel) * objects.at(index_hit_object)->transparency) * objects.at(index_hit_object)->color;           
    }
    else{
        // check if each object is a light source or not
        for (unsigned int i = 0; i < this->objects.size(); ++i) { 
            if (this->objects.at(i)->emission_color.x > 0) { 
                Vec3<float> transmission = Vec3<float>(1.0,1.0,1.0); 
                Vec3<float> light_direction = this->objects.at(i)->center - point_of_intersection; 
                light_direction.normalize(); 
                for (unsigned int j = 0; j < this->objects.size(); ++j) { 
                    if (i != j) { 
                        float t; 
                        if (objects.at(j)->compute_intersection(point_of_intersection + normal_at_point_of_intersection, light_direction, t)) { 
                            transmission = Vec3<float>(0.0,0.0,0.0); 
                            break; 
                        } 
                    } 
                } 
                color = color + (objects.at(index_hit_object)->color * transmission * std::max(float(0), normal_at_point_of_intersection.dot(light_direction)) * objects.at(i)->emission_color); 
            } 
        }
    }
    return color + objects.at(index_hit_object)->emission_color; 
};
void Render::add_sphere(Vec3<float> position,
                        float radius,
                        Vec3<float> sphere_color,
                        float reflectivity,
                        float transparency,
                        Vec3<float>  sphere_emission_color){
    // add sphere object to a vector of objects to render
    this->objects.push_back(std::unique_ptr<Object>  (new Sphere(position,radius,sphere_color,sphere_emission_color,transparency,reflectivity)));
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