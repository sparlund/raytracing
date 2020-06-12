
template <class T>
class Vec3
{
private:
    // positional values
    T x,y,z;

public:
    Vec3(T x, T y, T z): x(x), y(x), z(z){};
    ~Vec3(){};
    // return components of vector
    T get_x(){return x;};
    T get_y(){return y;};
    T get_z(){return z;};
    T length(){return sqrt(x^2 + y^2 + z^2);};
    // returned normalized vector
    void normalize();
    // want to overload a bunch of operators to perform vector calculations
    // set all to const as don't want to allow them to change object itself
    Vec3<T> operator+(T& t);
    Vec3<T> operator-(T& t);
    Vec3<T> operator*(T& t);
    T dot(Vec3<T>& second_vector);
};




