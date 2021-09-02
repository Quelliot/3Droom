#include"Camera.h"



Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

glm::mat4 Camera::lookAt(const glm::vec3 eye,const glm::vec3 center, const glm::vec3 up)
{
	glm::mat4 Matrix = glm::mat4(1.0f);
	glm::vec3 x, y, z;

	z = glm::normalize(eye - center);
	y = up;
	x = glm::normalize(glm::cross(y,z));
	y = glm::normalize(glm::cross(z,x));
	
	Matrix[0][0] = x.x;
	Matrix[1][0] = x.y;
	Matrix[2][0] = x.z;
	Matrix[3][0] = glm::dot(-x,eye);
	Matrix[0][1] = y.x;
	Matrix[1][1] = y.y;
	Matrix[2][1] = y.z;
	Matrix[3][1] = glm::dot(-y,eye);;
	Matrix[0][2] = z.x;
	Matrix[1][2] = z.y;
	Matrix[2][2] = z.z;
	Matrix[3][2] = glm::dot(-z,eye);
	Matrix[0][3] = 0;
	Matrix[1][3] = 0;
	Matrix[2][3] = 0;
	Matrix[3][3] = 1.0f;

	return Matrix;
	

}

glm::mat4 Camera:: perspective(float fovy, float aspect, float zNear, float zFar)
{
	glm::mat4 Matrix = glm::mat4(0.0f);
	if (aspect != 0 && zNear != zFar)
	{
		const float rad = fovy;
		const float tanHalfRad = tan(rad/2);

		Matrix[0][0] = 1 / (aspect * tanHalfRad);
		Matrix[1][1] = 1 / tanHalfRad;
		Matrix[2][2] = -(zFar + zNear) / (zFar - zNear);
		Matrix[2][3] = -1;
		Matrix[3][2] = -(2 * zFar * zNear) / (zFar - zNear);
		
		return Matrix;
	}
}

glm::mat4 Camera::Matrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position

	//view = glm::lookAt(Position, Position + orientation, up);
	view = lookAt(Position, Position + orientation, up);

	// Adds perspective to the scene
	//projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	projection = perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
	return cameraMatrix;
}





void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * orientation;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(orientation, up));
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -orientation;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(orientation, up));
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * up;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -up;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}
	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the orientation
		glm::vec3 neworientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		// Decides whether or not the next vertical orientation is legal or not
		if (abs(glm::angle(neworientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			orientation = neworientation;
		}

		// Rotates the orientation left and right
		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}

