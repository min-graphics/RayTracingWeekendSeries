#pragma once
#include"vectors.h"
#include<iostream>
#include<fstream>

void write_color(std::ostream& out, color pixel_color)
{
	// Write the translated [0,255] value of each color component.
	/*out << static_cast<int>(255.999 * pixel_color.x()) << ' '
		<< static_cast<int>(255.999 * pixel_color.y()) << ' '
		<< static_cast<int>(255.999 * pixel_color.z()) << std::endl;*/
}
void write_color(std::ostream& out, color pixel_color, int sample_per_pixel)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();
	
	//gamma������r = sqrt(scale*r)
	double scale = 1.0 / sample_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	// Write the translated [0,255] value of each color component.
	/*out << static_cast<int>(255.999 * clamp(r, 0.0, 1.0)) << ' '
		  << static_cast<int>(255.999 * clamp(g, 0.0, 1.0)) << ' '
		  << static_cast<int>(255.999 * clamp(b, 0.0, 1.0)) << std::endl;*/

}

//ÿ�������߻������غ�ֱ�ӷ��غ���ֵ
void write_color2ppm(std::ofstream& outfiles, color pixel_color)
{
	outfiles << static_cast<int>(255.999 * pixel_color.x()) << ' '
		     << static_cast<int>(255.999 * pixel_color.y()) << ' '
			 << static_cast<int>(255.999 * pixel_color.z()) << std::endl;
}
//
void write_color2ppm(std::ofstream& outfiles, color pixel_color, int sample_per_pixel)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	//ÿ�ζ�����ǰ������ֵ���Բ�������ֵȡ��ƽ����
	double scale = 1.0 / sample_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	/*r *= scale;
	g *= scale;
	b *= scale;*/

	// Write the translated [0,255] value of each color component.
	outfiles << static_cast<int>(255.999 * clamp(r, 0.0, 1.0)) << ' '
			 << static_cast<int>(255.999 * clamp(g, 0.0, 1.0)) << ' '
			 << static_cast<int>(255.999 * clamp(b, 0.0, 1.0)) << std::endl;

}
