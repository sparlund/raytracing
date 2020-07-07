#include "Vec3.h"
#ifndef SPHERE_H
#define SPHERE_H
class Sphere
{
public:
    float radius, radius_squared, transparency, reflection;
    Vec3<float> center, color, emission_color;
    bool compute_intersection(const Vec3<float>& origin,const Vec3<float>& direction,float& t0, float& t1)const;
    Vec3<float> get_center(){return this->center;};
    // constructor only assigns some private variables..
    Sphere(Vec3<float> center,
           float radius,
           Vec3<float> color,
           Vec3<float> emission_color,
           float transparency,
           float reflection):center(center),
                             color(color),
                             emission_color(emission_color),
                             transparency(transparency),
                             reflection(reflection),
                             radius(radius),
                             radius_squared(radius*radius)
                             {};
    // Destructor does nada
    ~Sphere(){};
};
#endif

