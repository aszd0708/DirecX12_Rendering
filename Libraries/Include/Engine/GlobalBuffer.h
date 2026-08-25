#pragma once

struct GlobalDesc
{
	Matrix V = Matrix::Identity;
	Matrix P = Matrix::Identity;
	Matrix VP = Matrix::Identity;
	Matrix VInv = Matrix::Identity;
};

struct ScreenSizeDesc
{
	int width;
	int height;

	Vec2 padding;
};

struct TransformDesc
{
	Matrix W = Matrix::Identity;
};
