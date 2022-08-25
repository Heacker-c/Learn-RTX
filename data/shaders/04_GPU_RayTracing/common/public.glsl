const int maxDepth = 30;
const float PI = 3.14159265358979323846f;

uniform vec2 viewSize;
uniform vec2 viewSizeInv;
uniform int shpereNum;
uniform int currentFrame;
uniform int recursionDepth;

uniform vec3 origin;
uniform vec3 horizontal;
uniform vec3 vertical;
uniform vec3 lower_left_corner;
uniform float lens_radius;
uniform vec3 axisU;
uniform vec3 axisV;

struct Ray
{
    bool bRaytracing;
    int materialIndex;
    int maxDepth;
    vec2 tInterval;
    vec3 orig;
    vec3 dir;
};

struct Sphere
{
    vec3 center;
    float radius;
    vec3 empty;
    int materialIndex;
};

struct HitRecord
{
    bool front_face;
    float t;
    vec3 p;
    vec3 normal;
};

struct Material
{
    vec3 albedo;
    int materialType;
    vec2 empty;
    float refractScale;
    float fuzz;
};

vec3 rayAt(inout Ray ray, float time)
{
    return ray.orig + ray.dir * time;
}

float length_2(vec3 pos)
{
    return pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
}