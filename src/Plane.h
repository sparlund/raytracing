#include "Object.h"
#ifndef PLANE_H
#define PLANE_H
class Plane:public Object
{
private:
    /* data */
public:
    bool compute_intersection(const Vec3<float>& normal,
                              const Vec3<float>& normal_vector_start,
                              const Vec3<float>& incoming_ray_start,
                              float& incoming_ray_dir)const;
    Plane(/* args */);
    ~Plane();
};

Plane::Plane(/* args */)
{
}

Plane::~Plane()
{
}
