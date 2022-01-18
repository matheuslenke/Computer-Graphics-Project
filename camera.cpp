#include "headers/camera.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <iostream>

Camera::~Camera()
{
}

void Camera::Activate()
{
	vec3 look = position + direction;

	gluLookAt(position.x, position.y, position.z, look.x, look.y, look.z, up_vec.x, up_vec.y, up_vec.z);
    position = vec3(0,0,0);
}

void Camera::ChangePosition(vec3 pos) {
    position = pos;
}

void Camera::Forward(GLdouble timeDiff)
{
	velocity = up_vec * m_scl * timeDiff;
	position = position + velocity;
}

void Camera::Back(GLdouble timeDiff)
{
	velocity = up_vec * (-m_scl) * timeDiff;
	position = position + velocity;
}

void Camera::Left(GLdouble timeDiff)
{
	velocity = left_vec * m_scl * timeDiff;
	position = position + velocity;
    // std::cout << "Position: " << position.x << " | " << position.y << " | " << position.z << std::endl;
}

void Camera::Right(GLdouble timeDiff)
{
	velocity = left_vec * (-m_scl) * timeDiff;
	position = position + velocity;
}



