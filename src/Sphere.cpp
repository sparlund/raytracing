#include "Sphere.h"
#include <cmath>

bool Sphere::compute_intersection(const Vec3<float>& origin,
                                  const Vec3<float>& direction,
                                  float& t)const{

    // vector from center of sphere to ray origin
    Vec3<float> length = this->center - origin;
    // dot product new vector and ray direction
    float dot_length_ray = length.dot(direction);
    // check if they intersect
    if (dot_length_ray < 0)
    {
        return false;
    }
    float distance = length.dot(length) - dot_length_ray*dot_length_ray;
    if (distance > radius_squared)
    {
        return false;
    }
    float tt = std::sqrt(this->radius_squared - distance);
    t = dot_length_ray + tt;
    return true;
};