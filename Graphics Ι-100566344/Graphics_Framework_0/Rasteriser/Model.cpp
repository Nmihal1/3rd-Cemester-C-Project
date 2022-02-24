#include <windowsx.h>
#include <math.h>
#include <algorithm>  
#include "Model.h"


//Triangle Class Implementation

Triangle::Triangle()
{
	_indices[0] = 0;
	_indices[1] = 0;
	_indices[2] = 0;
	_visible = true;
	_normal.SetX(0.0f);
	_normal.SetY(0.0f);
	_normal.SetZ(0.0f);
	_normal.SetW(1.0f);
	_center.SetX(0.0f);
	_center.SetY(0.0f);
	_center.SetZ(0.0f);
	_center.SetW(1.0f);
	_color = RGB(128, 128, 128);
}

Triangle::Triangle(int index0, int index1, int index2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
	_visible = true;
	_normal.SetX(0.0f);
	_normal.SetY(0.0f);
	_normal.SetZ(0.0f);
	_normal.SetW(1.0f);
	_center.SetX(0.0f);
	_center.SetY(0.0f);
	_center.SetZ(0.0f);
	_center.SetW(1.0f);
	_color = RGB(128, 128, 128);
}


Triangle::Triangle(const Triangle& other)
{
	_indices[0] = other.GetIndex(0);
	_indices[1] = other.GetIndex(1);
	_indices[2] = other.GetIndex(2);
	_visible = other.IsVisible();
	_normal = other.GetNormal();
	_center = other.GetCenter();
	_color = other.GetColor();
}

// Destructor does not need to do anything

Triangle::~Triangle()
{
}


int Triangle::GetIndex(int i) const
{
	return _indices[i];
}

bool Triangle::IsVisible() const
{
	return _visible;
}

void Triangle::SetVisible(bool value)
{
	_visible = value;
}


Vertex Triangle::GetNormal() const
{
	return _normal;
}

void Triangle::SetNormal(Vertex& normal)
{
	_normal = normal;
}

Vertex Triangle::GetCenter() const
{
	return _center;
}

void Triangle::SetCenter(Vertex& center)
{
	_center = center;
}

COLORREF Triangle::GetColor() const
{
	return _color;
}

void Triangle::SetColor(COLORREF color)
{
	_color = color;
}



Triangle& Triangle::operator=(const Triangle& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		_indices[0] = rhs.GetIndex(0);
		_indices[1] = rhs.GetIndex(1);
		_indices[2] = rhs.GetIndex(2);
		_visible = rhs.IsVisible();
		_normal = rhs.GetNormal();
		_center = rhs.GetCenter();
	}
	return *this;
}

//Model Class Implementation

Model::Model()
{
	_Ka = RGB(192, 192, 192);
	_Kd = RGB(192, 192, 192);
	_Ks = RGB(192, 192, 192);
	_shininess = 30.0f;
}

Model::~Model()
{
}

void Model::Clear()
{
	ClearTriangles();
	ClearVertices();
}

void Model::ClearVertices()
{
	_vertices.clear();
}

void Model::ClearTriangles()
{
	_triangles.clear();
}

vector<Triangle>& Model::GetTriangles()
{
	return _triangles;
}

vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

size_t Model::GetTriangleCount() const
{
	return _triangles.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex v;
	v = Vertex(x, y, z);
	_vertices.push_back(v);
}

void Model::AddTriangle(int i0, int i1, int i2)
{
	Triangle t;
	t = Triangle(i0, i1, i2);
	_triangles.push_back(t);
}

COLORREF Model::GetColorKa() const
{
	return _Ka;
}

void Model::SetColorKa(COLORREF color)
{
	_Ka = color;
}

COLORREF Model::GetColorKd() const
{
	return _Kd;
}

void Model::SetColorKd(COLORREF color)
{
	_Kd = color;
}

COLORREF Model::GetColorKs() const
{
	return _Ks;
}

void Model::SetColorKs(COLORREF color)
{
	_Ks = color;
}

float Model::GetShininess() const
{
	return _shininess;
}
void Model::SetShininess(float value)
{
	_shininess = value;
}


