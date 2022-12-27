#include "viewer.h"

class camera {

	float viewport_width;
	float viewport_height;
	glm::vec3 vup;
	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 dir;
	glm::vec3 look_at;

public:

	// neccesary default constructor
	camera() {}

	camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect_ratio)
	{
		this->vup = vup;
		this->look_at = lookat;
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / CAMERA_VIEWPORT);
		viewport_height = CAMERA_VIEWPORT * h;
		viewport_width = aspect_ratio * viewport_height;
		dir = lookfrom - lookat;
		auto w = normalize(dir);
		auto u = normalize(cross(vup, w));
		auto v = cross(w, u);

		origin = lookfrom;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
	}

	void resize_view(float aspect_ratio)
	{
		dir = origin - look_at;
		auto w = normalize(dir);
		auto u = normalize(cross(vup, w));
		auto v = cross(w, u);
		viewport_width = aspect_ratio * viewport_height;

		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
	}

	glm::mat4 move_look(float dx, float dy)
	{
		return lookat(look_at + glm::vec3(dx, dy, 0));
	}

	void move_origin(glm::vec3 shift)
	{
		glm::vec3 viewDir = look_at - origin;
		origin = origin + shift;
		auto view = lookat(origin + viewDir);
		dir = origin - look_at;
	}

	glm::mat4 lookat(glm::vec3 pt)
	{
		auto w = normalize(origin - pt);
		auto u = normalize(cross(vup, w));
		auto v = cross(w, u);

		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
		this->look_at = pt;

		return glm::lookAt(origin, look_at, vup);
	}

	glm::vec3 getLookDir()
	{
		glm::vec3 direction = origin - look_at;
		return direction / glm::length(direction);
	}
};