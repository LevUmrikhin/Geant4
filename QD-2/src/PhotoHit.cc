#include "PhotoHit.hh"


G4ThreadLocal G4Allocator<PhotoHit>* PhotoHitAllocator = nullptr;


PhotoHit::PhotoHit()
{}


PhotoHit::~PhotoHit() {}


G4bool PhotoHit::operator==(const PhotoHit& right) const
{
    return ( this == &right ) ? true : false;
}
