#include <iostream>

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"

#include "construction.hh"

#include "physics.hh"

int main(int argc, char** argv)
{

    // Initialize the Run Manager

    G4RunManager* runManager = new G4RunManager();

    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());

    runManager->Initialize();

    // Initialize the UI Executive
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);

    // Initialize the Visualization Manager
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // Get the UI Manager and execute the session
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    //Make Gears display created volumes

    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    UImanager->ApplyCommand("/vis/drawVolume");

    // Start the UI session
    ui->SessionStart();

    // Clean up
    delete visManager;
    delete runManager;
    delete ui;

    return 0;
}
