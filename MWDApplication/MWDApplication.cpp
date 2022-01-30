#include "MWDApplication.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "headers.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

#include "MWDModel.h"
#include "MWDCamera.h"
#include "MWDRenderer.h"
using namespace std;
#pragma region ImGui
void ImGuiInit(GLFWwindow* Hwindow) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext(NULL);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiViewportFlags_NoDecoration;

	ImGui_ImplGlfw_InitForOpenGL(Hwindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}
void ImGuiTerminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void ImGuiFrameBegin() {
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void ImGuiFrameEnd(GLFWwindow* Hwindow) {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* Hwindow = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(Hwindow);

	}
	int display_w, display_h;
	glfwGetFramebufferSize(Hwindow, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	/*glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);*/
	glfwSwapBuffers(Hwindow);
	glfwPollEvents();
}
#pragma endregion

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) ;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) ;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) ;
void processInput(GLFWwindow* window) ;

MWDCamera camera = MWDCamera(glm::vec3(0.0f, 0.0f, 3.0f));

int main() {
	#pragma region glfw��ʼ��
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* Hwindow = glfwCreateWindow(800, 600, "MWDEngine", NULL, NULL);
	if (!Hwindow) {
		cout << "���ڴ���ʧ��" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Hwindow);
	glfwSetFramebufferSizeCallback(Hwindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(Hwindow, mouse_callback);
	glfwSetScrollCallback(Hwindow, scroll_callback);
	stbi_set_flip_vertically_on_load(true);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "glad��ʼ��ʧ�ܣ�" << endl;
		glfwTerminate();
		return -1;
	}
	ImGuiInit(Hwindow);

#pragma endregion
	MWDModel model = MWDModel(string("model_0"),string("C:/Users/InputWindy/Desktop/MyRenderer/MWDEngine/objModel/car.obj"));
	MWDModel model1 = MWDModel(string("model_1"),string("C:/Users/InputWindy/Desktop/MyRenderer/MWDEngine/objModel/lucia/f.fbx"));
	//model1.meshes[0].m_material->SetSampler(string("diffuse_map"),string("C:/Users/InputWindy/Desktop/MyRenderer/MWDEngine/objModel/lucia/E3SwordMd090011.png"));
	MWDRenderer::GetMainRenderer()->SetCamera(&camera);
	MWDRenderer::GetMainRenderer()->SetScreenHeight(SCR_HEIGHT);
	MWDRenderer::GetMainRenderer()->SetScreenWidth(SCR_WIDTH);

	while (!glfwWindowShouldClose(Hwindow)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ImGuiFrameBegin();
		processInput(Hwindow);
		ImGui::ShowDemoWindow();
		model1.Draw();
		model.Draw();

		ImGuiFrameEnd(Hwindow);
	};

	ImGuiTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(MWDCamera::MWDCamera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(MWDCamera::MWDCamera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(MWDCamera::MWDCamera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(MWDCamera::MWDCamera_Movement::RIGHT, deltaTime);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

	camera.ProcessMouseMovement(xoffset*3, yoffset*3);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}