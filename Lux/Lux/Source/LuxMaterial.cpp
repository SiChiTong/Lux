#include "LuxPCH.h"
#include "LuxMaterial.h"


Lux::Core::Material::Material() :
m_Opacity(1.0f),
m_Reflectivity(0.0f),
m_Shininess(30.0f),
m_ShininessStrength(0.0f),
m_RefractionIndex(0.0f),
m_DiffuseColor(0.5f, 0.5f, 0.5f, 1.0f),
m_AmbientColor(0.1f, 0.1f, 0.1f, 1.0f),
m_EmissiveColor(0.0f, 0.0f, 0.0f, 1.0f),
m_SpecularColor(0.5f, 0.5f, 0.5f, 1.0f),
m_TransparentColor(0.0f, 0.0f, 0.0f, 0.0f)
{

}

Lux::Core::Material::Material(aiMaterial& a_Mat)
{
	aiString str;
	aiReturn retval;

	aiString name;
	retval = a_Mat.Get(AI_MATKEY_NAME, name);

	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the material name from file. The field will have its default value.";
	}
	else
	{
		m_Name = name.C_Str();
	}

	// Diffuse
	aiColor4D col;
	retval = a_Mat.Get(AI_MATKEY_COLOR_DIFFUSE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the diffuse color from file. The field will have its default value.";
	}
	else
	{
		m_DiffuseColor = Utility::ConvertVec4Assimp(col);
	}

	//Specular
	retval = a_Mat.Get(AI_MATKEY_COLOR_SPECULAR, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the specular color from file. The field will have its default value.";
	}
	else
	{
		m_SpecularColor = Utility::ConvertVec4Assimp(col);
	}

	// Emissive
	retval = a_Mat.Get(AI_MATKEY_COLOR_EMISSIVE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the emissive color from file. The field will have its default value.";
	}
	else
	{
		m_EmissiveColor = Utility::ConvertVec4Assimp(col);
	}

	// Reflective
	retval = a_Mat.Get(AI_MATKEY_COLOR_REFLECTIVE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the reflective color from file. The field will have its default value.";
	}
	else
	{
		m_ReflectiveColor = Utility::ConvertVec4Assimp(col);
	}

	// Ambient
	retval = a_Mat.Get(AI_MATKEY_COLOR_AMBIENT, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the ambient color from file. The field will have its default value.";
	}
	else
	{
		m_AmbientColor = Utility::ConvertVec4Assimp(col);
	}

	// Transparent
	retval = a_Mat.Get(AI_MATKEY_COLOR_TRANSPARENT, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the transparent color from file. The field will have its default value.";
	}
	else
	{
		m_AmbientColor = Utility::ConvertVec4Assimp(col);
	}

	// Shininess
	retval = a_Mat.Get(AI_MATKEY_SHININESS, m_Shininess);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the shininess from file. The field will have its default value.";
	}

	// Shininess strength
	retval = a_Mat.Get(AI_MATKEY_SHININESS_STRENGTH, m_ShininessStrength);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the shininess strength from file. The field will have its default value.";
	}

	// Refractiveness
	retval = a_Mat.Get(AI_MATKEY_REFRACTI, m_RefractionIndex);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the refraction index from file. The field will have its default value.";
	}

	// Opacity
	retval = a_Mat.Get(AI_MATKEY_OPACITY, m_Opacity);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the opacity from file. The field will have its default value.";
	}

	// Reflection
	retval = a_Mat.Get(AI_MATKEY_REFLECTIVITY, m_Reflectivity);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the reflectivity from file. The field will have its default value.";
	}
}

Lux::Core::Material::Material(const Material& a_Other)
{
	m_DiffuseColor = a_Other.m_DiffuseColor;
	m_SpecularColor = a_Other.m_SpecularColor;
	m_EmissiveColor = a_Other.m_EmissiveColor;
	m_AmbientColor = a_Other.m_AmbientColor;
	m_TransparentColor = a_Other.m_TransparentColor;
	m_ReflectiveColor = a_Other.m_ReflectiveColor;
	m_Shininess = a_Other.m_Shininess;
	m_Reflectivity = a_Other.m_Reflectivity;
	m_ShininessStrength = a_Other.m_ShininessStrength;
	m_Opacity = a_Other.m_Opacity;
	m_RefractionIndex = a_Other.m_RefractionIndex;
	m_Name = a_Other.m_Name;
}

