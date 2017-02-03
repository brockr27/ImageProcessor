#pragma once
#include "ImageReader.h"

class ImageWriter
{
public:
	std::ofstream file;

	ImageWriter(const char*);
	~ImageWriter();

	int writeFile(ImageReader&);
};

