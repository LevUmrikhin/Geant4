#include "DetectorConstruction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // DIMENSIONS
    auto sphi = 0.*deg;
    auto dphi = 360.*deg;
    auto worldR = 1.*m;


    // MATERIALS
    G4NistManager *nist = G4NistManager::Instance();

    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *water = nist->FindOrBuildMaterial("G4_WATER");
//    G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
    G4Material *glass = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    G4Material *CdS = new G4Material("CdS", 4.82*g/cm3, 2);
    CdS->AddElement(nist->FindOrBuildElement("Cd"), 50.0*perCent);
    CdS->AddElement(nist->FindOrBuildElement("S"), 50.0*perCent);

    G4Material *scint = new G4Material("Scint", 1.000*g/cm3, 2);
    scint->AddMaterial(water, 99.99653*perCent);
    scint->AddMaterial(CdS, 0.00347*perCent);

    //G4Material *fLXe = new G4Material("fLXe", 3.020*g/cm3, 1);
    //fLXe -> AddElement(nist->FindOrBuildElement("Xe"), 100.0*perCent);


    // FROM OMRI START
    G4double energy[32] = 
    {2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV, 2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV, 2.341*eV, 2.386*eV,
     2.433*eV, 2.481*eV, 2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV, 2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
     3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV, 3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV, 3.760*eV, 3.877*eV, 
     4.002*eV, 4.136*eV}; // energy of photons

    G4double rindexScint[32] = 
    {1.3435, 1.3440, 1.3445, 1.3450, 1.3455, 1.3460, 1.3465, 1.3470, 1.3475, 1.3480,
     1.3485, 1.3492, 1.3500, 1.3505, 1.3510, 1.3518, 1.3522, 1.3530, 1.3535, 1.3540,
     1.3545, 1.3550, 1.3555, 1.3560, 1.3568, 1.3572, 1.3580, 1.3585, 1.3590, 1.3595,
     1.3600, 1.3608}; // refractive index of scint

    G4double energyScint[32] = 
    {1.771*eV, 1.914*eV, 2.057*eV, 2.200*eV, 2.343*eV, 2.486*eV, 2.629*eV, 2.771*eV, 2.914*eV, 3.057*eV,
     3.200*eV, 3.343*eV, 3.486*eV, 3.629*eV, 3.771*eV, 3.914*eV, 4.057*eV, 4.200*eV, 4.343*eV, 4.486*eV,
     4.629*eV, 4.771*eV, 4.914*eV, 5.057*eV, 5.200*eV, 5.343*eV, 5.486*eV, 5.629*eV, 5.771*eV, 5.914*eV,
     6.057*eV, 6.200*eV};

    //for Scintillation 15/3/22
    G4double absorption[32] =
    { 3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m, 15.152*m, 17.241*m, 18.868*m, 20.000*m,
     26.316*m, 35.714*m, 45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m, 52.632*m, 47.619*m,
     45.455*m, 41.667*m, 37.037*m, 33.333*m, 30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m, 
     17.500*m, 14.500*m};
    
    G4double scintFast[32] =
    {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00};
    
    G4double scintSlow[32] =
    {0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00,
     8.00, 7.00, 6.00, 4.00, 3.00, 2.00, 1.00, 0.01, 1.00, 2.00,
     3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00,
     5.00, 4.00};
    
    G4MaterialPropertiesTable *mptScint = new G4MaterialPropertiesTable();
    //emission spectrum of QD, data taken from the paper's figure 5
    const G4int nEntries = 112;
G4double photonEnergy[nEntries] = { //energy in ev corresponding to wavelengths 
    2.451*eV, 2.457*eV, 2.496*eV, 2.525*eV, 2.540*eV, 2.561*eV, 2.590*eV, 2.622*eV, 2.640*eV, 2.663*eV, 
    2.690*eV, 2.699*eV, 2.712*eV, 2.722*eV, 2.732*eV, 2.743*eV, 2.749*eV, 2.764*eV, 2.770*eV, 2.762*eV,
    2.760*eV, 2.754*eV, 2.752*eV, 2.749*eV, 2.749*eV, 2.748*eV, 2.745*eV, 2.744*eV, 2.743*eV, 2.742*eV,
    2.740*eV, 2.739*eV, 2.737*eV, 2.736*eV, 2.727*eV, 2.728*eV, 2.722*eV, 2.718*eV, 2.712*eV, 2.713*eV,
    2.713*eV, 2.717*eV, 2.715*eV, 2.715*eV, 2.711*eV, 2.709*eV, 2.709*eV, 2.706*eV, 2.706*eV, 2.704*eV,
    2.701*eV, 2.700*eV, 2.698*eV, 2.694*eV, 2.695*eV, 2.693*eV, 2.691*eV, 2.688*eV, 2.681*eV, 2.667*eV,
    2.660*eV, 2.656*eV, 2.653*eV, 2.651*eV, 2.648*eV, 2.647*eV, 2.643*eV, 2.641*eV, 2.639*eV, 2.636*eV,
    2.634*eV, 2.632*eV, 2.631*eV, 2.628*eV, 2.627*eV, 2.625*eV, 2.623*eV, 2.622*eV, 2.620*eV, 2.618*eV,
    2.616*eV, 2.615*eV, 2.613*eV, 2.611*eV, 2.610*eV, 2.608*eV, 2.607*eV, 2.605*eV, 2.603*eV, 2.601*eV,
    2.600*eV, 2.598*eV, 2.597*eV, 2.595*eV, 2.594*eV, 2.592*eV, 2.590*eV, 2.589*eV, 2.587*eV, 2.585*eV,
    2.584*eV, 2.582*eV, 2.580*eV, 2.579*eV, 2.577*eV, 2.576*eV, 2.574*eV, 2.573*eV, 2.571*eV, 2.570*eV
};

