#pragma once

// -----------For use Application--------- //

// -------------Core--------------------
// Core
#include "Dark/Core/Log.h"
#include "Dark/Core/Core.h"
#include "Dark/Core/Layer.h"
#include "Dark/Core/Input.h"
#include "Dark/Core/KeyCodes.h"
#include "Dark/Core/Timestep.h"
#include "Dark/Core/Math/Math.h"
#include "Dark/Core/Application.h"
#include "Dark/Core/MouseButtonCodes.h"
#include "Dark/Core/Utility/ECS/Entity.h"
#include "Dark/Core/Utility/ECS/Component.h"
// Event
#include "Dark/Events/KeyEvent.h"
#include "Dark/Events/InputEvent.h"
#include "Dark/Events/MouseEvent.h"
#include "Dark/Events/ApplicationEvent.h"
// ImGui
#include "Dark/ImGui/ImGuiLayer.h"
//----------------------------------------

// ----------Renderer-----------------
#include "Dark/Renderer/Renderer.h"
#include "Dark/Renderer/RenderCommand.h"
#include "Dark/Renderer/VertexArray.h"
#include "Dark/Renderer/Buffer.h"
#include "Dark/Renderer/Framebuffer.h"
#include "Dark/Renderer/Camera.h"
#include "Dark/Renderer/OrthographicCamera.h"
#include "Dark/Renderer/OrthographicCameraController.h"
#include "Dark/Renderer/PrespectiveCamera.h"
// -----------------------------------

// ----------Resource-----------------
#include "Dark/Resource/Texture.h"
#include "Dark/Resource/Shader.h"
#include "Dark/Resource/ResourceManager.h"
// -----------------------------------

// --------------Scene---------------------
#include "Dark/Scene/Scene.h"
#include "Dark/Scene/Serialize.h"
// ----------------------------------------

//#include <Dark/Core/EntryPoint.h>