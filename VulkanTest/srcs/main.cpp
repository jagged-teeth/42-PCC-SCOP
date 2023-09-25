#include "scop.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <model> <texture>" << std::endl;
		return EXIT_FAILURE;
	}

	const std::string MODEL_PATH = "models/" + std::string(argv[1]);
	const std::string TEXTURE_PATH = "textures/" + std::string(argv[2]);

	try {
		Scop app(MODEL_PATH.c_str(), TEXTURE_PATH.c_str());
		app.run();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
