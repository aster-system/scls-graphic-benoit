/*
*
* This file represents the "MOB_Loader" mod.
* It adds a the ability to handle MOB.
* A MOB is an object stored in a file.
* It can handle texturing, VBOs and transformations.
*
*/

#pragma once

#include "../../headers/game.h"

namespace mob_loader
{
	// Returns the number of each chunks
	inline char get_data_number() { return 1; };
	inline char get_include_number() { return 0; };
	inline char get_texture_number() { return 2; };
	inline char get_vao_number() { return 4; };
	inline char get_vbo_number() { return 3; };
	// Returns the number of each datas into the data chunk
	inline unsigned short get_author_data_number() { return 1; };
	inline unsigned short get_version_data_number() { return 0; };
	// Returns the number of each texture file type
	inline unsigned char get_path_texture_number() { return 0; };
	inline unsigned char get_png_texture_number() { return 1; };
	// Returns the number of each sub data chunks
	inline unsigned char get_resize_texture_data_number() { return 0; };

	// Returns the signature of a mob file
	inline std::string get_mob_signature() { return "MATIX3DO"; };

	// Returns a char array of a vector of Shader_Attributes in the MOB format
	inline char* shader_program_variable_to_char_array(std::vector<Shader_Program_Variable> variables, unsigned int &size)
	{
		unsigned int current_pos = 0;
		char* result = new char[variables.size() * 4 + 1];
		result[0] = static_cast<char>(variables.size()); current_pos++;

		for (int i = 0; i < variables.size(); i++)
		{
			basix::put_2bytes_to_char_array(variables[i].type, result, current_pos, true); current_pos += 2;
			basix::put_2bytes_to_char_array(variables[i].vector_size, result, current_pos, true); current_pos += 2;
		}

		size = variables.size() * 4 + 1;

		return result;
	};

	struct Data_Chunk_Data {
		// Datas about a data into the data chunk in a MOB.
		unsigned short data_number = 0;
		unsigned short data_size = 0;
		char* data = 0;
	};

	struct Include_Data {
		// Datas about an included file in a MOB.
		std::string path = "";
		unsigned char type = 0;
	};

	struct Texture_Chunk {
		// Datas about a texture chunk.
		std::string name = "";
		unsigned char type = 0;
	};

	struct VAO_Chunk {
		// Datas about a VAO chunk.
		std::string name = "";
		std::string shader_name = "";
		unsigned char type = 0;
		std::string vbo_name = "";
	};

	struct VBO_Chunk {
		// Datas about a VBO chunk.
		unsigned int datas_size = 0;
		std::string name = "";
		unsigned char type = 0;
	};

	class MOB_Loader
	{
		/*
		* Class representing all the necessary stuff to load a MOB.
		*/
	public:
		// MOB_Loader constructor
		MOB_Loader();
		// MOB_Loader destructor
		~MOB_Loader();

		// Include a MOB path
		void include_mob(std::string path, unsigned char include_type = 0);
		// Load a MOB from a path
		Object* load_from_mob(std::string path, Scene* scene);

		// Returns if the MOB contains a data
		bool contains_data(unsigned short data_number);
		// Delete each data
		void delete_datas();

		// Getters and setters
		inline std::string get_author()
		{
			char* author_array = get_data(get_author_data_number()).data;
			unsigned int author_size = get_data(get_author_data_number()).data_size;

			if (author_array == 0) return "";

			std::string to_return = "";
			for (int i = 0; i < author_size; i++) to_return += author_array[i];
			return to_return;
		};
		inline std::vector<Include_Data> get_included_data() { return a_included_data; };
		inline Data_Chunk_Data get_data(unsigned int data)
		{
			Data_Chunk_Data to_return;
			if (contains_data(data))
			{
				to_return = a_datas[data];
			}
			return to_return;
		};
		inline std::map<unsigned short, Data_Chunk_Data>* get_datas() { return &a_datas; };
		inline int get_mbo_version()
		{
			char* version_array = get_data(get_version_data_number()).data;

			if (version_array == 0) return -1;

			return basix::extract_2bytes_from_char_array(version_array);
		};
		inline void set_data(unsigned short data_number, unsigned char size, char* data)
		{
			if (!contains_data(data_number))
			{
				Data_Chunk_Data current_data; current_data.data_number = data_number; current_data.data_size = size; current_data.data = data;
				a_datas[data_number] = current_data;
			}
			else
			{
				delete[] a_datas[data_number].data;

				a_datas[data_number].data = data;
				a_datas[data_number].data_number = data_number;
				a_datas[data_number].data_size = size;
			}
		};
	private:
		// Datas into the data chunk into this MOB
		std::map<unsigned short, Data_Chunk_Data> a_datas = std::map<unsigned short, Data_Chunk_Data>();
		// Included path into this MOB
		std::vector<Include_Data> a_included_data = std::vector<Include_Data>();
	};
}