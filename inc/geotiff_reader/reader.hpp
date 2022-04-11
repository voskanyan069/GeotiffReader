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
	
	int* dimensions();
	double* geotransform();
	PixelsMatrix raster_band(const int& z);
	template <typename T>
	PixelsMatrix get_array(int layer_idx, PixelsMatrix band_layer);

private:
	GDALDataset* m_dataset;
	int m_rows;
	int m_cols;
	int m_levels;
	int* m_dimensions;
	double* m_geotransform;
};

#endif // __GEOTIFF_READER_READER_HPP__
