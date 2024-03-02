#include "../headers/base_struct.h"

// To avoid a bug with std::codecvt_utf8<char>, go in the project setting > C/C++ > General > SDL Check and disable it

// Return the forward position according to a rotation
glm::vec3 calculate_forward(glm::vec3 rotation)
{
	float x = cos(glm::radians(rotation[1])); // Use trigonometry to calculate forward direction
	float z = sin(glm::radians(rotation[1]));

	return glm::vec3(x, 0, z);
}

// Return the x angle of a vector (without euler angle)
float get_vector_x_angle(glm::vec3 vector)
{
	float x = vector[0];
	float y = vector[1];
	float z = vector[2];
	float xy_hypothenus = glm::distance(glm::vec2(x, y), glm::vec2(0, 0));

	if (xy_hypothenus == 0) return 0;

	float angle = glm::asin(glm::abs(y) / xy_hypothenus);
	float final_angle = angle;

	if (x > 0)
	{
		if (y < 0)
		{
			final_angle = 3.1415 * 2.0 - angle;
		}
	}
	else
	{
		if (y < 0)
		{
			final_angle = 3.1415 + angle;
		}
		else
		{
			final_angle = 3.1415 - angle;
		}
	}

	return final_angle;
}

// Return the x angle of a vector (with euler angle)
float get_vector_x_angle(glm::vec3 vector, glm::vec3 forward, float rotation_y, unsigned int id)
{
	float x = vector[0] * forward[0];
	float y = vector[1];
	float z = vector[2] * forward[2];
	float xyz_hypothenus = glm::distance(glm::vec3(x, y, z), glm::vec3(0, 0, 0));

	if (xyz_hypothenus == 0)
	{
		if (rotation_y < 180)
		{
			return 0;
		}
		else
		{
			return 3.1415;
		}
	}

	float angle = glm::asin(glm::abs(y) / xyz_hypothenus);
	float final_angle = angle;

	if (rotation_y <= 90 || rotation_y > 270)
	{
		if (y < 0)
		{
			final_angle = 3.1415 * 2.0 - angle;
		}
	}
	else
	{
		if (y < 0)
		{
			final_angle = 3.1415 + angle;
		}
		else
		{
			final_angle = 3.1415 - angle;
		}
	}

	return final_angle;
}

// Return the y angle of a vector (with adjacent = X and opposition = Y)
float get_vector_y_angle(glm::vec3 vector, unsigned int id)
{
	float opposite = vector[2];
	float adjacent = vector[0];
	float hypothenus = glm::distance(glm::vec2(opposite, adjacent), glm::vec2(0, 0));

	if (hypothenus == 0)
	{
		return 0;
	}

	float angle = glm::acos(glm::abs(adjacent) / hypothenus);
	float final_angle = angle;

	if (adjacent < 0 && opposite < 0)
	{
		final_angle = 3.1415 + angle;
	}
	else if (opposite < 0 && adjacent >= 0)
	{
		final_angle = 3.1415 * 2.0 - angle;
	}
	else if (adjacent < 0)
	{
		final_angle = 3.1415 - angle;
	}
	else
	{
		final_angle = angle;
	}

	return final_angle;
}

// Normalize an angle and return it
float normalize_angle(float angle)
{
	while (angle < 0) angle += 360;
	while (angle >= 360) angle -= 360;
	return angle;
}

// Normalize a rotation and return it
glm::vec3 normalize_rotation(glm::vec3 rotation)
{
	while (rotation[0] >= 360)
	{
		rotation[0] -= 360;
	}
	while (rotation[0] < 0)
	{
		rotation[0] += 360;
	}

	while (rotation[1] >= 360)
	{
		rotation[1] -= 360;
	}
	while (rotation[1] < 0)
	{
		rotation[1] += 360;
	}

	while (rotation[2] >= 360)
	{
		rotation[2] -= 360;
	}
	while (rotation[2] < 0)
	{
		rotation[2] += 360;
	}

	return rotation;
}

