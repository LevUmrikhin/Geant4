#include "PhotoSD.hh"
#include "PhotoHit.hh"

#include <fstream>
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

MySensitiveDetector::MySensitiveDetector(const G4String& name,
                                         const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name)
{
    // Register the hits collection name
    collectionName.insert(hitsCollectionName);
}

MySensitiveDetector::~MySensitiveDetector()
{}

G4double MySensitiveDetector::GetQuantumEfficiency(G4double wavelength)
{
    // Bypassed for debug
    return 1.0;
}

void MySensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection =
        new PhotoHitsCollection(SensitiveDetectorName, collectionName[0]);

    auto hcID = G4SDManager::GetSDMpointer()
                    ->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);

    // Debug logging
    {
        std::ofstream out("hits.log", std::ios_base::app);
        out << "[Initialize] Created hits collection for '"
            << SensitiveDetectorName << "'\n";
    }
}

G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Only track optical photons
    G4Track* track = aStep->GetTrack();
    if (track->GetDefinition()->GetParticleName() != "opticalphoton")
        return false;

    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    auto vol = preStepPoint->GetTouchableHandle()->GetVolume();
    G4String volName = vol->GetName();

    {
        // Log each step's volume entry
        std::ofstream out("hits.log", std::ios_base::app);
        out << "[ProcessHits] Photon in volume: " << volName
            << " | KE(eV)= " << (track->GetKineticEnergy() / eV)
            << " | globalTime(ns)= " << (preStepPoint->GetGlobalTime() / ns)
            << "\n";
    }

    // Check if it's the Photocathode
    if (volName == "Photocathode") {

        G4double energy = preStepPoint->GetKineticEnergy(); // MeV
        G4double time = preStepPoint->GetGlobalTime();      // ns
        // Wavelength in nm
        G4double wavelength = (1.239841984e-3 * MeV * nm) / energy;

        {
            // Log the actual hit
            std::ofstream out("hits.log", std::ios_base::app);
            out << "[HIT] Photocathode HIT at time = "
                << (time / ns) << " ns, wavelength = "
                << (wavelength / nm) << " nm\n";
        }

        // Record the hit in the hits collection
        auto hit = new PhotoHit();
        hit->SetWavelength(wavelength);
        hit->SetTime(time);
        fHitsCollection->insert(hit);

        // Absorb photon
        track->SetTrackStatus(fStopAndKill);
    }

    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    // Debug: number of hits in this event
    {
        std::ofstream out("hits.log", std::ios_base::app);
        out << "[EndOfEvent] Hits in '"
            << SensitiveDetectorName
            << "': " << fHitsCollection->entries() << "\n";
    }
}
