#include "Meteor.h"
#include <vector>

Meteor::Meteor() :
	mPosition(0.0f, 0.0f, 0.0f),
	mVelocity(0.0f),
	mDiameter(1),
	mSpawned(false)
{
}

Meteor::~Meteor()
{
}

void Meteor::draw()
{
	gl::draw(mVBOMesh);
}


void Meteor::spawn( Vec3f pos )
{
	if(mSpawned)
		return;

	if(pos == mPosition)
		return;

	mPosition = pos;
	initMesh();
	mSpawned = true;
}


void Meteor::initMesh()
{
	//vbomesh stuff
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticColorsRGB();
	layout.setStaticPositions();

	uint32_t numVerts = 24;
	uint32_t numIndices = 24;
	mVBOMesh = gl::VboMesh(numVerts, numIndices, layout, GL_QUADS);

	//indices
	std::vector<uint32_t> indices;
	uint32_t i=0;
	while (i < numIndices)
	{
		indices.push_back(i);
		i++;
	}
	mVBOMesh.bufferIndices(indices);
	
	Vec3f vertices[8] =
	{
		Vec3f(mPosition.x - mDiameter, mPosition.y + mDiameter, mPosition.z + mDiameter), //v0
		Vec3f(mPosition.x - mDiameter, mPosition.y + mDiameter, mPosition.z - mDiameter), //v1
		Vec3f(mPosition.x + mDiameter, mPosition.y + mDiameter, mPosition.z - mDiameter), //v2
		Vec3f(mPosition.x + mDiameter, mPosition.y + mDiameter, mPosition.z + mDiameter), //v3
		Vec3f(mPosition.x - mDiameter, mPosition.y - mDiameter, mPosition.z + mDiameter), //v4
		Vec3f(mPosition.x - mDiameter, mPosition.y - mDiameter, mPosition.z - mDiameter), //v5
		Vec3f(mPosition.x + mDiameter, mPosition.y - mDiameter, mPosition.z - mDiameter), //v6
		Vec3f(mPosition.x + mDiameter, mPosition.y - mDiameter, mPosition.z + mDiameter)  //v7
	};
	
	//positions
	std::vector<Vec3f> positions;
	
	//quad front
	positions.push_back(Vec3f(vertices[0]));
	positions.push_back(Vec3f(vertices[3]));
	positions.push_back(Vec3f(vertices[7]));
	positions.push_back(Vec3f(vertices[4]));

	//quad top
	positions.push_back(Vec3f(vertices[0]));
	positions.push_back(Vec3f(vertices[1]));
	positions.push_back(Vec3f(vertices[2]));
	positions.push_back(Vec3f(vertices[3]));

	//quad back
	positions.push_back(Vec3f(vertices[1]));
	positions.push_back(Vec3f(vertices[2]));
	positions.push_back(Vec3f(vertices[6]));
	positions.push_back(Vec3f(vertices[5]));

	//quad bottom
	positions.push_back(Vec3f(vertices[5]));
	positions.push_back(Vec3f(vertices[6]));
	positions.push_back(Vec3f(vertices[7]));
	positions.push_back(Vec3f(vertices[4]));

	//quad left
	positions.push_back(Vec3f(vertices[5]));
	positions.push_back(Vec3f(vertices[1]));
	positions.push_back(Vec3f(vertices[0]));
	positions.push_back(Vec3f(vertices[4]));

	//quad right
	positions.push_back(Vec3f(vertices[7]));
	positions.push_back(Vec3f(vertices[3]));
	positions.push_back(Vec3f(vertices[2]));
	positions.push_back(Vec3f(vertices[6]));

	mVBOMesh.bufferPositions(positions);
}

