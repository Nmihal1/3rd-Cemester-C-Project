#include <windowsx.h>
#include "MD2Loader.h"
#include "Rasteriser.h"
#include "time.h"
#include "WinUser.h"

Rasteriser app;

//Implementations of Rasterizer behaviour

void Rasteriser::AddModel(Model& model)
{
	_models.push_back(model);
}

void Rasteriser::TransformModel(int model_inx, Matrix transform)
{
	if (model_inx > _models.size() - 1)
		return;

	_models[model_inx].ApplyTransform(transform);
}

void Rasteriser::SetViewTransform(Matrix view)
{
	_view.SetMatrix(view);
}

void Rasteriser::SetCameraTransform(Matrix cam)
{
	_camera.SetMatrix(cam);
}

void Rasteriser::SetProjectionTransform(Matrix proj)
{
	_projection.SetMatrix(proj);
}

void Rasteriser::SetViewport(int left, int right, int top, int bottom)
{
	_viewportrect.left = left;
	_viewportrect.top = top;
	_viewportrect.right = right;
	_viewportrect.bottom = bottom;
	_viewport.SetViewport(left, right, top, bottom);
}

void Rasteriser::DrawViewportFrame(HDC hdc)
{
	POINT points[4];
	//Draw Viewport Frame
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	points[0].x = (int)_viewportrect.left;      points[0].y = (int)_viewportrect.top;
	points[1].x = (int)_viewportrect.right - 1; points[1].y = (int)_viewportrect.top;
	points[2].x = (int)_viewportrect.right - 1; points[2].y = (int)_viewportrect.bottom - 1;
	points[3].x = (int)_viewportrect.left;      points[3].y = (int)_viewportrect.bottom - 1;

	Polygon(hdc, points, 4);

	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);

	pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hdc, pen);

	int width = (int)_viewportrect.right - (int)_viewportrect.left;
	int height = (int)_viewportrect.bottom - (int)_viewportrect.top;

	MoveToEx(hdc, (int)_viewportrect.left, (int)_viewportrect.top + height / 2, NULL);
	LineTo(hdc, (int)_viewportrect.right - 1, (int)_viewportrect.top + height / 2);

	MoveToEx(hdc, (int)_viewportrect.left + width / 2, (int)_viewportrect.top, NULL);
	LineTo(hdc, (int)_viewportrect.left + width / 2, (int)_viewportrect.bottom - 1);


	SelectObject(hdc, oldPen);
	DeleteObject(pen);
}

void Rasteriser::DrawTextFrame(HDC hdc)
{
	POINT points[4];
	wchar_t outstr[256];

	//Draw Text Frame
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	points[0].x = 700;       points[0].y = 50;
	points[1].x = 700 + 500; points[1].y = 50;
	points[2].x = 700 + 500; points[2].y = 50 + 180;
	points[3].x = 700;       points[3].y = 50 + 180;

	Polygon(hdc, points, 4);

	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);


	switch (_render_mode)
	{
	case  0: DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Draw: Wireframe");
		break;
	case  1: DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(128, 0, 0), L"Draw: Solid-Fill");
		break;
	case  2: DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(128, 0, 128), L"Draw: Flat-Shading");
		break;
	case  3: DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(128, 0, 128), L"Draw: Vertex-Shading");
		break;
	default: DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 128, 0), L"Draw: Wireframe");
		break;
	}

	if (_depthsort)	DrawString(hdc, 700 + 10, 50 + 30, 18, RGB(0, 128, 0), L"Depth Sorting: ON");
	else DrawString(hdc, 700 + 10, 50 + 30, 18, RGB(255, 0, 0), L"Depth Sorting: OFF");

	if (_facecull)	DrawString(hdc, 700 + 10, 50 + 50, 18, RGB(0, 128, 0), L"Back-face Culling: ON");
	else DrawString(hdc, 700 + 10, 50 + 50, 18, RGB(255, 0, 0), L"Back-face Culling: OFF");

	if (_ortho)	DrawString(hdc, 700 + 10, 50 + 70, 18, RGB(128, 128, 128), L"Projection: Ortho");
	else DrawString(hdc, 700 + 10, 50 + 70, 18, RGB(96, 96, 96), L"Projection: Perspective");

	swprintf(outstr, 256, L"Camera: RotX=%.2f; RotY=%.2f; RotZ=%.2f; PosZ=%.2f;\0", _camRx, _camRy, _camRz, _camZ);
	DrawString(hdc, 700 + 10, 50 + 90, 18, RGB(64, 64, 64), (LPCTSTR)outstr);

	if (_directional)	DrawString(hdc, 700 + 10, 50 + 110, 18, RGB(128, 128, 0), L"Light: Directional");
	else DrawString(hdc, 700 + 10, 50 + 110, 18, RGB(0, 192, 0), L"Light: Point-light");

	/*swprintf(outstr, 256, L"Light Position: PosX=%.2f; PosY=%.2f; PosZ=%.2f;\0",
		_light.GetLPos().GetX(), _light.GetLPos().GetY(), _light.GetLPos().GetZ());
	DrawString(hdc, 700 + 10, 50 + 130, 18, RGB(64, 64, 0), (LPCTSTR)outstr);

	swprintf(outstr, 256, L"Light Color: La(%d, %d, %d); Ld(%d, %d, %d); Ls(%d, %d, %d);\0",
		GetRValue(_light.GetColorLa()), GetGValue(_light.GetColorLa()), GetBValue(_light.GetColorLa()),
		GetRValue(_light.GetColorLd()), GetGValue(_light.GetColorLd()), GetBValue(_light.GetColorLd()),
		GetRValue(_light.GetColorLs()), GetGValue(_light.GetColorLs()), GetBValue(_light.GetColorLs()));
	DrawString(hdc, 700 + 10, 50 + 150, 18, RGB(96, 96, 0), (LPCTSTR)outstr);*/
}



