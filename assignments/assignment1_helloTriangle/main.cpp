#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float verticies[] = {
	//   X      Y     Z     R     G     B     A
	   -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color;
void main()
{
   Color = aColor;
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
};
)";
//ABOVE CAN USE R"()"; to avoid using \n and format as a multiline string by default

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec4 Color;
uniform vec4 updateColor;
void main()
{
    FragColor = Color;
	//FragColor = updateColor;
} 
)";

int main() {
	
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialization goes here!
	//VERTEX DATA!!!
	//vertex array object
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Create Vertex Buffer Object
	unsigned int VBO;  	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STREAM_DRAW); 	//Put Data Into Curretly Bound VBO
	//newer approach ^^^
	//glNamedBufferData(VBO, sizeof(verticies), verticies, GL_STATIC_DRAW);
	
	//position (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color (RGBA)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512]; //enough space to put output (to dump info log)
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	} else {
		printf("\nSHADER::VERTEX::COMPILATION_SUCCESS");
	}
	//Create and compile fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	} else {
		printf("\nSHADER::FRAGMENT::COMPILATION_SUCCESS\n");
	}

	//create and link program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR:SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}
	else {
		printf("SHADER::PROGRAM::LINKING_SUCCESS\n");
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Render loop
	while (!glfwWindowShouldClose(window)) {
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//activating shader
		glUseProgram(shaderProgram);

		//update things
		float time = glfwGetTime();

		//color
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "updateColor");
		//glUniform4f(vertexColorLocation, 1.0f*cos(time), 0.0f, 0.0f, 1.0f);
		//basically a variable inside the preset strings you can change in an outside loop

		//render things
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	printf("Shutting down...");
}
