#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <sstream>

namespace Camellia
{
	float* LoadRMD(char* bytearray)
	{
		int size = sizeof(bytearray)/4;
		std::stringstream data;
		data << bytearray;
		float* vertices = new float[size];
		/*int i = 0;
		while (i < size)
		{
			data.seekg(i * 4);
			data.read((char*)&vertices[i++], 4);
		}*/
		float poura;
		data.seekg(0);
		data.read((char*)&poura, 4);
		std::string texto = std::to_string(poura);
		MessageBox(0, texto.c_str(), "Teste", 0);
		data.seekg(4);
		data.read((char*)&vertices[1], 4);
		data.seekg(8);
		data.read((char*)&vertices[2], 4);
		return vertices;
	}
}