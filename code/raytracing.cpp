#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

// definição de estruturas basicas
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

// estrutura para uma esfera
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

// determina o caractere com base na intensidade
char getCharByIntensity(float intensity) {
    const std::string chars = " .,:;+*?%S#@";
    int index = intensity * (chars.length() - 1);
    index = std::max(0, std::min(static_cast<int>(chars.length() -1), index));
    return chars[index];
}

int main() {
    // configurações da cena
    const int width = 80;
    const int height = 40;
    const float aspectRatio = float(width) / float(height);

    // camera
    Vec3 cameraPos(0, 0, -3);
    Vec3 lookAt(0, 0, 0);
    Vec3 up(0, 1, 0);

    Vec3 forward = (lookAt - cameraPos).normalize();
    Vec3 right = Vec3(forward.z, 0, -forward.x).normalize();
    Vec3 cameraUp = Vec3(0, 1, 0);

    // objetos na cena
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3(0, 0, 0), 1.0f));
    spheres.push_back(Sphere(Vec3(-2, 0, 0), 1.0f));
    spheres.push_back(Sphere(Vec3(2, 0, 0), 1.0f));

    //luz
    Vec3 lightDir(-1, -1, -1);
    lightDir = lightDir.normalize();

    //renderizar
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // coordenadas normalizadas do pixel
            float px = (2.0f * (x + 0.5f) / width - 1.0f) * aspectRatio;
            float py = 1.0f - 2.0f * (y + 0.5f) / height;

            // criar raio
            Vec3 rayDir = (forward + right*px + cameraUp*py).normalize();
            Ray ray(cameraPos, rayDir);

            // testar interseção com esferas
            float minDist = std::numeric_limits<float>::max();
            bool hit = false;
            Vec3 hitNormal;

            for (const auto& sphere : spheres) {
                float t;
                if (sphere.intersect(ray, t) && t < minDist) {
                    minDist = t;
                    hit = true;
                    Vec3 hitPoint = ray.origin + ray.direction * t;
                    hitNormal = (hitPoint - sphere.center).normalize();
                }
            }

            // calculo da intensidade 
            float intensity = 0.0f;
            if (hit) {
                intensity = std::max(0.0f, dot(hitNormal, lightDir * -1.0f));
                // luz ambiente
                intensity = intensity * 0.7f + 0.3f;
            }
            // seleciona o caractere baseado na intensidade
            std::cout << getCharByIntensity(intensity);
        }
        std::cout << std::endl;
    }
    return 0;
}