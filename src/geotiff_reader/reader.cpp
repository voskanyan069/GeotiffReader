#include "geotiff_reader/reader.hpp"

GeotiffReader::GeotiffReader(const std::string& sTiffname)
{
	GDALAllRegister();

	m_geotiffDataset = (GDALDataset*) GDALOpen(sTiffname.c_str(), GA_ReadOnly);
	m_NROWS = GDALGetRasterYSize(m_geotiffDataset);
	m_NCOLS = GDALGetRasterXSize(m_geotiffDataset);
	m_NLEVELS = GDALGetRasterCount(m_geotiffDataset);
}

double* GeotiffReader::GetGeotransform()
{
    m_geotiffDataset->GetGeoTransform(m_geotransform);
    return m_geotransform;
}

int* GeotiffReader::GetDimensions()
{
	m_dimensions[0] = m_NROWS; 
	m_dimensions[1] = m_NCOLS;
	m_dimensions[2] = m_NLEVELS; 
	return m_dimensions;  
} 

PositionsMatrix GeotiffReader::GetRasterBand(const int& z)
{
	PositionsMatrix bandLayer = new float*[m_NROWS];
	switch(GDALGetRasterDataType(m_geotiffDataset->GetRasterBand(z))) {
		case  1: return GetArray<unsigned char>(z, bandLayer); 
		case  2: return GetArray<unsigned short>(z, bandLayer);
		case  3: return GetArray<short>(z, bandLayer);
		case  4: return GetArray<unsigned int>(z, bandLayer);
		case  5: return GetArray<int>(z, bandLayer);
		case  6: return GetArray<float>(z, bandLayer);
		case  7: return GetArray<double>(z, bandLayer);
		default: return nullptr;
	}
}

template<typename T>
PositionsMatrix GeotiffReader::GetArray(int iLayerIndex,
		PositionsMatrix oBandLayer)
{
	GDALDataType bandType = GDALGetRasterDataType(
			m_geotiffDataset->GetRasterBand(iLayerIndex));
	int nbytes = GDALGetDataTypeSizeBytes(bandType);
	T *rowBuff = (T*) CPLMalloc(nbytes*m_NCOLS);

	for(int row = 0; row < m_NROWS; ++row) {
		CPLErr e = m_geotiffDataset->GetRasterBand(iLayerIndex)->RasterIO(
				GF_Read, 0, row, m_NCOLS, 1, rowBuff,
				m_NCOLS, 1, bandType, 0, 0);
		if (e) {
			exit(1);
		}
		oBandLayer[row] = new float[m_NCOLS];
		for(int col = 0; col < m_NCOLS; ++col) {
			oBandLayer[row][col] = static_cast<float>(rowBuff[col]);
		}
	}
	CPLFree(rowBuff);
	return oBandLayer;
}

GeotiffReader::~GeotiffReader()
{
	GDALClose(m_geotiffDataset);
	GDALDestroyDriverManager();
}
