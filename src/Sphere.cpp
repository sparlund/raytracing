#include "sphere.h"

bool Sphere::compute_intersection(Vec3<float>& origin,Vec3<float>& direction,float& t0, float& t1){

    // vector from center of sphere to ray origin
    Vec3<float> length = center - origin;
    // dot product new vector and ray direction
    float dot_length_ray = length.dot(direction);
    // check if they intersect
    if (dot_length_ray < 0 )
    {
        return false;
    }
    float distance = length.dot(length) - dot_length_ray*dot_length_ray;
    if (distance > radius_squared)
    {
        return false;
    }
    float t = std::sqrt(radius_squared - distance);
    t0 = dot_length_ray - t;
    t1 = dot_length_ray + t;
    return true;
};