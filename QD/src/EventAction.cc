#include "EventAction.hh"


EventAction::EventAction()
{}


EventAction::~EventAction()
{}


PhotoHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
    auto hitsCollection = static_cast<PhotoHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));

    if ( ! hitsCollection ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hitsCollection ID " << hcID;
        G4Exception("EventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
    }

    return hitsCollection;
}

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{}


void EventAction::EndOfEventAction(const G4Event* event)
{
    if ( fPhotoHCID == -1 ) {
        fPhotoHCID = G4SDManager::GetSDMpointer()->GetCollectionID("PhotoHitsCollection");
    }

    auto PhotoHC = GetHitsCollection(fPhotoHCID, event);

    G4int nofHits = PhotoHC->entries();

    //G4cout << nofHits << G4endl; 

    auto analysisManager = G4AnalysisManager::Instance();

    if (nofHits != 0) {
        analysisManager->FillNtupleDColumn(0, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
        analysisManager->FillNtupleIColumn(1, PhotoHC->entries());
        for (int i=0; i<nofHits; i++){ 
            wavelengths.push_back((*PhotoHC)[i]->GetWavelength()); 
            times.push_back((*PhotoHC)[i]->GetTime());
        }
        analysisManager->AddNtupleRow();
    }
}

