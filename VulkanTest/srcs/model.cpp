// #include "scop.hpp"
// #define TINYOBJLOADER_IMPLEMENTATION
// #include "tiny_obj_loader.h"
// #include <unordered_map>

// void Scop::loadModel() {
//	tinyobj::attrib_t attrib;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//	std::string warn, err;

//	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH)) {
//		throw std::runtime_error(warn + err);
//	}

//	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

//	// Calculate center of mass
//	glm::vec3 sum(0.0f, 0.0f, 0.0f);
//	int vertexCount = attrib.vertices.size() / 3;

//	for (int i = 0; i < vertexCount; i++) {
//		sum.x += attrib.vertices[3 * i + 0];
//		sum.y += attrib.vertices[3 * i + 1];
//		sum.z += attrib.vertices[3 * i + 2];
//	}

//	glm::vec3 center = sum / (float)vertexCount;

//	// Substract center of mass from vertices
//	for (int i = 0; i < vertexCount; i++) {
//		attrib.vertices[3 * i + 0] -= center.x;
//		attrib.vertices[3 * i + 1] -= center.y;
//		attrib.vertices[3 * i + 2] -= center.z;
//	}

//	// Calculate scale
//	for (const auto &shape : shapes) {
//		for (const auto &index : shape.mesh.indices) {
//			Vertex vertex{};

//			vertex.pos = {attrib.vertices[3 * index.vertex_index + 0],
//						  attrib.vertices[3 * index.vertex_index + 1],
//						  attrib.vertices[3 * index.vertex_index + 2]};

//			vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
//							   1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

//			vertex.color = {1.0f, 1.0f, 1.0f};

//			computeUVs(vertex);
//			if (uniqueVertices.count(vertex) == 0) {
//				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
//				vertices.push_back(vertex);
//			}
//			indices.push_back(uniqueVertices[vertex]);
//		}
//	}
//}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

#include "objloader.hpp"
#include "scop.hpp"
#include <unordered_map>

void Scop::loadModel() {
	std::vector<glm::vec3> temp_vertices;

	if (!loadObj(MODEL_PATH, temp_vertices)) {
		throw std::runtime_error("failed to load model!");
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	// Calculate center of mass
	glm::vec3 sum(0.0f, 0.0f, 0.0f);
	int vertexCount = temp_vertices.size();

	for (const auto &vertex : temp_vertices) {
		sum += vertex;
	}

	glm::vec3 center = sum / (float)vertexCount;

	for (auto &vertex : temp_vertices) {
		vertex -= center;
	}

	// Load vertices
	for (size_t i = 0; i < temp_vertices.size(); i++) {
		Vertex vertex{};

		vertex.pos = temp_vertices[i];
		vertex.color = {1.0f, 1.0f, 1.0f};

		computeUVs(vertex);
		if (uniqueVertices.count(vertex) == 0) {
			uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
			vertices.push_back(vertex);
		}
		indices.push_back(uniqueVertices[vertex]);
	}
}

void Scop::computeUVs(Vertex &vertex) {
	float theta = atan2(vertex.pos.z, vertex.pos.x) / (2 * M_PI);
	float phi = acos(vertex.pos.y / length(vertex.pos)) / M_PI;

	vertex.texCoord.s = theta + 0.5f;
	vertex.texCoord.t = phi;
}
