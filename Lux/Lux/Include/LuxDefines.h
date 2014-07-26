#ifndef LUX_DEFINES_H
#define LUX_DEFINES_H

#define LuxAssert(a) assert(a)

#define LUX_THREAD_SAFE FALSE
#define LUX_LOGGING_ENABLED TRUE
#define LUX_ENTITY_MEMORY_POOL_INITIAL_SIZE 100
#define LUX_ENTITY_MEMORY_POOL_GROW_AMOUNT 20
#define LUX_COMPONENT_MEMORY_POOL_INITIAL_SIZE 100
#define LUX_COMPONENT_MEMORY_POOL_GROW_AMOUNT 20

namespace Lux
{
	using namespace glm;

	// Some typedefs
	typedef std::string String;
	typedef long long int64;
	typedef unsigned int TextureIndex;
}

#endif