#include"room.h"
#include"shader.h"

void draw(triangle TriangularFace,glm::mat4 Cameramatrix);
int main(void)
{
	model room;
	room.loadObj("object/room3.obj","object/room3.mtl");
	
	
	modelMatrix = glm::translate(modelMatrix, glm::vec3(5, 5, 5));

	// Initialize GLFW
	if (!glfwInit())
	{
		return -1;
	}


	//InitializeGraph();

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "Room"
	GLFWwindow* window = glfwCreateWindow(width, height, "Room", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	glViewport(0, 0, width, height); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glOrtho(0, width, 0, height, -1, 1); // essentially set coordinate system
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glLoadIdentity(); // same as above comment

	glfwSetWindowPos(window,100,100);

	while (!glfwWindowShouldClose(window))
	{
		//cleargraph();
		clear();
		//line(100,100,200,200,glm::vec3(1,0,0));
		glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute

	   // Render OpenGL here
		glEnable(GL_POINT_SMOOTH); // make the point circular

		camera.Inputs(window);
		glm::mat4 cameraMatrix = camera.Matrix(45.0f, 0.1f, 100.0f);

		for (int i = 0; i < room.TriangleData.size(); i++)
		{
			triangle TriangularFace = room.TriangleData.at(i);
			draw(TriangularFace,cameraMatrix);
		}
		glDisable(GL_SMOOTH);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}




	return 0;
}

glm::vec4 manage(glm::vec4 vertex)
{
	vertex.x = ((vertex.x / vertex.w) + 1) * width / 2 - width / 2;

	vertex.y = ((vertex.y / vertex.w) + 1) * height / 2 - height / 2;

	return vertex;
}

void draw(triangle TriangularFace,glm::mat4 CameraMatrix)
{

	glm::vec4 points[3];
	float intensity[3];
	for (int i = 0; i < 3; i++)
	{
		glm::vec4 v = glm:: vec4(TriangularFace.TriangleVertices[i].position, 1.0f);
		v =  CameraMatrix * modelMatrix * v;
		
		points[i] = manage(v);
		glm::vec3 lightDir = glm::normalize(glm::vec3(points[i])-lightPos);
		//intensity[i] = glm::dot(TriangularFace.TriangleVertices[i].normal,lightDir);
		intensity[i] = glm::max(glm::dot(TriangularFace.TriangleVertices[i].normal, lightDir), 0.0f);

	}
		
	PhongShading(points, TriangularFace);
	//gouraudShading(points, intensity, TriangularFace.mtl.kd);
	//drawTriangle(points, TriangularFace.mtl.kd);
}