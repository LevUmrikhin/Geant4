#include "ActionInitialization.hh"


MyActionInitialization::MyActionInitialization() : G4VUserActionInitialization()
{}


MyActionInitialization::~MyActionInitialization()
{}


void MyActionInitialization::BuildForMaster() const
{
    EventAction* eventAction = new EventAction;
    SetUserAction(new MyRunAction(eventAction));
}


void MyActionInitialization::Build() const
{
    SetUserAction(new MyPrimaryGeneratorAction());

    auto eventAction = new EventAction;
    SetUserAction(eventAction);

    SetUserAction(new MyRunAction(eventAction));
}

