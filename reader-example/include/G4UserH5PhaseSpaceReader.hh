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


#ifndef G4UserH5PhaseSpaceReader_h
#define G4UserH5PhaseSpaceReader_h 1

#include "G4VPrimaryGenerator.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class G4ParticleDefinition;

class G4UserH5PhaseSpaceReader : public G4VPrimaryGenerator
{
  public:
    G4UserH5PhaseSpaceReader();
     ~G4UserH5PhaseSpaceReader();

    // method from the base class
   //virtual void GeneratePrimaries(G4Event*);

public:

  void GeneratePrimaryVertex(G4Event* anEvent);   // Mandatory

  void SET_EVENT_GENERATION_FLAG(G4int FLAG);

  void INITIALIZE();

  void SET_PARAMETERS(G4String, bool, G4float, int, int);


    typedef struct
    {
       int    PART_PDGE;
       float  PART_WEIGHT
                ,PART_POS_X
                ,PART_POS_Y
                ,PART_POS_Z
                ,PART_DIR_X
                ,PART_DIR_Y
                ,PART_DIR_Z
                ,PART_KINETIC;


    }PhspData;

     typedef struct
     {
         G4int    EVENT_ID,
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
  private:
     G4int particle_pdge,Event_ID, particle_weight,Number_of_Entries,NUMBER_OF_SIMULATED_HISTORIES;
     G4float particle_time,pos_x, pos_y, pos_z, dir_x,dir_y,dir_z, Z_STOP, energy,Z_plane;
     G4String FileName;
     bool apply;
     G4int EVENT_GENERATION_FLAG, SPLITTING_FACTOR;

  void GENERATE_NEW_PART_ROT_SPLITTING(int incr,G4Event*& anEvent);
  void GENERATE_NEW_PART_SPLITTING(int incr,G4Event*& anEvent);
  void GENERATE_NEW_PARTICLE(int incr,G4Event*& ev);
  void READ_EVENT_DATA();
  void SET_PHASE_SPACE_FILE_NAME( G4String  _FileName);
  void READ_BEAM_DATA();
  void SET_Z_PLANE( G4double Z_plane,  bool apply );
  void READ_PHASE_SPACE_FILE();
  G4ParticleDefinition* GET_PARTICLE_DEFINITION(int& PDGE);


};

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#endif


