#include <gdal/gdal_priv.h>

#include "geotiff_reader/reader.hpp"
#include "base/system.hpp"

GeotiffReader::GeotiffReader(const std::string& filename)
	: m_dataset(nullptr)
	, m_rows(0)
	, m_cols(0)
	, m_levels(0)
	, m_dimensions(new int[3])
	, m_geotransform(new double[3])
{
	GDALAllRegister();
	m_dataset = (GDALDataset*) GDALOpen(filename.c_str(), GA_ReadOnly);
	m_rows = GDALGetRasterYSize(m_dataset);
	m_cols = GDALGetRasterXSize(m_dataset);
	m_levels = GDALGetRasterCount(m_dataset);
}

int* GeotiffReader::dimensions()
{
	m_dimensions[0] = m_rows;
	m_dimensions[1] = m_cols;
	m_dimensions[2] = m_levels;
	return m_dimensions;
}

double* GeotiffReader::geotransform()
{
	m_dataset->GetGeoTransform(m_geotransform);
	return m_geotransform;
}

#include <iostream>

PixelsMatrix GeotiffReader::raster_band(const int& z)
{
	PixelsMatrix band_layer = new float*[m_rows];
	switch(GDALGetRasterDataType(m_dataset->GetRasterBand(z)))
	{
		case  1: return get_array<unsigned char>(z, band_layer);
		case  2: return get_array<unsigned short>(z, band_layer);
		case  3: return get_array<short>(z, band_layer);
		case  4: return get_array<unsigned int>(z, band_layer);
		case  5: return get_array<int>(z, band_layer);
		case  6: return get_array<float>(z, band_layer);
		case  7: return get_array<double>(z, band_layer);
		default: return nullptr;
	}
}

template <typename T>
PixelsMatrix GeotiffReader::get_array(int layer_idx, PixelsMatrix band_layer)
{
	GDALDataType band_type = GDALGetRasterDataType(
			m_dataset->GetRasterBand(layer_idx));
	int nbytes = GDALGetDataTypeSizeBytes(band_type);
	T* row_buff = (T*) CPLMalloc(nbytes * m_cols);
	for(int row = 0; row < m_rows; ++row)
	{
		CPLErr e = m_dataset->GetRasterBand(layer_idx)->RasterIO(GF_Read, 0,
				row, m_cols, 1, row_buff, m_cols, 1, band_type, 0, 0);
		if (e)
		{
			SysUtil::error("Failed to read tiff file");
		}
		band_layer[row] = new float[m_cols];
		for(int col = 0; col < m_cols; ++col) {
			band_layer[row][col] = static_cast<float>(row_buff[col]);
		}
	}
	CPLFree(row_buff);
	return band_layer;
}

GeotiffReader::~GeotiffReader()
{
	GDALClose(m_dataset);
	GDALDestroyDriverManager();
}
