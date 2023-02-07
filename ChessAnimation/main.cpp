#include "camera.h"
#include "model.h"

#include <iostream>

// wymiary planszy:
// 5 pol = 1,046216965
// 1 pole = 0,209243393

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void DrawSuns(shader& shader, Model& sun);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float sunScale = 0.03f;

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	stbi_set_flip_vertically_on_load(false);
	glEnable(GL_DEPTH_TEST);

	srand(time(NULL));

	shader sceneShader("shaders\\chess_vertex.vs", "shaders\\chess_fragment.fs");
	//shader sphereShader("shaders\\chess_vertex.vs", "shaders\\chess_fragment.fs");
	//const char* path = "C:\\Users\\hlaw\\Downloads\\glass_chess\\Glass Chess.obj"; //<== sprawdzic potem
	//const char* path = "C:\\Users\\hlaw\\Downloads\\t3qqxibgic-CenterCitySciFi\\Center city Sci-Fi\\Center City Sci-Fi.obj";
	//const char* path = "C:\\Users\\hlaw\\Documents\\folder\\ChessAnimation\\ChessAnimation\\resources\\Chessset.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\chess_alone\\pieces.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\survival_backpack\\Survival_BackPack_2.fbx";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\uploads_files_2063903_chessPieces_1.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\backpack\\backpack.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\castle\\Castle_X6.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\ChessModel\\Chess1.obj";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\chess_set_new\\Scene\\Chess.obj";
	const char* path = "C:\\Users\\hlaw\\Downloads\\chess_with_textures\\chess_set_obj.obj";
	const char* sun_path = "C:\\Users\\hlaw\\Downloads\\sun\\Sun.obj";

	//const char* path = "C:\\Users\\hlaw\\Downloads\\Chess_1";
	//const char* path = "C:\\Users\\hlaw\\Downloads\\Board.obj";

	//const char* path = "C:\\Users\\hlaw\\Downloads\\chesses_nice\\PiezasAjedrez.obj";

	Model sceneModel(path);
	Model sunModel(sun_path);

	Board* board = Board::GetInstance();

	int frames = 1;
	int mvCnt = 0;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(window);


		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneShader.use();


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		sceneShader.setMat4("projection", projection);
		sceneShader.setMat4("view", view);


		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		sceneShader.setMat4("model", model);

		sceneModel.Draw(sceneShader);

		DrawSuns(sceneShader, sunModel);

		board->execute_moves();

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

float t = 0.0f;
void DrawSuns(shader& shader, Model& sun)
{
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(sunScale));
	shader.setMat4("model", model);

	glm::vec3 time = glm::vec3(cosf(t) - sinf(t), 1, cosf(t) + sinf(t));

	sun.Draw(shader, (glm::vec3(2.0f, 1.0f, 2.0f)*time)/sunScale);
	sun.Draw(shader, (glm::vec3(-2.0f, 1.0f, 2.0f)*time)/sunScale);
	sun.Draw(shader, (glm::vec3(2.0f, 1.0f, -2.0f)*time)/sunScale);
	sun.Draw(shader, (glm::vec3(-2.0f, 1.0f, -2.0f) *time)/sunScale);
	t += 0.0005;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}