//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#include "DetectorConstruction.hh"
#include "PhantomROGeometry.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "PhantomSD.hh"


DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }


DetectorConstruction::~DetectorConstruction()
{ }


G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // World and detector dimensions
  G4double world_sizeXY           = 2*m;
  G4double world_sizeZ            = 2*m;
  G4double detector_size          = 30*cm;

  G4Material* phantom_mat         = nist->FindOrBuildMaterial("G4_WATER");// Tungesten

  G4Material* world_mat           = nist->FindOrBuildMaterial("G4_Galactic");//Vacum
  
  G4Box* solidWorld               = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
      
  G4LogicalVolume* logicWorld     = new G4LogicalVolume(solidWorld, world_mat, "World");
                                   
  G4VPhysicalVolume* physWorld    = new G4PVPlacement(0, G4ThreeVector(),logicWorld,"World",  0, false,0,false);
                     
  //  detector
  //  
  G4Box* solidDetector            = new G4Box("Detector", 0.5*detector_size , 0.5*detector_size,  0.5*detector_size);
      
  G4LogicalVolume* logicdDetector = new G4LogicalVolume(solidDetector,phantom_mat,  "Detector");
               
  G4VPhysicalVolume* pDetector    = new G4PVPlacement(0,  G4ThreeVector(0.0,0.0,-20*cm), logicdDetector,  "pDetector", logicWorld,false,  0, false);
 
  G4SDManager* pSDManager         = G4SDManager::GetSDMpointer();

    //----definition des demi demension le long  x, y, z axis--------------------------
       phantomSizeX =0.5*cm ;
       phantomSizeY =0.5*cm;
       phantomSizeZ = 15*cm;

    //---- definition de nombre de voxels le long de trois axes x,y,z------------------
      // chaque voxel à un épaisseur de 0.5 cm

       numberOfVoxelsAlongX      = 1;
       numberOfVoxelsAlongY      = 1;
       numberOfVoxelsAlongZ      =30;

       G4String SdName           = "Phantom";
       mPhantomSD = new PhantomSD(SdName, G4ThreeVector( phantomSizeX, phantomSizeY, phantomSizeZ), numberOfVoxelsAlongX,  numberOfVoxelsAlongY,  numberOfVoxelsAlongZ);

       G4String ROGeometryName   = "PhantomROGeometry";
       phantomROGeometry         = new PhantomROGeometry(ROGeometryName,phantomSizeX,phantomSizeY, phantomSizeZ,numberOfVoxelsAlongX,numberOfVoxelsAlongY,numberOfVoxelsAlongZ);
       phantomROGeometry -> BuildROGeometry();
       mPhantomSD -> SetROgeometry(phantomROGeometry);
       pSDManager-> AddNewDetector(mPhantomSD);
       logicdDetector-> SetSensitiveDetector(mPhantomSD);

  return physWorld;
}

