#ifndef __GEOTIFF_HPP__
#define __GEOTIFF_HPP__

#include <gdal_priv.h>

typedef float** PositionsMatrix;

class Geotiff
{
public: 
	Geotiff(const char *tiffname);
	double *get_geotransform();
	int *get_dimensions();
	PositionsMatrix get_raster_band(int z);
	template<typename T>
	PositionsMatrix get_array(int layer_idx, PositionsMatrix band_layer);
	~Geotiff();

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

#endif // __GEOTIFF_HPP__
