#ifndef _BVH_H
#define _BVH_H

#include "AABB.h"
#include "HitableList.h"
#include <algorithm>

namespace RTX
{
    class BVH : public Hitable 
    {
    public:
        BVH() {}
        BVH(HittableList& list, RayPrecision t0, RayPrecision t1) : BVH(list, 0, list.objects.size(), t0, t1) {}
        BVH(HittableList& list, size_t start, size_t end, RayPrecision time0, RayPrecision time1)
        {
            int axis = Define::random_int(0, 2);
            auto comparator = (axis == 0) ? box_x_compare
                : (axis == 1) ? box_y_compare
                : box_z_compare;

            size_t object_span = end - start;

            if (object_span == 1) {
                left = right = list.objects[start];
            }
            else if (object_span == 2) {
                if (comparator(list.objects[start], list.objects[start + 1])) {
                    left = list.objects[start];
                    right = list.objects[start + 1];
                }
                else {
                    left = list.objects[start + 1];
                    right = list.objects[start];
                }
            }
            else {
                std::sort(list.objects.begin() + start, list.objects.begin() + end, comparator);

                auto mid = start + object_span / 2;
                left = ToPtr(new BVH(list, start, mid, time0, time1));
                right = ToPtr(new BVH(list, mid, end, time0, time1));
            }
            AABB box_left, box_right;
            if (!left->boundingBox(time0, time1, box_left)
                || !right->boundingBox(time0, time1, box_right))
                std::cerr << "No bounding box in bvh_node constructor.\n";
            box = AABB::surroundingBox(box_left, box_right);
        }
        virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
        {
            output_box = box;
            return true;
        }
        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
        {
            if (!box.hit(ray, t_min, t_max))
                return false;

            bool hit_left = left->hit(ray, t_min, t_max, rec);
            bool hit_right = right->hit(ray, t_min, hit_left ? rec.t : t_max, rec);

            return hit_left || hit_right;
        }
        static bool box_compare(const Hitable::CPtr a, const Hitable::CPtr b, int axis)
        {
            AABB box_a;
            AABB box_b;

            if (!a->boundingBox(0, 0, box_a) || !b->boundingBox(0, 0, box_b))
                std::cerr << "No bounding box in bvh_node constructor.\n";
            switch (axis)
            {
            case 0: return box_a.min().x < box_b.min().x;
            case 1: return box_a.min().y < box_b.min().y;
            case 2: return box_a.min().z < box_b.min().z;
            default:return false;
            }
        }
        static bool box_x_compare(const Hitable::CPtr a, const Hitable::CPtr b)
        {
            return box_compare(a, b, 0);
        }
        static bool box_y_compare(const Hitable::CPtr a, const Hitable::CPtr b)
        {
            return box_compare(a, b, 1);
        }
        static bool box_z_compare(const Hitable::CPtr a, const Hitable::CPtr b)
        {
            return box_compare(a, b, 2);
        }
    public:
        Hitable::CPtr left;
        Hitable::CPtr right;
        AABB box;
    };
}
#endif