#include "RunAction.hh"


MyRunAction::MyRunAction(EventAction* eventAction) : fEventAction(eventAction)
{
    // set printing event number per each event
    //G4RunManager::GetRunManager()->SetPrintProgress(1);

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetNtupleMerging(true);

    analysisManager->CreateNtuple("Hits", "Hits");
    analysisManager->CreateNtupleDColumn("primaryE");;
    analysisManager->CreateNtupleIColumn("hits");
    analysisManager->CreateNtupleDColumn("wavelength", fEventAction->GetWavelenths());
    analysisManager->CreateNtupleDColumn("time", fEventAction->GetTime());
    analysisManager->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->OpenFile("output.root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();
}

