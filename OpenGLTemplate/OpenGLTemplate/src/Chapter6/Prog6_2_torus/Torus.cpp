#include <cmath>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Torus.h"
Torus::Torus()
{
	prec = 48;
	inner = 0.5f;
	outer = 0.2f;
	init();
}

float Torus::toRadians(float degrees)
{
	return (degrees * 2.0f * 3.14159f) / 360.0f;
}


Torus::Torus(float in, float out, int precIn)
{
	prec = precIn;
	inner = in;
	outer = out;
	init();
}


void Torus::init()
{
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) {
		vertices.push_back(glm::vec3());
	}

	for (int i = 0; i < numVertices; i++) {
		texCoords.push_back(glm::vec2());
	}
	for (int i = 0; i < numVertices; i++) {
		normals.push_back(glm::vec3());
	}
	for (int i = 0; i < numVertices; i++) {
		sTangents.push_back(glm::vec3());
	}
	for (int i = 0; i < numVertices; i++) {
		tTangents.push_back(glm::vec3());
	}
	for (int i = 0; i < numIndices; i++) {
		indices.push_back(0);
	}

	// 计算第一个环
	for (int i = 0; i < prec + 1; i++) {
		float amt = toRadians(i * 360.0f / prec);

		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 initPos = rMat * glm::vec4(outer, 0.0f, 0.0f, 1.0f);

		vertices[i] = initPos + glm::vec3(inner, 0.0f, 0.0f);
		texCoords[i] = glm::vec2(0.0f, ((float)i) / float(prec));

		rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
		sTangents[i] = glm::vec3(0.0f, 0.0f, -1.0f);
		normals[i] = glm::cross(tTangents[i], sTangents[i]);

	}
}

int Torus::getNumVertices()
{
	return 0;
}

int Torus::getNumIndices()
{
	return 0;
}

std::vector<int> Torus::getIndices()
{
	return std::vector<int>();
}

std::vector<glm::vec3> Torus::getVertices()
{
	return std::vector<glm::vec3>();
}

std::vector<glm::vec2> Torus::getTexCoords()
{
	return std::vector<glm::vec2>();
}

std::vector<glm::vec3> Torus::getNormals()
{
	return std::vector<glm::vec3>();
}

std::vector<glm::vec3> Torus::getStangents()
{
	return std::vector<glm::vec3>();
}

std::vector<glm::vec3> Torus::getTtangents()
{
	return std::vector<glm::vec3>();
}
