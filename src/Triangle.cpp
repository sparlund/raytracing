#include "Triangle.h"
#include "Vec3.h"
bool Triangle::compute_intersection(const Vec3<float> &ray_origin,
                                    const Vec3<float> &ray_direction,
                                    float& t)const{
    Vec3<float> triangle_p1triangle_p2 = triangle_p2 - triangle_p1; 
    Vec3<float> triangle_p1triangle_p3 = triangle_p3 - triangle_p1; 
    Vec3<float> pvec = ray_direction.cross(triangle_p1triangle_p3); 
    Vec3<float> N = triangle_p1triangle_p2.cross(triangle_p1triangle_p3);
    // dot product with itself is square of vectors magnitude
    float normal_squared_magnitude = N.dot(N); 
    float determinant = triangle_p1triangle_p2.dot(pvec);
    float u,v;
    float inverse_determinant = 1 / determinant; 
 
    Vec3<float> tvec = ray_origin - triangle_p1; 
    u = tvec.dot(pvec) * inverse_determinant; 
    if (u < 0 || u > 1){
        return false;
    }

 
    Vec3<float> qvec = tvec.cross(triangle_p1triangle_p2); 
    v = ray_direction.dot(qvec) * inverse_determinant; 
    if (v < 0 || u + v > 1){
        return false;
    } 
 
    // check if ray is behind or in front of triangle
    t = triangle_p1triangle_p3.dot(qvec) * inverse_determinant; 
    if (t < 0){
        return false;
    }
    
    // P is intersection point on triangle in u-v system
    Vec3<float> P = ray_origin + ray_direction*t;
    // Need to check against the three edge vectors if P is inside triangle
    // edge 1
    Vec3<float> edge0 = triangle_p2 - triangle_p1; 
    Vec3<float> vp0 = P - triangle_p1; 
    Vec3<float> C = edge0.cross(vp0); 
    if (N.dot(C) < 0){
        return false;
    } 
 
    // edge 2
    Vec3<float> edge1 = triangle_p3 - triangle_p2; 
    Vec3<float> vp1 = P - triangle_p2; 
    C = edge1.cross(vp1); 
    if ((u = N.dot(C)) < 0){
        return false;
    } 
 
    // edge 3
    Vec3<float> edge2 = triangle_p1 - triangle_p3; 
    Vec3<float> vp2 = P - triangle_p3; 
    C = edge2.cross(vp2); 
    if ((v = N.dot(C)) < 0){
        return false;}
    // scale
    u /= normal_squared_magnitude; 
    v /= normal_squared_magnitude; 
    return true; 

};