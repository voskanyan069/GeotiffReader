#ifndef __GEOTIFF_TYPES_IMAGE_SIZE_HPP__
#define __GEOTIFF_TYPES_IMAGE_SIZE_HPP__

struct ImageSize
{
public:
	ImageSize(const int w, const int h);

	int width() const;
	int height() const;

private:
	const int m_width;
	const int m_height;
};

#endif // __GEOTIFF_TYPES_IMAGE_SIZE_HPP__