Lux::Core::Material::~Material()
{

}

const Lux::vec4& Lux::Core::Material::GetDiffuseColor() const
{
	return m_DiffuseColor;
}

const Lux::vec4& Lux::Core::Material::GetSpecularColor() const
{
	return m_SpecularColor;
}

const Lux::vec4& Lux::Core::Material::GetEmissiveColor() const
{
	return m_EmissiveColor;
}

const Lux::vec4& Lux::Core::Material::GetAmbientColor() const
{
	return m_AmbientColor;
}

const Lux::vec4& Lux::Core::Material::GetReflectiveColor() const
{
	return m_ReflectiveColor;
}

const Lux::vec4& Lux::Core::Material::GetTransparentColor() const
{
	return m_TransparentColor;
}

const float Lux::Core::Material::GetShininess() const
{
	return m_Shininess;
}

const float Lux::Core::Material::GetShininessStrength() const
{
	return m_ShininessStrength;
}

const float Lux::Core::Material::GetReflectivity() const
{
	return m_Reflectivity;
}

const float Lux::Core::Material::GetRefraction() const
{
	return m_RefractionIndex;
}

const float Lux::Core::Material::GetOpacity() const
{
	return m_Opacity;
}

void Lux::Core::Material::SetDiffuseColor(const vec4& a_Col)
{
	m_DiffuseColor = a_Col;
}

void Lux::Core::Material::SetSpecularColor(const vec4& a_Col)
{
	m_SpecularColor = a_Col;
}

void Lux::Core::Material::SetEmissiveColor(const vec4& a_Col)
{
	m_EmissiveColor = a_Col;
}

void Lux::Core::Material::SetAmbientColor(const vec4& a_Col)
{
	m_AmbientColor = a_Col;
}

void Lux::Core::Material::SetReflectiveColor(const vec4& a_Col)
{
	m_ReflectiveColor = a_Col;
}

void Lux::Core::Material::SetTransparentColor(const vec4& a_Col)
{
	m_TransparentColor = a_Col;
}

void Lux::Core::Material::SetShininess(float a_Factor)
{
	m_Shininess = a_Factor;
}

void Lux::Core::Material::SetShininessStrength(float a_Factor)
{
	m_ShininessStrength = a_Factor;
}

void Lux::Core::Material::SetReflectivity(float a_Factor)
{
	m_Reflectivity = a_Factor;
}

void Lux::Core::Material::SetRefraction(float a_Factor)
{
	m_RefractionIndex = a_Factor;
}

void Lux::Core::Material::SetOpacity(float a_Factor)
{
	m_Opacity = a_Factor;
}

Lux::Core::Material & Lux::Core::Material::operator=(const Material& a_Other)
{
	m_DiffuseColor = a_Other.m_DiffuseColor;
	m_SpecularColor = a_Other.m_SpecularColor;
	m_EmissiveColor = a_Other.m_EmissiveColor;
	m_AmbientColor = a_Other.m_AmbientColor;
	m_TransparentColor = a_Other.m_TransparentColor;
	m_ReflectiveColor = a_Other.m_ReflectiveColor;
	m_Shininess = a_Other.m_Shininess;
	m_Reflectivity = a_Other.m_Reflectivity;
	m_ShininessStrength = a_Other.m_ShininessStrength;
	m_Opacity = a_Other.m_Opacity;
	m_RefractionIndex = a_Other.m_RefractionIndex;
	m_Name = a_Other.m_Name;
	return *this;
}

const Lux::String Lux::Core::Material::GetName() const
{
	return m_Name;
}

void Lux::Core::Material::SetName(const String& a_Name)
{
	m_Name = a_Name;
}
