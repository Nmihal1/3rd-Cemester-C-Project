#pragma once
#include <windows.h>
#include <vector>
#include "Vertex_4D.h"

using namespace std;

//Light Class Declaration

class Light
{
public:
	Light();
	~Light();
	Light(const Light& other);
	//Light's position
	Vertex GetPosition() const;
	void SetLightPosition(Vertex& pos);
	//Light's directional
	bool IsDirectional() const;
	void SetDirectional(bool value);
	//light's Colors
	COLORREF GetColorLa() const;
	void SetColorLa(COLORREF color);
	COLORREF GetColorLd() const;
	void SetColorLd(COLORREF color);
	COLORREF GetColor() const;
	void SetColor(COLORREF color);

	// Assignment operator
	Light& operator= (const Light& rhs);

private:
	COLORREF _La;
	COLORREF _Ld;
	COLORREF _Ls;
	Vertex _Lpos;
	bool _directional;
};

