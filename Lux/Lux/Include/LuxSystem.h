#ifndef LUX_SYSTEM_H
#define LUX_SYSTEM_H

namespace Lux
{
	class SceneManager;
	class System
	{
	public:
		System();
		virtual ~System();

		virtual void ProcessUpdate(const float a_DeltaTime) = 0;

		virtual bool Init(SceneManager* a_SceneManager);
	protected:
		SceneManager* m_SceneManager;
	};
}

#endif