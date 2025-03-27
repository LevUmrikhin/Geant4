#pragma once

#include "G4VSensitiveDetector.hh"
#include "PhotoHit.hh"
#include "G4RandomTools.hh"
#include <vector>

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

class G4Step;
class G4HCofThisEvent;

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(const G4String& name, const G4String& hitsCollectionName);
    ~MySensitiveDetector() override;

    // methods from base class
    void   Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hitCollection) override;
    G4double GetQuantumEfficiency(G4double wavelength);

private:
    PhotoHitsCollection* fHitsCollection = nullptr;
};

