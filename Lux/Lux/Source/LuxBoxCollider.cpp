#include "LuxPCH.h"
#include "LuxBoxCollider.h"

Lux::Physics::BoxCollider::BoxCollider() : Collider(), m_HalfExtents(0.5f)
{

}

Lux::Physics::BoxCollider::BoxCollider(const BoxCollider& a_Other)
{
	m_HalfExtents = a_Other.m_HalfExtents;
	m_Shape = a_Other.m_Shape;
	m_IsTrigger = a_Other.m_IsTrigger;
}

Lux::Physics::BoxCollider::~BoxCollider()
{

}

void Lux::Physics::BoxCollider::Reset()
{
	Collider::Reset();
	m_HalfExtents = vec3(1.0f);
}

const Lux::vec3 Lux::Physics::BoxCollider::GetHalfExtents() const
{
	return m_HalfExtents;
}

void Lux::Physics::BoxCollider::SetHalfExtents(const vec3& a_HlfExtents)
{
	LuxAssert(a_HlfExtents != vec3(0));
	m_HalfExtents = a_HlfExtents;

	if (m_Shape)
	{
		m_Shape->setGeometry(PxBoxGeometry(Utility::ConvertVec3ToPhysX(m_HalfExtents)));
	}
}
