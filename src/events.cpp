#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "events.h"
#include <iostream>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	Controls	*con;

	con = static_cast<Controls *>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			con->keys[key] = true;
		else if (action == GLFW_RELEASE)
			con->keys[key] = false;
	}
	scancode = 0;
	mods = 0;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	Controls	*con;
	const float	sensitivity = 0.1f;
	float		xoffset;
	float		yoffset;

	con = static_cast<Controls *>(glfwGetWindowUserPointer(window));
	if (con->first_m)
	{
		con->last_x = xpos;
		con->last_y = ypos;
		con->first_m = false;
	}
	xoffset = xpos - con->last_x;
	yoffset = con->last_y - ypos;
	con->last_x = xpos;
	con->last_y = ypos;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	con->yaw += xoffset;
	con->pitch += yoffset;
	if (con->pitch > 89.0f)
		con->pitch = 89.0f;
	if (con->pitch < -89.0f)
		con->pitch = -89.0f;
}