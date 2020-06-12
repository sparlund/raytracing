template<typename T>
class Vec3
{
private:
    // positional values
    T x,y,z;

public:
    Vec3(T x, T y, T z): x(x), y(x), z(z)();
    ~Vec3();
    // returned normalized vector
    Vec3& normalize();
    // want to overload a bunch of operators to perform vector calculations
    Vec3<T> operator+ (T& t) const;
    Vec3<T> operator- (T& t) const;
    Vec3<T> operator* (T& t) const;
    Vec3<T> operator+ (T& t) const;

};

Vec3::Vec3(/* args */)
{
}

Vec3::~Vec3()
{
}



