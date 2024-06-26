#include "workbench.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "config.hpp"
#include "time.hpp"

static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,
		-1.0f,
		0.0f,
		1.0f,
		-1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		-1.0f,
		-1.0f,
		0.0f,
		-1.0f,
		1.0f,
		0.0f,
};

GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const *VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const *FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLCanvas::GLCanvas(size_t width, size_t height)
	: Canvas(width, height)
{
}

void GLCanvas::init()
{
	glGenTextures(1, &m_glTexture);
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Config::WIDTH, Config::HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	unbind();
}

void GLCanvas::bind() {
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
}

void GLCanvas::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLCanvas::update() {
	bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Config::WIDTH, Config::HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, m_buf.data());
	unbind();
}

Workbench::Workbench(Scene* scene)
	: m_canvas(Config::WIDTH, Config::HEIGHT)
{
	m_scene = scene;
	if (glfwInit() == GLFW_FALSE)
	{
		exit(1);
	}
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(1920, 1080, "ray-tracer", nullptr, nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		exit(2);
	}
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwMakeContextCurrent(m_window);


	gladLoadGL();
	glViewport(0, 0, 1920, 1080);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// This will identify our vertex buffer
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &m_vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	m_programID = LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
	glUseProgram(m_programID);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(m_window, &m_currX, &m_currY);
	m_prevX = m_currX;
	m_prevY = m_currY;
	m_targetDirection = scene->camera.direction;
	m_targetOrigin = scene->camera.origin;

	m_camOutFile.open("camera.txt");
	if (!m_camOutFile) {
		std::cerr << "Failed to open camera.txt file for recording" << std::endl;
	}

	m_canvas.init();
}

Workbench::~Workbench()
{
	glfwTerminate();
}

void Workbench::set_texture(GLuint texID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(m_programID, "tex"), 0);
}

void Workbench::refresh()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glVertexAttribPointer(
			0,				// attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,				// size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,				// stride
			(void *)0 // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Workbench::poll_events()
{
	glfwPollEvents();
}

bool Workbench::should_continue()
{
	return !glfwWindowShouldClose(m_window) && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void Workbench::copyGBufferIntoCanvas() {
	for (int i = 0; i < Config::HEIGHT; i++) {
		for (int j = 0; j < Config::WIDTH; j++) {
			vec3 col;
			col[0] = m_gbuffer.getPixel(i, j).r;
			col[1] = m_gbuffer.getPixel(i, j).g;
			col[2] = m_gbuffer.getPixel(i, j).b;
			m_canvas.setPixel(j, i, col);
		}
	}
}

void Workbench::recordCameraPosition() {
	Camera c = m_scene->camera;

	m_camOutFile << c.origin.x() << " " << c.origin.y() << " " << c.origin.z() << " ";
	m_camOutFile << c.direction.x() << " " << c.direction.y() << " " << c.direction.z() << std::endl;
}

void Workbench::finish()
{
	while(should_continue())
	{
		double dt = 1/60.0;
		handleMouseInput();
		handleKeyboardInput(dt);
		updateCinematicCamera(dt);
		render_gbuffer(m_scene, &m_gbuffer);
		if ((m_frame % Config::SAVE_INTERVAL) == 0) {
			recordCameraPosition();
			m_gbuffer.save("gbuffer/frame_" + std::to_string(m_frame) + ".npy");
		}
		m_frame++;
		copyGBufferIntoCanvas();
		m_canvas.update();
		set_texture(m_canvas.m_glTexture);
		refresh();
	}
	m_camOutFile.close();
}

void Workbench::handleKeyboardInput(double dt) {
	double speed = 8.0;
	vec3 forward(m_scene->camera.direction.x(), m_scene->camera.direction.y(), 0.0);
	vec3 right = cross(forward, vec3(0, 0, 1));
	forward = forward.normalize();
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		m_targetOrigin += forward * speed * dt;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		m_targetOrigin -= right * speed * dt;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		m_targetOrigin -= forward * speed * dt;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		m_targetOrigin += right * speed * dt;
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_targetOrigin[2] -= speed * dt;
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_targetOrigin[2] += speed * dt;
	}

}

double toRad(double degrees) {
	return degrees * (3.1415926535897932385 / 180.0);
}

void Workbench::handleMouseInput() {
	m_prevX = m_currX;
	m_prevY = m_currY;
	glfwGetCursorPos(m_window, &m_currX, &m_currY);
	double deltaX = m_currX - m_prevX;
	double deltaY = m_currY - m_prevY;
	static double pitch = 0;
	static double yaw = 90;
	pitch -= deltaY * 0.3;
	yaw -= deltaX * 0.3;
	if (pitch < -89) {
		pitch = -89;
	}
	if (pitch > 89) {
		pitch = 89;
	}
	vec3 direction;
	
	direction[0] = cos(toRad(yaw)) * cos(toRad(pitch));
	direction[2] = sin(toRad(pitch));
	direction[1] = sin(toRad(yaw)) * cos(toRad(pitch));

	m_targetDirection = direction;
}

void Workbench::updateCinematicCamera(double dt) {
	double cinematicSpeed = 10.0;
	m_scene->camera.origin += (m_targetOrigin - m_scene->camera.origin) * cinematicSpeed * dt;
	m_scene->camera.direction += (m_targetDirection - m_scene->camera.direction) * cinematicSpeed * dt;
	m_scene->camera.direction = m_scene->camera.direction.normalize();
}