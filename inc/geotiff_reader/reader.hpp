#ifndef __GEOTIFF_READER_READER_HPP__
#define __GEOTIFF_READER_READER_HPP__

#include <gdal/gdal_priv.h>

typedef float** PositionsMatrix;

class GeotiffReader
{
public: 
	GeotiffReader(const char *tiffname);
	double *get_geotransform();
	int *get_dimensions();
	PositionsMatrix get_raster_band(int z);
	template<typename T>
	PositionsMatrix get_array(int layer_idx, PositionsMatrix band_layer);
	~GeotiffReader();

private:
	const char *filename;
	GDALDataset *geotiff_dataset;
	double geotransform[6];
	int dimensions[3];
	void *null;
	int NROWS;
	int NCOLS;
	int NLEVELS;
};

#endif // __GEOTIFF_READER_READER_HPP__
