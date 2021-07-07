#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include "DiamondSquareAlg.h"
#include "Voronoi.h"
#include <glm/gtc/random.hpp>
#define RESTART 99999

using std::vector;
using namespace glm;

const int iterationTimes = 10;
const float width = 64.f;
const float initialSpread = 32.0f;
const int randomPointsNum = 20;


class Terrain {
public:
	vector<glm::vec3> v;
	int N;
	float hMax = -999.f;
	float hMin = 999.f;
	Terrain(float ratio);
	~Terrain();

	void init();
	void update(float ratio);
private:
	DiamondSquareAlg* diamondSquareAlg;
	Voronoi* vor;
};


GLuint create_terrain_vao(vector<vec3>* v, int N);
void DrawTerrain(GLuint vao, int N);



bool comp(const vec3& a, const vec3& b);


