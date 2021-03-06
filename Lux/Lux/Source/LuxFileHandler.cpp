#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShaderFileParser.h"
#include "LuxFileHandler.h"

Lux::Core::FileHandler& Lux::Core::FileHandler::GetInstance()
{
	static FileHandler instance;
	return instance;
}

Lux::Core::FileHandler::~FileHandler()
{
	if (PHYSFS_isInit())
	{
		int res = PHYSFS_deinit();

		if (res == 0)
		{
			String errstr("Error destroying File System class : ");
			errstr.append(PHYSFS_getLastError());
			Utility::ThrowError(errstr);
		}
	}
}

Lux::Core::FileHandler::FileHandler()
{
	int res = PHYSFS_init(nullptr);

	if (res == 0)
	{
		String errstr("Error initializing File System class : ");
		errstr.append(PHYSFS_getLastError());
		Utility::ThrowError(errstr);
	}
	LUX_LOG(Utility::logINFO) << "File system created successfully.";
}

bool Lux::Core::FileHandler::AddResourcePath(const String a_Path)
{
	int res = PHYSFS_addToSearchPath(a_Path.c_str(), 0);

	if (res == 0)
	{
		LUX_LOG(Utility::logWARNING) << "Directory: " << a_Path << " was not added to the search path. " << PHYSFS_getLastError();
		return false;
	}

	return true;
}

bool Lux::Core::FileHandler::RemoveResourcePath(const String a_Path)
{
	int res = PHYSFS_removeFromSearchPath(a_Path.c_str());

	if (res == 0)
	{
		LUX_LOG(Utility::logWARNING) << "Directory " << a_Path << " was not removed from the search path. " << PHYSFS_getLastError();
		return false;
	}

	return true;
}

bool Lux::Core::FileHandler::ResourcePathExists(const String a_Path)
{
	char** path = PHYSFS_getSearchPath();

	if (path == nullptr)
	{
		String errstr("Error fetching the search path. ");
		errstr.append(PHYSFS_getLastError());
		Utility::ThrowError(errstr);
	}

	// Search for the string inside the current search path
	bool retval = false;
	for (char** i = path; i != nullptr; i++)
	{
		if (a_Path.compare(*i) == 0)
		{
			retval = true;
			break;
		}
	}
	PHYSFS_freeList(path); // Free the list
	return retval;
}

bool Lux::Core::FileHandler::FileExists(const String a_Path)
{
	int res = PHYSFS_exists(a_Path.c_str());

	if (res == 0)
	{
		return false;
	}

	return true;
}

Lux::Core::OpenedFile* Lux::Core::FileHandler::OpenFile(const String a_File, FileOpenMode a_OpenMode)
{
	OpenedFile* retFile = nullptr;
	switch (a_OpenMode)
	{
	case Lux::Core::FileHandler::FILE_OPEN_READ:
		retFile = PHYSFS_openRead(a_File.c_str());
		break;
	case Lux::Core::FileHandler::FILE_OPEN_WRITE:
		retFile = PHYSFS_openWrite(a_File.c_str());
		break;
	case Lux::Core::FileHandler::FILE_OPEN_APPEND:
		retFile = PHYSFS_openAppend(a_File.c_str());
		break;
	default:
		Utility::ThrowError("Unsupported file open mode.");
		break;
	}

	if (retFile == nullptr)
	{
		Utility::ThrowError("Could not open specified file: " + a_File + " . " + PHYSFS_getLastError());
	}

	return retFile;
}

Lux::Core::FileInfo* Lux::Core::FileHandler::LoadFileInMemory(const String a_Path)
{
	FileInfo* retInfo = nullptr;

	OpenedFile* filePtr = OpenFile(a_Path, FILE_OPEN_READ);
	int64 fileSize = GetFileLength(filePtr);

	retInfo = new FileInfo((unsigned int)fileSize);
	LoadFileDataInto(filePtr, retInfo);
	CloseOpenedFile(filePtr);
	return retInfo;
}

Lux::int64 Lux::Core::FileHandler::GetFileLength(OpenedFile* a_File)
{
	int64 ret = PHYSFS_fileLength(a_File);

	if (ret == -1)
	{
		String errstr("Could not get file length. ");
		errstr.append(PHYSFS_getLastError());
		Utility::ThrowError(errstr);
	}

	return ret;
}

void Lux::Core::FileHandler::LoadFileDataInto(OpenedFile* a_File, FileInfo* a_Dest)
{
	int64 length_read = PHYSFS_read(a_File, a_Dest->m_RawData, a_Dest->m_DataLength, 1);

	if (length_read != 1)
	{
		String errstr("Could not read file data into memory. ");
		errstr.append(PHYSFS_getLastError());
		Utility::ThrowError(errstr);
	}
}

void Lux::Core::FileHandler::DeleteLoadedFileInfo(FileInfo* a_FileInfo)
{
	Utility::SafePtrDelete(a_FileInfo);
}

void Lux::Core::FileHandler::CloseOpenedFile(OpenedFile* a_File)
{
	if (a_File == nullptr)
	{
		return;
	}

	int res = PHYSFS_close(a_File);

	if (res == 0)
	{
		String errstr("Could not close file. ");
		errstr.append(PHYSFS_getLastError());
		Utility::ThrowError(errstr);
	}

	a_File = nullptr;
}

Lux::Core::ShaderFileParser& Lux::Core::FileHandler::GetShaderParser(Key& a_Key)
{
	return m_ShaderParserMap.at(a_Key);
}

const bool Lux::Core::FileHandler::ShaderParserExists(Key& a_Key)
{
	int count = m_ShaderParserMap.count(a_Key);

	if (count > 0)
		return true;

	return false;
}

void Lux::Core::FileHandler::CreateShaderParser(Key& a_Key)
{
	m_ShaderParserMap.insert(std::make_pair(a_Key, ShaderFileParser()));
}

void Lux::Core::FileHandler::DestroyShaderParser(Key& a_Key)
{
	m_ShaderParserMap.erase(a_Key);
}
