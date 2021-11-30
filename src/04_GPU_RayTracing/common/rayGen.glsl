
Ray getRay(uint index)
{
    seed = tea(index, currentFrame);
    float r1 = rnd(seed.x);
    float r2 = rnd(seed.y);
    float sq = sqrt(1.0 - r2);
    vec2 shake = vec2(cos(2.0 * PI * r1) * sq, sin(2.0 * PI * r1) * sq);
    vec2 uv = (gl_GlobalInvocationID.xy + shake) * viewSizeInv;

    vec3 rd = lens_radius * random_in_unit_sphere();
    vec3 offset = axisU * rd.x + axisV * rd.y;

    Ray ray;
    ray.orig = origin + offset;
    ray.dir = lower_left_corner + uv.x * horizontal + uv.y * vertical - origin - offset;
    ray.bRaytracing = true;
    return ray;
}
