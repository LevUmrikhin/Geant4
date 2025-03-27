#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "Randomize.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
{
    G4int nParticles = 1;
    fParticleGun = new G4ParticleGun(nParticles);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    auto photon = particleTable->FindParticle("opticalphoton");

    fParticleGun->SetParticleDefinition(photon);
    fParticleGun->SetParticleEnergy(3.0 * eV); // Blue-visible photon
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
    delete fParticleGun;
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    // Emit photon from the center of WhiteBox water volume
    G4double x0 = 0, y0 = 0, z0 = 15 * cm; // Center of box
    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    // Random direction (uniform over sphere)
    G4double theta = acos(1 - 2 * G4UniformRand());
    G4double phi   = 2 * CLHEP::pi * G4UniformRand();
    G4double dx = sin(theta) * cos(phi);
    G4double dy = sin(theta) * sin(phi);
    G4double dz = cos(theta);
    G4ThreeVector dir(dx, dy, dz);
    fParticleGun->SetParticleMomentumDirection(dir);

    // Random polarization orthogonal to direction
    G4ThreeVector perp = dir.orthogonal();
    G4ThreeVector pol = perp.rotate(dir, 2 * CLHEP::pi * G4UniformRand()).unit();
    fParticleGun->SetParticlePolarization(pol);

    // Generate primary vertex
    fParticleGun->GeneratePrimaryVertex(event);
}
