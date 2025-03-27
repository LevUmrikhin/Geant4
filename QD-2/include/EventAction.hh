#pragma once

#include "G4UserEventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

#include "PhotoSD.hh"
#include "PhotoHit.hh"

#include "globals.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void   EndOfEventAction(const G4Event* event) override;

    std::vector<G4double>& GetWavelengths() { return wavelengths; }
    std::vector<G4double>& GetTime() { return times; }

private:
    PhotoHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;

    G4int fPhotoHCID = -1;

    std::vector<G4double> wavelengths;
    std::vector<G4double> times; 
};