// Rotate a vector on the x axis in the horary sense
glm::vec3 rotate_vector_x(glm::vec3 vector, glm::vec3 rotation, glm::vec3 forward, unsigned int id)
{
	glm::vec3 to_return = glm::vec3(vector[0] * forward[0], vector[1], vector[2] * forward[2]);
	const glm::vec3 TO_RETURN_START = to_return;

	// Calculate the angle of the position
	float hypothenus_xz = glm::distance(glm::vec2(to_return[0], to_return[2]), glm::vec2(0, 0));
	float hypothenus_xyz = glm::distance(to_return, glm::vec3(0, 0, 0));

	if (hypothenus_xyz == 0) return to_return;

	float angle_y = glm::degrees(get_vector_y_angle(to_return));
	float angle = get_vector_x_angle(to_return, forward, angle_y);

	// Calculate the position in the local circle
	float final_angle = angle + glm::radians(rotation[0]);
	float final_xz = glm::cos(final_angle) * hypothenus_xyz;

	to_return[0] = final_xz * forward[0];
	to_return[1] = glm::sin(final_angle) * hypothenus_xyz;
	to_return[2] = final_xz * forward[2];

	std::cout << "E " << final_xz << " " << hypothenus_xyz << " " << forward[0] << " " << forward[2] << " " << glm::degrees(final_angle) << std::endl;

	// Calculate the final position
	vector[0] += to_return[0] - TO_RETURN_START[0];
	vector[1] = to_return[1];
	vector[2] += to_return[2] - TO_RETURN_START[2];

	return vector;
}

// Rotate a vector on the y axis in the horary sense
glm::vec3 rotate_vector_y(glm::vec3 vector, float rotation, unsigned int id)
{
	glm::vec3 to_return = vector;

	// Calculate the angle in a local XZ circle with Y angle
	glm::vec2 difference_position = glm::vec2(vector[0], vector[2]);
	
	// Calculate the angle of the position
	float hypothenus = glm::distance(difference_position, glm::vec2(0, 0));

	float angle = get_vector_y_angle(vector);

	// Calculate the position in the local circle
	float final_angle = angle + glm::radians(rotation);
	glm::vec2 final_position = glm::vec2(glm::cos(final_angle) * hypothenus, glm::sin(final_angle) * hypothenus);

	// Calculate the final position
	to_return[0] = final_position[0];
	to_return[2] = final_position[1]; //*/

	return to_return;
}

// Rotate a vector around a rotating point
glm::vec3 rotate_vector(glm::vec3 vector, glm::vec3 rotation, glm::vec3 position, glm::vec3 rotation_multiplier, bool protection, unsigned int id)
{
	rotation = normalize_rotation(rotation);

	vector -= position;
	vector = rotate_vector_y(vector, rotation[1]);

	glm::vec3 forward = calculate_forward(rotation);
	forward = glm::vec3(1, 0, 0);

	glm::vec3 to_return = rotate_vector_x(vector, rotation, forward);

	// Calculate the angle of the position
	/*float x = vector[0];
	float y = vector[1];
	float z = vector[2];
	float xz_hypothenus = glm::distance(glm::vec3(x, 0, z), glm::vec3(0, 0, 0));
	float xyz_hypothenus = glm::distance(glm::vec3(x, y, z), glm::vec3(0, 0, 0));

	float y_angle = get_vector_y_angle(vector);

	glm::vec3 forward = calculate_forward(rotation);

	float x_angle = get_vector_x_angle(to_return, rotation[1]) + glm::radians(rotation[0]);
	if(x < 0) x_angle = get_vector_x_angle(to_return, rotation[1]) - glm::radians(rotation[0]);

	// Calculate the X rotation
	to_return[0] = glm::cos(y_angle) * glm::cos(x_angle) * xyz_hypothenus;
	to_return[1] = glm::sin(x_angle) * xyz_hypothenus;
	to_return[2] = glm::sin(y_angle) * glm::cos(x_angle) * xyz_hypothenus;//*/

	if (id == 180) std::cout << "M " << vector[0] << " " << vector[1] << " " << vector[2] << " " << to_return[0] << " " << to_return[1] << " " << to_return[2] << std::endl;

	return to_return;
}

