#ifndef __GEOTIFF_HPP__
#define __GEOTIFF_HPP__

#include <iostream>
#include <string>
#include <gdal_priv.h>

typedef float** PositionsMatrix;

class Geotiff
{
public: 
	Geotiff(const char* tiffname);
	double* GetGeoTransform();
	int* GetDimensions();
	PositionsMatrix GetRasterBand(int z);
	template<typename T>
	PositionsMatrix GetArray2D(int layerIndex, PositionsMatrix bandLayer);
	~Geotiff();

private:
	const char* filename;
	GDALDataset* geotiffDataset;
	double geotransform[6];
	int dimensions[3];
	int NROWS, NCOLS, NLEVELS;
};

#endif // __GEOTIFF_HPP__