void Model::CopyModel(Model& rhs)
{
	vector<Triangle> triangles;
	vector<Vertex> vertices;


	triangles = rhs.GetTriangles();
	vertices = rhs.GetVertices();
	for (int i = 0; i < rhs.GetTriangleCount(); ++i)
	{
		// Call model member function to add a new polygon to the list of triangles
		AddTriangle(triangles[i].GetIndex(0), triangles[i].GetIndex(1), triangles[i].GetIndex(2));
	}

	// Vertex array initialization
	for (int i = 0; i < rhs.GetVertexCount(); ++i)
	{
		// Call model member function to add a new vertex to the list of vertices		
		AddVertex(vertices[i].GetX(), vertices[i].GetY(), vertices[i].GetZ());
	}

	_Ka = rhs.GetColorKa();
	_Kd = rhs.GetColorKd();
	_Ks = rhs.GetColorKs();
	_shininess = rhs.GetShininess();

}



void Model::ApplyTransform(const Matrix& transform)
{
	Vertex vC;
	for (size_t i = 0; i < _vertices.size(); i++)
	{
		_vertices[i] = transform * _vertices[i];
	}
	for (size_t i = 0; i < _triangles.size(); i++)
	{
		vC = transform * _triangles[i].GetCenter();
		_triangles[i].SetCenter(vC);
	}
}

void Model::Dehomogenise()
{
	for (size_t i = 0; i < _vertices.size(); i++)
	{
		_vertices[i].Dehomogenise();
	}
}

// Calculates Face Normals of Model
void Model::CalculateNormals(bool CCW)
{
	Vertex v0, v1, v2, vA, vB, vN;

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		v0 = _vertices[_triangles[i].GetIndex(0)];
		v1 = _vertices[_triangles[i].GetIndex(1)];
		v2 = _vertices[_triangles[i].GetIndex(2)];

		vA = v1 - v0;
		vB = v2 - v0;

		if (CCW)
			vN = vA.CrossProduct(vB);
		else
			vN = vB.CrossProduct(vA);

		vN.Normalise();
		_triangles[i].SetNormal(vN);
	}
}

// Calculates Face Center of Model
void Model::CalculateCenters()
{
	Vertex v0, v1, v2, vC;

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		v0 = _vertices[_triangles[i].GetIndex(0)];
		v1 = _vertices[_triangles[i].GetIndex(1)];
		v2 = _vertices[_triangles[i].GetIndex(2)];

		vC.SetX((v0.GetX() + v1.GetX() + v2.GetX()) / 3.0f);
		vC.SetY((v0.GetY() + v1.GetY() + v2.GetY()) / 3.0f);
		vC.SetZ((v0.GetZ() + v1.GetZ() + v2.GetZ()) / 3.0f);
		vC.SetW(1.0f);

		_triangles[i].SetCenter(vC);
	}
}

// Marks Backfaces of Model
void Model::MarkBackfaces(float eX, float eY, float eZ, bool ortho)
{
	float result;
	Vertex v0, v1, v2, vN, vC, vE; //Vertex, normal, center and eye vectors

	for (size_t i = 0; i < _triangles.size(); i++)
	{

		vN = _triangles[i].GetNormal();
		vC = _triangles[i].GetCenter();

		if (ortho) vE = Vertex(eX, eY, eZ);
		else vE = Vertex(eX, eY, eZ) - vC;

		result = vN.DotProduct(vE);

		if (result <= 0)
			_triangles[i].SetVisible(false);
		else
			_triangles[i].SetVisible(true);

	}
}

// Apply Depth Sorting on Model Triangles
void Model::ApplyDepthSorting()
{

	std::sort(_triangles.begin(), _triangles.end(), [](const Triangle& lhs, const Triangle& rhs) {return lhs.GetCenter().GetZ() < rhs.GetCenter().GetZ(); });

}


void Model::DrawWireFrame(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;

	if (_triangles.size() == 0)
	{
		return;
	}
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HGDIOBJ oldPen = SelectObject(hdc, pen);

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i].IsVisible() == true)
		{
			v0_inx = _triangles[i].GetIndex(0);
			v1_inx = _triangles[i].GetIndex(1);
			v2_inx = _triangles[i].GetIndex(2);

			MoveToEx(hdc, (int)_vertices[v0_inx].GetX(), (int)_vertices[v0_inx].GetY(), NULL);

			LineTo(hdc, (int)_vertices[v1_inx].GetX(), (int)_vertices[v1_inx].GetY());
			LineTo(hdc, (int)_vertices[v2_inx].GetX(), (int)_vertices[v2_inx].GetY());
			LineTo(hdc, (int)_vertices[v0_inx].GetX(), (int)_vertices[v0_inx].GetY());
		}

	}
	SelectObject(hdc, oldPen);
	DeleteObject(pen);

}

