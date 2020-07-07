#include <cmath>
#include "Vec3.h"

// overload some operators..
template<class T>
Vec3<T> Vec3<T>::operator+(const Vec3<T> t) const{
    return Vec3<T>(x+t.x,y+t.y,z+t.z);
};
template<class T>
Vec3<T> Vec3<T>::operator-(const Vec3<T> t) const{
    return Vec3<T>(x-t.x,y-t.y,z-t.z);
};
template<class T>
Vec3<T> Vec3<T>::operator-(){
    return Vec3<T>(-x,-y,-z);
};
template<class T>
Vec3<T> Vec3<T>::operator*(const T& t) const{
    return Vec3<T>(x*t,y*t,z*t);
};
template<class T>
Vec3<T> Vec3<T>::operator*(const Vec3<T>& t) const{
    return Vec3<T>(x*t.x,y*t.y,z*t.z);
};
// scalar product
template<class T>
T Vec3<T>::dot(const Vec3<T> second_vector) const{
    // dot component returns a scalar
    return x*second_vector.x + y*second_vector.y + z*second_vector.z;
};

template<class T>
Vec3<T>& Vec3<T>::normalize() 
{ 
    T normal_2 = x*x + y*y + z*z; 
    if (normal_2 > 0) { 
        T inverse_normal = 1 / sqrt(normal_2); 
        x = x*inverse_normal, y = y*inverse_normal, z = z*inverse_normal; 
    } 
    return *this; 
} 
// create specific template
template class Vec3<float>;

