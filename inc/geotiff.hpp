#ifndef _GEOTIFF_HPP
#define _GEOTIFF_HPP

#include <iostream>
#include <string>
#include <gdal_priv.h>

#if 0
	#include <cpl_conv.h>
	#include <gdalwarper.h>
	#include <stdlib.h>
#endif

typedef float** PositionsMatrix;

class Geotiff
{
public: 
	Geotiff(const char* tiffname);
#if 0
	const char* GetFileName();
	const char* GetProjection();
	double GetNoDataValue();
#endif
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

#endif // _GEOTIFF_HPP
