#include "../headers/mob_loader.h"

namespace mob_loader
{
	// MOB_Loader constructor
	MOB_Loader::MOB_Loader()
	{

	}

	// Include a MOB path
	void MOB_Loader::include_mob(std::string path)
	{

	}

	// Load a MOB from a path
	Object* MOB_Loader::load_from_mob(std::string path, Scene* scene)
	{
		Game* game = (Game*)scene->get_game_struct();

		// Check if the path exists
		if (!basix::file_exists(path))
		{
			game->error("MOB Loader", "The path \"" + path + "\" does not exists.");
			return 0;
		}

		// Get the signature of the path
		char* signature_input = new char[8];
		std::vector<unsigned int> signature_size = std::vector<unsigned int>();
		for (int i = 0; i < 8; i++) signature_size.push_back(1);
		basix::read_file_binary(path, signature_input, signature_size);
		std::string signature = ""; for (int i = 0; i < 8; i++) signature += signature_input[i];
		delete[] signature_input; signature_input = 0;

		// Check the signature of the file
		if (signature_input != "MATIX3DO")
		{
			game->error("MOB Loader", "The path \"" + path + "\" is not a MOB file (its signature is \"" + signature + "\" but the \"MATIX3DO\" signature is wanted).");
			return 0;
		}

		// Get the size of the path
		char* size_chunk_input = new char[1];
		std::vector<unsigned int> size_chunk_size = std::vector<unsigned int>(); size_chunk_size.push_back(4);
		basix::read_file_binary(path, size_chunk_input, size_chunk_size, 8);
		unsigned int size_chunk = (unsigned int)size_chunk_input[0];
		delete[] size_chunk_input; size_chunk_input = 0;

		// Analyze each chunks
		unsigned int current_pos = 12;
		for (int i = 0; i < size_chunk; i++)
		{
			// Read the type of the chunk
			char* type_chunk_input = new char[1];
			std::vector<unsigned int> type_chunk_size = std::vector<unsigned int>(); type_chunk_size.push_back(1);
			basix::read_file_binary(path, type_chunk_input, type_chunk_size, current_pos);
			unsigned int type_chunk = (unsigned int)type_chunk_input[0];
			delete[] type_chunk_input; type_chunk_input = 0;
			current_pos++;

			if (type_chunk == 0) // If the chunk is a include chunk
			{
				// Read the number of include path
				char* include_size_input = new char[1];
				std::vector<unsigned int> include_size_size = std::vector<unsigned int>(); include_size_size.push_back(1);
				basix::read_file_binary(path, include_size_input, include_size_size, current_pos);
				unsigned char include_size = (unsigned char)include_size_input[0];
				delete[] include_size_input; include_size_input = 0;
				current_pos++;

				// Read each path
				for (int i = 0; i < include_size; i++)
				{
					// Read the size of the include path
					char* path_size_input = new char[1];
					std::vector<unsigned int> path_size_size = std::vector<unsigned int>(); path_size_size.push_back(1);
					basix::read_file_binary(path, path_size_input, path_size_size, current_pos);
					unsigned char path_length = (unsigned char)path_size_input[0];
					delete[] path_size_input; path_size_input = 0;
					current_pos++;

					// Read the path of the include
					char* path_input = new char[path_length];
					std::vector<unsigned int> path_size = std::vector<unsigned int>();
					for(int i = 0;i < path_length;i++) path_size.push_back(1);
					basix::read_file_binary(path, path_input, path_size, current_pos);
					std::string path_to_tinclude = ""; for (int i = 0; i < path_length; i++) path += path_input[i];
					delete[] path_input; path_input = 0;
					current_pos += path_length;

					// Include the MOB
					include_mob(path_to_tinclude);
				}
			}
			else if (type_chunk == 1) // If the chunk is a texture
			{

			}
		}
	}

	// MOB_Loader destructor
	MOB_Loader::~MOB_Loader()
	{

	}
}