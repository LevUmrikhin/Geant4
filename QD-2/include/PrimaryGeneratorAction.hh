#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    MyPrimaryGeneratorAction();
    ~MyPrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*) override;

private:
    G4ParticleGun* fParticleGun;
};
