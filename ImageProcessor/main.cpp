/* Created by Brock Rikkers
	This is a program that allows you to read in a .ppm image file and manipulate
	it in several different ways. The output filename must be different than the
	input filename. The file's must be of .ppm (Portable Pixmap format) extension as well.
	https://en.wikipedia.org/wiki/Netpbm_format
*/
#include <iostream>
#include <string>
#include "ImageReader.h"

using namespace std;

void darkenImage(float percent, ImageReader& reader) {
	float amount = percent / 100;

	for (int i = 0; i < reader.arraySize; i++) {
		Pixel p = reader.getPixel(i);
		p.red -= (p.red * amount);
		p.green -= (p.green * amount);
		p.blue -= (p.blue * amount);
			
		if (p.red < 0)
			p.red = 0;
		if (p.green < 0)
			p.green = 0;
		if (p.blue < 0)
			p.blue = 0;
		
		reader.setPixel(i, p);
	}
}

void lightenImage(float percent, ImageReader& reader) {
	float amount = percent / 100;

	for (int i = 0; i < reader.arraySize; i++) {
		Pixel p = reader.getPixel(i);
		p.red += (p.red * amount);
		p.green += (p.green * amount);
		p.blue += (p.blue * amount);

		if (p.red > 255)
			p.red = 255;
		if (p.green > 255)
			p.green = 255;
		if (p.blue > 255)
			p.blue = 255;

		reader.setPixel(i, p);
	}
}

/* Flips the image along whatever axis specified by swapping
 * the two pixels that are equidistant from the axis using 
 * an x or y offset.
 */
void flipImageAxis(string axis, ImageReader& reader) {
	if (axis == "horizontal") {
		for (int y = 0; y < reader.imageHeight / 2; y++) {
			int yOffset = reader.imageHeight - 1 - y;
			for (int x = 0; x < reader.imageWidth; x++) {
				Pixel p = reader.getPixel(x, y);
				Pixel p2 = reader.getPixel(x, yOffset);
				
				reader.setPixel(x, yOffset, p);
				reader.setPixel(x, y, p2);
			}
		}

	}
	else if (axis == "vertical") {
		for (int x = 0; x < reader.imageWidth / 2; x++) {
			int xOffset = reader.imageWidth - 1 - x;
			for (int y = 0; y < reader.imageHeight; y++) {
				Pixel p = reader.getPixel(x, y);
				Pixel p2 = reader.getPixel(xOffset, y);

				reader.setPixel(xOffset, y, p);
				reader.setPixel(x, y, p2);
			}
		}
	}

}

/* Flips the image 90/180/270 degrees by creating a new Pixel
 * vector, using math to figure out the new position, and then
 * swapping the new vector with the ImageReader's pixel vector.
 * For 180 or 270, it just calls this method (degree/90) times.
*/
void flipImageDegree(int degree, ImageReader& reader) {
	if (degree == 90) {
		std::vector<Pixel> newArray(reader.arraySize);
		int newWidth = reader.imageHeight;
		int newHeight = reader.imageWidth;
		for (int y = 0; y < reader.imageHeight; y++) {
			for (int x = 0; x < reader.imageWidth; x++) {
				int newX = newWidth - 1 - y;
				int newY = x;
				int newPos = newX + (newY*newWidth);
				Pixel p = reader.getPixel(x, y);

				newArray.at(newPos) = p;
			}
		}

		reader.pixelArray = newArray;
		reader.imageHeight = newHeight;
		reader.imageWidth = newWidth;
	}
	else if (degree == 180) {
		flipImageDegree(90, reader);
		flipImageDegree(90, reader);
	}
	else if (degree == 270) {
		flipImageDegree(90, reader);
		flipImageDegree(90, reader);
		flipImageDegree(90, reader);
	}
}

int main(int argc, char *argv[]) {
	string filename;
	string outputfilename;
	string filepath;
	string filepathOut;
	string fileQuestion;

	// Grabs the current directory and chops off the last two levels of folders
	string directory = argv[0];
	directory.erase(directory.find_last_of('\\'));
	directory.erase(directory.find_last_of('\\'));

	cout << "Search the current directory(type cd) for the file," << endl << "or type in the whole filepath(type fp)?" << endl
		<< "Current Directory: " << directory << endl;
	cin >> fileQuestion;
	if (fileQuestion == "cd") {
		cout << "Enter the name of the file (ie. Image.ppm): ";
		cin >> filename;
		cout << "Enter the output filename (ie. Imageout.ppm: ";
		cin >> outputfilename;

		// Appends the slash and the filename so the result will be something like
		// C:\Users\ImageProcessor\Image.ppm
		filepath = directory.append("\\");
		filepath.append(filename);
		filepathOut = directory.append("\\");
		filepathOut.append(outputfilename);
	}
	else {
		cout << "Enter the filepath: ";
		cin >> filepath;
		cout << "Enter the output filepath: ";
		cin >> filepathOut;
	}

	ImageReader reader(filepath.c_str());
	
	int input = -1;
	while (input != 0) {
		cout << "User commands: 0 - Exits program" << endl
			<< "1 - Saves image to specified filename" << endl
			<< "2 - Flips image by (90, 180, 270) degrees" << endl
			<< "3 - Flips image on the horizontal or vertical axis" << endl
			<< "4 - Darkens image by a certain percentage" << endl
			<< "5 - Lightens image by a certain percentage" << endl;
		cin >> input;

		if (input == 1) {
			bool result = reader.writeFile(filepathOut.c_str());
			if (result == true)
				cout << "Saved" << endl;
			else
				cout << "File not saved!" << endl;
		}
		else if (input == 2) {
			int degree;
			cout << "How many degrees (90, 180, or 270): " << endl;
			cin >> degree;
			flipImageDegree(degree, reader);
			cout << "Flipped " << degree << " degrees" << endl;
		}
		else if (input == 3) {
			string axis;
			cout << "Which axis (horizontal or vertical): " << endl;
			cin >> axis;
			flipImageAxis(axis, reader);
			cout << "Flipped " << axis << endl;
		}
		else if (input == 4) {
			float percent;
			cout << "By what percent: " << endl;
			cin >> percent;
			darkenImage(percent, reader);
			cout << "Darkened" << endl;
		}
		else if (input == 5) {
			int percent;
			cout << "By what percent: " << endl;
			cin >> percent;
			lightenImage(percent, reader);
			cout << "Lightened" << endl;
		}
	}

	return 0;
}