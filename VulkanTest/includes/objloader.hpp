#pragma once

#include <cstring>
#include <fstream>
#include "glmd.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

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

// #pragma once

// #include <cstring>
// #include <fstream>
// #include <glm/glm.hpp>
// #include <iostream>
// #include <vector>

// bool loadObj(const char *filepath, std::vector<glm::vec3> &out_vertices,
//			 std::vector<glm::vec2> *out_texcoords = nullptr,
//			 std::vector<glm::vec3> *out_normals = nullptr) {
//	std::vector<unsigned int> vertex_indices, texcoord_indices, normal_indices;
//	std::vector<glm::vec3> temp_vertices;
//	std::vector<glm::vec2> temp_texcoords;
//	std::vector<glm::vec3> temp_normals;

//	FILE *file = fopen(filepath, "r");
//	unsigned int index;
//	float x, y, z;
//	int res, scannedItems;
//	char lineHeader[128], lineEndCheck;

//	if (file == nullptr) {
//		std::cout << "Cannot open .obj file\n";
//		return false;
//	}

//	while (1) {
//		res = fscanf(file, "%s", lineHeader);
//		if (res == EOF)
//			break;

//		if (strcmp(lineHeader, "v") == 0) {
//			if (fscanf(file, "%f %f %f\n", &x, &y, &z) != 3) {
//				std::cout << "Error reading vertex data\n";
//				fclose(file);
//				return false;
//			}
//			temp_vertices.push_back(glm::vec3(x, y, z));
//		} else if (strcmp(lineHeader, "vt") == 0) {
//			if (fscanf(file, "%f %f\n", &x, &y) != 2) {
//				std::cout << "Error reading texture coordinate data\n";
//				fclose(file);
//				return false;
//			}
//			temp_texcoords.push_back(glm::vec2(x, y));
//		} else if (strcmp(lineHeader, "vn") == 0) {
//			if (fscanf(file, "%f %f %f\n", &x, &y, &z) != 3) {
//				std::cout << "Error reading vertex normal data\n";
//				fclose(file);
//				return false;
//			}
//			temp_normals.push_back(glm::vec3(x, y, z));
//		} else if (strcmp(lineHeader, "f") == 0) {
//			std::vector<unsigned int> polygonIndices, textureIndices, normalIndices;
//			unsigned int textureIndex, normalIndex;
//			int hasTexture = 0, hasNormal = 0;

//			do {
//				hasTexture = hasNormal = 0;
//				if ((scannedItems =
//						 fscanf(file, "%u/%u/%u", &index, &textureIndex, &normalIndex)) == 3) {
//					hasTexture = hasNormal = 1;
//				} else {
//					fseek(file, -scannedItems, SEEK_CUR);
//					if ((scannedItems = fscanf(file, "%u//%u", &index, &normalIndex)) == 2) {
//						hasNormal = 1;
//					} else {
//						fseek(file, -scannedItems, SEEK_CUR);
//						if ((scannedItems = fscanf(file, "%u/%u", &index, &textureIndex)) == 2) {
//							hasTexture = 1;
//						} else {
//							fseek(file, -scannedItems, SEEK_CUR);
//							scannedItems = fscanf(file, "%u", &index);
//						}
//					}
//				}

//				if (scannedItems != 1 && scannedItems != 2 && scannedItems != 3) {
//					std::cout << "Error reading face indices\n";
//					fclose(file);
//					return false;
//				}

//				polygonIndices.push_back(index - 1);
//				if (hasTexture)
//					textureIndices.push_back(textureIndex - 1);
//				if (hasNormal)
//					normalIndices.push_back(normalIndex - 1);
//			} while ((scannedItems = fscanf(file, "%c", &lineEndCheck)) != EOF &&
//					 lineEndCheck != '\n');

//			// Triangulate the polygon if it has more than 3 vertices
//			for (unsigned int i = 1; i < polygonIndices.size() - 1; ++i) {
//				vertex_indices.push_back(polygonIndices[0]);
//				vertex_indices.push_back(polygonIndices[i]);
//				vertex_indices.push_back(polygonIndices[i + 1]);
//				if (hasTexture) {
//					texcoord_indices.push_back(textureIndices[0]);
//					texcoord_indices.push_back(textureIndices[i]);
//					texcoord_indices.push_back(textureIndices[i + 1]);
//				}
//				if (hasNormal) {
//					normal_indices.push_back(normalIndices[0]);
//					normal_indices.push_back(normalIndices[i]);
//					normal_indices.push_back(normalIndices[i + 1]);
//				}
//			}
//		}
//	}

//	fclose(file);

//	for (unsigned int i = 0; i < vertex_indices.size(); ++i) {
//		out_vertices.push_back(temp_vertices[vertex_indices[i]]);
//		if (out_texcoords != nullptr)
//			out_texcoords->push_back(hasTexture ? temp_texcoords[texcoord_indices[i]]
//												: glm::vec2(0.f));
//		if (out_normals != nullptr)
//			out_normals->push_back(hasNormal ? temp_normals[normal_indices[i]] : glm::vec3(0.f));
//	}

//	return true;
//}
