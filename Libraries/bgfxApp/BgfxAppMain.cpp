/*
 * Copyright 2011-2024 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "common/common.h"
#include "common/bgfx_utils.h"
#include "common/imgui/imgui.h"
#include "../Libraries/glfw/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../Libraries/glfw/glfw3native.h"

#include "../Libraries/bgfx/include/bgfx/bgfx.h"
// #include "../Libraries/bigg/bigg_imgui.hpp"

#include "../Libraries/glm/glm/glm.hpp"
#include "../Libraries/glm/glm/ext/matrix_transform.hpp"
#include "../Libraries/glm/glm/ext/matrix_clip_space.hpp"
#include "../Libraries/glm/glm/gtx/euler_angles.hpp"

#include <bgfx/platform.h>
#include <filesystem>

GLFWwindow* mWindow = nullptr;
uint32_t mWidth = 1280;
uint32_t mHeight = 720;
uint32_t mReset = 0;
const char* mTitle = "Title";

void reset(uint32_t flags = 0)
{
	mReset = flags;
	bgfx::reset(mWidth, mHeight, mReset);
	// imguiReset(uint16_t(getWidth()), uint16_t(getHeight()));

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, uint16_t(mWidth), uint16_t(mHeight));
}

bgfx::ShaderHandle myLoadShader(const char* FILENAME)
{
	const char* shaderPath = "???";

	switch (bgfx::getRendererType()) {
	case bgfx::RendererType::Noop:
	// case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
	case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
	case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
	case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
	case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
	case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
	}

	size_t shaderLen = strlen(shaderPath);
	size_t fileLen = strlen(FILENAME);
	char* filePath = (char*)malloc(shaderLen + fileLen + 1);

	memcpy(filePath, shaderPath, shaderLen);
	memcpy(&filePath[shaderLen], FILENAME, fileLen);
	filePath[shaderLen + fileLen] = '\0';

	FILE* file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	return bgfx::createShader(mem);
}

struct PosColorVertex
{
	float x;
	float y;
	float z;
	uint32_t abgr;
	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	}
	static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};
static const uint16_t s_cubeTriList[] = { 2, 1, 0, 2, 3, 1, 5, 6, 4, 7, 6, 5, 4, 2, 0, 6, 2, 4, 3, 5, 1, 3, 7, 5, 1, 4, 0, 1, 5, 4, 6, 3, 2, 7, 3, 6 };

int Run(int argc, char** argv, bgfx::RendererType::Enum type = bgfx::RendererType::Count,
	uint16_t vendorId = BGFX_PCI_ID_NONE, uint16_t deviceId = 0, bgfx::CallbackI* callback = NULL,
	bx::AllocatorI* allocator = NULL)
{
	// Initialize the glfw
	if (!glfwInit())
	{
		return -1;
	}

	// Create a window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, NULL, NULL);
	if (!mWindow)
	{
		glfwTerminate();
		return -1;
	}

	// Setup input callbacks
	// glfwSetWindowUserPointer(mWindow, this);
	// glfwSetKeyCallback(mWindow, keyCallback);
	// glfwSetCharCallback(mWindow, charCallback);
	// glfwSetCharModsCallback(mWindow, charModsCallback);
	// glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
	// glfwSetCursorPosCallback(mWindow, cursorPosCallback);
	// glfwSetCursorEnterCallback(mWindow, cursorEnterCallback);
	// glfwSetScrollCallback(mWindow, scrollCallback);
	// glfwSetDropCallback(mWindow, dropCallback);
	// glfwSetWindowSizeCallback(mWindow, windowSizeCallback);

	// Setup bgfx
	bgfx::PlatformData platformData;
	memset(&platformData, 0, sizeof(platformData));
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(mWindow);
	platformData.ndt = glfwGetX11Display();
#elif BX_PLATFORM_OSX
	platformData.nwh = glfwGetCocoaWindow(mWindow);
#elif BX_PLATFORM_WINDOWS
	platformData.nwh = glfwGetWin32Window(mWindow);
#endif // BX_PLATFORM_
	bgfx::setPlatformData(platformData);

	// Init bgfx
	bgfx::Init init;
	init.platformData = platformData;
	init.type = type;
	init.vendorId = vendorId;
	init.deviceId = deviceId;
	init.callback = callback;
	init.allocator = allocator;
	bgfx::init(init);

	// Setup ImGui
	// imguiInit(mWindow);

	// Initialize the application
	reset();

	// Loop until the user closes the window
	float lastTime = 0.f;
	float dt = 0.f;
	float time = 0.f;
	float elapsedTime = 0.f;

	PosColorVertex::init();
	bgfx::VertexBufferHandle mVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
	bgfx::IndexBufferHandle mIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
	bgfx::setDebug(BGFX_DEBUG_TEXT);

	bgfx::ShaderHandle vsh = myLoadShader("vs_cubes.bin");
	bgfx::ShaderHandle fsh = myLoadShader("fs_cubes.bin");
	bgfx::ProgramHandle mProgram = bgfx::createProgram(vsh, fsh, true);

	while (!glfwWindowShouldClose(mWindow))
	{
		time = (float)glfwGetTime();
		dt = time - lastTime;
		lastTime = time;

		glfwPollEvents();
		// imguiEvents(dt);
		// ImGui::NewFrame();
		// update(dt);
		// ImGui::Render();
		// imguiRender(ImGui::GetDrawData());

		elapsedTime += dt;
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -35.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(mWidth) / mHeight, 0.1f, 100.0f);
		bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
		bgfx::setViewRect(0, 0, 0, uint16_t(mWidth), uint16_t(mHeight));
		bgfx::touch(0);

		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				glm::mat4 mtx = glm::identity<glm::mat4>();
				mtx = glm::translate(mtx, glm::vec3(15.0f - float(xx) * 3.0f, -15.0f + float(yy) * 3.0f, 0.0f));
				mtx *= glm::yawPitchRoll(elapsedTime + xx * 0.21f, elapsedTime + yy * 0.37f, 0.0f);
				bgfx::setTransform(&mtx[0][0]);
				bgfx::setVertexBuffer(0, mVbh);
				bgfx::setIndexBuffer(mIbh);
				bgfx::setState(BGFX_STATE_DEFAULT);
				bgfx::submit(0, mProgram);
			}
		}

		// bgfx::setDebug(BGFX_DEBUG_STATS);
		bgfx::frame();
	}

	// Shutdown application and glfw
	// int ret = shutdown();
	// imguiShutdown();
	bgfx::shutdown();
	glfwTerminate();
	return 0;
}

int main(int _argc, char** _argv)
{
	// _main_(_argc, _argv);
	Run(_argc, _argv);
	int bp = 0;
}