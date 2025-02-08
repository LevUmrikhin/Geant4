#include "PrimaryGeneratorAction.hh"


MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction()
{
    theParticleSource = new G4GeneralParticleSource();
}


MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
    delete theParticleSource;
}


void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    theParticleSource->GeneratePrimaryVertex(anEvent);
}

