#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

enum CameraMovement
{
	CAM_FORWARD,
	CAM_BACKWARD,
	CAM_LEFT,
	CAM_RIGHT
};

class Camera
{
public:
	Camera(glm::vec3 pos);
	~Camera();

	glm::mat4 getViewMatrix() { return glm::lookAt(cameraPos, cameraPos + front, up); }

	void keyboardMovement(CameraMovement type, float deltaTime);
	void mouseMovement(float xOffset, float yOffset);
	void scrollMovement(float yOffset);
	float getFOV() { return fov; }

private:
	void updateCameraVectors();

	glm::vec3 cameraPos; 
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;

	float yaw;
	float pitch;
	float fov;
	  
	float speed = 2.0f;
	const float sensitivity = 1.0f; 
};

Camera::Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f))
{  
	cameraPos = pos;
	yaw = -90.0f;
	pitch = 0.0f; 
	fov = 45.0f;

	updateCameraVectors();
	 
}



Camera::~Camera()
{
}

void Camera::keyboardMovement(CameraMovement type, float deltaTime)
{ 

	float velocity = speed * deltaTime; 
	if (type == CAM_FORWARD)
	{ 
		this->cameraPos += velocity * front;
	}
	if (type == CAM_BACKWARD)
	{ 
		this->cameraPos -= velocity * front;
	}
	if (type == CAM_LEFT)
	{ 
		this->cameraPos -= velocity * right;
	}
	if (type == CAM_RIGHT)
	{ 
		this->cameraPos += velocity * right;
	}

	updateCameraVectors();
}


void Camera::mouseMovement(float xOffset, float yOffset)
{

	// Mouse event handling
	/*
	1. Calculate the mouse's offset since the last frame.
	2. Add the offset values to the camera's yaw and pitch values.
	3. Add some constraints to the minimum/maximum pitch values.
	4. Calculate the direction vector.
	*/ 
	// 1. 

	xOffset *= sensitivity;
	yOffset *= sensitivity;

	// 2.
	yaw += xOffset;
	pitch += yOffset;

	// 3.
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCameraVectors();
}

void Camera::scrollMovement(float yOffset)
{ 
	fov -= (float)yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void Camera::updateCameraVectors()
{

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, front));
}