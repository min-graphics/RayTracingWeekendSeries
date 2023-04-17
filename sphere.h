#pragma once

#include "hittable.h"
#include "vectors.h"

//怎么定义一个球体，球心和半径即可。bool hit，是否击中的函数
class sphere : public hittable 
{
public:

    sphere() = default;
    sphere(point3 cen, double r,shared_ptr<material> m) : center(cen), radius(r),mat_ptr(m) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:

    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

};

//
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const 
{

    vec3 oc = r.origin() - center;

    float a = r.direction().length_squared();
    float half_b = dot(oc, r.direction());
    float c = oc.length_squared() - radius * radius;

    float discriminant = half_b * half_b - a * c;//

    if (discriminant < 0) return false;//当小于0时，说明没有根，没有击中

    float sqrtd = sqrt(discriminant);//
 
    float root = (-half_b - sqrtd) / a; //root是最近的一个根,中间+sqrtd的是另一个根，优先选择最近根

    if (root < t_min || t_max < root) //检测最近的根是否在平面内，不在的话则却检查另一个根是不是在平面内，都不在就返回没有击中
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}