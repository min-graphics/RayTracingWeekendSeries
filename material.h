#pragma once
#include"utils.h"
#include"hittable.h"

struct hit_record;

class material 
{
public:
    //生成散射后光线的函数，在函数中使用hit_record传入参数，就可以不用传入一大堆变量了。
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
    double fuzz;//模糊程度，为0时不模糊，为1时会变得模糊

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

//透明的材质，水，玻璃，钻石等，绝缘体材质。这里光线衰减率为1，也就是不衰减，玻璃表面不吸收光的能量。
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
        attenuation = color(1.0, 1.0, 1.0);//衰减系数等于1，不衰减
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = unit_vector(r.direction());

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;//这个bool值来判断到底有没有发生折射，有时候所有的光线都不发生折射，转而发生了反射，又称为全内反射。
        vec3 direction;

        if (cannot_refract||reflectance(cos_theta,refraction_ratio)>random_double())
        {
            direction = reflect(unit_direction, rec.normal);//全部都是反射
        }
        else
        {
            direction = refract(unit_direction, rec.normal, refraction_ratio);//可以折射
        }
      
        scattered = ray(rec.p, direction);
        return true;
    }
    //施利克近似
    static double reflectance(double cosine, double ref_idx)
    {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};