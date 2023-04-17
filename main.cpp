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


std::ofstream outfiles("C:\\Users\\53190\\Desktop\\新的一个\\输出文件夹\\最后一幕1.ppm", std::ios::out);

//void write_color(std::ostream& out, color pixel_color);
//void write_color2ppm(std::ofstream& outfiles, color pixel_color);

color ray_color(const ray& r, const hittable& world, int depth) 
{
    
    hit_record rec;

    // depth是剩余bounce次数
    if (depth <= 0)
        return color(0, 0, 0);//当到达递归次数，就直接返回黑色

   
    if (world.hit(r, 0.001, infinity, rec)) 
    {

        ray scattered;//散射光线

        color attenuation;//这个attenuation是衰减率

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//根据球体的材质指针来决定到底要怎么进行分散。
        {
            return attenuation * ray_color(scattered, world, depth - 1);//衰减率*下一次递归的，递归函数
        }
        return color(0, 0, 0);
    }

    //背景要根据y进行插值，因此要先求单位向量。
    vec3 unit_direction = unit_vector(r.direction());//要求单位向量，因为只有单位向量，y的取值才是[-1,1]
    auto t = 0.5 * (unit_direction.y() + 1.0);//因为要进行插值，所以插值系数把[-1.1]映射到[0,1]
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);//进行蓝白插值，这就是背景色
}

hittable_list random_scene() 
{
    hittable_list world;//渲染物体的列表

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));//lambertian的参数是albedo
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));//构造函数，球的参数是（圆心，半径，材质）

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
    const float aspect_ratio = 3.0 / 2.0;//宽高比
    const int image_width = 1200;//宽
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;//采样点数目
    const int max_depth = 50;//最大深度，也就是最大迭代次数
   
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
    point3 lookfrom(13, 2, 3);//相机坐标位置
    point3 lookat(0, 0, 0);//看向的点的位置
    vec3 vup(0, 1, 0);//相机朝上坐标
    float dist_to_focus = 10.0f;
    double aperture = 0.1;

    //这个
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    outfiles<< "P3\n" << image_width << " " << image_height << "\n255\n";


    //自己试试用for_each来加速


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