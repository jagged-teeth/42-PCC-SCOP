#pragma once

#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

// bool loadObj(const char *filepath, std::vector<glm::vec3> &out_vertices,
//			 std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals) {
//	std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
//	std::vector<glm::vec3> temp_vertices;
//	std::vector<glm::vec2> temp_uvs;
//	std::vector<glm::vec3> temp_normals;

//	FILE *file = fopen(filepath, "r");

//	while (1) {
//		char lineHeader[128];
//		int res = fscanf(file, "%s", lineHeader);
//		if (res == EOF)
//			break;
//		if (strcmp(lineHeader, "v") == 0) {
//			glm::vec3 vertex;
//			if (fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z) != 3) {
//				std::cerr << "Error parsing 'v' line in OBJ file" << std::endl;
//				return false;
//			}
//			temp_vertices.push_back(vertex);
//		} else if (strcmp(lineHeader, "vt") == 0) {
//			glm::vec2 uv;
//			if (fscanf(file, "%f %f\n", &uv.x, &uv.y) != 2) {
//				std::cerr << "Error parsing 'vt' line in OBJ file" << std::endl;
//				return false;
//			}
//			temp_uvs.push_back(uv);
//		} else if (strcmp(lineHeader, "vn") == 0) {
//			glm::vec3 normal;
//			if (fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z) != 3) {
//				std::cerr << "Error parsing 'vn' line in OBJ file" << std::endl;
//				return false;
//			}
//			temp_normals.push_back(normal);
//		} else if (strcmp(lineHeader, "f") == 0) {
//			std::string vertex1, vertex2, vertex3;
//			unsigned int vertex_index[3], uv_index[3], normal_index[3];
//			int matches =
//				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &uv_index[0],
//					   &normal_index[0], &vertex_index[1], &uv_index[1], &normal_index[1],
//					   &vertex_index[2], &uv_index[2], &normal_index[2]);
//			if (matches != 9) {
//				std::cout << "File can't be read by our simple parser : ( Try exporting with "
//							 "other options\n";
//				fclose(file);
//				return false;
//			}
//			vertex_indices.push_back(vertex_index[0]);
//			vertex_indices.push_back(vertex_index[1]);
//			vertex_indices.push_back(vertex_index[2]);
//			uv_indices.push_back(uv_index[0]);
//			uv_indices.push_back(uv_index[1]);
//			uv_indices.push_back(uv_index[2]);
//			normal_indices.push_back(normal_index[0]);
//			normal_indices.push_back(normal_index[1]);
//			normal_indices.push_back(normal_index[2]);
//		}
//	}

//	for (unsigned int i = 0; i < vertex_indices.size(); i++) {
//		out_vertices.push_back(temp_vertices[vertex_indices[i] - 1]);
//		out_uvs.push_back(temp_uvs[uv_indices[i] - 1]);
//		out_normals.push_back(temp_normals[normal_indices[i] - 1]);
//	}
//	fclose(file);
//	return true;
//}

bool loadObj(const char *filepath, std::vector<glm::vec3> &out_vertices) {
	std::vector<unsigned int> vertex_indices;
	std::vector<glm::vec3> temp_vertices;

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
			glm::vec3 vertex;
			if (fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z) != 3) {
				std::cout << "Error while reading vertices data\n";
				return false;
			}
			temp_vertices.push_back(vertex);
		} else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertex_index[3];
			if (fscanf(file, "%d %d %d\n", &vertex_index[0], &vertex_index[1], &vertex_index[2]) !=
				3) {
				std::cout << "Error while reading face indices\n";
				fclose(file);
				return false;
			}
			vertex_indices.push_back(vertex_index[0]);
			vertex_indices.push_back(vertex_index[1]);
			vertex_indices.push_back(vertex_index[2]);
		}
	}

	for (unsigned int i = 0; i < vertex_indices.size(); i++) {
		out_vertices.push_back(temp_vertices[vertex_indices[i] - 1]);
	}
	fclose(file);
	return true;
}
