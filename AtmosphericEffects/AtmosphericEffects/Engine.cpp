#include "Engine.hpp"

#include <iostream>
#include <iomanip>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/string_cast.hpp>

#include "Utils.hpp"

Engine::Engine()
	: width(DefaultWidth), height(DefaultHeight), running(false), wireframe(false), time(1.0f), fullscreen(false), displayFps(false), aerial(false), vsync(true)
{
}

Engine::~Engine()
{

}

int Engine::execute()
{
	if (!onInit())
		return -1;

	float pTime;
	float aTime;

	float fps;
	while (running)
	{
		pTime = clock.getElapsedTime().asSeconds();

		onUpdate();
		onDraw();

		aTime = clock.getElapsedTime().asSeconds();

		fps = FrameCounter::getFps(aTime - pTime);
		if(displayFps)
			std::cout << "fps: " << fps << std::endl;
	}

	onExit();

	return 0;
}

bool Engine::onInit()
{
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 1;
	// settings.attributeFlags = sf::ContextSettings::Core;

	window.create(sf::VideoMode(DefaultWidth, DefaultHeight), WindowTitle, sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);

	camera.create(vec3(0.0f, 2.0f, 0.0f));

	view = camera.getView();
	projection = glm::perspective(glm::radians(75.f), (float)width / (float)height, 0.1f, 1000.0f);

	clock.restart();

	atmosphere.create(200.f);

	running = true;
	return true;
}

void Engine::onUpdate()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		camera.onEvent(event);

		if (event.type == sf::Event::Closed)
		{
			running = false;
		}
		else if (event.type == sf::Event::Resized)
		{
			width = event.size.width;
			height = event.size.height;

			projection = glm::perspective(glm::radians(75.f), (float)width / (float)height, 0.1f, 1000.0f);

			glViewport(0, 0, event.size.width, event.size.height);
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::V:
				vsync = !vsync;
				
				window.setVerticalSyncEnabled(vsync);
				std::cout << "Vsync " << (vsync ? "on" : "off") << std::endl;

				break;
			case sf::Keyboard::Escape:
				running = false;
				break;
			case sf::Keyboard::Space:
				if (event.key.control)
					toggleFullScreen();
				break;
			case sf::Keyboard::Add:
				time += 0.2f;
				std::cout << "Time factor: " << std::fixed << std::setprecision(2) << time << std::endl;
				break;
			case sf::Keyboard::Subtract:
				time -= 0.2f;
				std::cout << "Time factor: " << std::fixed << std::setprecision(2) << time << std::endl;
				break;
			case sf::Keyboard::R:
				atmosphere.loadAttributes("atts.json");
				break;
			case sf::Keyboard::F:
				displayFps = !displayFps;
				break;
			case sf::Keyboard::C:
				toggleAerial();
				break;
			case sf::Keyboard::M:
				toggleWireframe();
			default:
				break;
			}
		}
	}

	camera.onUpdate();
	atmosphere.onUpdate(time * 0.15f);

	if(aerial)
		camera.setPosition(atmosphere.getSunPosition());

	view = camera.getView();
}

void Engine::onDraw()
{
	float timeValue = clock.getElapsedTime().asSeconds();
	GLfloat time = cos(timeValue) * 0.3 + 0.2;

	// std::cout << time << std::endl;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	atmosphere.draw(view, projection);

	window.display();
}

void Engine::onExit()
{
	window.close();
}

void Engine::toggleWireframe()
{
	if (!wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	wireframe = !wireframe;
}

void Engine::toggleAerial()
{
	aerial = !aerial;

	if(!aerial)
	{ 
		camera.create(vec3(0.0f, 2.0f, 0.0f));
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void Engine::toggleFullScreen()
{
	fullscreen = !fullscreen;

	sf::VideoMode videoMode;

	window.close();

	if (fullscreen)
	{
		auto modes = sf::VideoMode::getFullscreenModes();

		if (modes.empty())
		{ 
			std::cerr << "Fullscreen not supported" << std::endl;
			fullscreen = false;
		}
		
		videoMode = modes[0];
	}
	else
	{
		videoMode = sf::VideoMode(DefaultWidth, DefaultHeight);
	}

	width = videoMode.width;
	height = videoMode.height;

	window.create(videoMode, WindowTitle, fullscreen ? sf::Style::Fullscreen : sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(75.f), (float)width / (float)height, 0.1f, 1000.0f);

	clock.restart();
	atmosphere.create(200.f);
}