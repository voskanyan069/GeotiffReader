#ifndef __GEOTIFF_READER_READER_HPP__
#define __GEOTIFF_READER_READER_HPP__

#include <string>

class GDALDataset;
typedef float** PixelsMatrix;

class GeotiffReader
{
public:
	GeotiffReader(const std::string& filename);
	~GeotiffReader();
	
	void image_dimensions(int& row, int& col);
	void geotransform(double& px_width, double& px_height,
			double& min_x, double& max_y);
	PixelsMatrix raster_band();
	void get_array(int layer_idx);

private:
	GDALDataset* m_dataset;
	int m_rows;
	int m_cols;
	int m_levels;
	PixelsMatrix m_bandlayer;
};

#endif // __GEOTIFF_READER_READER_HPP__
