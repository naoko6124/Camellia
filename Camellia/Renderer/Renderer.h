#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace Camellia
{
	class Renderer
	{
	public:
		void Init(HWND hWnd)
		{
			mD3D = Direct3DCreate9(D3D_SDK_VERSION);

			D3DPRESENT_PARAMETERS d3dpp = { 0 };
			d3dpp.BackBufferWidth = 800;
			d3dpp.BackBufferHeight = 600;
			d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.hDeviceWindow = hWnd;
			d3dpp.Windowed = TRUE;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
			d3dpp.Flags = 0;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

			mD3D->CreateDevice(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &mDevice);
		}
		void BeginScene()
		{
			mDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			mDevice->BeginScene();
		}
		void EndScene()
		{
			mDevice->EndScene();
			mDevice->Present(0, 0, 0, 0);
		}
		ID3DXMesh* CreateMesh()
		{
			ID3DXMesh* mesh;
			D3DXCreateTeapot(mDevice, &mesh, 0);
			D3DXCreateMeshFVF(12, 8, D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL, mDevice, &mesh);

			D3DXVECTOR3 vertices[8][2] = {
				{ { -1.0f, -1.0f, -1.0f }, {-2.0f, -2.0f, -2.0f} },
				{ { -1.0f,  1.0f, -1.0f }, {-2.0f,  2.0f, -2.0f} },
				{ {  1.0f,  1.0f, -1.0f }, { 2.0f,  2.0f, -2.0f} },
				{ {  1.0f, -1.0f, -1.0f }, { 2.0f, -2.0f, -2.0f} },
				{ { -1.0f, -1.0f,  1.0f }, {-2.0f, -2.0f,  2.0f} },
				{ { -1.0f,  1.0f,  1.0f }, {-2.0f,  2.0f,  2.0f} },
				{ {  1.0f,  1.0f,  1.0f }, { 2.0f,  2.0f,  2.0f} },
				{ {  1.0f, -1.0f,  1.0f }, { 2.0f, -2.0f,  2.0f} },
			};

			D3DXVECTOR3* vertexBuffer;
			mesh->LockVertexBuffer(0, (void**)&vertexBuffer);
			memcpy(vertexBuffer, &vertices, sizeof(vertices));
			mesh->UnlockVertexBuffer();

			WORD indices[] = {
				0, 1, 2, 2, 3, 0,
				7, 6, 5, 5, 4, 7,
				3, 2, 6, 6, 7, 3,
				4, 5, 1, 1, 0, 4,
				4, 0, 3, 3, 7, 4,
				1, 5, 6, 6, 2, 1
			};

			WORD* indexBuffer = 0;
			mesh->LockIndexBuffer(0, (void**)&indexBuffer);
			memcpy(indexBuffer, &indices, sizeof(indices));
			mesh->UnlockIndexBuffer();

			return mesh;
		}
		float rot = 0.0f;
		void DrawMesh(ID3DXMesh* mesh)
		{
			D3DXMATRIX model;
			D3DXMatrixRotationY(&model, rot);
			rot += 0.0005f;
			mDevice->SetTransform(D3DTS_WORLD, &model);

			D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
			D3DXVECTOR3 targetPoint(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 worldUp(0.0f, 1.0f, 0.0f);
			D3DXMATRIX V;
			D3DXMatrixLookAtLH(&V, &position, &targetPoint, &worldUp);
			mDevice->SetTransform(D3DTS_VIEW, &V);

			D3DXMATRIX proj;
			D3DXMatrixPerspectiveFovLH(
				&proj, D3DXToRadian(60.0f), (float)320 / (float)240, 1.0, 1000.0f);
			mDevice->SetTransform(D3DTS_PROJECTION, &proj);

			mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			//mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			mDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
			mDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

			D3DXCOLOR lightColor(1.0f, 1.0f, 1.0f, 1.0f);
			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(light));
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Ambient = lightColor * 0.4f;
			light.Diffuse = lightColor;
			light.Specular = lightColor * 0.6f;
			light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			mDevice->SetLight(0, &light);
			mDevice->LightEnable(0, TRUE);

			D3DMATERIAL9 white = { 0 };
			white.Diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
			white.Ambient = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
			white.Specular = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
			white.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			white.Power = 5.0f;

			mDevice->SetMaterial(&white);

			mesh->DrawSubset(0);
		}
	private:
		IDirect3D9* mD3D;
		IDirect3DDevice9* mDevice;
	};
}