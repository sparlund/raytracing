#include "Vec3.h"
class Sphere
{
private:
    Vec3<float> center, color, emission_color;
    float radius, radius_squared transparency, reflection;


    /* data */
public:
    bool compute_intersection(Vec3<float>& origin,Vec3<float>& direction,float& t0, float& t1);
    // constructor only assigns some private variables..
    Sphere(Vec3<float>& center,
           Vec3<float>& color
           Vec3<float>& emission_color,
           float& transparency,
           float& reflection):center(center),color(color),emission_color(emission_color),transparency(transparency),reflection(reflection){};
    // Destructor does nada
    ~Sphere();
};


