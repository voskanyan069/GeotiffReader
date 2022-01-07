#include "geotiff.hpp"

Geotiff::Geotiff(const char* tiffname)
{
	filename = tiffname; 
	GDALAllRegister();

	geotiffDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	NROWS   = GDALGetRasterYSize( geotiffDataset ); 
	NCOLS   = GDALGetRasterXSize( geotiffDataset ); 
	NLEVELS = GDALGetRasterCount( geotiffDataset );
}

double* Geotiff::GetGeoTransform()
{
    geotiffDataset->GetGeoTransform(geotransform);
    return geotransform;
}

int* Geotiff::GetDimensions()
{
	dimensions[0] = NROWS; 
	dimensions[1] = NCOLS;
	dimensions[2] = NLEVELS; 
	return dimensions;  
} 

PositionsMatrix Geotiff::GetRasterBand(int z)
{
	PositionsMatrix bandLayer = new float*[NROWS];
	switch( GDALGetRasterDataType(geotiffDataset->GetRasterBand(z)) )
	{
		case  1: return GetArray2D<unsigned char>(z,bandLayer); 
		case  2: return GetArray2D<unsigned short>(z,bandLayer);
		case  3: return GetArray2D<short>(z,bandLayer);
		case  4: return GetArray2D<unsigned int>(z,bandLayer);
		case  5: return GetArray2D<int>(z,bandLayer);
		case  6: return GetArray2D<float>(z,bandLayer);
		case  7: return GetArray2D<double>(z,bandLayer);
		default: return NULL;  
	}
}

template<typename T>
PositionsMatrix Geotiff::GetArray2D(int layerIndex, PositionsMatrix bandLayer)
{
	GDALDataType bandType = GDALGetRasterDataType(
			geotiffDataset->GetRasterBand(layerIndex));
	int nbytes = GDALGetDataTypeSizeBytes(bandType);
	T *rowBuff = (T*) CPLMalloc(nbytes*NCOLS);

	for(int row = 0; row < NROWS; ++row)
	{
		CPLErr e = geotiffDataset->GetRasterBand(layerIndex)->RasterIO(
				GF_Read, 0, row,NCOLS, 1, rowBuff, NCOLS, 1, bandType, 0, 0);
		if(!(e == 0))
		{
			std::cout << "Warning: Unable to read scanline in Geotiff!"
				<< std::endl;
			exit(1);
		}
		bandLayer[row] = new float[NCOLS];
		for(int col = 0; col < NCOLS; ++col)
		{
			bandLayer[row][col] = static_cast<float>(rowBuff[col]);
		}
	}
	CPLFree(rowBuff);
	return bandLayer;
}

Geotiff::~Geotiff()
{
	GDALClose(geotiffDataset);
	GDALDestroyDriverManager();
}
