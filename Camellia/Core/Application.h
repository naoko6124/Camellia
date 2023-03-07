#pragma once

#include "Window.h"
#include "Renderer/Renderer.h"
#include "Assets/FileSystem.h"
#include "Assets/Model.h"

namespace Camellia
{
	class Application
	{
	public:
		void Run()
		{
			FileSystem fs;
			fs.Open("C:/Users/henri/Documents/Projects/teste.pak");
			char* teste = fs.Get("export.rmd");
			float* model = LoadRMD(teste);
			std::string texto = std::to_string(model[0]);
			texto.append(",").append(std::to_string(model[1]));
			texto.append(",").append(std::to_string(model[2]));
			MessageBox(0, texto.c_str(), "Teste", 0);
			delete[] model;
			fs.Close();

			mWindow.Init();
			mRenderer.Init(mWindow.hWnd);
			ID3DXMesh* mMesh = mRenderer.CreateMesh();
			while (mWindow.isRunning)
			{
				mWindow.GetEvents();
				mRenderer.BeginScene();
				mRenderer.DrawMesh(mMesh);
				mRenderer.EndScene();
			}
		}
	private:
		Window mWindow;
		Renderer mRenderer;
	};
}