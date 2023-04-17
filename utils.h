#pragma once
#include<cmath>
#include<limits>
#include<memory>
#include<stdlib.h>
#include<random>

#include"ray.h"


//using
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//常量
const double pi = 3.141592653;
const double infinity = std::numeric_limits<double>::infinity();

inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}
inline double radians_to_degress(double radians)
{
	return radians * 180.0 / pi;
}

//返回一个[0,1）之间的随机值，取不到1。注意这个取不到1是非常重要的，有时候我们能从这个特性中获得好处。
inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}
//返回一个[min,max)之间的随机值
inline double random_double(double min, double max)
{
	return min + random_double() * (max - min);
}
//另一种方式实现随机数，需要添加random头文件
inline double random_double(float min, float max)
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}
inline double clamp(double x, double min = 0.0, double max = 1.0)
{
	return x >= max ? max : x <= min ? min : x;
}