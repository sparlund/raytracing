#include "Object.h"
#include "Vec3.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H
class Triangle : public Object
{
private:
public:
    // TODO: add setters and getters
    Vec3<float> triangle_p1;
    Vec3<float> triangle_p2;
    Vec3<float> triangle_p3;
    // only one color per triangle for now..
    Vec3<float> center,color,emission_color;
    bool compute_intersection(const Vec3<float> &ray_origin,const Vec3<float> &ray_direction,float& t)const;
    Triangle(Vec3<float> triangle_p1,
             Vec3<float> triangle_p2,
             Vec3<float> triangle_p3,
             Vec3<float> color,
             Vec3<float> emission_color):
             triangle_p1(triangle_p1),
             triangle_p2(triangle_p2),
             triangle_p3(triangle_p3),
             color(color),
             emission_color(emission_color)
             {
                 this->center = Vec3<float>((triangle_p1.x + triangle_p2.x + triangle_p3.x)/3,
                                            (triangle_p1.y + triangle_p2.y + triangle_p3.y)/3,
                                            (triangle_p1.z + triangle_p2.z + triangle_p3.z)/3);
             };
    ~Triangle(){};
};
#endif
