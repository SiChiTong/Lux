#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxMaterial.h"
#include "LuxSubMesh.h"
#include "LuxMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerDX11.h"
#include "LuxTexture.h"
#include "LuxShader.h"
#include "LuxFileHandler.h"
#include "LuxShaderFileParser.h"
#include "LuxRenderWindow.h"
#include "LuxRenderWindowDX11.h"
#include "LuxMeshDX11.h"
#include "LuxSubMeshDX11.h"

#ifndef YY_NO_UNISTD_H
#define YY_NO_UNISTD_H
#endif
extern "C"
{
#include "LuxFileScanner.h"
}

Lux::Core::Internal::ResourceHandlerDX11::~ResourceHandlerDX11()
{
	MeshMap::iterator it;

	for (it = m_MeshMap.begin(); it != m_MeshMap.end(); ++it)
	{
		it->second.reset();
	}
	m_MeshMap.clear();
	m_LoadedFilenameMeshes.clear();

	MaterialMap::iterator it2;

	for (it2 = m_MaterialMap.begin(); it2 != m_MaterialMap.end(); ++it2)
	{
		it2->second.reset();
	}
	m_MaterialMap.clear();

	TextureMap::iterator it3;

	for (it3 = m_TextureMap.begin(); it3 != m_TextureMap.end(); ++it3)
	{
		it3->second.reset();
	}
	m_TextureMap.clear();

	ShaderMap::iterator it4;

	for (it4 = m_ShaderMap.begin(); it4 != m_ShaderMap.end(); ++it4)
	{
		it4->second.reset();
	}
	m_ShaderMap.clear();
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	// Have we already loaded this mesh before? If we have just return it.
	Mesh* loadedMesh = GetLoadedMesh(a_File);
	if (loadedMesh != nullptr)
	{
		return loadedMesh;
	}

	FileInfo* file = FileHandler::GetInstance().LoadFileInMemory(a_File);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(file->m_RawData, file->m_DataLength, a_PostProcessFlags);
	if (scene == nullptr)
	{
		String errStr("Failed to read file. ");
		errStr.append(importer.GetErrorString());
		Utility::ThrowError(errStr);
	}

	if (scene->mNumMeshes < 1)
	{
		String errStr("The file " + a_File + " does not contain any meshes.");
		Utility::ThrowError(errStr);
	}

	// Materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* impMat = scene->mMaterials[i];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		Material* myMat = new Material(*impMat);
		AddMaterialToMap(matName, myMat);

		// Load all textures from all the types. 
		// If there are no textures from the specified type the function will just do nothing.
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DIFFUSE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SPECULAR);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_AMBIENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_EMISSIVE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_NORMALS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_HEIGHT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_LIGHTMAP);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_REFLECTION);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DISPLACEMENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_OPACITY);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SHININESS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_UNKNOWN);
	}

	// Creating a mesh and adding sub meshes
	MeshDX11* retMesh = new MeshDX11(scene->mNumMeshes, scene->mNumAnimations);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		SubMeshDX11* mesh = new SubMeshDX11(*scene->mMeshes[i], m_RenderWindow->GetDeviceContextPtr());
		retMesh->AddSubMesh(mesh);
	}

	// Animation data (if it exists)
	if (scene->HasAnimations())
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			MeshAnimation* animData = new MeshAnimation(*scene->mAnimations[i]);
			retMesh->AddAnimation(animData);
		}
	}

	AddMeshToMap(a_EntityName, retMesh);
	AddFileNameToMap(a_File, retMesh);
	Utility::SafePtrDelete(file);
	return retMesh;
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	if (a_Info == nullptr)
	{
		Utility::ThrowError("Failed to create entity from Memory. The passed FileInfo pointer is NULL.");
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(a_Info->m_RawData, a_Info->m_DataLength, a_PostProcessFlags);
	if (scene == nullptr)
	{
		String errStr("Failed to read file. ");
		errStr.append(importer.GetErrorString());
		Utility::ThrowError(errStr);
	}

	if (scene->mNumMeshes < 1)
	{
		String errStr("The file does not contain any meshes.");
		Utility::ThrowError(errStr);
	}

	// Materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* impMat = scene->mMaterials[i];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		Material* myMat = new Material(*impMat);
		AddMaterialToMap(matName, myMat);

		// Load all textures from all the types. 
		// If there are no textures from the specified type the function will just do nothing.
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DIFFUSE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SPECULAR);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_AMBIENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_EMISSIVE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_NORMALS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_HEIGHT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_LIGHTMAP);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_REFLECTION);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DISPLACEMENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_OPACITY);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SHININESS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_UNKNOWN);
	}

	// Creating an entity and adding meshes
	MeshDX11* retEntity = new MeshDX11(scene->mNumMeshes, scene->mNumAnimations);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		SubMeshDX11* mesh = new SubMeshDX11(*scene->mMeshes[i], m_RenderWindow->GetDeviceContextPtr());
		retEntity->AddSubMesh(mesh);
	}

	// Animation data (if it exists)
	if (scene->HasAnimations())
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			MeshAnimation* animData = new MeshAnimation(*scene->mAnimations[i]);
			retEntity->AddAnimation(animData);
		}
	}

	AddMeshToMap(a_EntityName, retEntity);
	return retEntity;
}

