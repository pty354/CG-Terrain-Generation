#include "Controller.h"

Controller::Controller(Camera* camera, int w, int h)
{
	cam = camera;
	win_w = w;
	win_h = h;
}

Controller::~Controller()
{
}

void Controller::update() {
	if (!useCursor)
		glutSetCursor(GLUT_CURSOR_NONE);
	Move();
}
void Controller::Move()
{
	cam->pos += (hor * right() + ver * cam->dir + gravity * cam->up) * step;
}

void Controller::keyboard(unsigned char key, int x, int y)
{
	
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		ver = 1.0f;
		break;
	case 's':
		ver = -1.0f;
		break;
	case 'a':
		hor = -1.0f;
		break;
	case 'd':
		hor = 1.0f;
		break;
	case 'f':
		gravity = 1.0f;
		break;
	case 'v':
		gravity = -1.0f;
		break;
	default:
		break;
	}
}

void Controller::keyboard_up(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		ver = 0.0f;
		break;
	case 's':
		ver = 0.0f;
		break;
	case 'a':
		hor = 0.0f;
		break;
	case 'd':
		hor = 0.0f;
		break;
	case 'f':
		gravity = 0.0f;
		break;
	case 'v':
		gravity = 0.0f;
		break;
	default:
		break;
	}

}

