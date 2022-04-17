#include <gdal/gdal_priv.h>

#include "geotiff_reader/reader.hpp"
#include "base/system.hpp"

GeotiffReader::GeotiffReader(const std::string& filename)
	: m_dataset(nullptr)
	, m_rows(0)
	, m_cols(0)
	, m_levels(0)
{
	GDALAllRegister();
	m_dataset = (GDALDataset*) GDALOpen(filename.c_str(), GA_ReadOnly);
	m_rows = GDALGetRasterYSize(m_dataset);
	m_cols = GDALGetRasterXSize(m_dataset);
	m_levels = GDALGetRasterCount(m_dataset);
	m_bandlayer = new float*[m_rows];
}

void GeotiffReader::image_dimensions(int& width, int& height)
{
	width = m_rows;
	height = m_cols;
}

void GeotiffReader::geotransform(double& px_width, double& px_height,
		double& min_x, double& max_y)
{
	double* gt = new double[6];
	m_dataset->GetGeoTransform(gt);
	px_width = gt[1];
	px_height = gt[5];
	min_x = gt[0];
	max_y = gt[3];
	delete[] gt;
}

PixelsMatrix GeotiffReader::raster_band()
{
	const int z = 1;
	GDALGetRasterDataType(m_dataset->GetRasterBand(z));
	get_array(z);
	return m_bandlayer;
}

void GeotiffReader::get_array(int layer_idx)
{
	GDALDataType band_type = GDALGetRasterDataType(
			m_dataset->GetRasterBand(layer_idx));
	int nbytes = GDALGetDataTypeSizeBytes(band_type);
	short* row_buff = (short*) CPLMalloc(nbytes * m_cols);
	for(int row = 0; row < m_rows; ++row)
	{
		CPLErr e = m_dataset->GetRasterBand(layer_idx)->RasterIO(GF_Read, 0,
				row, m_cols, 1, row_buff, m_cols, 1, band_type, 0, 0);
		if (e)
		{
			SysUtil::error("Failed to read tiff file");
		}
		m_bandlayer[row] = new float[m_cols];
		for(int col = 0; col < m_cols; ++col) {
			m_bandlayer[row][col] = static_cast<float>(row_buff[col]);
		}
	}
	CPLFree(row_buff);
}

GeotiffReader::~GeotiffReader()
{
	for (int i = 0; i < m_rows; ++i)
	{
		delete[] m_bandlayer[i];
	}
	delete[] m_bandlayer;
	GDALClose(m_dataset);
}
