#include "ImageReader.h"
#include "Pixel.h"
#include <string>
#include <sstream>

using namespace std;

/* Constructor that does various error checking before reading the image and
 * creating the array of pixels.
 */
ImageReader::ImageReader(const char* filepath) {
	if (filepath == NULL) {
		cerr << "Filename provided was invalid: " << filepath << endl;
		exit(1);
	 }

	file.open(filepath);
	if (!file.is_open()) {
		cerr << "Couldn't open file!" << endl;
		exit(1);
	}

	bool errorCheck = readHeader();
	if (errorCheck == false) {
		cerr << "Couldn't read file!" << endl;
		file.close();
		exit(1);
	}

	arraySize = (imageHeight * imageWidth);
	pixelArray.reserve(arraySize);
	for (int i = 0; i < arraySize; i++) {
		Pixel p;
		pixelArray.push_back(p);
	}

	populatePixelArray();

	file.close();
}

/* Checks if either filestream is still open and closes them if so,
 * before deleting the object.
 */
ImageReader::~ImageReader() {
	if (file.is_open()) {
		file.close();
	}
	if (outputfile.is_open()) {
		outputfile.close();
	}
}

/* Reads the header of the file and gets the various data.
 * Returns true if its the valid format ("P3"), false if otherwise.
 * This works and is complete.
 */
bool ImageReader::readHeader() {
	int count = 0;
	string word;

	while (count < 4) {
		file >> word;
		
		// Checks for a comment line and if it is not then continue.
		if (word.at(0) != '#') {
			switch (count) {
			case 0:
				fileFormat = word;
				break;
			case 1:
				imageWidth = stoi(word);
				break;
			case 2:
				imageHeight = stoi(word);
				break;
			case 3:
				maxColorValue = stoi(word);
				break;
			default:
				break;
			}
			count++;
		}
		else {
			// Will ignore everything until '\n' is found, so it'll ignore the whole line
			file.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	
	if (fileFormat == "P3")
		return true;
	else
		return false;
}

/* Reads the rest of the file (the actual image) and puts the pixels into
 * an array.
 * This works and is complete.
 */
void ImageReader::populatePixelArray() {
	int rgbCount = 0;
	int arrayCount = 0;
	Pixel p;
	string line;

	while (arrayCount < arraySize) {
		getline(file, line);
		if (line != "") {
			istringstream iss(line);
			uint16_t temp;
			iss >> temp;
			unsigned char c = temp;

			switch (rgbCount) {
			case 0:
				p.red = c;
				break;
			case 1:
				p.green = c;
				break;
			case 2:
				p.blue = c;
				break;
			default:
				break;
			}

			if (rgbCount >= 2) {
				pixelArray.at(arrayCount) = p;
				rgbCount = 0;
				arrayCount++;
			}
			else {
				rgbCount++;
			}
		}
	}
}

/* Returns a Pixel with the same values as the one at the (x*y) spot in the array.
 */
Pixel ImageReader::getPixel(int x, int y) {
	Pixel p;
	if (x >= 0 && x < imageWidth && y >= 0 && y < imageHeight) {
		int pos = x + (y*imageWidth);
		p = pixelArray.at(pos);
	}
	else {
		cerr << "Pixel not found! ImageReader::getPixel(" << x << "," << y << ")" << endl;
	}
	return p;
}

/* Returns a Pixel with the same values as the one at the 'pos' spot in the array.
*/
Pixel ImageReader::getPixel(int pos) {
	Pixel p;
	if (pos <= arraySize) {
		p = pixelArray.at(pos);
	}
	else {
		cerr << "Pixel not found! ImageReader::getPixel(" << pos << ")" << endl;
	}
	return p;
}

/* Sets the pixel at the (x*y) spot in the array with the given Pixel's values.
 */
void ImageReader::setPixel(int x, int y, Pixel p) {
	if (x >= 0 && x < imageWidth && y >= 0 && y < imageHeight) {
		int pos = x + (y*imageWidth);
		Pixel& pr = pixelArray.at(pos);
		pr.red = p.red;
		pr.green = p.green;
		pr.blue = p.blue;
	}
	else {
		cerr << "Pixel not found! ImageReader::setPixel(" << x << "," << y << ")" << endl;
	}
}

/* Sets the pixel at the 'pos' spot in the array with the given Pixel's values.
*/
void ImageReader::setPixel(int pos, Pixel p) {
	if (pos <= arraySize) {
		Pixel& pr = pixelArray.at(pos);
		pr.red = p.red;
		pr.green = p.green;
		pr.blue = p.blue;
	}
	else {
		cerr << "Pixel not found! ImageReader::setPixel(" << pos << ")" << endl;
	}
}

/* Writes the image to the file speciefied by the constructor's parameter
 *  using a ImageReader reference. Returns 0 if successful, 1 if otherwise.
 */
bool ImageReader::writeFile(const char* filepath) {
	if (filepath == NULL) {
		cerr << "Filename provided was invalid: " << filepath << endl;
		return 1;
	}
	outputfile.open(filepath);

	if (!outputfile.is_open()) {
		cerr << "Couldn't open output file!" << endl;
		cout << "Couldn't open output file!" << endl;
		return 1;
	}

	outputfile << fileFormat << endl;
	outputfile << imageWidth << " " << imageHeight << endl;
	outputfile << maxColorValue << endl;

	int rgbCount = 0;
	for (int i = 0; i < arraySize; i++) {
		Pixel p = pixelArray.at(i);
		// Required to convert to ACII values
		int r = p.red;
		int g = p.green;
		int b = p.blue;

		outputfile << r << endl;
		outputfile << g << endl;
		outputfile << b << endl;
	}

	outputfile.close();
	return 0;
}