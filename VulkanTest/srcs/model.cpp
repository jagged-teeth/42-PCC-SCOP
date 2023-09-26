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
