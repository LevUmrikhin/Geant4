#include "DetectorConstruction.hh"
#include "PhotoSD.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "G4Element.hh"
#include "G4SubtractionSolid.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VSensitiveDetector.hh"

MyDetectorConstruction::MyDetectorConstruction()
 : G4VUserDetectorConstruction(),
   logicPhotocathode(nullptr)
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    // --------------------------------------------------
    // 1) WORLD
    // --------------------------------------------------
    G4NistManager* nist = G4NistManager::Instance();
    auto solidWorld = new G4Box("World", 1.*m, 1.*m, 1.*m);
    auto logicWorld = new G4LogicalVolume(solidWorld,
                                          nist->FindOrBuildMaterial("G4_AIR"),
                                          "World");
    auto physWorld  = new G4PVPlacement(nullptr, G4ThreeVector(),
                                        logicWorld, "World",
                                        nullptr, false, 0);

    // --------------------------------------------------
    // 2) TANK + WATER
    // --------------------------------------------------
    // Tank is 2x1x1 m; Water inside minus 2 cm margin
    G4ThreeVector tankSize(2.0*m, 1.0*m, 1.0*m);
    auto solidTank = new G4Box("TankShell",
                               tankSize.x()/2, 
                               tankSize.y()/2, 
                               tankSize.z()/2);

    auto acrylic = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    auto logicTank = new G4LogicalVolume(solidTank, acrylic, "TankShell");
    new G4PVPlacement(nullptr, {}, logicTank, "TankShell",
                      logicWorld, false, 0);

    G4ThreeVector waterSize = tankSize - G4ThreeVector(2*cm, 2*cm, 2*cm);
    auto solidWater = new G4Box("WaterVolume",
                                waterSize.x()/2,
                                waterSize.y()/2,
                                waterSize.z()/2);

    auto water = nist->FindOrBuildMaterial("G4_WATER");
    {
        // Simple optical properties for water
        const G4int nEntries = 2;
        G4double energies[nEntries]      = {1.5 * eV, 6.2 * eV};
        G4double rIndexWater[nEntries]  = {1.33, 1.33};
        G4double absLengthWater[nEntries]= {100.*m, 100.*m};

        auto mptWater = new G4MaterialPropertiesTable();
        mptWater->AddProperty("RINDEX",    energies, rIndexWater,  nEntries);
        mptWater->AddProperty("ABSLENGTH", energies, absLengthWater,nEntries);
        water->SetMaterialPropertiesTable(mptWater);
    }

    auto logicWater =
        new G4LogicalVolume(solidWater, water, "WaterVolume");
    auto physWater  =
        new G4PVPlacement(nullptr, {}, logicWater, "WaterVolume",
                          logicTank, false, 0);

    // --------------------------------------------------
    // 3) White Box Shell + Water Fill inside it
    // --------------------------------------------------
    {
        // Outer/inner dimensions of the "box"
        G4ThreeVector boxOuter(20*cm, 20*cm, 20*cm);
        G4ThreeVector boxInner = boxOuter - G4ThreeVector(2*cm,2*cm,2*cm);
        G4ThreeVector whiteBoxPos(0,0,15*cm);

        // Polystyrene
        auto whiteMat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
        {
            const G4int n=2;
            G4double e[n]  = {1.5*eV, 6.2*eV};
            G4double ri[n] = {1.59, 1.59};
            auto mptWhite = new G4MaterialPropertiesTable();
            mptWhite->AddProperty("RINDEX", e, ri, n);
            whiteMat->SetMaterialPropertiesTable(mptWhite);
        }

        // Outer + Inner subtracted => shell
        auto outer = new G4Box("Outer",
                               boxOuter.x()/2, 
                               boxOuter.y()/2, 
                               boxOuter.z()/2);
        auto inner = new G4Box("Inner",
                               boxInner.x()/2, 
                               boxInner.y()/2, 
                               boxInner.z()/2);
        auto hollow = new G4SubtractionSolid("Hollow", outer, inner);

        // "Open face" cut out
        G4double faceThick = 1.01*cm;
        auto openFace = new G4Box("Face",
                                  boxOuter.x()/2 +1*mm,
                                  boxOuter.y()/2 +1*mm,
                                  faceThick/2);

        auto shell = new G4SubtractionSolid("Shell", hollow, openFace,
                                            nullptr,
                                            G4ThreeVector(0,0,
                                                -boxOuter.z()/2+faceThick/2));

        auto logicShell =
            new G4LogicalVolume(shell, whiteMat, "WhiteBoxShell");

        // Fill = water inside the shell
        auto fill = new G4Box("Fill",
                              boxInner.x()/2, 
                              boxInner.y()/2, 
                              boxInner.z()/2);
        auto logicFill = 
            new G4LogicalVolume(fill, water, "BoxWaterFill");

        // Place both in the main water volume
        auto physShell = new G4PVPlacement(nullptr, 
                                           whiteBoxPos, 
                                           logicShell,
                                           "WhiteBoxShell", 
                                           logicWater, 
                                           false, 0);

        auto physFill  = new G4PVPlacement(nullptr, 
                                           whiteBoxPos, 
                                           logicFill,
                                           "BoxWaterFill", 
                                           logicWater, 
                                           false, 0);

        // ---- Tyvek optical surface ----
        auto mptTyvek = new G4MaterialPropertiesTable();
        {
            const G4int n=2;
            G4double e[n] = {1.5*eV, 6.2*eV};
            G4double re[n]= {0.98, 0.98}; // 98% reflectivity
            G4double ef[n]= {0.0,  0.0 }; // 0% detection/absorption
            mptTyvek->AddProperty("REFLECTIVITY", e, re, n);
            mptTyvek->AddProperty("EFFICIENCY",   e, ef, n);
        }
        auto tyvek = new G4OpticalSurface("Tyvek");
        tyvek->SetType(dielectric_dielectric);
        tyvek->SetModel(unified);
        tyvek->SetFinish(groundfrontpainted);
        tyvek->SetMaterialPropertiesTable(mptTyvek);

        // ---- (A) "Fill" <-> "Shell" boundaries ----
        new G4LogicalBorderSurface("Tyvek_FillToShell",
                                   physFill, physShell, tyvek);
        new G4LogicalBorderSurface("Tyvek_ShellToFill",
                                   physShell, physFill, tyvek);

        // ---- (B) "Shell" <-> "WaterVolume" boundaries ----
        // If you also want Tyvek reflection on the *outside* 
        // of the shell in the main water volume, enable these:
        new G4LogicalBorderSurface("Tyvek_WaterToShell",
                                   physWater, physShell, tyvek);
        new G4LogicalBorderSurface("Tyvek_ShellToWater",
                                   physShell, physWater, tyvek);
    }

    // --------------------------------------------------
    // 4) PMT: (No glass shell; only the photocath remains)
    // --------------------------------------------------
    // Gold for photocath
    auto gold = new G4Material("Gold", 19.32*g/cm3, 1);
    gold->AddElement(nist->FindOrBuildElement("Au"), 1);
    {
        const G4int n=2;
        G4double e[n] = {1.5*eV, 6.2*eV};
        G4double rGold[n] = {1.0, 1.0};
        auto mptGold = new G4MaterialPropertiesTable();
        mptGold->AddProperty("RINDEX", e, rGold, n);
        gold->SetMaterialPropertiesTable(mptGold);
    }

    // Photocathode: Hemisphere r=9.8..9.9
    auto cathodeS = new G4Sphere("Photocathode",
                                 9.8*cm,  // inner
                                 9.9*cm,  // outer
                                 0.*deg, 360.*deg,
                                 0.*deg, 90.*deg);

    logicPhotocathode =
        new G4LogicalVolume(cathodeS, gold, "Photocathode");
    logicPhotocathode->SetName("Photocathode");

    // Place the photocathode directly in the water at z=-15 cm
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,-15*cm),
                      logicPhotocathode, "Photocathode",
                      logicWater, false, 0);

    // Return the world
    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    auto sd = new MySensitiveDetector("PhotoSD", "PhotoHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);

    // Bind the Photocathode volume to the SD
    if (logicPhotocathode) {
        logicPhotocathode->SetSensitiveDetector(sd);
        G4cout << "[ConstructSDandField] Bound PhotoSD to Photocathode" << G4endl;
    } else {
        G4cout << "[ConstructSDandField] logicPhotocathode is null" << G4endl;
    }
}
