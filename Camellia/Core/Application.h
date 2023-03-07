#pragma once

#include "Window.h"
#include "Renderer/Renderer.h"
#include "Assets/FileSystem.h"

namespace Camellia
{
	class Application
	{
	public:
		void Run()
		{
			FileSystem fs;
			fs.Open("C:/Users/henri/Documents/Projects/teste.pak");
			char* teste = fs.Get("oi.txt");
			MessageBox(0, teste, "oi.txt", 0);
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