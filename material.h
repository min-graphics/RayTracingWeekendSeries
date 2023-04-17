#pragma once
#include"utils.h"
#include"hittable.h"

struct hit_record;

class material 
{
public:
    //����ɢ�����ߵĺ������ں�����ʹ��hit_record����������Ϳ��Բ��ô���һ��ѱ����ˡ�
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};


class Lambertian :public material
{
private:
    color albedo;

public:
    Lambertian(const color& a) :albedo(a){}

    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered)const override
    {

        vec3 scatter_dir = rec.normal + random_unit_vector();

        //catch degenerate scatter direction
        if (scatter_dir.near_zero())
        {
            scatter_dir = rec.normal;
        }

        scattered = ray(rec.p, scatter_dir);
        attenuation = albedo;
        return true;
    }
};
class metal :public material
{
private:
    color albedo;
    double fuzz;//ģ���̶ȣ�Ϊ0ʱ��ģ����Ϊ1ʱ����ģ��

public:
    metal(const color& a,double f)
        :albedo(a),fuzz(f>=1?1:f)
    {}

    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered)const override
    {
        vec3 reflected = reflect(unit_vector(r.direction()), rec.normal);
        scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(),rec.normal) > 0);
    }
};

//͸���Ĳ��ʣ�ˮ����������ʯ�ȣ���Ե����ʡ��������˥����Ϊ1��Ҳ���ǲ�˥�����������治���չ��������
class dieletric :public material
{
private:
    double ir;

public:
    dieletric(const double r)
        :ir(r)
    {}
    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);//˥��ϵ������1����˥��
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = unit_vector(r.direction());

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;//���boolֵ���жϵ�����û�з������䣬��ʱ�����еĹ��߶����������䣬ת�������˷��䣬�ֳ�Ϊȫ�ڷ��䡣
        vec3 direction;

        if (cannot_refract||reflectance(cos_theta,refraction_ratio)>random_double())
        {
            direction = reflect(unit_direction, rec.normal);//ȫ�����Ƿ���
        }
        else
        {
            direction = refract(unit_direction, rec.normal, refraction_ratio);//��������
        }
      
        scattered = ray(rec.p, direction);
        return true;
    }
    //ʩ���˽���
    static double reflectance(double cosine, double ref_idx)
    {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};