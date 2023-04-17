#pragma once

#include "hittable.h"
#include "vectors.h"

//��ô����һ�����壬���ĺͰ뾶���ɡ�bool hit���Ƿ���еĺ���
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

    if (discriminant < 0) return false;//��С��0ʱ��˵��û�и���û�л���

    float sqrtd = sqrt(discriminant);//
 
    float root = (-half_b - sqrtd) / a; //root�������һ����,�м�+sqrtd������һ����������ѡ�������

    if (root < t_min || t_max < root) //�������ĸ��Ƿ���ƽ���ڣ����ڵĻ���ȴ�����һ�����ǲ�����ƽ���ڣ������ھͷ���û�л���
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