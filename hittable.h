#pragma once

#include<memory>
#include "ray.h"


class material;

//����Ҫ����һ�Ѳ������д����
struct hit_record {
    point3 p;//��������
    vec3 normal;//���㷨������
    shared_ptr<material> mat_ptr;//���㴦�������ԣ���һ������ָ��ָ��material
    double t;//������PO�����ϵĲ���λ��
    bool front_face;//�Ƿ���������

    void set_face_normal(const ray& r, const vec3& outward_normal) 
    {
        front_face = dot(r.direction(), outward_normal) < 0;//����������͹���r�Ƿ���ģ��򵥵Ľ������뷨����������жϣ���������������ͬ���Ǿ��Ǵ��ڲ������ڱ��档�෴�������ⲿ��������档
        normal = front_face ? outward_normal : -outward_normal;//
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};