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


#ifndef H5PhaseSpaceWriter_h
#define H5PhaseSpaceWriter_h 1
#include "G4ThreeVector.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VTouchable.hh"
#include <vector>
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class G4Track;
class H5PhaseSpaceWriter : public G4VSensitiveDetector
{
public:
  H5PhaseSpaceWriter(G4String name, G4float Z_STOP);
  ~H5PhaseSpaceWriter();
typedef struct
{
    G4int    PART_PDGE;
    G4float  PART_WEIGHT
            ,PART_POS_X
            ,PART_POS_Y
            ,PART_DIR_X
            ,PART_DIR_Y
            ,PART_DIR_Z
            ,PART_KINETIC;


}PhspData;

  typedef struct
  {
      G4int
               EVENT_ID,
               NUMBER_OF_ENTRIES;

  }EventData;

  typedef struct
  {
     G4int    NUMBER_OF_HISORIES;
     G4float     Z_STOP;

  }BeamData;
 //
  PhspData tmp;
  EventData tmp_event;
  //
  BeamData  myBeamData[1];
  //
  std::vector<PhspData>    myPhspData;
  std::vector<EventData>   myEventData;

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*aStep, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  void PHOTONS_ENERGY(G4double);
  void ELECTRONS_ENERGY(G4double);
  void POSITRONS_ENERGY(G4double);
  void PROTONS_ENERGY(G4double);
  void NEUTRONS_ENERGY(G4double);
  void FILL_DATA (G4Step*& aStep );
  void WRITE_PHSP_FILE( G4int i);
  void SET_PHASE_SPACE_FILE_NAME( G4String FILE_NAME);
  void SUMMARY();
private:

G4String FILE_NAME;
G4float  Z_STOP;
};
#endif


