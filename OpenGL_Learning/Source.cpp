#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "cameraFly.h"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

//camera
CameraFly cameraFly(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool firstMouse = true;

//a function to change the viewport size every time a window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//this is called on every mouse move
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//default position
	if (firstMouse) //true initially
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//calculating mouse offset movement since last frame
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; //reversed since y-coordinates range from bottom to top

	lastX = xpos;
	lastY = ypos;
	
	cameraFly.ProcessMouseMovement(xoffset, yoffset);
}

//a function to process user input
void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraFly.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraFly.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraFly.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraFly.ProcessKeyboard(RIGHT, deltaTime);
}

int main()
{
	//initialize and configure GLFW
	//------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//this line is needed for Mac OS X
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//creating a window object
	//-------------------------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "Main window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create Main window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //hide cursor and capture it

	//initializing GLAD
	//--------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//registering callback functions
	//----------------------------------------------------------------
	//viewport resize function to activate on every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//mouse movement capture
	glfwSetCursorPosCallback(window, mouse_callback);

	// build and compile our shader program
	// --------------------------------------------------------------------
	Shader shaderProgram("Shaders/3.3.shader.vs", "Shaders/3.3.shader.fs");
		
	//setting up vertex data
	//----------------------------------------
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
/*
	//Indices to reduce the number of used vertices
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
*/
	//initialize Vertex Array Object, Vertex Buffer Object and bind them
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
/*
	//Element Buffer Object to use indices
	unsigned int ebo;
	glGenBuffers(1, &ebo);
*/
	glBindVertexArray(vao);
	//copy array of vertices in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

/*
	//copy our index array in a buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
*/
	/*
	* Configuring vertex position attribute pointers:
	* first parameter - which attribute to configure (location in shader)
	* second - size of a vertex attribute
	* third - a type of vertex data specified by GL_
	* fourth - normalisation on/off
	* fifth - stride between consecutive vertex attributes
	* sixth - data offset
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //unbind vao when we are done configuring
/*
	//Configure vertex colour pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
*/
	//configure texture pointers
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//generating a texture
	//------------------------------------------------------------------------------------------
	unsigned int texture01, texture02;
	unsigned char* data;
	int width, height, nrChannels;
	//container texture
	glGenTextures(1, &texture01);
	glBindTexture(GL_TEXTURE_2D, texture01);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//hashtag flipit
	stbi_set_flip_vertically_on_load(true);
	//load and generate the texture
	data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture01" << std::endl;
	}
	stbi_image_free(data);
	//smileyface texture
	glGenTextures(1, &texture02);
	glBindTexture(GL_TEXTURE_2D, texture02);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture02" << std::endl;
	}
	stbi_image_free(data);

	shaderProgram.use(); // don't forget to activate the shader before setting uniforms!  
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture01"), 0); // set it manually
	shaderProgram.setInt("texture02", 1); // or with shader class

	//making a separate VAO for the light object
	//---------------------------------------------------------------------------
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//render loop
	//------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input processing
		//--------------------
		processInput(window);

		//rendering options
		//-----------------------------------
		glClearColor(0.2f, 0.3f, 0.4f, 0.5f); //set a picked colour
		glEnable(GL_DEPTH_TEST); //draw only pixels that are not obstructed
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //use our picked colour to clear the colour buffer

		//drawing a shader object
		//-----------------------

		//binding textures with corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture01);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture02);
		shaderProgram.use();

		//transformation matrices for camera effect
		//pass projection matrix to shader (in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(cameraFly.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		shaderProgram.setMat4("projection", projection);
		//camera-view transformation
		glm::mat4 view = cameraFly.GetViewMatrix();
		shaderProgram.setMat4("view", view);

		//seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glBindVertexArray(vao);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
		for (unsigned int i = 0; i < 10; i++)
		{
			//calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); //identity
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
//			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			shaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//buffer swap and I/O events
		//--------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//cleaning up
	//-----------------------------
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &ebo);

	glfwTerminate();

	return 0;
}
