#include "LuxPCH.h"
#include "LuxObjectPool.h"
#include "LuxObserverPtr.h"
#include "LuxRenderWindow.h"
#include "LuxPlatform.h"
#include "LuxKeyCodes.h"
#include "LuxEntity.h"
#include "LuxKey.h"
#include "LuxModel.h"
#include "LuxMeshRenderer.h"
#include "LuxTransform.h"
#include "LuxComponent.h"
#include "LuxMaterial.h"
#include "LuxCamera.h"
#include "LuxLight.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSystem.h"
#include "LuxShader.h"
#include "LuxBehaviourSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxEventSystem.h"
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"
#include "LuxTimer.h"
#include "LuxHelpers.h"
#include "LuxResourceHandler.h"
#include "LuxBehaviour.h"
#include "LuxFreeLookCamera.h"
#include "LuxPhysicsMaterial.h"
#include "LuxConvexMeshCollider.h"
#include "LuxPhysicsSystem.h"
#include "LuxRigidBody.h"
#include "LuxDynamicRigidBody.h"
#include "LuxStaticRigidBody.h"
#include "LuxBoxCollider.h"
#include "LuxSphereCollider.h"
#include "LuxCapsuleCollider.h"
#include "LuxTextureSampler.h"
#include "LuxFileHandler.h"
#include "LuxApplication.h"

Lux::Application::Application() :
m_Platform(nullptr), m_SceneManager(nullptr), m_ResourceHandler(nullptr)
{

}

Lux::Application::~Application()
{
	Utility::SafePtrDelete(m_ResourceHandler);
	Utility::SafePtrDelete(m_SceneManager);
	Utility::SafePtrDelete(m_Platform);
}

bool Lux::Application::Initialize(Utility::AppInitOptions& a_AppInitOptions)
{
	// Configure logging system
	Utility::FILELog::ReportingLevel() = a_AppInitOptions.m_LoggingLevel;
	FILE* logFile = 0;
	fopen_s(&logFile, "Lux.log", "w");
	Utility::Output2FILE::Stream() = logFile;

	LUX_LOG(Utility::logINFO) << "Logger started.";

	m_Platform = Core::Platform::Create(a_AppInitOptions.m_PlatformType);
	bool platformInit = m_Platform->Init(a_AppInitOptions);

	if (!platformInit)
		return false;

	// Init the Scene Manager
	m_SceneManager = new Core::SceneManager(m_Platform->GetRenderWindow());

	// Init Resource Handler
	m_ResourceHandler = Core::ResourceHandler::Create(a_AppInitOptions.m_PlatformType, m_Platform->GetRenderWindow());

	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("../EngineAssets"))
		return false;

	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("../EngineAssets/textures"))
		return false;

	LoadDefaultResources();

	// Init Components
	LoadComponentTypes();
	LoadSystemTypes();

	return true;
}

const bool Lux::Application::ShouldQuit()
{
	bool ret = false;
	if (m_Platform->ShouldQuit())
	{
		return true;
	}

	return false;
}

void Lux::Application::Run()
{
	bool quit = false;
	Utility::Timer timer;
	float dt = 0.0f;
	bool result = true;
	// Main loop
	while (!quit)
	{
		dt = (float)timer.GetAsSeconds();
		timer.Reset();
		result = Update(dt);
		CheckResult(result);
		InternalUpdate(dt);
		quit = ShouldQuit();
	}
}

void Lux::Application::InternalUpdate(const float a_DeltaTime)
{
	m_SceneManager->ProcessUpdate(a_DeltaTime);
}

void Lux::Application::CheckResult(bool res)
{
	if (!res)
	{
		LUX_LOG(Utility::logERROR) << "Fatal error during program execution. Main loop function returned false. Throwing exception...";
		MessageBox(nullptr, "Fatal error during execution.Aborting program.", "Error", MB_ICONERROR | MB_SETFOREGROUND);
		throw std::invalid_argument("Main loop function returned false.");
	}
}

bool Lux::Application::LoadComponentTypes()
{
	m_SceneManager->RegisterNewComponentType<Core::Transform>();
	m_SceneManager->RegisterNewComponentType<Graphics::MeshRenderer>();
	m_SceneManager->RegisterNewComponentType<Graphics::Camera>();
	m_SceneManager->RegisterNewComponentType<Core::FreeLookCamera>();
	m_SceneManager->RegisterNewComponentType<Graphics::Light>();

	// Physics
	m_SceneManager->RegisterNewComponentType<Physics::DynamicRigidBody>();
	m_SceneManager->RegisterNewComponentType<Physics::StaticRigidBody>();
	m_SceneManager->RegisterNewComponentType<Physics::BoxCollider>();
	m_SceneManager->RegisterNewComponentType<Physics::SphereCollider>();
	m_SceneManager->RegisterNewComponentType<Physics::CapsuleCollider>();
	m_SceneManager->RegisterNewComponentType<Physics::ConvexMeshCollider>();

	//Setup Component Dependencies
	m_SceneManager->RegisterComponentTypeDependency<Graphics::Camera, Core::Transform>();
	m_SceneManager->RegisterComponentTypeDependency<Core::FreeLookCamera, Graphics::Camera>();
	m_SceneManager->RegisterComponentTypeDependency<Graphics::MeshRenderer, Core::Transform>();

	m_SceneManager->RegisterComponentTypeDependency<Physics::DynamicRigidBody, Core::Transform>();
	m_SceneManager->RegisterComponentTypeDependency<Physics::StaticRigidBody, Core::Transform>();

	return true;
}

bool Lux::Application::LoadSystemTypes()
{
	m_SceneManager->RegisterNewSystemType<Graphics::RenderingSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Core::Transform, Graphics::RenderingSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Graphics::MeshRenderer, Graphics::RenderingSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Graphics::Camera, Graphics::RenderingSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Graphics::Light, Graphics::RenderingSystem>();

	m_SceneManager->RegisterNewSystemType<Core::EventSystem>();

	m_SceneManager->RegisterNewSystemType<Core::BehaviourSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Core::FreeLookCamera, Core::BehaviourSystem>();

	m_SceneManager->RegisterNewSystemType<Physics::PhysicsSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Physics::DynamicRigidBody, Physics::PhysicsSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Physics::StaticRigidBody, Physics::PhysicsSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Core::Transform, Physics::PhysicsSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Physics::BoxCollider, Physics::PhysicsSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Physics::SphereCollider, Physics::PhysicsSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Physics::CapsuleCollider, Physics::PhysicsSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Physics::ConvexMeshCollider, Physics::PhysicsSystem>();

	return true;
}

void Lux::Application::LoadDefaultResources()
{
	m_ResourceHandler->CreateTextureSampler(LUX_DEFAULT_TEX_SAMPLER_NAME, Lux::Core::TextureSamplerOptions());
	m_ResourceHandler->CreateTexture2DFromFile(LUX_DEFAULT_TEX, LUX_DEFAULT_TEX);
}
