#pragma once

#include <fstream>
#include <unordered_map>

namespace Camellia
{
	class FileSystem
	{
	public:
		bool Open(const char* path)
		{
			std::ifstream pak(path);
			if (!pak)
				return false;

			int count, offset;
			pak.seekg(0);
			pak.read((char*)&count, 4);
			pak.seekg(4);
			pak.read((char*)&offset, 4);

			for (int i = 0; i < count; i++)
			{
				char filename[32];
				pak.seekg(8 + 40 * i);
				pak.read((char*)&filename, 32);

				int length;
				pak.seekg(8 + 40 * i + 32);
				pak.read((char*)&offset, 4);
				pak.seekg(8 + 40 * i + 32 + 4);
				pak.read((char*)&length, 4);

				char* data = new char[length];
				pak.seekg(offset);
				pak.read(data, length);
				files[filename] = data;
			}

			pak.close();
			return true;
		}
		void Close()
		{
			for (auto file : files)
			{
				delete[] file.second;
			}
		}
		char* Get(char* filename)
		{
			return files[filename];
		}
	private:
		std::unordered_map<std::string, char*> files;
	};
}