void Model::DrawSolidFill(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;
	POINT points[3];
	POINT point;

	if (_triangles.size() == 0)
	{
		return;
	}
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(0, 255, 255));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i].IsVisible() == true)
		{
			v0_inx = _triangles[i].GetIndex(0);
			v1_inx = _triangles[i].GetIndex(1);
			v2_inx = _triangles[i].GetIndex(2);

			point.x = (int)_vertices[v0_inx].GetX(); point.y = (int)_vertices[v0_inx].GetY();
			points[0] = point;
			point.x = (int)_vertices[v1_inx].GetX(); point.y = (int)_vertices[v1_inx].GetY();
			points[1] = point;
			point.x = (int)_vertices[v2_inx].GetX(); point.y = (int)_vertices[v2_inx].GetY();
			points[2] = point;

			Polygon(hdc, points, 3);

			point.x = (int)_triangles[i].GetCenter().GetX(); point.y = (int)_triangles[i].GetCenter().GetY();

			MoveToEx(hdc, point.x, point.y, NULL);

			LineTo(hdc, point.x + 1, point.y);
			LineTo(hdc, point.x + 1, point.y + 1);
			LineTo(hdc, point.x, point.y + 1);
			LineTo(hdc, point.x, point.y);

		}

	}
	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);

}

void Model::CalcFlatShading(HDC hdc, Light light, float eX, float eY, float eZ, bool ortho)
{
	COLORREF faceColor;
	Vertex vN, vC, vL, vE, vH;
	float cD, cS;
	float totR, totG, totB;

	if (_triangles.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < _triangles.size(); i++)
	{

		vN = _triangles[i].GetNormal();
		vN.Normalise();
		vC = _triangles[i].GetCenter();

		if (light.IsDirectional()) vL = light.GetPosition();
		else vL = light.GetPosition() - vC;
		vL.Normalise();

		cD = vN.DotProduct(vL);

		if (ortho) vE = Vertex(eX, eY, eZ);
		else vE = Vertex(eX, eY, eZ) - vC;
		vE.Normalise();

		vH = vL + vE;
		vH.Normalise();

		cS = pow(vN.DotProduct(vH), GetShininess());

		if (cD <= 0) { cD = 0; cS = 0; }

		totR = (GetRValue(GetColorKa()) * GetRValue(light.GetColorLa()) / 255.0f +
			cD * GetRValue(GetColorKd()) * GetRValue(light.GetColorLd()) / 255.0f +
			cS * GetRValue(GetColorKs()) * GetRValue(light.GetColor()) / 255.0f);
		totG = (GetGValue(GetColorKa()) * GetGValue(light.GetColorLa()) / 255.0f +
			cD * GetGValue(GetColorKd()) * GetGValue(light.GetColorLd()) / 255.0f +
			cS * GetGValue(GetColorKs()) * GetGValue(light.GetColor()) / 255.0f);
		totB = (GetBValue(GetColorKa()) * GetBValue(light.GetColorLa()) / 255.0f +
			cD * GetBValue(GetColorKd()) * GetBValue(light.GetColorLd()) / 255.0f +
			cS * GetBValue(GetColorKs()) * GetBValue(light.GetColor()) / 255.0f);

		if (totR > 255.0f) totR = 255.0f;
		if (totG > 255.0f) totG = 255.0f;
		if (totB > 255.0f) totB = 255.0f;

		faceColor = RGB((int)totR, (int)totG, (int)totB);

		_triangles[i].SetColor(faceColor);

	}

}

void Model::DrawFlatShading(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;
	POINT points[3];
	POINT point;
	COLORREF faceColor;

	if (_triangles.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i].IsVisible() == true)
		{
			v0_inx = _triangles[i].GetIndex(0);
			v1_inx = _triangles[i].GetIndex(1);
			v2_inx = _triangles[i].GetIndex(2);

			point.x = (int)_vertices[v0_inx].GetX(); point.y = (int)_vertices[v0_inx].GetY();
			points[0] = point;
			point.x = (int)_vertices[v1_inx].GetX(); point.y = (int)_vertices[v1_inx].GetY();
			points[1] = point;
			point.x = (int)_vertices[v2_inx].GetX(); point.y = (int)_vertices[v2_inx].GetY();
			points[2] = point;

			faceColor = _triangles[i].GetColor();

			HPEN pen = CreatePen(PS_SOLID, 1, faceColor);
			HGDIOBJ oldPen = SelectPen(hdc, pen);

			HBRUSH brush = CreateSolidBrush(faceColor);
			HBRUSH oldBrush = SelectBrush(hdc, brush);

			Polygon(hdc, points, 3);

			SelectBrush(hdc, oldBrush);
			DeleteObject(brush);

			SelectPen(hdc, oldPen);
			DeleteObject(pen);

		}

	}


}



