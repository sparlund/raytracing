#ifndef VEC3_H
#define VEC3_H

template <class T>
class Vec3
{

public:
    // positional values
    T x,y,z;
    Vec3(T x, T y, T z): x(x), y(y), z(z){};
    // init
    Vec3(){};
    ~Vec3(){};
    T length(){return sqrt(x*x + y*y + z*z);};
    // normalize vector in place
    Vec3<T>& normalize();
    // want to overload a bunch of operators to perform vector calculations
    // set all to const as don't want to allow them to change object itself
    Vec3<T> operator/=(const T &t)const;
    Vec3<T> operator+(const Vec3<T> t)const;
    Vec3<T> operator-(const Vec3<T> t)const;
    Vec3<T> operator-();
    Vec3<T> operator*(const Vec3<T>& t)const;
    Vec3<T> operator*(const T& t)const;
    T dot(const Vec3<T> second_vector)const;
    Vec3<T> cross(const Vec3<T> second_vector)const;
};
#endif