#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float f) const { return Vec3(x * f, y * f, z * f); }
    Vec3 operator/(float f) const { return Vec3(x / f, y / f, z / f); }

    float length() const { return std::sqrt(x*x + y*y + z*z); }
    Vec3 normalize() const { return *this / length(); }
};

float dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

//estrutura para representar um raio
struct Ray{
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d.normalize()) {}
};

struct Sphere {
    Vec3 center;
    float radius;

    Sphere(const Vec3& c, float r) : center(c), radius(r) {}

    bool intersect(const Ray& ray, float& t) const{
        Vec3 oc = ray.origin - center;
        float a = dot(ray.direction, ray.direction);
        float b = 2.0f * dot(oc, ray.direction);
        float c = dot(oc, oc) - radius*radius;
        float discriminant = b*b - 4*a*c;

        if (discriminant < 0) return false;

        float sqrtd = std::sqrt(discriminant);
        float t0 = (-b - sqrtd) / (2*a);
        float t1 = (-b + sqrtd) / (2*a);

        t = (t0 < t1) ? t0 : t1;
        return true;

    }
};

char getCharByIntensity(float intensity) {
    const std::string chars = " .,:;+*?%S#@";
    int index = intensity * (chars.length() - 1);
    index = std::max(0, std::min(static_cast<int>(chars.length() -1), index));
    return chars[index];
}

int main() {
    // configurações da cena
    // camera
    // objetos na cena
    //luz
    //renderizar
}