// Transform a string to an uppercase string
std::string to_uppercase(std::string str)
{
	std::string result = "";
	for (int i = 0; i < str.length(); i++)
	{
		char chr = str[i];
		int chr_ascii = chr;

		if(chr_ascii >= 97 && chr_ascii <= 122)
		{
			result += chr_ascii - 32;
		}
		else if (chr == ';')
		{
			result = ".";
		}
		else if (chr == ':')
		{
			result = "/";
		}
		else
		{
			result += chr;
		}
	}
	return result;
}

unsigned int Transform_Object::object_count = 0;

// Transform_Object contructor
Transform_Object::Transform_Object(Transform_Object *a_parent, glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale) : parent(0), position(a_position), rotation(a_rotation), scale(a_scale)
{
	set_parent(a_parent);
	calculate_direction();

	// Handle the id
	id = Transform_Object::object_count;
	Transform_Object::object_count++;
}

// Transform_Object copy constructor
Transform_Object::Transform_Object(const Transform_Object& copy): Transform_Object(copy.parent, copy.position, copy.rotation, copy.scale)
{

}

// Add an animation to the object
void Transform_Object::add_animation(float duration, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale)
{
	add_animation(duration, get_position(), get_rotation(), get_scale(), final_position, final_rotation, final_scale);
}

// Add an animation to the object with the base position
void Transform_Object::add_animation(float duration, glm::vec3 base_position, glm::vec3 base_rotation, glm::vec3 base_scale, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale)
{
	Transform_Animation animation;
	animation.base_position = base_position;
	animation.base_rotation = base_rotation;
	animation.base_scale = base_scale;
	animation.duration = duration;
	animation.final_position = final_position;
	animation.final_rotation = final_rotation;
	animation.final_scale = final_scale;
	(*get_animations()).push_back(animation);
}

// Add an animation to the object with the position
void Transform_Object::add_position_animation(float duration, glm::vec3 base_position, glm::vec3 final_position)
{
	Transform_Animation animation;
	animation.base_position = base_position;
	animation.duration = duration;
	animation.final_position = final_position;
	animation.modify_rotation = false;
	animation.modify_rotation = false;
	animation.modify_scale = false;
	(*get_animations()).push_back(animation);
}

// Add an animation to the object with the  rotation
void Transform_Object::add_rotation_animation(float duration, glm::vec3 base_rotation, glm::vec3 final_rotation, bool change_plan_rotation)
{
	Transform_Animation animation;
	animation.base_rotation = base_rotation;
	animation.change_plan_rotation = change_plan_rotation;
	animation.duration = duration;
	animation.final_rotation = final_rotation;
	animation.modify_position = false;
	animation.modify_scale = false;
	(*get_animations()).push_back(animation);
}

// Apply to a child matrix the parent position model
glm::mat4 Transform_Object::apply_parent_position_model_matrix(glm::mat4 matrix)
{
	// Move the matrix
	glm::vec3 movement = get_absolute_position(this);
	matrix = glm::translate(matrix, movement);

	return matrix;
}

// Apply the parent position to a vector
glm::vec3 Transform_Object::get_absolute_position(Transform_Object* asker)
{
	// Apply parent position position
	glm::vec3 local_vector = glm::vec3(0, 0, 0);
	glm::vec3 vector = glm::vec3(0, 0, 0);
	if (get_parent() != 0)vector = get_parent()->get_absolute_position(asker);

	// Move the vector
	glm::vec3 position = get_position();
	// if (get_parent() != 0) position = rotate_vector(position, get_parent()->get_absolute_plan_rotation(), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, get_id());
	local_vector += position;

	// Apply anchor rotation
	/*glm::vec3 anchor = get_anchored_position();
	if (get_parent() != 0) anchor = rotate_vector(anchor, get_parent()->get_absolute_plan_rotation(), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, get_id());
	glm::vec3 rotated_vector = rotate_vector(anchor, get_plan_rotation(), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), asker == this ? true : false, get_id() + 100);
	glm::vec3 final_anchor = rotated_vector - anchor;
	local_vector += final_anchor; //*/

	return vector + local_vector;
}

