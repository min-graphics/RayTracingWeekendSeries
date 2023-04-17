#pragma once
#include<iostream>
#include"vectors.h"

class ray
{
public:
	ray() {};
	ray(const point3& original, const vec3& direction) :orig(original), dir(direction) {}

	point3 origin()const { return orig; }
	vec3 direction()const { return dir; }

	point3 at(double t)const
	{
		return orig + t * dir;
	}
	point3 operator[](double t)const
	{
		return orig + t * dir;
	}
private:
	point3 orig;
	vec3 dir;
};