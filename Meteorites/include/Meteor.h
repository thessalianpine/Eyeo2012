#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"

using namespace cinder;

class Meteor
{

public:

	Meteor();
	~Meteor();

	void draw();
	void spawn(Vec3f pos);
	
	inline void setPosition(Vec3f pos)
	{
		mPosition = pos;
	}

private:

	void initMesh();

	Vec3f			mPosition;
	float			mVelocity;
	int				mDiameter;
	bool			mSpawned;
	gl::VboMesh		mVBOMesh;
};