void Lux::Core::Internal::ResourceHandlerDX11::LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType)
{
	aiString texName;
	unsigned int numTextures = a_Mat->GetTextureCount(a_TexType);

	if (!numTextures)
	{
		return;
	}

	for (unsigned int i = 0; i < numTextures; i++)
	{
		aiReturn texFound = a_Mat->GetTexture(a_TexType, i, &texName);
		if (texFound != AI_SUCCESS)
		{
			LUX_LOG(Utility::logWARNING) << "Could not find texture " << texName.C_Str();
			continue;
		}
		CreateTextureFromFile(String(texName.C_Str()), String(texName.C_Str()));
	}
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerDX11::CreateTextureFromFile(const String& a_File, const String& a_TexName)
{
	// TODO
	return nullptr;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerDX11::CreateTextureFromMemory(FileInfo* a_Info, const String& a_TexName)
{
	// TODO
	return nullptr;
}

Lux::Core::Shader* Lux::Core::Internal::ResourceHandlerDX11::CreateShaderFromFile(const String& a_File, const String& a_ShaderName)
{
	// TODO
	return nullptr;
}

Lux::Core::Material* Lux::Core::Internal::ResourceHandlerDX11::CreateMaterial(const String& a_Name)
{
	Material* mat = new Material();
	mat->SetName(a_Name);
	AddMaterialToMap(a_Name, mat);
	return mat;
}

#if LUX_THREAD_SAFE == TRUE
void Lux::Core::Internal::ResourceHandlerDX11::AddMeshToMap(const String& a_Str, Mesh* a_Ent)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_MeshMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Mesh>(a_Ent)));
}
Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::GetMesh(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_MeshMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerDX11::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_MaterialMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Material>(a_Mat)));
}

Lux::Core::Material* Lux::Core::Internal::ResourceHandlerDX11::GetMaterial(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_MaterialMap.at(Key(a_Name)).get();
}

bool Lux::Core::Internal::ResourceHandlerDX11::MaterialExists(const String& a_Name)
{
	Key k(a_Name);
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	unsigned int count = m_MaterialMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::MeshExists(const String& a_Name)
{
	Key k(a_Name);
	std::unique_lock<std::mutex> lock(m_MeshMapMutex); // Upon construction of the lock the mutex will be immediately locked
	unsigned int count = m_MeshMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::TextureExists(const String& a_Name)
{
	Key k(a_Name);
	std::unique_lock<std::mutex> lock(m_TextureMapMutex); // Upon construction of the lock the mutex will be immediately locked
	unsigned int count = m_TextureMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerDX11::GetTexture(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	return m_TextureMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerDX11::AddTextureToMap(const String& a_Str, Texture* a_Tex)
{
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	m_TextureMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Texture>(a_Tex)));
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeleteTexture(const String& a_Name)
{
	if (!TextureExists(a_Name))
	{
		LUX_LOG(Utility::logWARNING) << "Could not delete texture with name. " << a_Name << " Texture doesn't exist.";
		return false;
	}
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	m_TextureMap.at(Key(a_Name)).reset();
	m_TextureMap.erase(Key(a_Name));

	return true;
}

void Lux::Core::Internal::ResourceHandlerDX11::AddFileNameToMap(const String& a_Str, Mesh* a_Ent)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex);
	m_LoadedFilenameMeshes.insert(std::make_pair(Key(a_Str), a_Ent));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::GetLoadedMesh(const String& a_FileStr)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex);
	MeshMapSimple::iterator it = m_LoadedFilenameMeshes.find(Key(a_FileStr));

	if (it != m_LoadedFilenameMeshes.end())
	{
		return it->second;
	}

	return nullptr;
}