G4double emissionSpectrum[nEntries] = { //relative intensity, no number
    0.0259, 0.0229, 0.0229, 0.0321, 0.0352, 0.0475, 0.0568, 0.0722, 0.0845, 0.0968,
    0.1122, 0.1245, 0.1337, 0.1491, 0.1645, 0.1768, 0.1953, 0.2261, 0.2507, 0.2692,
    0.2907, 0.3123, 0.3338, 0.3707, 0.3492, 0.3861, 0.4077, 0.4231, 0.4415, 0.4600,
    0.4754, 0.4908, 0.5093, 0.5246, 0.5708, 0.5585, 0.5924, 0.6139, 0.6478, 0.6324,
    0.6293, 0.7032, 0.6878, 0.6662, 0.7278, 0.7709, 0.7524, 0.8017, 0.7863, 0.8232,
    0.8479, 0.8663, 0.8817, 0.9125, 0.9002, 0.9248, 0.9464, 0.9710, 0.9894, 0.9710,
    0.9464, 0.9279, 0.9094, 0.8909, 0.8694, 0.8509, 0.8263, 0.7986, 0.7801, 0.7555,
    0.7340, 0.7155, 0.6970, 0.6724, 0.6539, 0.6324, 0.6170, 0.5985, 0.5770, 0.5554,
    0.5339, 0.5154, 0.4939, 0.4692, 0.4508, 0.4323, 0.4138, 0.3984, 0.3800, 0.3584,
    0.3307, 0.3061, 0.2846, 0.2599, 0.2353, 0.2107, 0.1891, 0.1707, 0.1491, 0.1276,
    0.1091, 0.0937, 0.0845, 0.0691, 0.0568, 0.0445, 0.0383, 0.0260, 0.0260, 0.0198,
    0.0198, 0.0229
};
    //dependant on energy
    mptScint->AddProperty("RINDEX", energyScint, rindexScint, 32);
    mptScint->AddProperty("ABSLENGTH", energyScint, absorption, 32);
    mptScint->AddProperty("SCINTILLATIONCOMPONENT1", energyScint, scintFast, 32);
    mptScint->AddProperty("SCINTILLATIONCOMPONENT2", energyScint, scintSlow, 32);
    //independant on energy
    mptScint->AddConstProperty("SCINTILLATIONYIELD", 1357./MeV); //  1357./MeV originally ???
    mptScint->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1.*ns);       //fast time constant of scintillator
    mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10.*ns);      //slow time constant of scintillator
    mptScint->AddConstProperty("SCINTILLATIONYIELD1", 0.8, 0.5);
    mptScint->AddConstProperty("SCINTILLATIONYIELD2", 0.8, 0.5);
    // Check that group velocity is calculated from RINDEX
    if(mptScint->GetProperty("RINDEX")->GetVectorLength() != mptScint->GetProperty("GROUPVEL")->GetVectorLength())
    {
        G4ExceptionDescription ed;
        ed << "Error calculating group velocities. Incorrect number of entries in group velocity material property vector.";
        G4Exception("OpNovice::OpNoviceDetectorConstruction", "OpNovice001",  FatalException, ed);
    }

    scint->SetMaterialPropertiesTable(mptScint);
    
    // Set the Birks Constant for the scintillator (assumption)
    scint->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

    // Air
    G4double rindexAir[2] = {1.00, 1.00};
    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable(); //Air (worldMat) properties table
    mptAir->AddProperty("RINDEX", energy, rindexAir, 2);
    air->SetMaterialPropertiesTable(mptAir);

    //Glass
    G4double rindexGlass[2] = {1.47, 1.47};
    G4double glass_AbsLength[2] = {4.2 * m, 4.2 * m};
     G4MaterialPropertiesTable *mptGlass = new G4MaterialPropertiesTable();       //Glass properties table
    mptGlass->AddProperty("ABSLENGTH", energy, glass_AbsLength, 2);
    mptGlass->AddProperty("RINDEX", energy, rindexGlass, 2);
    glass->SetMaterialPropertiesTable(mptGlass);


 

    // THE WORLD
    auto worldS = new G4Box("worldS", worldR, worldR, worldR); // name, x/2, y/2, z/2
    auto worldL = new G4LogicalVolume(worldS, air, "worldL"); // solid, material, name
    auto worldP = new G4PVPlacement(0,                // no rotation
                                    G4ThreeVector(),  // at (0, 0, 0)
                                    worldL,           // its logical volume
                                    "worldP",         // its name
                                    0,                // its mother volume
                                    false,            // no boolean operation
                                    0,                // copy number
                                    true);            // check for overlaps
    worldL->SetVisAttributes(G4VisAttributes::GetInvisible());


    // STAND
    auto standS = new G4Box("standS", 0.06*m, 0.06*m, 0.07325*m);
    standL = new G4LogicalVolume(standS, Al, "standL");
    new G4PVPlacement(0,
                      G4ThreeVector(-0.30*m, 0.*m, -0.205*m),
                      standL,
                      "standP",
                      worldL,
                      false,
                      0,
                      true);
    G4VisAttributes *standVisAtt = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9, 1));
    standVisAtt->G4VisAttributes::SetForceSolid(true);
    standL->SetVisAttributes(standVisAtt);

    // BOTTLE
    auto bottle1S = new G4Tubs("bottle1S", 0, 0.040*m, 0.095*m, sphi, dphi);
    auto bottle2S = new G4Tubs("bottle2S", 0, 0.037*m, 0.087*m, sphi, dphi);
    auto bottleS = new G4SubtractionSolid ("bottleS", bottle1S, bottle2S);
    
    // BOTTLENECK 
    auto neckOuterCone = new G4Cons("neckOuterCone", 0.037*m, 0.040*m, 
                                                      0.013*m, 0.015*m, 
                                                      0.025*m, 0.0*deg, 360.0*deg);
    auto neckInnerCone = new G4Cons("neckInnerCone", 0.035*m, 0.037*m, 0.011*m, 0.013*m, 0.025*m, 0.0*deg, 360.0*deg);
    auto neckSolid = new G4SubtractionSolid ("neckSolid", neckOuterCone, neckInnerCone);
    auto bottleWithNeck = new G4UnionSolid("bottleWithNeck", bottleS, neckSolid, nullptr, G4ThreeVector(0, 0, 0.1075*m));
    
    bottleL = new G4LogicalVolume(bottleWithNeck, glass, "bottleL");
    //!!!
    
    new G4PVPlacement(0,
                      G4ThreeVector(-0.30*m, 0.*m, -0.036*m),
                      bottleL,
                      "bottleP",
                      worldL,
                      false,
                      0,
                      true);
    G4VisAttributes *bottleVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
    bottleVisAtt->G4VisAttributes::SetForceSolid(true);
    bottleL->SetVisAttributes(bottleVisAtt);

    // QD
    //auto qdS = new G4Tubs("qdS", 0.*m, 0.037*m, 0.032*m, 0., 2.0*CLHEP::pi);
    auto qdS = new G4Tubs("qdS", 0.*m, 0.037*m, 0.067*m, 0., 2.0*CLHEP::pi);
    qdL = new G4LogicalVolume(qdS, scint, "qdL");
    new G4PVPlacement(0,
                      G4ThreeVector(0*m, 0*m, -0.0545*m),
                      qdL,
                      "qdP",
                      bottleL,
                      false,
                      0,
                      true);
    G4VisAttributes *qdVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.5, 1));
    qdVisAtt->G4VisAttributes::SetForceSolid(true);
    qdL->SetVisAttributes(qdVisAtt);

    // PMT
    auto pmtS = new G4Sphere("pmtS", 0.07*m, 0.076*m, 0.*CLHEP::pi, 1.*CLHEP::pi, 0.*CLHEP::pi, 1.*CLHEP::pi);
    pmtL = new G4LogicalVolume(pmtS, glass, "pmtL");
    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateZ(-0.5*CLHEP::pi);
    new G4PVPlacement(rotationMatrix,
                      G4ThreeVector(-0.15*m, 0.*m, -0.09*m),
                      pmtL,
                      "pmtP",
                      worldL,
                      false,
                      0,
                      true);
    G4VisAttributes *pmtVisAtt = new G4VisAttributes(G4Colour(0.7, 0.2, 0.7, 0.7));
    pmtVisAtt->G4VisAttributes::SetForceSolid(true);
    pmtL->SetVisAttributes(pmtVisAtt);


    return worldP;
}


void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *PhotoSD = new MySensitiveDetector("PhotoSD", "PhotoHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(PhotoSD);
    SetSensitiveDetector("pmtL", PhotoSD);
}

