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

//����
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

//����һ��[0,1��֮������ֵ��ȡ����1��ע�����ȡ����1�Ƿǳ���Ҫ�ģ���ʱ�������ܴ���������л�úô���
inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}
//����һ��[min,max)֮������ֵ
inline double random_double(double min, double max)
{
	return min + random_double() * (max - min);
}
//��һ�ַ�ʽʵ�����������Ҫ���randomͷ�ļ�
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