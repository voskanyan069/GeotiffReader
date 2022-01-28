#ifndef __GEOTIFF_STRUCTURES_IMAGE_SIZE_HPP__
#define __GEOTIFF_STRUCTURES_IMAGE_SIZE_HPP__

struct ImageSize
{
	int width;
	int height;

	ImageSize(int w, int h) : width(w), height(h)
	{
	}
};


#endif // __GEOTIFF_STRUCTURES_IMAGE_SIZE_HPP__
