uvec2 seed;
//产生随机数
uvec2 tea(uint val0, uint val1)
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

    return uvec2(v0, v1);
}

uint lcg(uint prev)
{
    uint LCG_A = 1664525u;
    uint LCG_C = 1013904223u;
    prev = (LCG_A * prev + LCG_C);
    return prev & 0x00FFFFFF;
}

uint whash(uint seed)
{
    seed = (seed ^ uint(61)) ^ (seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> uint(4));
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> uint(15));
    return seed;
}
float rnd(uint prev)
{
    seed.x = whash(seed.y);
    seed.y = whash(seed.x);
    return (float(lcg(prev)) / float(0x01000000));
}

float random(float min, float max)
{
    return (max - min) * (rnd(seed.x) - 0.5);
}

float rand(uint n)
{
    return fract(sin(mod(dot(float(n), 12.9898), 3.1415926f)) * 43758.5453);
}

vec3 randomVec3()
{
    return vec3(rnd(seed.x), rnd(seed.y), rnd(seed.x * seed.y));
}

vec3 randomVec3(float min, float max)
{
    return vec3(random(min, max), random(min, max), random(min, max));
}

vec3 random_in_unit_sphere()
{
    while (true)
    {
        vec3 p = randomVec3(-1.0, 1.0) * 0.78539815;
        if (length_2(p) > 1.0) continue;
        return p;
    }
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