// Apply to a child matrix the parent rotation model
glm::mat4 Transform_Object::apply_parent_rotation_model_matrix(glm::mat4 matrix, Transform_Object* child)
{
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 forward = glm::vec3(1, 0, 0);
	glm::vec3 right = glm::vec3(0, 0, 1);

	// Apply parent rotation model matrix
	if (get_parent() != 0)
	{
		matrix = get_parent()->apply_parent_rotation_model_matrix(matrix);
	}

	// Rotate matrix from the plan
	glm::vec3 rotation = get_plan_rotation(true);
	if (child == this)
	{
		matrix = glm::rotate(matrix, glm::radians(-rotation[1]), up);
	}
	else
	{
		matrix = glm::rotate(matrix, glm::radians(-rotation[1]), up);
	}
	matrix = glm::rotate(matrix, glm::radians(rotation[0]), forward) * get_parent_rotation_multiplier()[0];
	matrix = glm::rotate(matrix, glm::radians(rotation[2]), right);

	return matrix;
}

// Calculate the direction vector of the transform object
void Transform_Object::calculate_direction()
{
	glm::vec3 rotation = get_absolute_plan_rotation() * glm::vec3(1, -1, 1);
	float x = cos(glm::radians(rotation[1])) * cos(glm::radians(0.0f)); // Use trigonometry to calculate forward direction
	float y = sin(glm::radians(0.0f));
	float z = sin(glm::radians(rotation[1])) * cos(glm::radians(0.0f));

	float yaw_add = 90; // Use trigonometry to calculate up direction
	float x_r = cos(glm::radians(rotation[1])) * cos(glm::radians(0 + yaw_add));
	float y_r = sin(glm::radians(yaw_add));
	float z_r = sin(glm::radians(rotation[1])) * cos(glm::radians(0 + yaw_add));

	forward = glm::normalize(glm::vec3(x, y, z));
	up = glm::normalize(glm::vec3(x_r, y_r, z_r));
	right = glm::normalize(glm::cross(forward, up)); // Use vector calcul to calculate right and up direction

	for (int i = 0; i < get_children()->size(); i++)
	{
		(*get_children())[i]->calculate_direction();
	}
}

// Return the global up to apply rotation
glm::vec3 Transform_Object::get_global_rotation_modifier()
{
	if (get_parent() != 0)
	{
		glm::vec3 rotation = parent->get_rotation();
		float x = cos(glm::radians(rotation[1])) * (cos(glm::radians(rotation[0]))); // Use trigonometry to calculate forward direction
		float y = sin(glm::radians(rotation[0]));
		float z = sin(glm::radians(rotation[1])) * (cos(glm::radians(rotation[0])));

		glm::vec3 global_forward = glm::normalize(glm::vec3(x, y, z));
		glm::vec3 global_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward)); // Use vector calcul to calculate right and up direction
		glm::vec3 global_up = glm::normalize(glm::cross(forward, right));

		// return global_up;
	}
	return glm::vec3(1, 1, 1);
}

// Return the transformation matrix of the object
glm::mat4 Transform_Object::get_model_matrix()
{
	glm::mat4 matrix = glm::mat4(1.0f);

	// Move matrix
	matrix = apply_parent_position_model_matrix(matrix);

	// Rotate matrix from the plan
	glm::vec3 rotation = get_absolute_plan_rotation(true);
	matrix = glm::rotate(matrix, glm::radians(rotation[1]), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation[0]), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation[2]), glm::vec3(0, 0, 1));

	rotation = get_rotation();
	matrix = glm::rotate(matrix, glm::radians(rotation[1]), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation[0]), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation[2]), glm::vec3(0, 0, 1));

	// Scale matrix
	matrix = glm::scale(matrix, get_scale());

	return matrix;
}