//clock_t c = clock();
//float timer = 7000.0f;
/*if (c < 10000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Facecull: on");

	//Values(10);
}

else if (c > 10000.0f && c < 12000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Facecull: off");

}
else if (c > 12000.0f && c < 15000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Depthsort: on");
	_depthsort = true;
}
else if (c > 15000.0f && c < 18000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Depthsort: on");
}


else if (c > 18000.0f && c < 21000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Depthsort: off");
}


else if (c > 21000.0f && c < 28000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Ortho: on");
}

else if (c > 28000.0f && c < 32000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Solid-Fill: on, Directional Light: on");
}

else if (c > 32000.0f && c < 37000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Directional: off, Scaling down");
}
else if (c > 37000.0f && c < 41000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Scaling up");
}
else if (c > 41000.0f && c < 42000.0f)
{
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Rotating, Scaling up");
}
else if (c > 42000.0f && c < 53000.0f)
{
	//Values(5);
	Values(14);
}
else if (c > 53000.0f && c < 56000.0f)
{
	//Values(7);
	Values(13);
}
else if (c > 56000.0f && c < 60000.0f)
{
	Values(12);
	//Values(10);
}*/


void Rasteriser::DrawString(HDC hdc, int xPos, int yPos, int fSize, COLORREF textColor, LPCTSTR text)
{
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(fSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		//SetTextColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, textColor);
		SetBkColor(hdc, RGB(255, 255, 255));

		// Display the text string.  
		TextOut(hdc, xPos, yPos, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}

void Rasteriser::RenderScene(HDC hdc)
{
	Model model;
	Vertex lightPos;

	float eX, eY, eZ; // Get the eye position
	eX = -_camera.GetValueFromMatrix(0, 3);
	eY = -_camera.GetValueFromMatrix(1, 3);
	eZ = -_camera.GetValueFromMatrix(2, 3);

	DrawViewportFrame(hdc);

	for (size_t i = 0; i < _models.size(); i++)
	{
		model = _models[i];

		model.ApplyTransform(_camera);

		model.CalculateCenters();

		if (_depthsort)	model.ApplyDepthSorting();

		model.CalculateCenters();
		model.CalculateNormals(true);

		lightPos = _camera * _lPos;
		_light.SetLightPosition(lightPos);

		if (_render_mode == 2) model.CalcFlatShading(hdc, _light, eX, eY, eZ, _ortho);

		model.ApplyTransform(_projection);
		model.Dehomogenise();

		model.CalculateCenters();
		model.CalculateNormals(true);

		if (_facecull)
		{
			model.MarkBackfaces(eX, eY, eZ, _ortho);
		}

		model.ApplyTransform(_viewport);

		switch (_render_mode)
		{
		case  0: model.DrawWireFrame(hdc);
			break;
		case  1: model.DrawSolidFill(hdc);
			break;
		case  2: model.DrawFlatShading(hdc);
			break;
		default: model.DrawWireFrame(hdc);
			break;
		}

	}
	DrawTextFrame(hdc);
}


void Rasteriser::InitializeScene(HWND _hWnd)
{
	// Load a model and add it in scene's model list
	Model MyModel;
	MD2Loader MyLoader;

	//Load and Add the first model
	MyLoader.LoadModel("cube.md2", MyModel);
	MyModel.SetColorKa(RGB(45, 3, 3));
	MyModel.SetColorKd(RGB(157, 11, 11));
	MyModel.SetColorKs(RGB(186, 79, 79));
	MyModel.SetShininess(76.8f);
	AddModel(MyModel);


	// Set a model's transformation matrix
	Matrix model_transform, transform;
	// Initialise transformations to Identity
	model_transform.SetIdentity();
	transform.SetIdentity();

	// Transform the first model that you have in the scene
	TransformModel(0, model_transform);

	// Initialise transformations to Identity
	model_transform.SetIdentity();
	transform.SetIdentity();
	//model_transform = transform * model_transform;
	transform.SetRotationY(-90);
	model_transform = transform * model_transform;

	// Transform the model that you have in the scene
	TransformModel(1, model_transform);


	// Set a specific client area of the window as the screen viewport
	SetViewport(50, 50 + 600, 50, 50 + 600);

	// Set the camera matrix
	_camRx = 0.0f;
	_camRy = 0.0f;
	_camRz = 0.0f;
	_camZ = 50.0f;
	Matrix camera;
	camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
	SetCameraTransform(camera);

	// Set the projection matrix
	_ortho = false;
	Matrix projection;
	if (_ortho) projection.SetOrtho(6.0f);
	else projection.SetPerspective(200.0f, 1.0f);

	// Set the viewvolume matrix
	Matrix volume;
	volume.SetViewvolume(-200, +200, -200, +200, +200, -200);

	// Set the overall view matrix = viewvolume matrix * projection matrix
	Matrix view;
	view = volume * projection;
	SetProjectionTransform(view);

	_render_mode = 0;
	//_depthsort = false;
	//_facecull = false;
	//_directional = true;
	_light.SetDirectional(_directional);
	_lPos.SetX(-100.0f);
	_lPos.SetY(100.0f);
	_lPos.SetZ(100.0f);
	_light.SetLightPosition(_lPos);
	_light.SetColorLa(RGB(32, 32, 32));
	_light.SetColorLd(RGB(255, 255, 0));
	_light.SetColor(RGB(255, 255, 255));

	_light.SetColorLa(RGB(128, 128, 128));
	_light.SetColorLd(RGB(255, 255, 255));
	_light.SetColor(RGB(255, 255, 255));

}


void Rasteriser::Update(HWND hWnd)
{
	clock_t c = clock();
	//float timer = 7000.0f;
	if (c < 10000.0f)
	{
		Values(14);
		_facecull = true;

		//Values(10);
	}

	else if (c > 10000.0f && c < 12000.0f)
	{
		Values(14);
		_facecull = false;
	}
	else if (c > 12000.0f && c < 15000.0f)
	{
		Values(14);
		_depthsort = true;
	}
	else if (c > 15000.0f && c < 18000.0f)
	{
		Values(14);
		_depthsort = true;
	}


	else if (c > 18000.0f && c < 21000.0f)
	{
		Values(13);
		_depthsort = false;
	}


	else if (c > 21000.0f && c < 28000.0f)
	{
		Values(12);
		//Values(0);
		_render_mode = 1;
		_ortho = true;
	}

	else if (c > 28000.0f && c < 32000.0f)
	{
		Values(12);
		//Values(0);
		_render_mode = 2;
		_directional = true;
		_ortho = true;
	}

	else if (c > 32000.0f && c < 37000.0f)
	{
		_render_mode = 0;
		_directional = false;
		Values(12);
		Values(17);
	}
	else if (c > 37000.0f && c < 41000.0f)
	{
		Values(13);
		Values(18);
	}
	else if (c > 41000.0f && c < 42000.0f)
	{
		Values(16);
		Values(18);
	}
	else if (c > 42000.0f && c < 53000.0f)
	{
		//Values(5);
		Values(14);
	}
	else if (c > 53000.0f && c < 56000.0f)
	{
		//Values(7);
		Values(13);
	}
	else if (c > 56000.0f && c < 60000.0f)
	{
		Values(12);
		//Values(10);
	}


}

void Rasteriser::Values(int action)
{
	Matrix camera, projection, volume, view;

	switch (action)
	{
	case  0: if (_render_mode < 2)
		_render_mode = _render_mode + 1; else _render_mode = 0;
		break;
	case  1: _ortho = !_ortho;

		if (_ortho)
			projection.SetOrtho(6.0f);
		else projection.SetPerspective(200.0f, 1.0f);

		volume.SetViewvolume(-200, +200, -200, +200, +200, -200);
		view = volume * projection;
		SetProjectionTransform(view);
		break;
	case  2: //_facecull = !_facecull;
		_facecull = true;
		_depthsort = false;
		break;
	case  3: //_depthsort = !_depthsort;
		_depthsort = true;
		_facecull = false;
		break;
	case  4: _directional = !_directional;
		_light.SetDirectional(_directional);
		break;
	case 5:	 _camRx = 0.0f;
		_camRy = 0.0f;
		_camRz = 50.0f;
		_camZ = 0.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 10:	 _camRx = 0.0f;
		_camRy = 0.0f;
		_camRz = 0.0f;
		_camZ = 50.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 11:	 _camRx = (int)(_camRx + 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 12:	_camRx = (int)(_camRx - 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 13:	_camRy = (int)(_camRy + 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 14:	_camRy = (int)(_camRy - 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 15:	_camRz = (int)(_camRz + 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 16:	_camRz = (int)(_camRz - 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 17:	_camZ = _camZ + 10.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 18:	_camZ = _camZ - 10.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 19:	_camRx = 60.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	}

}





