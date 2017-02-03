#include "ImageWriter.h"
#include "ImageReader.h"
#include "Pixel.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ImageWriter::ImageWriter(const char* filename) {
	if (filename == NULL) {
		cerr << "Filename provided was invalid: " << filename << endl;
		exit(1);
	}
	file.open(filename);
	if (!file.is_open()) {
		cerr << "Couldn't open file!" << endl;
		exit(1);
	}
}


ImageWriter::~ImageWriter() {
	if (file.is_open())
		file.close();
}

/* Writes the image to the file speciefied by the constructor's parameter
*  using a ImageReader reference.
*  This all works and is complete.
*/
int ImageWriter::writeFile(ImageReader& reader) {
	if (!file.is_open()) {
		cerr << "File not open!" << endl;
		return 1;
	}
	
	file << reader.fileFormat << endl;
	file << reader.imageWidth << " " << reader.imageHeight << endl;
	file << reader.maxColorValue << endl;

	int rgbCount = 0;
	Pixel p;
	for (int i = 0; i < reader.arraySize; i++) {
		p = reader.pixelArray[i];
		int r = p.red;
		int g = p.green;
		int b = p.blue;

		file << r << endl;
		file << g << endl;
		file << b << endl;
	}

	file.close();
}