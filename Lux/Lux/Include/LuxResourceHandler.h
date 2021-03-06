#ifndef LUX_RESOURCE_HANDLER_H
#define LUX_RESOURCE_HANDLER_H
#include "LuxObserverPtr.h"
namespace Lux
{
	namespace Core
	{
		class Material;
		class Shader;
		class Mesh;
		class Model;
		class Key;
		class Texture;
		class Texture1D;
		class Texture2D;
		class Texture3D;
		class TextureSampler;
		struct TextureSamplerOptions;
		struct FileInfo;
		class RenderWindow;
		class PhysicsMaterial;

		class ResourceHandler
		{
		public:

			static ResourceHandler* Create(Utility::PlatformType a_PlatformType, RenderWindow* a_RenderWindow);
			virtual ~ResourceHandler();

			virtual ObserverPtr<Model> CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags) = 0;
			virtual ObserverPtr<Model> CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags) = 0;
			virtual ObserverPtr<Texture2D> CreateTexture2DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName = LUX_DEFAULT_TEX_SAMPLER_NAME) = 0;
			virtual ObserverPtr<Texture2D> CreateTexture2DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName = LUX_DEFAULT_TEX_SAMPLER_NAME) = 0;
			virtual ObserverPtr<Texture1D> CreateTexture1DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName = LUX_DEFAULT_TEX_SAMPLER_NAME) = 0;
			virtual ObserverPtr<Texture1D> CreateTexture1DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName = LUX_DEFAULT_TEX_SAMPLER_NAME) = 0;
			virtual ObserverPtr<Texture3D> CreateTexture3DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName = LUX_DEFAULT_TEX_SAMPLER_NAME) = 0;
			virtual ObserverPtr<Texture3D> CreateTexture3DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName = LUX_DEFAULT_TEX_SAMPLER_NAME) = 0;
			virtual ObserverPtr<Shader> CreateShaderFromFile(const String& a_File, const String& a_ShaderName) = 0;
			virtual ObserverPtr<Material> CreateMaterial(const String& a_Name) = 0;
			virtual ObserverPtr<TextureSampler> CreateTextureSampler(const String& a_Name, TextureSamplerOptions& a_InitOptions) = 0;
			virtual ObserverPtr<PhysicsMaterial> CreatePhysicsMaterial(const String& a_Name, float a_Restitution = 0.0f, float a_DynamicFriction = 0.0f, float a_StaticFriction = 0.0f) = 0;

			virtual ObserverPtr<Model> GetMesh(const String& a_Name) = 0;
			virtual ObserverPtr<Shader> GetShader(const String& a_Name) = 0;
			virtual ObserverPtr<Material> GetMaterial(const String& a_Name) = 0;
			virtual ObserverPtr<Texture2D> GetTexture2D(const String& a_Name) = 0;
			virtual ObserverPtr<Texture3D> GetTexture3D(const String& a_Name) = 0;
			virtual ObserverPtr<Texture1D> GetTexture1D(const String& a_Name) = 0;
			virtual ObserverPtr<TextureSampler> GetTextureSampler(const String& a_Name) = 0;
			virtual ObserverPtr<PhysicsMaterial> GetPhysicsMaterial(const String& a_Name) = 0;
			virtual bool MaterialExists(const String& a_Name) = 0;
			virtual bool MeshExists(const String& a_Name) = 0;
			virtual bool ShaderExists(const String& a_Name) = 0;
			virtual bool Texture2DExists(const String& a_Name) = 0;
			virtual bool Texture1DExists(const String& a_Name) = 0;
			virtual bool Texture3DExists(const String& a_Name) = 0;
			virtual bool TextureSamplerExists(const String& a_Name) = 0;
			virtual bool PhysicsMaterialExists(const String& a_Name) = 0;
			virtual bool DeleteTexture2D(const String& a_Name) = 0;
			virtual bool DeleteTexture1D(const String& a_Name) = 0;
			virtual bool DeleteTexture3D(const String& a_Name) = 0;
			virtual bool DeleteTextureSampler(const String& a_Name) = 0;
			virtual bool DeletePhysicsMaterial(const String& a_Name) = 0;

		protected:
			ResourceHandler();
			ResourceHandler(ResourceHandler const&);// Don't Implement
			void operator=(ResourceHandler const&);// Don't implement

		};
	}
}

#endif