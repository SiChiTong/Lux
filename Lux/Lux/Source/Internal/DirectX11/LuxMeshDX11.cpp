#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxSubMesh.h"
#include "LuxSubMeshDX11.h"
#include "LuxMeshAnimation.h"
#include "LuxMesh.h"
#include "LuxMeshDX11.h"

Lux::Core::Internal::MeshDX11::MeshDX11() : 
m_NumSubMeshes(0),
m_AnimationData(nullptr),
m_SubMeshes(nullptr),
m_NumAnimations(0)
{

}

Lux::Core::Internal::MeshDX11::MeshDX11(unsigned int a_NumMeshes, unsigned int a_NumAnims) :
m_NumSubMeshes(a_NumMeshes),
m_AnimationData(nullptr),
m_NumAnimations(a_NumAnims)
{
	if (m_NumSubMeshes)
	{
		m_SubMeshes = new SubMeshDX11*[m_NumSubMeshes];
	}

	if (m_NumAnimations)
	{
		m_AnimationData = new MeshAnimation*[m_NumAnimations];
	}
}

Lux::Core::Internal::MeshDX11::~MeshDX11()
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

void Lux::Core::Internal::MeshDX11::AddSubMesh(SubMesh* a_Mesh)
{
	LuxAssert(a_Mesh);
	static unsigned int ctr = 0;

	if (ctr >= m_NumSubMeshes)
	{
		Utility::ThrowError("Could not insert mesh in entity. Mesh index too high.");
	}

	m_SubMeshes[ctr] = (SubMeshDX11*)a_Mesh;
	ctr++;
}

void Lux::Core::Internal::MeshDX11::AddAnimation(MeshAnimation* a_Anim)
{
	LuxAssert(a_Anim);
	static unsigned int ctr = 0;

	if (ctr >= m_NumSubMeshes)
	{
		Utility::ThrowError("Could not insert animation in entity. Animation index too high.");
	}

	m_AnimationData[ctr] = a_Anim;
	ctr++;
}

void Lux::Core::Internal::MeshDX11::ConnectWithShader(Shader* a_Shader)
{

}

const unsigned int Lux::Core::Internal::MeshDX11::GetNumSubMeshes()
{
	return m_NumSubMeshes;
}

Lux::Core::SubMesh* Lux::Core::Internal::MeshDX11::GetSubMesh(const unsigned int a_Index)
{
	return m_SubMeshes[a_Index];
}