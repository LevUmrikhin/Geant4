#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4StepPoint.hh"

G4int SteppingAction::fReflectionCount = 0;

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto track = step->GetTrack();
    if (track->GetDefinition()->GetParticleName() != "opticalphoton")
        return;

    auto postStepPoint = step->GetPostStepPoint();
    if (postStepPoint->GetStepStatus() != fGeomBoundary)
        return;

    // Locate the boundary process if not already cached
    static G4OpBoundaryProcess* boundary = nullptr;
    if (!boundary) {
        auto processManager = track->GetDefinition()->GetProcessManager();
        G4ProcessVector* procVec = processManager->GetProcessList();
        for (G4int i = 0; i < procVec->size(); ++i) {
            auto proc = (*procVec)[i];
            if (auto op = dynamic_cast<G4OpBoundaryProcess*>(proc)) {
                boundary = op;
                break;
            }
        }
    }

    if (!boundary) return;

    auto status = boundary->GetStatus();

    if (
        status == FresnelReflection ||
        status == TotalInternalReflection ||
        status == SpikeReflection ||
        status == LobeReflection ||
        status == LambertianReflection ||
        status == BackScattering
    ) {
        auto preVol = step->GetPreStepPoint()->GetPhysicalVolume();
        auto postVol = step->GetPostStepPoint()->GetPhysicalVolume();
        G4String preName = preVol ? preVol->GetName() : "null";
        G4String postName = postVol ? postVol->GetName() : "null";

        if (
            (preName == "BoxWaterFill" && postName == "WhiteBoxShell") ||
            (preName == "WhiteBoxShell" && postName == "BoxWaterFill")
        ) {
            fReflectionCount++;
            G4cout << "🪞 Reflection counted on Tyvek (border surface)" << G4endl;
        }
    }
}

G4int SteppingAction::GetReflectionCount() {
    return fReflectionCount;
}

void SteppingAction::ResetReflectionCount() {
    fReflectionCount = 0;
}
