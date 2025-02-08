#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4UnitsTable.hh"
#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"

#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include <iomanip>


class PhotoHit : public G4VHit
{
public:
    PhotoHit();
    PhotoHit(const PhotoHit&) = default;
    ~PhotoHit() override;

    // operators
    PhotoHit& operator=(const PhotoHit&) = default;
    G4bool operator==(const PhotoHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // set methods
    void SetWavelength (G4double wavelength) { fWavelength = wavelength; };
//    void SetEnergy (G4double energy) { fEnergy = energy; };
     void SetTime   (G4double time)   { fTime   = time;   };

    // get methods
    G4double GetWavelength() const { return fWavelength; };
//    G4double GetEnergy() const { return fEnergy; };
      G4double GetTime()   const { return fTime;   };
  
private:
    G4double fWavelength;
    //G4double fEnergy;
    G4double fTime;
};


typedef G4THitsCollection<PhotoHit> PhotoHitsCollection;

extern G4ThreadLocal G4Allocator<PhotoHit>* PhotoHitAllocator;


inline void* PhotoHit::operator new(size_t)
{
    if(!PhotoHitAllocator)
        PhotoHitAllocator = new G4Allocator<PhotoHit>;
    return (void *) PhotoHitAllocator->MallocSingle();
}

inline void PhotoHit::operator delete(void *hit)
{
    PhotoHitAllocator->FreeSingle((PhotoHit*) hit);
}



