#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include "stb_image.h"
#include "Cube.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Sphere.h"

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"


/*Globals*/
Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
double lastX = 0.0;
double lastY = 0.0;
bool firstMouse = true;
float fov = 45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		camera.ProcessMouseMovement((float)xoffset, (float)yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(fov, (float)yoffset);
}

void ProcessKeyInput(GLFWwindow* window ,float deltatime) 
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltatime);
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1200, 900, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW Window." << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*Load extension loader library*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*Set size of window where our graphics will be shown*/
	glViewport(0, 0, 1200, 900);

	/*Let GLFW know we want to resize our viewport if the window is resized*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable opengl depth-testing
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	/*Set callback for cursor controls*/
	glfwSetCursorPosCallback(window, mouse_callback);

	/*set scrool wheel callback*/
	glfwSetScrollCallback(window, scroll_callback);

	/*Capture mouse in window*/
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_TEXTURE_2D);

	float quadVertices[] =
	{
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	};

	VertexArray VAO;
	VertexBuffer VBO(quadVertices, sizeof(quadVertices));

	VAO.Bind();
	VBO.Bind();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	/*Geometries*/
	Sphere sphere(1.0f, 72, 36, true);
	Cube cube(1.0f, 1.0f, 1.0f);

	glm::vec3 color(1.0f);
	float metallic = 0.0f;
	float roughness = 0.05f;
	float ao = 0.0f;
	bool textured = false;

	/*Shaders*/
	Shader shaderIBL("shaders/IBLvert.glsl", "shaders/IBLfrag.glsl");
	//Shader shaderPBRText("shaders/PBRTextureVert.glsl", "shaders/PBRTextureFrag.glsl");
	Shader shaderIBLText("shaders/IBLVert.glsl", "shaders/IBLTextureFrag.glsl");

	Shader shaderHDR("shaders/hdrvert.glsl", "shaders/hdrfrag.glsl");
	Shader cubeMapShader("shaders/cubemapvert.glsl", "shaders/cubemapfrag.glsl");
	Shader irradianceShader("shaders/convolutionvert.glsl", "shaders/convolutionfrag.glsl");
	Shader preFilterShader("shaders/hdrvert.glsl", "shaders/prefilterfrag.glsl");
	Shader brdfShader("shaders/brdfvert.glsl", "shaders/brdffrag.glsl");

	stbi_set_flip_vertically_on_load(1);

	Texture hdrTexture;
	hdrTexture.Bind2D();
	hdrTexture.LoadHdrMap("textures/hdr/newport_loft.hdr");

	Texture ironalbedo;
	Texture ironnorm;
	Texture ironao;
	Texture ironrough;
	Texture ironmetallic;

	stbi_set_flip_vertically_on_load(0);

	shaderIBLText.use();
	shaderIBLText.setInt("Albedo", 0);
	shaderIBLText.setInt("normalMap", 1);
	shaderIBLText.setInt("Metallic", 2);
	shaderIBLText.setInt("Roughness", 3);
	shaderIBLText.setInt("AO", 4);
	
	ironalbedo.Bind2D();
	ironalbedo.LoadTexture2D("textures/pbr/albedo.png");
	ironnorm.Bind2D();
	ironnorm.LoadTexture2D("textures/pbr/normal.png");
	ironmetallic.Bind2D();
	ironmetallic.LoadTexture2D("textures/pbr/metallic.png");
	ironrough.Bind2D();
	ironrough.LoadTexture2D("textures/pbr/roughness.png");
	ironao.Bind2D();
	ironao.LoadTexture2D("textures/pbr/ao.png");

	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	unsigned int envCubeMap;
	glGenTextures(1, &envCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
	for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureView[] =
	{
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	shaderHDR.use();
	shaderHDR.setInt("hdrTexture", 0);
	shaderHDR.setMat4f("projection", captureProjection);
	hdrTexture.ActivateTexture(0);
	hdrTexture.Bind2D();

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; i++)
		{
			shaderHDR.setMat4f("view", captureView[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cube.Draw();
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	irradianceShader.use();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4f("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; i++)
		{
			irradianceShader.setMat4f("view", captureView[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cube.Draw();
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
		}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	preFilterShader.use();
	preFilterShader.setMat4f("projection", captureProjection);
	preFilterShader.setInt("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; mip++)
		{
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			preFilterShader.setFloat1f("roughness", roughness);

			for (unsigned int i = 0; i < 6; i++)
			{
				preFilterShader.setMat4f("view", captureView[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				cube.Draw();
			}
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VAO.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
	VAO.UnBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

	cubeMapShader.use();
	cubeMapShader.setInt("cubeMap", 0);

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	std::vector<glm::vec3> lightColors(4, glm::vec3(0.0f));
	std::vector<glm::vec3> lightPositions(4, glm::vec3(0.0f));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ProcessInput(window);
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessKeyInput(window, deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(fov), 1200.0f / 900.0f, 0.1f, 100.f);

		if (textured)
		{
			shaderIBLText.use();
			shaderIBLText.setInt("irradianceMap", 5);
			shaderIBLText.setInt("prefilterMap", 6);
			shaderIBLText.setInt("brdfLUT", 7);
		}
		else
		{
			shaderIBL.use();
			shaderIBL.setInt("irradianceMap", 5);
			shaderIBL.setInt("prefilterMap", 6);
			shaderIBL.setInt("brdfLUT", 7);
		}

		/*Options to modify model color*/
		ImGui::Begin("Model Options");
			ImGui::Checkbox("Texture", &textured);
			
			if (textured)
			{
				shaderIBLText.use();
				shaderIBLText.setMat4f("model", model);
				shaderIBLText.setMat4f("view", view);
				shaderIBLText.setMat4f("projection", projection);
				shaderIBLText.setVec3f("camPos", camera.GetPosition());
				shaderIBLText.setFloat1f("Divisions", 1.0f);

				ironalbedo.ActivateTexture(0);
				ironalbedo.Bind2D();
				ironnorm.ActivateTexture(1);
				ironnorm.Bind2D();
				ironmetallic.ActivateTexture(2);
				ironmetallic.Bind2D();
				ironrough.ActivateTexture(3);
				ironrough.Bind2D();
				ironao.ActivateTexture(4);
				ironao.Bind2D();
			}
			else
			{
				ImGui::ColorEdit3("Color", glm::value_ptr(color));
				ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f, "%.5f");
				ImGui::SliderFloat("Roughness", &roughness, 0.05f, 1.0f, "%.5f");
				ImGui::SliderFloat("Ambient Occlusion", &ao, 0.0f, 100.0f);

				shaderIBL.use();
				shaderIBL.setMat4f("model", model);
				shaderIBL.setMat4f("view", view);
				shaderIBL.setMat4f("projection", projection);
				shaderIBL.setVec3f("camPos", camera.GetPosition());
				shaderIBL.setVec3f("Albedo", color);
				shaderIBL.setFloat1f("Metallic", metallic);
				shaderIBL.setFloat1f("Roughness", roughness);
				shaderIBL.setFloat1f("AO", ao);
				shaderIBL.setFloat1f("Divisions", 1.0f);
			}

			sphere.Draw();

		ImGui::End();

		/*Options for modifying light data*/
		ImGui::Begin("Light Options");
			for (int i = 0; i < 4; i++)
			{
				std::string label = "Light " + std::to_string(i + 1);
				std::string colorlabel = "Color## " + std::to_string(i + 1);
				std::string lightlabel = "Position## " + std::to_string(i + 1);

				ImGui::Text(label.c_str());
				ImGui::SliderFloat3(colorlabel.c_str(), glm::value_ptr(lightColors[i]), 0.0f, 1000.0f);
				ImGui::SliderFloat3(lightlabel.c_str(), glm::value_ptr(lightPositions[i]), -10.0f, 10.0f);

				if (textured)
				{
					shaderIBLText.setVec3f("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
					shaderIBLText.setVec3f("lightColors[" + std::to_string(i) + "]", lightColors[i]);
				}
				else
				{
					shaderIBL.setVec3f("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
					shaderIBL.setVec3f("lightColors[" + std::to_string(i) + "]", lightColors[i]);
				}
			}
		ImGui::End();

		/*Setup textures*/
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

		cubeMapShader.use();
		cubeMapShader.setMat4f("view", view);
		cubeMapShader.setMat4f("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
		cube.Draw();

		//brdfShader.use();
		//VAO.Bind();
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
		//VAO.UnBind();

		//ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}