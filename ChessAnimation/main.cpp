#include "camera.h"


viewer sceneViewer;
camera sceneCamera;

float curr_x = .0f, curr_y = .0f;
float prev_x = 0, prev_y = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void move_camera();

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chess", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	srand(time(NULL));


	shader sceneShader("vertex.vs", "fragment.fs");
	//const char* path = "C:\\Users\\hlaw\\Downloads\\glass_chess\\Glass Chess.obj"; //<== sprawdzic potem
	//const char* path = "C:\\Users\\hlaw\\Downloads\\t3qqxibgic-CenterCitySciFi\\Center city Sci-Fi\\Center City Sci-Fi.obj";
	const char* path = "C:\\Users\\hlaw\\Documents\\folder\\ChessAnimation\\ChessAnimation\\resources\\Chessset.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\survival_backpack\\Survival_BackPack_2.fbx";

	model sceneModel(path);
	sceneCamera = camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0,1,0), 90, (float)SCR_WIDTH / (float)SCR_HEIGHT);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneShader.use();

		// view/projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 view = glm::mat4(1.0f);
		//ourShader.setMat4("projection", projection);
		//ourShader.setMat4("view", view);
		// render the loaded model
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//ourShader.setMat4("model", model);

		sceneViewer.bindUniforms(sceneShader);
		sceneModel.draw(sceneShader);
		move_camera();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_W:
		//sceneViewer.transformView(glm::vec3(0.0f, 0.0f, 0.2f));
		break;
	case GLFW_KEY_S:
		//sceneViewer.transformView(glm::vec3(0.0f, 0.0f, -0.2f));
		break;
	case GLFW_KEY_D:
		//sceneViewer.transformView(glm::vec3(-0.2f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_A:
		//sceneViewer.transformView(glm::vec3(0.2f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_SPACE:
		break;
	case GLFW_KEY_LEFT_SHIFT:
		break;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	/*int xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);*/
	/*float horizontalAngle = MOUSE_SPEED * float(SCR_WIDTH / 2 - xpos);
	float verticalAngle = MOUSE_SPEED * float(SCR_HEIGHT / 2 - ypos);
	sceneViewer.rotate(horizontalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	sceneViewer.rotate(verticalAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);*/

	curr_x = 2.0f * xpos / SCR_WIDTH - 1.0f;
	curr_y = 2.0f * (SCR_HEIGHT - ypos) / SCR_HEIGHT - 1.0f;
}

void move_camera()
{
	sceneViewer.setView(sceneCamera.move_look(MOUSE_SPEED * (curr_x - prev_x), MOUSE_SPEED * (curr_y - prev_y)));

	prev_x = curr_x;
	prev_y = curr_y;
}