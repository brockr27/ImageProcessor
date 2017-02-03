#pragma once
#include "Pixel.h"

class Image
{
public:
	Pixel** originalImage;
	Pixel** alteredImage;

	Image();
	~Image();
};

