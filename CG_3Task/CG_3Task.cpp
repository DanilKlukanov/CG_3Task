#pragma once
//c++
#include <iostream>
//glew
#include <GL/glew.h>
//sfml
#include <SFML/Window.hpp>
//my_math
#include "math/math4.h"
#include "math/vec3.h"
//shaders
#include "sup/shaderLoader.h"
#include "sup/stb_image.h"
//class_camera
#include "Class_camera.h"
#include "shader.h"
//class_texture
#include "Texture.h"
//shader
#include "Shader.h"
//vao vbo
#include "Vertex_array.h"
#include "Vertex_buffer.h"
//model
#include "model/Model.h"
//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int height = 800;
int width = 600;

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::Window window(sf::VideoMode(height, width, 24), "TaskThree",
		sf::Style::Titlebar | sf::Style::Close, settings);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Error:: glew not init =(" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	sf::Mouse::setPosition(sf::Vector2i(height / 2, width / 2), window);
	Class_camera camera;

	Shader models("shaders/models.vs", "shaders/models.fs");
	Shader skybox("shaders/skybox.vs", "shaders/skybox.fs");

	float vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	Vertex_array skyboxVAO;
	skyboxVAO.Bind();
	Vertex_buffer skyboxVBO(vertices, sizeof(vertices) / sizeof(float));
	skyboxVAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::string path = "textures/skybox/";
	std::vector<std::string> faces {
		path + "right.png",
		path + "left.png",
		path + "top.png",
		path + "bottom.png",
		path + "front.png",
		path + "back.png"
	};

	Texture skyboxTexture(faces);

	skybox.Use();
	skybox.SetInt("skybox", 0);

	Model objects[] = {
		Model("obj/planet/planet.obj"),
		Model("obj/bomb/Bomb.obj"),
		Model("obj/rock/rock.obj")
	};

	vec3 pos_model[] = {
		vec3(1.5f,  2.0f, -2.5f),
		vec3(2.5f,  0.2f, 0.5f),
		vec3(-1.3f,  0.2f, -1.5f)
	};

	bool isGo = true;
	while (true) {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGo = false;
				break;
			case sf::Event::KeyPressed:
				camera.KeyboardInput();
				break;
			case sf::Event::MouseMoved:
				camera.MouseInput(window);
				break;
			default:
				break;
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		math4 view(camera.GetViewMatrix());
		math4 projection(camera.GetProjectionMatrix());

		models.Use();
		models.SetMat4("projection", projection);
		models.SetMat4("view", view);

		math4 model_one(1.0f);
		model_one = model_one.Translate(pos_model[0]);
		model_one = model_one.Scale(vec3(0.4f));
		models.SetMat4("model", model_one);
		objects[0].Draw(models);

		math4 model_two(1.0f);
		model_two = model_two.Translate(pos_model[1]);
		model_two = model_two.Scale(vec3(2.5f));
		models.SetMat4("model", model_two);
		objects[1].Draw(models);

		math4 model_three(1.0f);
		model_three = model_three.Translate(pos_model[2]);
		model_three = model_three.Scale(vec3(0.7f));
		models.SetMat4("model", model_three);
		objects[2].Draw(models);

		glDepthFunc(GL_LEQUAL);
		skybox.Use();
		view = camera.GetRightView();
		skybox.SetMat4("view", view);
		skybox.SetMat4("projection", projection);

		skyboxVAO.Bind();
		skyboxTexture.Bind(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		skyboxVAO.Bind(0);
		glDepthFunc(GL_LESS);

		window.display();
	}

	skyboxVAO.Delete();
	skyboxVBO.Delete();

	window.close();
	return 0;

}