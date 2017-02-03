#include "Pixel.h"

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b)
{
	red = r;
	green = g;
	blue = b;
}

Pixel::Pixel() {
	red = 0;
	green = 0;
	blue = 0;
}

Pixel::~Pixel() {

}
