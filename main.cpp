#include<iostream>
#include<fstream>
#include"hittable_list.h"
#include"utils.h"
#include"color.h"
#include"camera.h"
#include"ray.h"
#include"vectors.h"
#include"sphere.h"
#include"material.h"


std::ofstream outfiles("C:\\Users\\53190\\Desktop\\�µ�һ��\\����ļ���\\���һĻ1.ppm", std::ios::out);

//void write_color(std::ostream& out, color pixel_color);
//void write_color2ppm(std::ofstream& outfiles, color pixel_color);

color ray_color(const ray& r, const hittable& world, int depth) 
{
    
    hit_record rec;

    // depth��ʣ��bounce����
    if (depth <= 0)
        return color(0, 0, 0);//������ݹ��������ֱ�ӷ��غ�ɫ

   
    if (world.hit(r, 0.001, infinity, rec)) 
    {

        ray scattered;//ɢ�����

        color attenuation;//���attenuation��˥����

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//��������Ĳ���ָ������������Ҫ��ô���з�ɢ��
        {
            return attenuation * ray_color(scattered, world, depth - 1);//˥����*��һ�εݹ�ģ��ݹ麯��
        }
        return color(0, 0, 0);
    }

    //����Ҫ����y���в�ֵ�����Ҫ����λ������
    vec3 unit_direction = unit_vector(r.direction());//Ҫ��λ��������Ϊֻ�е�λ������y��ȡֵ����[-1,1]
    auto t = 0.5 * (unit_direction.y() + 1.0);//��ΪҪ���в�ֵ�����Բ�ֵϵ����[-1.1]ӳ�䵽[0,1]
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);//�������ײ�ֵ������Ǳ���ɫ
}

hittable_list random_scene() 
{
    hittable_list world;//��Ⱦ������б�

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));//lambertian�Ĳ�����albedo
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));//���캯������Ĳ����ǣ�Բ�ģ��뾶�����ʣ�

    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {

            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) 
            {
                
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) 
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) 
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else 
                {
                    // glass
                    sphere_material = make_shared<dieletric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    shared_ptr<dieletric> material1 = make_shared<dieletric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    shared_ptr<Lambertian> material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    shared_ptr<metal> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() 
{

    // Image
    const float aspect_ratio = 3.0 / 2.0;//��߱�
    const int image_width = 1200;//��
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;//��������Ŀ
    const int max_depth = 50;//�����ȣ�Ҳ��������������
   
    // World
    hittable_list world = random_scene();

    /*hittable_list world;

    auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(color(0.1,0.2,0.5));
    auto material_left   = make_shared<dieletric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2),0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0),    0.5,   material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0),   0.5,   material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0),  -0.4,   material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0),    0.5,   material_right));*/

    /*auto R = cos(pi/4);
    hittable_list world;

    auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dieletric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));*/


    //camera
    point3 lookfrom(13, 2, 3);//�������λ��
    point3 lookat(0, 0, 0);//����ĵ��λ��
    vec3 vup(0, 1, 0);//�����������
    float dist_to_focus = 10.0f;
    double aperture = 0.1;

    //���
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    outfiles<< "P3\n" << image_width << " " << image_height << "\n255\n";


    //�Լ�������for_each������


    for (int j = image_height - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) 
            {
                auto u = (i + random_double()) / image_width;
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world,max_depth);
            }
            write_color2ppm(outfiles, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}