#pragma once
class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	
	
private:
	shared_ptr<Geometry<VertexTextureNormalTangentData>> _geometry;
};

