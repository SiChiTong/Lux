#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxSystemFactory.h"

Lux::Core::SystemFactory::SystemFactory()
{

}

Lux::Core::SystemFactory::~SystemFactory()
{
	m_SystemTypeMap.clear();
}
