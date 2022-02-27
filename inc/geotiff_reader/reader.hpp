#ifndef __GEOTIFF_READER_READER_HPP__
#define __GEOTIFF_READER_READER_HPP__

#include <gdal/gdal_priv.h>

typedef float** PositionsMatrix;

class GeotiffReader
{
public: 
	GeotiffReader(const std::string& sTiffname);
	double* GetGeotransform();
	int* GetDimensions();
	PositionsMatrix GetRasterBand(const int& z);
	template<typename T>
	PositionsMatrix GetArray(int iLayerIndex, PositionsMatrix oBandLayer);
	~GeotiffReader();

private:
	GDALDataset* m_geotiffDataset;
	double m_geotransform[6];
	int m_dimensions[3];
	int m_NROWS;
	int m_NCOLS;
	int m_NLEVELS;
};

#endif // __GEOTIFF_READER_READER_HPP__
