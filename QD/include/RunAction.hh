#pragma once

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "G4AnalysisManager.hh"
#include "globals.hh"
#include "EventAction.hh"

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction(EventAction* eventAction);
    ~MyRunAction();
    
    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

private:
    EventAction* fEventAction = nullptr;
};

