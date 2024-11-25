#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	RegisterPhysics (new G4EmStandardPhysics()); //electro-magnetic interaction
	RegisterPhysics (new G4OpticalPhysics()); //optical photons
}

MyPhysicsList::~MyPhysicsList()
{}