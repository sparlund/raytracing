#include <math.h>
#include "Vec3.h"



// overload some operators..
template<class T>
Vec3<T> Vec3<T>::operator+(T& t) {
    return Vec3<T>(this->x+t,this->y+t,this->z+t);
};
template<class T>
Vec3<T> Vec3<T>::operator-(T& t) {
    return Vec3<T>(x-t,y-t,z-t);
};
template<class T>
Vec3<T> Vec3<T>::operator*(T& t) {
    return Vec3<T>(x*t,y*t,z*t);
};
template<class T>
T Vec3<T>::dot(Vec3<T>& second_vector) {
    // dot component returns a scalar
    return x*second_vector.get_x() + y*second_vector.get_y() + z*second_vector.get_z();
};

template<class T>
void Vec3<T>::normalize() 
{ 
    T normal_2 = x^2 + y^2 + z^2; 
    if (normal_2 > 0) { 
        T inverse_normal = 1 / sqrt(normal_2); 
        x = x*inverse_normal, y = y*inverse_normal, z = z*inverse_normal; 
    } 
    return; 
} 
