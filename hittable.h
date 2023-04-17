#pragma once

#include<memory>
#include "ray.h"


class material;

//把需要传递一堆参数进行打包。
struct hit_record {
    point3 p;//交点坐标
    vec3 normal;//交点法线向量
    shared_ptr<material> mat_ptr;//交点处材料属性，用一个智能指针指向material
    double t;//交点在PO向量上的参数位置
    bool front_face;//是否是在正面

    void set_face_normal(const ray& r, const vec3& outward_normal) 
    {
        front_face = dot(r.direction(), outward_normal) < 0;//向外的向量和光线r是反向的，简单的将光线与法线做点乘来判断，如果法线与光线相同，那就是从内部击中内表面。相反则是在外部击中外表面。
        normal = front_face ? outward_normal : -outward_normal;//
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};