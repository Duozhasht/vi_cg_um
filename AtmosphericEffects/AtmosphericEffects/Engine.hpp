#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Window.hpp>
#include "Types.hpp"

#include "Program.hpp"
#include "Camera.hpp"

#include "Model.hpp"
#include "Atmosphere.hpp"

#include "Slider.hpp"

class Engine
{
public:
	Engine();
	~Engine();

	int execute();

private:
	bool onInit();
	void onUpdate();
	void onDraw();
	void onExit();

	void toggleWireframe();
	void toggleAerial();

	sf::Clock clock;

	sf::Window window;
	int width, height;

	bool running;
	bool wireframe;

	Program shaders;
	Program atmosphereShaders;

	GLuint VAO, VBO, EBO;
	GLuint texture;

	mat4 model;
	mat4 view;
	mat4 projection;

	GLint modelLoc;
	GLint viewLoc;
	GLint projectionLoc;

	Camera camera;
	Model floor;
	Model sphere;

	Atmosphere atmosphere;
	Slider attribute[Atmosphere::TotalAttributes];

	float time;
	bool displayFps;
	bool aerial;
};

#endif