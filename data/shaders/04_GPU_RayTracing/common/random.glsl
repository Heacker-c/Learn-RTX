//产生随机数

uint tea(uint val0, uint val1)
{
    uint v0 = val0;
    uint v1 = val1;
    uint s0 = 0;

    for (uint n = 0; n < 16; n++)
    {
        s0 += 0x9e3779b9;
        v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
        v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
    }

    return v0;
}

uint wseed;

uint whash(uint seed)
{
    seed = (seed ^ uint(61)) ^ (seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> uint(4));
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> uint(15));
    return seed * 0x125E591;
}

void seed(uint val0, uint val1)
{
    wseed = tea(val0, val1);
}

float rand()
{
	wseed = whash(wseed);

	return float(wseed) / 4294967296.0;
}

float random(float min, float max)
{
    return (max - min) * (rand() - 0.5);
}

vec3 randomVec3()
{
    return vec3(rand(), rand(), rand());
}

vec3 randomVec3(float min, float max)
{
    return vec3(random(min, max), random(min, max), random(min, max));
}

vec3 random_in_unit_sphere()
{
#if 1
    vec3 p = randomVec3(-1.0, 1.0)/* * 0.866025403784*/;
    float len_2 = length_2(p);
    if(len_2 > 1.0)
        p /= sqrt(len_2);
    return p;
#else
    const float pi = 3.14159265358979323846f;
/*
    float u = random(0.0, 1.0);
    float v = random(0.0, 1.0);
    float theta = pi * u;
    float phi = acos(v - 1.0);
    float x = sin(theta) * sin(phi);
    float y = cos(theta) * sin(phi);
    float z = cos(phi);
    return vec3(x, y, z);
//*/
//*
    float phi = random(0.0, 1.0) * pi;
    float theta = random(-1.0, 1.0) * pi;
    return vec3(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
//*/
    vec3 p = randomVec3(-1.0, 1.0);
    float len_2 = length_2(p);
    if(len_2 > 1.0)
        p /= sqrt(len_2);
    return p;
#endif
}

vec3 random_unit_vector()
{
    return normalize(random_in_unit_sphere());
}

vec3 random_in_hemisphere(vec3 normal)
{
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}