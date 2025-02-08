#include "PhotoSD.hh"

MySensitiveDetector::MySensitiveDetector(const G4String& name, const G4String& hitsCollectionName) : G4VSensitiveDetector(name)
{
    collectionName.insert(hitsCollectionName);
}

MySensitiveDetector::~MySensitiveDetector()
{}

G4double MySensitiveDetector::GetQuantumEfficiency(G4double wavelength) {
if(wavelength >= 276 && wavelength < 277) return 9.67E-02; if (wavelength >= 277 && wavelength < 279) return 1.79E-01; if (wavelength >= 279 && wavelength < 281) return 2.46E-01; if (wavelength >= 281 && wavelength < 282) return 3.30E-01; if (wavelength >= 282 && wavelength < 284) return 4.54E-06; if (wavelength >= 284 && wavelength < 286) return 6.25E-01; if (wavelength >= 286 && wavelength < 289) return 8.40E-01; if (wavelength >= 289 && wavelength < 292) return 1.16E+00; if (wavelength >= 292 && wavelength < 296) return 1.59E+00; if (wavelength >= 296 && wavelength < 300) return 2.13E+00; if (wavelength >= 300 && wavelength < 303) return 2.92E+00; if (wavelength >= 303 && wavelength < 306) return 4.00E+00; if (wavelength >= 306 && wavelength < 308) return 5.40E+00; if(wavelength >= 308 && wavelength < 311) return 7.43E+00; if (wavelength >= 311 && wavelength < 317) return 1.00E+01; if (wavelength >= 317 && wavelength < 323) return 1.36E+01; if (wavelength >= 323 && wavelength < 328) return 1.58E+01; if (wavelength >= 328 && wavelength < 337) return 1.77E+01; if (wavelength >= 337 && wavelength < 345) return 1.98E+01; if (wavelength >= 345 && wavelength < 354) return 2.15E+01; if (wavelength >= 354 && wavelength < 365) return 2.32E+01; if (wavelength >= 365 && wavelength < 375) return 2.39E+01; if (wavelength >= 375 && wavelength < 386) return 2.44E+01; if (wavelength >= 386 && wavelength < 391) return 2.43E+01; if(wavelength >= 391 && wavelength < 394) return 2.43E+01; if (wavelength >= 394 && wavelength < 398) return 2.42E+01; if (wavelength >= 398 && wavelength < 401) return 2.41E+01; if (wavelength >= 401 && wavelength < 404) return 2.42E+01; if (wavelength >= 404 && wavelength < 407) return 2.42E+01; if (wavelength >= 407 && wavelength < 410) return 2.41E+01; if (wavelength >= 410 && wavelength < 411) return 2.41E+01; if (wavelength >= 411 && wavelength < 415) return 2.41E+01; if (wavelength >= 415 && wavelength < 417) return 2.41E+01; if (wavelength >= 417 && wavelength < 419) return 2.40E+01; if (wavelength >= 419 && wavelength < 421) return 2.40E+01; if(wavelength >= 421 && wavelength < 422) return 2.40E+01; if (wavelength >= 422 && wavelength < 424) return 2.37E+01; if (wavelength >= 424 && wavelength < 426) return 2.36E+01; if (wavelength >= 426 && wavelength < 428) return 2.36E+01; if (wavelength >= 428 && wavelength < 430) return 2.35E+01; if (wavelength >= 430 && wavelength < 431) return 2.35E+01; if (wavelength >= 431 && wavelength < 432) return 2.34E+01; if (wavelength >= 432 && wavelength < 435) return 2.33E+01; if (wavelength >= 435 && wavelength < 437) return 2.32E+01; if (wavelength >= 437 && wavelength < 439) return 2.32E+01; if (wavelength >= 439 && wavelength < 441) return 2.29E+01; if(wavelength >= 441 && wavelength < 443) return 2.29E+01; if (wavelength >= 443 && wavelength < 445) return 2.26E+01; if (wavelength >= 445 && wavelength < 447) return 2.25E+01; if (wavelength >= 447 && wavelength < 448) return 2.24E+01; if (wavelength >= 448 && wavelength < 450) return 2.23E+01; if (wavelength >= 450 && wavelength < 451) return 2.22E+01; if (wavelength >= 451 && wavelength < 452) return 2.21E+01; if (wavelength >= 452 && wavelength < 454) return 2.18E+01; if (wavelength >= 454 && wavelength < 455) return 2.17E+01; if (wavelength >= 455 && wavelength < 457) return 2.17E+01; if (wavelength >= 457 && wavelength < 459) return 2.16E+01; if(wavelength >= 459 && wavelength < 460) return 2.15E+01; if (wavelength >= 460 && wavelength < 462) return 2.14E+01; if (wavelength >= 462 && wavelength < 463) return 2.13E+01; if (wavelength >= 463 && wavelength < 467) return 2.11E+01; if (wavelength >= 467 && wavelength < 469) return 2.07E+01; if (wavelength >= 469 && wavelength < 470) return 2.05E+01; if (wavelength >= 470 && wavelength < 471) return 2.03E+01; if (wavelength >= 471 && wavelength < 473) return 2.02E+01; if (wavelength >= 473 && wavelength < 476) return 2.00E+01;  return 0.0; }



void MySensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    // Create hits collection
    fHitsCollection = new PhotoHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce
    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}


G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory*)
{
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
//    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    auto fPDG = track->GetDefinition()->GetPDGEncoding();  //particleType
    auto fEnergy = preStepPoint->GetKineticEnergy();       //energy in MeV
    auto fTime = preStepPoint->GetGlobalTime();
    auto wavelength = 0.0012398/fEnergy;                   //Wavelength in micrometers

    if (abs(fPDG)==22 && wavelength>275 && wavelength<477) { //not compulsury but pleasant to look at
        G4double quantumEfficiency = GetQuantumEfficiency(wavelength);
        if (G4UniformRand()*100 < quantumEfficiency) {    //this is a condition with probability of being satisfied, which is the same as the Q.E. 
        //Random returns a value from 0 to 1 (source: documentation). Hammamatsu values are from 0 to 100. that's why its random*100
          PhotoHit* newHit = new PhotoHit();
          newHit->SetWavelength(wavelength);
          newHit->SetTime(fTime);
          fHitsCollection->insert(newHit);
        }
    }

    return true;
}


void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
//    auto nofHits = fHitsCollection->entries();
//    G4cout << "-------->Hits Collection: in this event they are " << nofHits << G4endl;
//    for ( std::size_t i=0; i<nofHits; ++i ) (*fHitsCollection)[i]->Print();
}

