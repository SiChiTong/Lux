#ifndef LUX_TEXTURE_2D_OGL_H
#define LUX_TEXTURE_2D_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class Texture2DOGL : public Texture2D
			{
			public:
				Texture2DOGL(unsigned int a_ImgWidth, unsigned int a_ImgHeight, unsigned char* a_Bits);
				virtual ~Texture2DOGL();

				virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program);
				virtual void Unbind();

			private:
				unsigned int m_TextureID;
				unsigned int m_LastSlot;
			};
		}
	}
}

#endif