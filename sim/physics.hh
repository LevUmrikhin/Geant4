#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh" //classname is always equal to filename 
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

class MyPhysicsList : public G4VModularPhysicsList
{
public: 
	MyPhysicsList();
	~MyPhysicsList();
};

#endif