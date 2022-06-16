#include <gdal/gdal_priv.h>
#include <gdal/cpl_error.h>

#include "geotiff_types/geo_exception.hpp"
#include "geotiff_reader/reader.hpp"
#include "base/system.hpp"

const int Z_LAYER_INDEX = 1;

void gdal_error_handler(CPLErr cpl_err, int err_no,
        const char* msg)
{
    switch (cpl_err)
    {
        case CE_Fatal:
        case CE_Failure:
            throw GeoException(msg, err_no);
            break;
    }
}

GeotiffReader::GeotiffReader(const std::string& filename)
	: m_dataset(nullptr)
    , m_band(nullptr)
	, m_rows(0)
	, m_cols(0)
	, m_levels(0)
    , m_geotransform(new double[6])
    , m_invgeotransform(new double[6])
    , m_is_geotransform_read(false)
{
    CPLSetErrorHandler(::gdal_error_handler);
    GDALAllRegister();
    m_dataset = (GDALDataset*) GDALOpen(filename.c_str(), GA_ReadOnly);
    m_band = GDALRasterBand::FromHandle(GDALGetRasterBand(
                m_dataset, Z_LAYER_INDEX));
    m_rows = GDALGetRasterYSize(m_dataset);
    m_cols = GDALGetRasterXSize(m_dataset);
    m_levels = GDALGetRasterCount(m_dataset);
    m_bandlayer = new float*[m_rows];
}

double* GeotiffReader::geotransform()
{
    m_is_geotransform_read = true;
	m_dataset->GetGeoTransform(m_geotransform);
    return m_geotransform;
}

double* GeotiffReader::inv_geotransform()
{
    if (!m_is_geotransform_read)
    {
        geotransform();
    }
    GDALInvGeoTransform(m_geotransform, m_invgeotransform);
    return m_invgeotransform;
}

int GeotiffReader::value_at(const int x, const int y)
{
    int32_t value = 0;
    GDALRasterIO(m_band, GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Int32, 0, 0);
    return value;
}

void GeotiffReader::read_data()
{
    get_array(Z_LAYER_INDEX);
}

void GeotiffReader::get_array(int layer_idx)
{
	GDALDataType band_type = GDALGetRasterDataType(m_band);
	int nbytes = GDALGetDataTypeSizeBytes(band_type);
	short* row_buff = (short*) CPLMalloc(nbytes * m_cols);
	for(int row = 0; row < m_rows; ++row)
	{
		CPLErr e = m_band->RasterIO(GF_Read, 0,
				row, m_cols, 1, row_buff, m_cols, 1, band_type, 0, 0);
		if (e)
		{
			throw GeoException("Failed to read tiff file", 1);
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
    delete[] m_geotransform;
    delete[] m_invgeotransform;
	GDALClose(m_dataset);
}
