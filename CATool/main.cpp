#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("usage:\n");
		printf("- CATool create [YOUR FS NAME].pak FILE1.TXT FILE2.PNG FILE3.BMP ...\n");
		printf("- CATool export [YOUR FS NAME].pak FILE.TXT\n");
		printf("- CATool export [YOUR FS NAME].pak\n");
		printf("- CATool list [YOUR FS NAME].pak\n");
		return -1;
	}

	if (strcmp(argv[1], "create") == 0)
	{
		printf("Creating %s\n\n", argv[2]);
		std::ofstream pak(argv[2]);
		
		int count = (argc - 3);
		int offset = (count * 40 + 8);

		pak.write((const char*)&count, 4);
		pak.write((const char*)&offset, 4);

		for (int i = 3; i < argc; i++)
		{
			printf("Adding file header of %s: ", argv[i]);
			std::string filename(argv[i]);
			pak.write(filename.c_str(), 32);
			std::ifstream file(argv[i]);
			if (!file)
			{
				printf("failed!");
				continue;
			}
			file.seekg(0, std::ios::end);
			int length = file.tellg();
			printf("%d bytes\n", length);
			pak.write((const char*)&offset, 4);
			pak.write((const char*)&length, 4);
			offset += length;
			file.close();
		}

		for (int i = 3; i < argc; i++)
		{
			printf("Adding file content of %s\n", argv[i]);
			std::ifstream file(argv[i]);
			if (!file)
			{
				printf("failed!");
				continue;
			}
			file.seekg(0, std::ios::end);
			int length = file.tellg();
			char* data = new char[length];
			file.seekg(0);
			file.read(data, length);
			pak.write((const char*)data, length);
			delete[] data;
			file.close();
		}

		pak.close();
		return 0;
	}

	if (strcmp(argv[1], "extract") == 0)
	{
		std::ifstream pak(argv[2]);

		if (!pak)
		{
			printf("File %s not found!", argv[2]);
			return -1;
		}

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

			if (argc == 4 && strcmp(filename, argv[3]) != 0)
				continue;

			printf("Extracting %s\n", filename);
			int length;
			pak.seekg(8 + 40 * i + 32);
			pak.read((char*)&offset, 4);
			pak.seekg(8 + 40 * i + 32 + 4);
			pak.read((char*)&length, 4);
			
			char* data = new char[length];
			pak.seekg(offset);
			pak.read(data, length);

			std::ofstream file(filename);
			file.write((const char*)data, length);
			file.close();
			delete[] data;
		}

		pak.close();
		return 0;
	}


	if (strcmp(argv[1], "list") == 0)
	{
		std::ifstream pak(argv[2]);

		if (!pak)
		{
			printf("File %s not found!", argv[2]);
			return -1;
		}

		int count, offset;
		pak.seekg(0);
		pak.read((char*)&count, 4);
		pak.seekg(4);
		pak.read((char*)&offset, 4);

		printf("Files (%d):\n", count);

		for (int i = 0; i < count; i++)
		{
			char filename[32];
			pak.seekg(8 + 40 * i);
			pak.read((char*)&filename, 32);
			printf("- %s\n", filename);
		}

		pak.close();
		return 0;
	}

	printf("Unknown command: %s", argv[1]);
	
	return 0;
}