void Lux::Core::Internal::ResourceHandlerDX11::AddShaderToMap(const String& a_Str, Shader* a_Shader)
{
	std::unique_lock<std::mutex> lock(m_ShaderMapMutex);
	m_ShaderMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Shader>(a_Shader)));
}

bool Lux::Core::Internal::ResourceHandlerDX11::ShaderExists(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_ShaderMapMutex);
	Key k(a_Name);
	unsigned int count = m_ShaderMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Lux::Core::Shader* Lux::Core::Internal::ResourceHandlerDX11::GetShader(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_ShaderMapMutex);
	return m_ShaderMap.at(Key(a_Name)).get();
}

#else
void Lux::Core::Internal::ResourceHandlerDX11::AddMeshToMap(const String& a_Str, Mesh* a_Ent)
{
	m_MeshMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Mesh>(a_Ent)));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::GetMesh(const String& a_Name)
{
	return m_MeshMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerDX11::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	m_MaterialMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Material>(a_Mat)));
}

Lux::Core::Material* Lux::Core::Internal::ResourceHandlerDX11::GetMaterial(const String& a_Name)
{
	return m_MaterialMap.at(Key(a_Name)).get();
}

bool Lux::Core::Internal::ResourceHandlerDX11::MaterialExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_MaterialMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::MeshExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_MeshMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::TextureExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_TextureMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerDX11::GetTexture(const String& a_Name)
{
	return m_TextureMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerDX11::AddTextureToMap(const String& a_Str, Texture* a_Tex)
{
	m_TextureMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Texture>(a_Tex)));
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeleteTexture(const String& a_Name)
{
	if (!TextureExists(a_Name))
	{
		LUX_LOG(Utility::logWARNING) << "Could not delete texture with name. " << a_Name << " Texture doesn't exist.";
		return false;
	}
	m_TextureMap.at(Key(a_Name)).reset();
	m_TextureMap.erase(Key(a_Name));

	return true;
}

void Lux::Core::Internal::ResourceHandlerDX11::AddFileNameToMap(const String& a_Str, Mesh* a_Ent)
{
	m_LoadedFilenameMeshes.insert(std::make_pair(Key(a_Str), a_Ent));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::GetLoadedMesh(const String& a_FileStr)
{
	MeshMapSimple::iterator it = m_LoadedFilenameMeshes.find(Key(a_FileStr));

	if (it != m_LoadedFilenameMeshes.end())
	{
		return it->second;
	}

	return nullptr;
}

void Lux::Core::Internal::ResourceHandlerDX11::AddShaderToMap(const String& a_Str, Shader* a_Shader)
{
	m_ShaderMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Shader>(a_Shader)));
}

bool Lux::Core::Internal::ResourceHandlerDX11::ShaderExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_ShaderMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Lux::Core::Shader* Lux::Core::Internal::ResourceHandlerDX11::GetShader(const String& a_Name)
{
	return m_ShaderMap.at(Key(a_Name)).get();
}

#endif // LUX_THREAD_SAFE

Lux::Core::Internal::ResourceHandlerDX11::ResourceHandlerDX11(RenderWindowDX11* a_RenderWindow) :
m_RenderWindow(a_RenderWindow)
{
	LUX_LOG(Utility::logINFO) << "Resource Handler created successfully.";
}
