#ifndef __GEOTIFF_READER_READER_HPP__
#define __GEOTIFF_READER_READER_HPP__

#include <string>

class GDALDataset;
class GDALRasterBand;
typedef float** PixelsMatrix;

class GeotiffReader
{
public:
	GeotiffReader(const std::string& filename);
	~GeotiffReader();
	
	double* geotransform();
    double* inv_geotransform();
    void read_data();
    int value_at(const int x, const int y);

private:
	void get_array(int layer_idx);

private:
	int m_rows;
	int m_cols;
	int m_levels;
    double* m_geotransform;
    double* m_invgeotransform;
    bool m_is_geotransform_read;
	PixelsMatrix m_bandlayer;
	GDALDataset* m_dataset;
    GDALRasterBand* m_band;
};

#endif // __GEOTIFF_READER_READER_HPP__
