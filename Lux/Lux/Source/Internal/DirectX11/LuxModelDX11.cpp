#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxMesh.h"
#include "LuxMeshDX11.h"
#include "LuxMeshAnimation.h"
#include "LuxModel.h"
#include "LuxModelDX11.h"

Lux::Core::Internal::ModelDX11::ModelDX11() : 
m_NumSubMeshes(0),
m_AnimationData(nullptr),
m_SubMeshes(nullptr),
m_NumAnimations(0),
m_AnimCtr(0),
m_SubMeshCtr(0)
{

}

Lux::Core::Internal::ModelDX11::ModelDX11(unsigned int a_NumMeshes, unsigned int a_NumAnims) :
m_NumSubMeshes(a_NumMeshes),
m_AnimationData(nullptr),
m_NumAnimations(a_NumAnims),
m_AnimCtr(0),
m_SubMeshCtr(0)
{
	if (m_NumSubMeshes)
	{
		m_SubMeshes = new MeshDX11*[m_NumSubMeshes];
	}

	if (m_NumAnimations)
	{
		m_AnimationData = new MeshAnimation*[m_NumAnimations];
	}
}

Lux::Core::Internal::ModelDX11::~ModelDX11()
{
	for (unsigned int i = 0; i < m_NumSubMeshes; i++)
	{
		Utility::SafePtrDelete(m_SubMeshes[i]);
	}
	Utility::SafeArrayDelete(m_SubMeshes);

	for (unsigned int i = 0; i < m_NumAnimations; i++)
	{
		Utility::SafePtrDelete(m_AnimationData[i]);
	}
	Utility::SafeArrayDelete(m_AnimationData);

	m_NumSubMeshes = 0;
	m_NumAnimations = 0;
}

void Lux::Core::Internal::ModelDX11::AddMesh(Mesh* a_Mesh)
{
	LuxAssert(a_Mesh);

	if (m_SubMeshCtr >= m_NumSubMeshes)
	{
		Utility::ThrowError("Could not insert mesh in entity. Mesh index too high.");
	}

	m_SubMeshes[m_SubMeshCtr] = (MeshDX11*)a_Mesh;
	m_SubMeshCtr++;
}

void Lux::Core::Internal::ModelDX11::AddAnimation(MeshAnimation* a_Anim)
{
	LuxAssert(a_Anim);

	if (m_AnimCtr >= m_NumSubMeshes)
	{
		Utility::ThrowError("Could not insert animation in entity. Animation index too high.");
	}

	m_AnimationData[m_AnimCtr] = a_Anim;
	m_AnimCtr++;
}

const unsigned int Lux::Core::Internal::ModelDX11::GetNumMeshes()
{
	return m_NumSubMeshes;
}

Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ModelDX11::GetMesh(const unsigned int a_Index)
{
	return ObserverPtr<Mesh>(m_SubMeshes[a_Index]);
}

void Lux::Core::Internal::ModelDX11::SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat)
{
	for (unsigned int i = 0; i < m_NumSubMeshes; i++)
	{
		m_SubMeshes[i]->SetMaterialProperties(a_Mat);
	}
}

void Lux::Core::Internal::ModelDX11::SetShader(ObserverPtr<Shader>& a_Shader)
{
	for (unsigned int i = 0; i < m_NumSubMeshes; i++)
	{
		m_SubMeshes[i]->SetShader(a_Shader);
	}
}

void Lux::Core::Internal::ModelDX11::SetTexture(TextureIndex a_Idx, Core::ObserverPtr<Texture2D>& a_Tex)
{
	for (unsigned int i = 0; i < m_NumSubMeshes; i++)
	{
		m_SubMeshes[i]->SetTexture(a_Idx, a_Tex);
	}
}
