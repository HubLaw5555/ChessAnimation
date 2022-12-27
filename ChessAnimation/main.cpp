#include "camera.h"


viewer sceneViewer;
camera sceneCamera;

float curr_x = .0f, curr_y = .0f;
float prev_x = 0, prev_y = 0;
glm::vec3 shift = glm::vec3(0, 0, 0);
glm::vec3 position = glm::vec3(0, 0, 0);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void move_camera(GLFWwindow* window);

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
	//const char* path = "C:\\Users\\hlaw\\Documents\\folder\\ChessAnimation\\ChessAnimation\\resources\\Chessset.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\chess_alone\\pieces.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\survival_backpack\\Survival_BackPack_2.fbx";
	const char* path = "C:\\Users\\hlaw\\Downloads\\uploads_files_3059059_ChessModel\\Chess1.obj";

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
		sceneShader.setVec3("observerPosition", position);
		sceneViewer.bindUniforms(sceneShader);
		sceneModel.draw(sceneShader);
		move_camera(window);

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
		shift.z -= KEY_BATCH;
		break;
	case GLFW_KEY_S:
		//sceneViewer.transformView(glm::vec3(0.0f, 0.0f, -0.2f));
		shift.z += KEY_BATCH;
		break;
	case GLFW_KEY_D:
		//sceneViewer.transformView(glm::vec3(-0.2f, 0.0f, 0.0f));
		shift.x += KEY_BATCH;
		break;
	case GLFW_KEY_A:
		shift.x -= KEY_BATCH;
		//sceneViewer.transformView(glm::vec3(0.2f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_SPACE:
		shift.y += KEY_BATCH;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		shift.y -= KEY_BATCH;
		break;
	case GLFW_KEY_LEFT:
		sceneViewer.rotate(ROTATION_BATCH, glm::vec3(0, 1, 0));
		break;
	case GLFW_KEY_RIGHT:
		sceneViewer.rotate(-ROTATION_BATCH, glm::vec3(0, 1, 0));
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

void move_camera(GLFWwindow* window)
{
	sceneCamera.move_origin(shift);
	sceneViewer.transformView(shift);
	sceneViewer.setView(sceneCamera.move_look(MOUSE_SPEED * (curr_x - prev_x), MOUSE_SPEED * (curr_y - prev_y)));

	prev_x = curr_x;
	prev_y = curr_y;
	position += shift;
	shift = glm::vec3(0, 0, 0);
	//glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
	//curr_x = prev_x;
	//curr_x = prev_x;
}