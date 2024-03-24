#include "../headers/mob_loader.h"

namespace mob_loader
{
	// MOB_Loader constructor
	MOB_Loader::MOB_Loader()
	{

	}

	// Returns if the MOB contains a data
	bool MOB_Loader::contains_data(unsigned short data_number)
	{
		for (std::map<unsigned short, Data_Chunk_Data>::iterator it = a_datas.begin(); it != a_datas.end(); it++)
		{
			if (it->first == data_number) return true;
		}
		return false;
	}

	// Delete each data
	void MOB_Loader::delete_datas()
	{
		for (int i = 0; i < a_datas.size(); i++)
		{
			delete[] a_datas[i].data;
		}
		a_datas.clear();
	}

	// Include a MOB path
	void MOB_Loader::include_mob(std::string path, unsigned char include_type)
	{
		// Create the include data
		Include_Data data; data.type = include_type; data.path = path;
		a_included_data.push_back(data);

		std::cout << "Include " << path << " from type " << (int)include_type << std::endl;
	}

	// Load a MOB from a path
	Object* MOB_Loader::load_from_mob(std::string path, Scene* scene)
	{
		Game* game = (Game*)scene->get_game_struct();

		// Check if the path exists
		if (!scls::file_exists(path))
		{
			game->error("MOB Loader", "The path \"" + path + "\" does not exists.");
			return 0;
		}

		// Get the signature of the path
		char* signature_input = new char[8];
		scls::read_file_binary(path, signature_input, 8);
		std::string signature = ""; for (int i = 0; i < 8; i++)
		{
			signature += signature_input[i];
		}
		delete[] signature_input; signature_input = 0;

		// Check the signature of the file
		if (signature != "MATIX3DO")
		{
			game->error("MOB Loader", "The path \"" + path + "\" is not a MOB file (its signature is \"" + signature + "\" but the \"MATIX3DO\" signature is wanted).");
			return 0;
		}

		// Get the size of the path
		char* size_chunk_input = new char[2];
		scls::read_file_binary(path, size_chunk_input, 2, 8);
		unsigned int size_chunk = 0;
		size_chunk = scls::extract_2bytes_from_char_array(size_chunk_input, 0, true);
		delete[] size_chunk_input; size_chunk_input = 0;

		// Analyze each chunks
		unsigned int current_pos = 10;
		for (int i = 0; i < size_chunk; i++)
		{
			// Read the type of the chunk
			char* type_chunk_input = new char[1];
			scls::read_file_binary(path, type_chunk_input, 1, current_pos);
			unsigned char type_chunk = (unsigned char)type_chunk_input[0];
			delete[] type_chunk_input; type_chunk_input = 0;
			current_pos++;

			if (type_chunk == get_include_number()) // If the chunk is a include chunk
			{
				// Read the number of include path
				char* include_size_input = new char[1];
				scls::read_file_binary(path, include_size_input, 1, current_pos);
				unsigned char include_size = (unsigned char)include_size_input[0];
				delete[] include_size_input; include_size_input = 0;
				current_pos++;

				// Read each path
				for (int i = 0; i < include_size; i++)
				{
					// Read the type of the include
					char* type_input = new char[1];
					scls::read_file_binary(path, type_input, 1, current_pos);
					unsigned char type = (unsigned char)type_input[0];
					delete[] type_input; type_input = 0;
					current_pos++;

					// Read the size of the include path
					char* path_size_input = new char[1];
					scls::read_file_binary(path, path_size_input, 1, current_pos);
					unsigned char path_length = (unsigned char)path_size_input[0];
					delete[] path_size_input; path_size_input = 0;
					current_pos++;

					// Read the path of the include
					char* path_input = new char[path_length];
					std::vector<unsigned int> path_size = std::vector<unsigned int>();
					scls::read_file_binary(path, path_input, path_length, current_pos);
					std::string path_to_include = ""; for (int i = 0; i < path_length; i++) path_to_include += path_input[i];
					delete[] path_input; path_input = 0;
					current_pos += path_length;

					// Include the MOB
					include_mob(path_to_include, type);
				}
			}
			else if (type_chunk == get_data_number()) // If the chunk is a data container
			{
				// Read the number of datas
				char* include_size_input = new char[2];
				scls::read_file_binary(path, include_size_input, 2, current_pos);
				unsigned short include_size = scls::extract_2bytes_from_char_array(include_size_input, 0, true);
				delete[] include_size_input; include_size_input = 0;
				current_pos += 2;

				// Read each datas
				for (int i = 0; i < include_size; i++)
				{
					// Read the number of the data
					char* data_number_input = new char[2];
					scls::read_file_binary(path, data_number_input, 2, current_pos);
					unsigned short data_number = scls::extract_2bytes_from_char_array(data_number_input, 0, true);
					delete[] data_number_input; data_number_input = 0;
					current_pos += 2;

					// Read the size of the data
					char* data_size_input = new char[1];
					scls::read_file_binary(path, data_size_input, 1, current_pos);
					unsigned char data_length = (unsigned char)data_size_input[0];
					delete[] data_size_input; data_size_input = 0;
					current_pos++;

					// Read the path of the include
					char* data_input = new char[data_length];
					scls::read_file_binary(path, data_input, data_length, current_pos);
					current_pos += data_length;

					// Set the data
					set_data(data_number, data_length, data_input);
				}
			}
			else
			{
				std::map<unsigned short, Data_Chunk_Data> datas = std::map<unsigned short, Data_Chunk_Data>();
				datas[get_resize_texture_data_number()] = Data_Chunk_Data();

				// Read the number of datas
				char* include_size_input = new char[2];
				scls::read_file_binary(path, include_size_input, 2, current_pos);
				unsigned short include_size = scls::extract_2bytes_from_char_array(include_size_input, 0, true);
				delete[] include_size_input; include_size_input = 0;
				current_pos += 2;

				// Read each datas
				for (int i = 0; i < include_size; i++)
				{
					// Read the number of the data
					char* data_number_input = new char[2];
					scls::read_file_binary(path, data_number_input, 2, current_pos);
					unsigned short data_number = scls::extract_2bytes_from_char_array(data_number_input, 0, true);
					delete[] data_number_input; data_number_input = 0;
					current_pos += 2;

					// Read the size of the data
					char* data_size_input = new char[1];
					scls::read_file_binary(path, data_size_input, 1, current_pos);
					unsigned char data_length = (unsigned char)data_size_input[0];
					delete[] data_size_input; data_size_input = 0;
					current_pos++;

					// Read the path of the include
					char* data_input = new char[data_length];
					scls::read_file_binary(path, data_input, data_length, current_pos);
					current_pos += data_length;

					// Set the data
					Data_Chunk_Data data; data.data_number = data_number; data.data_size = data_length; data.data = data_input;
					datas[data_number] = data;
				}

				// Read size of the name of the object
				char* name_size_input = new char[1];
				scls::read_file_binary(path, name_size_input, 1, current_pos);
				unsigned char name_size = name_size_input[0];
				delete[] name_size_input; name_size_input = 0;
				current_pos++;

				// Get the name of the object
				char* name_input = new char[name_size];
				scls::read_file_binary(path, name_input, name_size, current_pos);
				std::string name = ""; for (int i = 0; i < name_size; i++) name += name_input[i];
				delete[] name_input; name_input = 0;
				current_pos += name_size;

				// Get the type of the object
				char* type_input = new char[1];
				scls::read_file_binary(path, type_input, 1, current_pos);
				unsigned char type = type_input[0];
				delete[] type_input; type_input = 0;
				current_pos++;

				// Get the size of the object
				char* size_input = new char[4];
				scls::read_file_binary(path, size_input, 4, current_pos);
				unsigned int size = scls::extract_4bytes_from_char_array(size_input, 0, true);
				delete[] size_input; size_input = 0;
				current_pos += 4;

				if (type_chunk == get_texture_number()) // Load a texture
				{
					if (type == get_png_texture_number()) // Load the texture as PNG datas
					{
						// Get the data of the texture
						char* texture_input = new char[size];
						scls::read_file_binary(path, texture_input, size, current_pos);
						bool flip_y = false;
						bool resize = false;
						if (datas[get_resize_texture_data_number()].data != 0)
						{
							resize = datas[get_resize_texture_data_number()].data[0];
						}
						if (datas[get_flip_y_texture_data_number()].data != 0)
						{
							flip_y = datas[get_flip_y_texture_data_number()].data[0];
						}
						Texture* texture = game->new_texture(name);
						texture->get_image()->_load_from_binary_PNG(texture_input, size);
						if (flip_y) texture->get_image()->flip_y();
						texture->load_texture();
						delete[] texture_input; texture_input = 0;
					}
				}
				else if (type_chunk == get_vbo_number()) // Load a VBO
				{
					if (type == 0)
					{
						// Get the data of the VBO
						char* vbo_input = new char[size];
						scls::read_file_binary(path, vbo_input, size, current_pos);
						VBO* vbo = game->new_vbo(name);
						vbo->load_from_binary(vbo_input);
						delete[] vbo_input; vbo_input = 0;
					}
				}
				else if (type_chunk == get_vao_number()) // Load a VAO
				{
					if (type == 0)
					{
						// Get the data of the VAO
						char* vao_input = new char[size];
						scls::read_file_binary(path, vao_input, size, current_pos);
						VAO* vao = game->new_vao(name, vao_input);
						delete[] vao_input; vao_input = 0;
					}
				}
				current_pos += size;

				for (std::map<unsigned short, Data_Chunk_Data>::iterator it = datas.begin(); it != datas.end(); it++)
				{
					if (it->second.data != 0) delete[] it->second.data;
				}
			}
		}
		return 0;
	}

	// MOB_Loader destructor
	MOB_Loader::~MOB_Loader()
	{
		delete_datas();
	}
}
