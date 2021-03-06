//#include "imgui_impl_glut.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.h"
#define yAxis glm::vec3(0,1,0)
#define xAxis glm::vec3(1,0,0)
#define zAxis glm::vec3(0,0,1)
class Controller
{
public:
	Controller(Camera* camera, int w, int h);
	~Controller();
	float step = 0.15f;
	float hor = 0, ver = 0, gravity = 0;
	glm::vec3 right() { return glm::cross(cam->dir, cam->up); }
	void Move();
	Camera* cam;
	void update();

	void keyboard(unsigned char key, int x, int y);
	void keyboard_up(unsigned char key, int x, int y);

private:
	float yaw = 0.f, pitch = 0.f;
	int win_w, win_h;
	float mouseX, mouseY;
	bool useCursor = false;
};

