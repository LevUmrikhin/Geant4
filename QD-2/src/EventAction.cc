#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PhotoHit.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4Exception.hh"
#include "G4AnalysisManager.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ios.hh"

EventAction::EventAction()
    : fPhotoHCID(-1)
{}

EventAction::~EventAction()
{}

PhotoHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
    auto hitsCollection =
        static_cast<PhotoHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
    if (!hitsCollection) {
        G4ExceptionDescription msg;
        msg << "Cannot access hitsCollection ID " << hcID;
        G4Exception("EventAction::GetHitsCollection()",
                    "MyCode0003", FatalException, msg);
    }
    return hitsCollection;
}

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
    // Clear per-event data
    wavelengths.clear();
    times.clear();

    // Fix: ensure hits collection ID is initialized
    if (fPhotoHCID == -1) {
        fPhotoHCID = G4SDManager::GetSDMpointer()
                         ->GetCollectionID("PhotoHitsCollection");
        G4cout << "[Init] fPhotoHCID = " << fPhotoHCID << G4endl;
    }
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    // Retrieve hits collection
    auto photoHC = GetHitsCollection(fPhotoHCID, event);
    G4int nofHits = photoHC->entries();

    // Grab the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Get primary energy (assuming there's exactly 1 primary vertex/particle)
    G4double primaryEnergy = 0.0;
    auto vertex = event->GetPrimaryVertex();
    if (vertex) {
        auto primary = vertex->GetPrimary();
        if (primary) {
            primaryEnergy = primary->GetKineticEnergy();
        }
    }

    // Fill the local vectors with wavelength/time from the hits
    for (int i = 0; i < nofHits; ++i) {
        wavelengths.push_back((*photoHC)[i]->GetWavelength());
        times.push_back((*photoHC)[i]->GetTime());
    }

    // Fill main ntuple columns
    analysisManager->FillNtupleDColumn(0, primaryEnergy);
    analysisManager->FillNtupleIColumn(1, nofHits);

    // Optional: fill vector columns if defined
    // analysisManager->FillNtupleDColumn(2, wavelengths);
    // analysisManager->FillNtupleDColumn(3, times);

    analysisManager->AddNtupleRow();

    // Reflection count debug
    static const bool verbose = true;
    if (verbose) {
        G4cout << "[Reflection Count] This event: "
               << SteppingAction::GetReflectionCount() << G4endl;
    }
    SteppingAction::ResetReflectionCount();
}
