#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Types.hpp"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

const GLfloat Yaw = 0.0f;
const GLfloat Pitch = 0.0f;
const GLfloat Speed = 3.0f;
const GLfloat MoveSensitivity = 0.01f;
const GLfloat ScrollSensitivity = 0.1f;

class Camera
{
public:
	Camera(vec3 t = vec3(0.0f, 0.0f, 0.0f), float r = 3.0f, GLfloat y = Yaw, GLfloat p = Pitch)
		:
		speed(Speed),
		moveSensitivity(MoveSensitivity),
		scrollSensitivity(ScrollSensitivity),
		zoom(1.0f),
		target(t),
		radius(r),
		yaw(y),
		pitch(p)
	{
		calculate();
		toggled = false;
	}

	void create(vec3 t = vec3(0.0f, 0.0f, 0.0f), float r = 3.0f, vec3 center = vec3(0.0f, 0.0f, 0.0f), float y = Yaw, float p = Pitch);
	void onEvent(const sf::Event& event);

	mat4 getView();

private:
	void calculate();

	void onMouseMove(int xoffset, int yoffset);
	void onMouseWheel(float delta);

	int lastX, lastY;

	vec3 center;
	vec3 target;
	vec3 position;
	vec3 up;

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	float radius;

	// Camera options
	GLfloat speed;

	GLfloat moveSensitivity;
	GLfloat scrollSensitivity;

	GLfloat zoom;

	bool toggled;
};

#endif