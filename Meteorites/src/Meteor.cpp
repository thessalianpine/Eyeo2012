#include "Meteor.h"

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

}

