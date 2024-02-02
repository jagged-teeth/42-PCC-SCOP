#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "glmd.hpp"

bool loadObj(const char *filepath, std::vector<Vec3> &out_vertices) {
	std::vector<unsigned int> vertex_indices;
	std::vector<Vec3> temp_vertices;

	FILE *file = fopen(filepath, "r");

	if (file == nullptr) {
		std::cout << "Cannot open obj file\n";
		return false;
	}

	while (1) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			Vec3 vertex;
			if (fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z) != 3) {
				std::cout << "Error while reading vertices data\n";
				return false;
			}
			temp_vertices.push_back(vertex);
		} else if (strcmp(lineHeader, "f") == 0) {
			std::vector<unsigned int> polygonIndices;
			unsigned int index;
			char lineEndCheck;
			int scannedItems;

			do {
				scannedItems = fscanf(file, "%u", &index);
				if (scannedItems != 1) {
					std::cout << "Error while reading face indices\n";
					fclose(file);
					return false;
				}
				polygonIndices.push_back(index - 1);
			} while (scannedItems =
						 fscanf(file, "%c", &lineEndCheck) != EOF && lineEndCheck != '\n');

			for (unsigned int i = 1; i < polygonIndices.size() - 1; i++) {
				vertex_indices.push_back(polygonIndices[0]);
				vertex_indices.push_back(polygonIndices[i]);
				vertex_indices.push_back(polygonIndices[i + 1]);
			}
		}
	}

	for (unsigned int i = 0; i < vertex_indices.size(); i++) {
		out_vertices.push_back(temp_vertices[vertex_indices[i]]);
	}
	fclose(file);
	return true;
}
