#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "Pixel.h"

class ImageReader {
public:
	std::ifstream file;
	std::ofstream outputfile;
	std::string fileFormat;
	int imageWidth;
	int imageHeight;
	int maxColorValue;
	std::vector<Pixel> pixelArray;
	int arraySize;

	ImageReader(const char*);
	~ImageReader();
	
	Pixel getPixel(int, int);
	Pixel getPixel(int);
	void setPixel(int, int, Pixel);
	void setPixel(int, Pixel);
	bool writeFile(const char*);

private:
	bool readHeader();
	void populatePixelArray();
};

