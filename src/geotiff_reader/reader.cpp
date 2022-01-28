#include "geotiff_reader/reader.hpp"

GeotiffReader::GeotiffReader(const char* tiffname) : filename(tiffname)
{
	GDALAllRegister();

	geotiff_dataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	NROWS = GDALGetRasterYSize(geotiff_dataset); 
	NCOLS = GDALGetRasterXSize(geotiff_dataset); 
	NLEVELS = GDALGetRasterCount(geotiff_dataset);
}

double* GeotiffReader::get_geotransform()
{
    geotiff_dataset->GetGeoTransform(geotransform);
    return geotransform;
}

int* GeotiffReader::get_dimensions()
{
	dimensions[0] = NROWS; 
	dimensions[1] = NCOLS;
	dimensions[2] = NLEVELS; 
	return dimensions;  
} 

PositionsMatrix GeotiffReader::get_raster_band(int z)
{
	PositionsMatrix bandLayer = new float*[NROWS];
	switch(GDALGetRasterDataType(geotiff_dataset->GetRasterBand(z))) {
		case  1: return get_array<unsigned char>(z, bandLayer); 
		case  2: return get_array<unsigned short>(z, bandLayer);
		case  3: return get_array<short>(z, bandLayer);
		case  4: return get_array<unsigned int>(z, bandLayer);
		case  5: return get_array<int>(z, bandLayer);
		case  6: return get_array<float>(z, bandLayer);
		case  7: return get_array<double>(z, bandLayer);
		default: return NULL;  
	}
}

template<typename T>
PositionsMatrix GeotiffReader::get_array(int layer_idx,
		PositionsMatrix band_layer)
{
	GDALDataType band_type = GDALGetRasterDataType(
			geotiff_dataset->GetRasterBand(layer_idx));
	int nbytes = GDALGetDataTypeSizeBytes(band_type);
	T *row_buff = (T*) CPLMalloc(nbytes*NCOLS);

	for(int row = 0; row < NROWS; ++row) {
		CPLErr e = geotiff_dataset->GetRasterBand(layer_idx)->RasterIO(
				GF_Read, 0, row, NCOLS, 1, row_buff, NCOLS, 1, band_type, 0, 0);
		if (e)
			exit(1);
		band_layer[row] = new float[NCOLS];
		for(int col = 0; col < NCOLS; ++col) {
			band_layer[row][col] = static_cast<float>(row_buff[col]);
		}
	}
	CPLFree(row_buff);
	return band_layer;
}

GeotiffReader::~GeotiffReader()
{
	GDALClose(geotiff_dataset);
	GDALDestroyDriverManager();
}
