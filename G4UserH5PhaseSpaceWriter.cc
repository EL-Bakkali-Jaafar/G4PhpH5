/*
 * Copyright (C) 2015 Jaafar EL Bakkali
 * <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *
 *   AUTHOR:
 *
 *   Jaafar EL Bakkali, PhD in Radiation and Nuclear Systems
 *   e-mail: bahmedj@gmail.com
 *   University of AbdelMalek Essaadi, Morocco
 *   Faculty of Science Tetouan

 <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 * For documentation
 * see http://G4PhspH5.sourceforge.net
 *
 * - 13/02/2015: public version 1.0
 *
 <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/

#include "G4UserH5PhaseSpaceWriter.hh"

G4UserH5PhaseSpaceWriter * G4UserH5PhaseSpaceWriter::theInstance = 0;

G4UserH5PhaseSpaceWriter * G4UserH5PhaseSpaceWriter::GetInstance()
{
  if (!theInstance) theInstance = new G4UserH5PhaseSpaceWriter();
  return theInstance;
}


#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include <algorithm>
#include <set>
#include <sstream>
#include <vector>
#include "H5PhaseSpaceWriter.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "H5PhaseSpaceWriter.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"


G4UserH5PhaseSpaceWriter::G4UserH5PhaseSpaceWriter()
{


}


G4UserH5PhaseSpaceWriter::~G4UserH5PhaseSpaceWriter()
{

}


/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
/*<><>            SET_PARAMETERS method                                                 <><>*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/

void G4UserH5PhaseSpaceWriter::SET_PARAMETERS( G4String _FILE_NAME,G4double zstop, G4double X_PLANE_HALF_SIZE,G4double Y_PLANE_HALF_SIZE,  G4LogicalVolume*& logicWorld)
{

    // Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();


        G4double detector_sizeZ  = 1*mm;


        G4Material* vaccum_mat = nist->FindOrBuildMaterial("G4_Galactic");

    //  detector
    //
       G4Box* solidDetector=new G4Box("Detector", X_PLANE_HALF_SIZE/mm, Y_PLANE_HALF_SIZE/mm, detector_sizeZ);

       G4LogicalVolume* logicdDetector =new G4LogicalVolume(solidDetector,vaccum_mat,"Detector");

       new G4PVPlacement(0, G4ThreeVector(0,0,zstop/mm+detector_sizeZ),logicdDetector,"pDetector",logicWorld,false,0,false);


       G4String SdName = "Phantom";
       G4SDManager* pSDManager = G4SDManager::GetSDMpointer();
       mSD = new H5PhaseSpaceWriter(SdName,zstop);
       mSD->SET_PHASE_SPACE_FILE_NAME(_FILE_NAME);
       pSDManager-> AddNewDetector(mSD);
       logicdDetector-> SetSensitiveDetector(mSD);

}