// Move the object
void Transform_Object::move(glm::vec3 a_movement)
{
	movement = get_movement() + a_movement * get_position_move_multiplier();
}

// Remove an object from the children
void Transform_Object::remove_child(Transform_Object* object)
{
	std::vector<Transform_Object*>* children = get_children();
	for (std::vector<Transform_Object*>::iterator it = children->begin(); it != children->end(); it++)
	{
		if ((*it) == object) { children->erase(it); break; } // Verify each child
	}
}

// Scale the object
void Transform_Object::rescale(glm::vec3 a_scale)
{
	set_scale(glm::vec3(get_scale()[0] * a_scale[0], get_scale()[1] * a_scale[1], get_scale()[2] * a_scale[2]));
}

// Reset animation in the object
void Transform_Object::reset_animation(bool reset_position)
{
	get_animations()->clear();
	if (reset_position)
	{
		set_position_animation(glm::vec3(0, 0, 0));
	}
}

// Reset softly the object
void Transform_Object::soft_reset()
{
	set_position(get_position() + get_movement());
	set_movement(glm::vec3(0.0, 0.0, 0.0));
}

// Update the animation for this transform object
void Transform_Object::update_animation()
{
	if (is_animation_playing() && get_animations()->size() > 0)
	{
		Transform_Animation* animation = get_current_animation();

		if (animation->modify_position) set_position_animation(get_current_animation_position());
		if (animation->modify_rotation)
		{
			if (get_current_animation()->change_plan_rotation)
			{
				set_plan_rotation(get_current_animation_rotation());
			}
			else
			{
				set_rotation(get_current_animation_rotation());
			}
		}
		if (animation->modify_scale) set_scale(get_current_animation_scale());

		// Calculate new transform
		float animation_purcentate = animation->state / animation->duration;
		if (animation_purcentate >= 1)
		{
			get_animations()->pop_back();
		}
	}
}

// Transform_Object destructor
Transform_Object::~Transform_Object()
{

}

// Camera constructor
Camera::Camera(glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale): Transform_Object(0, a_position, a_rotation, a_scale)
{

}

// Return the projection matrix
glm::mat4 Camera::get_projection(int window_height, int window_width)
{
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(get_fov()), float(window_width) / float(window_height), 1.0f / get_far(), get_far());

	return projection;
}

// Return the view matrix
glm::mat4 Camera::get_view()
{
	return glm::lookAt(get_absolute_position(), get_looked_position(), get_up());
}

// Camera destructor
Camera::~Camera()
{

}

// Base_Struct constructor
Base_Struct::Base_Struct(double& a_mouse_x, double& a_mouse_y, std::string a_exec_path): exec_path(a_exec_path), mouse_x(a_mouse_x), mouse_y(a_mouse_y), last_mouse_x(a_mouse_x), last_mouse_y(a_mouse_y)
{
	
}

// Cout an error in the program
void Base_Struct::error(std::string thrower, std::string error_content)
{
	basix::print("Error", thrower, error_content);
}

// Return if a file formatted with the struct context
std::string Base_Struct::file_formatted(std::string path)
{
	std::string access = basix::cut_string(path, "/")[0];
	std::string current_path = (std::filesystem::current_path().string());
	std::vector<std::string> cutted_path = basix::cut_string(current_path, "\\");
	unsigned short point_count = 0;
	unsigned short size_to_delete = 0;

	for (int i = 1; i < path.size(); i++) // Count the . in the path
	{
		if (path[i] == '.')
		{
			point_count++;
			size_to_delete += cutted_path[cutted_path.size() - (i + 1)].size() + 1;
		}
		else
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				point_count += 1;
			}
			break;
		}
	}

	path = current_path.substr(0, current_path.size() - size_to_delete) + "/" + path.substr(point_count, path.size() - point_count);

	return path;
}

// Return the projection matrix
glm::mat4 Base_Struct::get_projection()
{
	return camera.get_projection(get_window_height(), get_window_width());
}

// Base_Struct destructor
Base_Struct::~Base_Struct()
{

}