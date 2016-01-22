#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Types.hpp"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

const GLfloat Yaw = 0.0f;
const GLfloat Pitch = 0.0f;
const GLfloat Speed = 3.0f;
const GLfloat MoveSensitivity = 0.05f;
const GLfloat RotateSensitivity = 0.01f;
const GLfloat ScrollSensitivity = 0.1f;

class Camera
{
public:
	Camera(vec3 pos = vec3(0.0f, 1.0f, 0.0f), vec3 dir = vec3(0.f, 0.f, 1.0f), GLfloat y = Yaw, GLfloat p = Pitch)
		:
		speed(Speed),
		moveSensitivity(MoveSensitivity),
		scrollSensitivity(ScrollSensitivity),
		rotateSensitivity(RotateSensitivity),
		zoom(1.0f),
		position(pos),
		front(dir),
		yaw(y),
		pitch(p)
	{
		calculate();
		toggled = false;
	}

	void create(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec3 dir = vec3(0.f, 0.f, 1.0f), float y = Yaw, float p = Pitch);
	void onEvent(const sf::Event& event);
	void onUpdate();

	void setPosition(vec3 pos) { position = pos; }

	mat4 getView();

private:
	void calculate();

	void onMouseMove(int xoffset, int yoffset);
	void onMouseWheel(float delta);

	int lastX, lastY;

	vec3 front;
	vec3 position;
	vec3 up;
	vec3 right;

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat speed;

	GLfloat moveSensitivity;
	GLfloat rotateSensitivity;
	GLfloat scrollSensitivity;

	GLfloat zoom;

	bool toggled;
};

#endif