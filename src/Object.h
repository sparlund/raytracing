
#include "Vec3.h"
#ifndef OBJECT_H
#define OBJECT_H
// TODO: ABC with virtual shit doesnt work..
// Object is an ABC (abstract base class) for different objects (plane, sphere, triangles)
class Object
{
private:
public:
    // center is geometrical center position, color is RGB for complete object,  emission_color is the color is excites
    Vec3<float> center,color,emission_color;
    float transparency, reflection, reflectivity;
    // All types of object need method to see if ray intersects it
    // need to implement a few variants for different sub-classes
    // sphere
    virtual bool compute_intersection(const Vec3<float>& origin,
                                      const Vec3<float>& direction,
                                      float& t)const=0;
    Object(){};
    ~Object(){};
};
#endif