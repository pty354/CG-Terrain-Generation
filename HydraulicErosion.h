#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "Mathfunc.h"
#include <omp.h>

using std::vector;
using namespace glm;

const float Ke = 0.01f;//evaporation rate
const float Ks = 0.1f;//dissolving rate
const float Kc = 0.01f;
const float dt = 0.5f; //time-step to control whole process
const float density = 1.0f;  //increase this will make particle move faster

const float minVol = 0.01f;
const float friction = 0.05f;

void HydraulicErosion(vector<vec3>* v, int N, int times);
