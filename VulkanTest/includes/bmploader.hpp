// Write a BMP parser idea
#include <fstream>
#include <iostream>
#include <vector>

struct BMPHeader {
	char signature[2];
	uint32_t fileSize;
	uint32_t reserved;
	uint32_t dataOffset;
};

struct BMPInfoHeader {
	uint32_t headerSize;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bitDepth;
	uint32_t compression;
	uint32_t imageSize;
	int32_t xPixelsPerMeter;
	int32_t yPixelsPerMeter;
	uint32_t colorsUsed;
	uint32_t colorsImportant;
};

std::vector<uint8_t> loadBMP(const std::string &filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return {};
	}

	BMPHeader header;
	file.read(reinterpret_cast<char *>(&header), sizeof(BMPHeader));

	if (header.signature[0] != 'B' || header.signature[1] != 'M') {
		std::cerr << "Invalid BMP file: " << filename << std::endl;
		return {};
	}

	BMPInfoHeader infoHeader;
	file.read(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));

	if (infoHeader.bitDepth != 24) {
		std::cerr << "Unsupported BMP format: " << filename << std::endl;
		return {};
	}

	std::vector<uint8_t> imageData(infoHeader.imageSize);
	file.seekg(header.dataOffset, std::ios::beg);
	file.read(reinterpret_cast<char *>(imageData.data()), infoHeader.imageSize);

	return imageData;
}
