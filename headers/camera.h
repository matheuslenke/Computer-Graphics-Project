#ifndef CAMERA_H
#define	CAMERA_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include "utils.h"

class Camera
{
private:
	vec3 position;
	vec3 direction;
	vec3 left_vec;
	vec3 up_vec;
	vec3 velocity;
	float m_scl= 0.1;

public:
	Camera(vec3 pos) {
        position = vec3(pos);
        direction = vec3(0, 0, -1);
        left_vec = vec3(-1, 0, 0);
        up_vec = vec3(0, 1, 0);
        velocity = vec3(0, 0, 0);
    }
	~Camera();
	void Activate();
    void ChangePosition(vec3 pos);

	void Forward();
	void Back();
	void Left(GLdouble timeDiff);
	void Right(GLdouble timeDiff);
};

#endif  // CAMERA_H