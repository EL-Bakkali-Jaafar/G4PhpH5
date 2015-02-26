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

#include <sstream>
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4Circle.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include <stdio.h>
#include "H5PhaseSpaceWriter.hh"
#include "G4RunManager.hh"
#include <vector>
#include "G4SystemOfUnits.hh"
#include "H5Cpp.h"
#include "G4Run.hh"
#include "G4SliceTimer.hh"

G4SliceTimer *timer;
using    namespace std;
static   int TOTAL_NUMBER_OF_TRACKS    =  0,
             NUMBER_OF_PHSP_FILE       =  1,
             NUMBER_OF_PHOTONS         =  0,
             NUMBER_OF_ELECTRONS       =  0,
             NUMBER_OF_POSITRONS       =  0,
             NUMBER_OF_PROTONS         =  0,
             NUMBER_OF_NEUTRONS        =  0,
             NUMBER_OF_PARTICLES       =  0,
             NUMBER_OF_ENTRIES         =  0,
             NUMBER_OF_ACTIVE_EVENT    =  0;
int NUMBER_OF_SIMULATED_HISTORIES      =  0,
    EVENT_ID                           =  0;

G4float           ELECTRON_ENERGY_MAX  =  0,
                  ELECTRON_ENERGY_MIN  =  0,
                  ELECTRON_ENERGY_MEAN =  0,
                  PHOTON_ENERGY_MAX    =  0,
                  PHOTON_ENERGY_MIN    =  0,
                  PHOTON_ENERGY_MEAN   =  0,
                  POSITRON_ENERGY_MAX  =  0,
                  POSITRON_ENERGY_MIN  =  0,
                  POSITRON_ENERGY_MEAN =  0,
                  PROTON_ENERGY_MAX    =  0,
                  PROTON_ENERGY_MIN    =  0,
                  PROTON_ENERGY_MEAN   =  0,
                  NEUTRON_ENERGY_MAX   =  0,
                  NEUTRON_ENERGY_MIN   =  0,
                  NEUTRON_ENERGY_MEAN  =  0,
                  POST_POSITION_Z      =  0,
                  PRE_POSITION_Z       =  0,
                  POST_ENERGY          =  0,
                  PRE_ENERGY           =  0;

int TRACK_ID=0;

H5PhaseSpaceWriter::H5PhaseSpaceWriter(G4String name, G4float Z_STOP)
    : G4VSensitiveDetector(name)

{
      timer = new G4SliceTimer();
      this->Z_STOP=Z_STOP/mm;

      G4cout<<" CREATION OF NEW PHASE-SPACE HDF5-BASED FORMAT AT Z="<<  this->Z_STOP<< " mm"<< G4endl;

    }

H5PhaseSpaceWriter::~H5PhaseSpaceWriter()
{ 
 delete timer;
}

void H5PhaseSpaceWriter::Initialize(G4HCofThisEvent*)
{
   G4RunManager* runManager                  =  G4RunManager::GetRunManager();
   NUMBER_OF_SIMULATED_HISTORIES             =  runManager->GetCurrentRun()->GetNumberOfEventToBeProcessed();
   myBeamData[0].NUMBER_OF_HISORIES          =  NUMBER_OF_SIMULATED_HISTORIES;
   myBeamData[0].Z_STOP                      =   this->Z_STOP;
}


G4bool H5PhaseSpaceWriter::ProcessHits(G4Step* aStep , G4TouchableHistory*)
{
    FILL_DATA (aStep );
    TOTAL_NUMBER_OF_TRACKS++;
    NUMBER_OF_ENTRIES++;
    aStep ->GetTrack()->SetTrackStatus(fStopAndKill);
    return true;

}

void H5PhaseSpaceWriter::EndOfEvent(G4HCofThisEvent* )
{
    G4RunManager* runManager =   G4RunManager::GetRunManager();

    const G4Event * event = runManager->GetCurrentEvent();
    EVENT_ID=event->GetEventID();
    if (NUMBER_OF_ENTRIES>0){
    tmp_event.EVENT_ID                      =  EVENT_ID;
    tmp_event.NUMBER_OF_ENTRIES             =  NUMBER_OF_ENTRIES;
    myEventData.push_back(tmp_event);
    NUMBER_OF_ACTIVE_EVENT++;
}
    WRITE_PHSP_FILE (NUMBER_OF_SIMULATED_HISTORIES-1);
    NUMBER_OF_ENTRIES=0;
}

void H5PhaseSpaceWriter::WRITE_PHSP_FILE (G4int ID_OF_LAST_EVENT)
{

    G4RunManager* runManager =   G4RunManager::GetRunManager();
    const G4Event * event    =   runManager->GetCurrentEvent();
    G4int event_id           =   event->GetEventID();

    if( event_id== ID_OF_LAST_EVENT) {
    const std::string DatasetName("PhspData");
    const std::string DatasetName_event("EventData");
    const std::string DatasetName_histories("BeamData");
    const std::string MEMBER_EVENTID("_EVENTID");
    const std::string MEMBER_ENTERIES("_ENTERIES");
    const std::string MEMBER_PART_PDGE("_PART_PDGE");
    const std::string MEMBER_PART_WEIGHT("_PART_WEIGHT");
    const std::string MEMBER_PART_POS_X("_PART_POS_X");
    const std::string MEMBER_PART_POS_Y("_PART_POS_Y");
    const std::string MEMBER_PART_DIR_X("_PART_DIR_X");
    const std::string MEMBER_PART_DIR_Y("_PART_DIR_Y");
    const std::string MEMBER_PART_DIR_Z("_PART_DIR_Z");
    const std::string MEMBER_PART_KINETIC("_PART_KINETIC");
    const std::string MEMBER_HISTORIES("_NUMBER_OF_HISTORIES");
    const std::string MEMBER_Z_STOP("_Z_STOP");


    // the array of each length of multidimentional data.
    hsize_t dim[1], dim_event[1], dim_histories[1];

    dim_histories[0] = sizeof(myBeamData) / sizeof(BeamData);
    dim[0]        =  myPhspData.size();
    dim_event[0]  =  myEventData.size();

    // the length of dim
    int rank                = sizeof(dim) / sizeof(hsize_t);
    int rank_event          = sizeof(dim_event) / sizeof(hsize_t);
    int rank_histories      = sizeof(dim_histories) / sizeof(hsize_t);


    // defining the datatype to pass HDF5
    H5::CompType mtype(sizeof(PhspData));
    mtype.insertMember(MEMBER_PART_WEIGHT,  HOFFSET(PhspData, PART_WEIGHT),   H5::PredType::NATIVE_FLOAT);
    mtype.insertMember(MEMBER_PART_POS_X,   HOFFSET(PhspData, PART_POS_X),    H5::PredType::NATIVE_FLOAT );
    mtype.insertMember(MEMBER_PART_PDGE,    HOFFSET(PhspData, PART_PDGE),     H5::PredType::NATIVE_INT   );
    mtype.insertMember(MEMBER_PART_POS_Y,   HOFFSET(PhspData, PART_POS_Y),    H5::PredType::NATIVE_FLOAT );
    mtype.insertMember(MEMBER_PART_DIR_X,   HOFFSET(PhspData, PART_DIR_X),    H5::PredType::NATIVE_FLOAT );
    mtype.insertMember(MEMBER_PART_DIR_Y,   HOFFSET(PhspData, PART_DIR_Y),    H5::PredType::NATIVE_FLOAT );
    mtype.insertMember(MEMBER_PART_DIR_Z,   HOFFSET(PhspData, PART_DIR_Z),    H5::PredType::NATIVE_FLOAT );
    mtype.insertMember(MEMBER_PART_KINETIC, HOFFSET(PhspData, PART_KINETIC),  H5::PredType::NATIVE_FLOAT );


    H5::CompType mtype_event(sizeof(EventData));
    mtype_event.insertMember(MEMBER_EVENTID,  HOFFSET(EventData, EVENT_ID),                  H5::PredType::NATIVE_INT);
    mtype_event.insertMember(MEMBER_ENTERIES, HOFFSET(EventData, NUMBER_OF_ENTRIES),         H5::PredType::NATIVE_INT);

    H5::CompType mtype_histories(sizeof(BeamData));
    mtype_histories.insertMember(MEMBER_HISTORIES,  HOFFSET(BeamData, NUMBER_OF_HISORIES),   H5::PredType::NATIVE_INT);
    mtype_histories.insertMember(MEMBER_Z_STOP,     HOFFSET(BeamData, Z_STOP),               H5::PredType::NATIVE_FLOAT);


    string FILE_NAME = this->FILE_NAME+".h5";

    // preparation of a dataset and a file.
    H5::DataSpace space(rank, dim);
    H5::DataSpace space_event(rank_event, dim_event);
    H5::DataSpace space_histories(rank_histories, dim_histories);


    H5::H5File *file                 = new H5::H5File(FILE_NAME, H5F_ACC_TRUNC	);
    H5::DataSet *dataset             = new H5::DataSet(file->createDataSet(DatasetName, mtype, space));
    H5::DataSet *dataset_event       = new H5::DataSet(file->createDataSet(DatasetName_event, mtype_event, space_event));
    H5::DataSet *dataset_histories   = new H5::DataSet(file->createDataSet(DatasetName_histories, mtype_histories, space_histories));

    dataset_histories->write(myBeamData, mtype_histories);

    EventData *pEventData          =  new EventData[myEventData.size()];

    PhspData *pPhspData             =  new PhspData[myPhspData.size()];

    for (unsigned int i = 0; i < myPhspData.size(); i++){

    pPhspData[i].PART_PDGE           =  myPhspData[i].PART_PDGE;
    pPhspData[i].PART_WEIGHT         =  myPhspData[i].PART_WEIGHT;
    pPhspData[i].PART_POS_X          =  myPhspData[i].PART_POS_X ;// mm
    pPhspData[i].PART_POS_Y          =  myPhspData[i].PART_POS_Y;
    pPhspData[i].PART_DIR_X          =  myPhspData[i].PART_DIR_X;
    pPhspData[i].PART_DIR_Y          =  myPhspData[i].PART_DIR_Y ;
    pPhspData[i].PART_DIR_Z          =  myPhspData[i].PART_DIR_Z;
    pPhspData[i].PART_KINETIC        =  myPhspData[i].PART_KINETIC ; //MeV

 };

 for (unsigned int i = 0; i < myEventData.size(); i++){

    pEventData[i].EVENT_ID           =  myEventData[i].EVENT_ID    ;
    pEventData[i].NUMBER_OF_ENTRIES  =  myEventData[i].NUMBER_OF_ENTRIES;


 }
     myEventData.clear();

     dataset->write(pPhspData, mtype);

     myPhspData.clear();

    delete[] pPhspData;

    delete[] pEventData;

    delete   dataset_event;
    delete   dataset_histories;
    delete   dataset;
    delete   file;

    // WRITE SUMMARY OF SIMULATION
    SUMMARY();
    //

    NUMBER_OF_PHOTONS       = 0;
    NUMBER_OF_ELECTRONS     = 0;
    NUMBER_OF_PROTONS       = 0;
    NUMBER_OF_NEUTRONS      = 0;
    NUMBER_OF_POSITRONS     = 0;
    NUMBER_OF_PARTICLES     = 0;
    PHOTON_ENERGY_MIN       = 0;
    PHOTON_ENERGY_MIN       = 0;
    PHOTON_ENERGY_MEAN      = 0;
    ELECTRON_ENERGY_MIN     = 0;
    ELECTRON_ENERGY_MIN     = 0;
    ELECTRON_ENERGY_MEAN    = 0;
    POSITRON_ENERGY_MIN     = 0;
    POSITRON_ENERGY_MIN     = 0;
    POSITRON_ENERGY_MEAN    = 0;
    PROTON_ENERGY_MIN       = 0;
    PROTON_ENERGY_MIN       = 0;
    PROTON_ENERGY_MEAN      = 0;
    NEUTRON_ENERGY_MIN      = 0;
    NEUTRON_ENERGY_MIN      = 0;
    NEUTRON_ENERGY_MEAN     = 0;
    TOTAL_NUMBER_OF_TRACKS  =-1;
    NUMBER_OF_PHSP_FILE++;
    }
}

void H5PhaseSpaceWriter::FILL_DATA ( G4Step*& aStep )
{
tmp.PART_PDGE              =  aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
tmp.PART_WEIGHT            =  aStep->GetTrack()->GetWeight();
tmp.PART_POS_X             =  aStep->GetPreStepPoint()->GetPosition().x()/mm;
tmp.PART_POS_Y             =  aStep->GetPreStepPoint()->GetPosition().y()/mm;
tmp.PART_DIR_X             =  aStep->GetPreStepPoint()->GetMomentumDirection().x()/mm;
tmp.PART_DIR_Y             =  aStep->GetPreStepPoint()->GetMomentumDirection().y()/mm;
tmp.PART_DIR_Z             =  aStep->GetPreStepPoint()->GetMomentumDirection().z()/mm;
tmp.PART_KINETIC           =  (aStep->GetTrack()->GetKineticEnergy()/MeV);


switch(tmp.PART_PDGE)
  {
  case 22:
    // GAMMA
    NUMBER_OF_PHOTONS++;
    PHOTONS_ENERGY   (  tmp.PART_KINETIC         );
    break;
  case 11:
  // ELECTRON

    NUMBER_OF_ELECTRONS++;
    ELECTRONS_ENERGY (  tmp.PART_KINETIC         );

    break;
  case -11:

   // POSITRON
    NUMBER_OF_POSITRONS++;
    POSITRONS_ENERGY (  tmp.PART_KINETIC         );
    break;
  case 2112:
  // NEUTRON

    NUMBER_OF_PROTONS++;
    PROTONS_ENERGY   (  tmp.PART_KINETIC         );

    break;
  case 2122:
     // PROTON

    NUMBER_OF_NEUTRONS++;
    NEUTRONS_ENERGY  (  tmp.PART_KINETIC         );
    break;
  default:
    G4cout << "ERROR:  PARTICLE CODE NOT SUPPORTED  AT EVENT ID: "
       << G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()
       << "." << G4endl;
  }

myPhspData.push_back(tmp);

 /*............................*/  NUMBER_OF_PARTICLES++;


}


void H5PhaseSpaceWriter::PHOTONS_ENERGY(G4double Energy )
{
G4double TEMP = Energy;

if (PHOTON_ENERGY_MAX < TEMP ) PHOTON_ENERGY_MAX  = Energy;

if (PHOTON_ENERGY_MAX > TEMP ) PHOTON_ENERGY_MIN  = Energy;

/*.........................*/ PHOTON_ENERGY_MEAN =  PHOTON_ENERGY_MEAN+Energy;

}

void H5PhaseSpaceWriter::ELECTRONS_ENERGY(G4double Energy )
{
G4double TEMP = Energy;

if (ELECTRON_ENERGY_MAX < TEMP ) ELECTRON_ENERGY_MAX  = Energy;

if (ELECTRON_ENERGY_MAX > TEMP ) ELECTRON_ENERGY_MIN  = Energy;

/*.........................*/ ELECTRON_ENERGY_MEAN    = ELECTRON_ENERGY_MEAN+Energy;

}

void H5PhaseSpaceWriter::POSITRONS_ENERGY(G4double Energy )
{
G4double TEMP = Energy;

if (POSITRON_ENERGY_MAX < TEMP ) POSITRON_ENERGY_MAX  = Energy;

if (POSITRON_ENERGY_MAX > TEMP ) POSITRON_ENERGY_MIN  = Energy;

/*............................*/ POSITRON_ENERGY_MEAN = POSITRON_ENERGY_MEAN+Energy;

}

void H5PhaseSpaceWriter::NEUTRONS_ENERGY(G4double Energy )
{
G4double TEMP = Energy;

if (NEUTRON_ENERGY_MAX < TEMP ) NEUTRON_ENERGY_MAX   = Energy;

if (NEUTRON_ENERGY_MAX > TEMP ) NEUTRON_ENERGY_MIN   = Energy;

/*............................*/ NEUTRON_ENERGY_MEAN = NEUTRON_ENERGY_MEAN+Energy;

}
void H5PhaseSpaceWriter::PROTONS_ENERGY(G4double Energy )
{
G4double TEMP = Energy;

if (PROTON_ENERGY_MAX < TEMP ) PROTON_ENERGY_MAX    = Energy;

if (PROTON_ENERGY_MAX > TEMP ) PROTON_ENERGY_MIN    = Energy;

/*............................*/ PROTON_ENERGY_MEAN = PROTON_ENERGY_MEAN+Energy;

}

void H5PhaseSpaceWriter::clear()
{} 

void H5PhaseSpaceWriter::DrawAll()
{}

void H5PhaseSpaceWriter::PrintAll()
{}

void H5PhaseSpaceWriter::SUMMARY()
{

timer->Stop();

ofstream Summary_file;

G4String fileName=this->FILE_NAME+".summary";
                                      Summary_file.open(fileName, std::ios::out);
                                      Summary_file << "G4PHSPH5 V1.O: A GEANT4 PHASE-SPACE I/O  BASED ON HDF5 FORMAT, DEVELOPED BY JAAFAR EL BAKKALI, BAHMEDJ@GMAIL.COM, TETOUAN-MOROCCO, 02.13.2015 "<< G4endl;
                                      Summary_file << "----------------------------------------------------------------------------------------------------------------------------------------------"<< G4endl;

                                      Summary_file<< setw(43) << "CREATION OF PHASE-SPACE NAMED # "        <<this->FILE_NAME                                        <<"# , AT Z-PLANE="                                                        <<this->Z_STOP<<" mm." <<G4endl;
                                      Summary_file<< setw(13) << "ELAPSED TIME:"                           << setw(40) << timer->GetUserElapsed()                   <<" s "                                                                                           <<G4endl;
                                      Summary_file<< setw(20) <<" NUMBER OF  SIMULATED HISTORIES: "        << setw(20) << NUMBER_OF_SIMULATED_HISTORIES                                                                                                               <<G4endl;
                                      Summary_file<< setw(20) <<" NUMBER OF ACTIVE EVENT: "                << setw(20) << NUMBER_OF_ACTIVE_EVENT                    <<",("<<100*( NUMBER_OF_ACTIVE_EVENT/(double)NUMBER_OF_SIMULATED_HISTORIES)<<"%)"                 <<G4endl;
if (NUMBER_OF_PHOTONS >   0)          Summary_file<< setw(20) << "TOTAL NUMBER OF PHOTONS: "               << setw(20) << NUMBER_OF_PHOTONS                                                                                                                           <<G4endl;
if (NUMBER_OF_ELECTRONS > 0)          Summary_file<< setw(20) << "TOTAL NUMBER OF ELECTRONS: "             << setw(20) << NUMBER_OF_ELECTRONS                                                                                                                         <<G4endl;
if (NUMBER_OF_POSITRONS > 0)          Summary_file<< setw(20) <<" TOTAL NUMBER OF POSITRONS: "             << setw(20) << NUMBER_OF_POSITRONS                                                                                                                         <<G4endl;
if (NUMBER_OF_NEUTRONS >  0)          Summary_file<< setw(20) << "TOTAL NUMBER OF NEUTRONS: "              << setw(20) << NUMBER_OF_NEUTRONS                                                                                                                          <<G4endl;
if (NUMBER_OF_PROTONS >   0)          Summary_file<< setw(20) <<" TOTAL NUMBER OF PROTONS: "               << setw(20) << NUMBER_OF_PROTONS                                                                                                                           <<G4endl;
if (NUMBER_OF_PHOTONS >   0)          Summary_file<< setw(20) <<" PHOTONS   PERCENT: "                     << setw(20) <<100*(NUMBER_OF_PHOTONS/ (double)NUMBER_OF_PARTICLES)  <<" %"                                                                                 <<G4endl;
if (NUMBER_OF_ELECTRONS > 0)          Summary_file<< setw(20) <<" ELECTRONS PERCENT: "                     << setw(20) <<100*(NUMBER_OF_ELECTRONS/(double)NUMBER_OF_PARTICLES) <<" %"                                                                                 <<G4endl;
if (NUMBER_OF_POSITRONS > 0)          Summary_file<< setw(20) <<" POSITRONS PERCENT: "                     << setw(20) <<100*(NUMBER_OF_POSITRONS/(double)NUMBER_OF_PARTICLES) <<" %"                                                                                 <<G4endl;
if (NUMBER_OF_PROTONS >   0)          Summary_file<< setw(20) <<" PROTONS   PERCENT: "                     << setw(20) << 100*(NUMBER_OF_PROTONS/(double)NUMBER_OF_PARTICLES)   <<" %"                                                                                <<G4endl;
if (NUMBER_OF_NEUTRONS >  0)          Summary_file<< setw(20) <<" NEUTRONS  PERCENT: "                     << setw(20) << 100*(NUMBER_OF_NEUTRONS/(double)NUMBER_OF_PARTICLES)  <<" %"                                                                                <<G4endl;
                                      Summary_file<< setw(20) <<" ----------------------- "                                                                                                                                                                           <<G4endl;
if (NUMBER_OF_PHOTONS >   0)          Summary_file<< setw(20) <<" PHOTONS   MAX ENERGY:   "                << PHOTON_ENERGY_MAX                                     <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_PHOTONS >   0)          Summary_file<< setw(20) <<" PHOTONS   MIN ENERGY:   "                << PHOTON_ENERGY_MIN                                     <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_PHOTONS >   0)          Summary_file<< setw(20) <<" PHOTONS   MEAN ENERGY:  "                << PHOTON_ENERGY_MEAN/NUMBER_OF_PHOTONS                  <<" MeV"                                                                                          <<G4endl;
                                      Summary_file<< setw(20) <<" ----------------------- "                                                                                                                                                                           <<G4endl;
if (NUMBER_OF_ELECTRONS > 0)          Summary_file<< setw(20) <<" ELECTRONS MAX ENERGY:   "                << ELECTRON_ENERGY_MAX                                   <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_ELECTRONS > 0)          Summary_file<< setw(20) <<" ELECTRONS MIN ENERGY:   "                << ELECTRON_ENERGY_MIN                                   <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_ELECTRONS > 0)          Summary_file<< setw(20) <<" ELECTRONS MEAN ENERGY:  "                << ELECTRON_ENERGY_MEAN/NUMBER_OF_ELECTRONS              <<" MeV"                                                                                          <<G4endl;
                                      Summary_file<< setw(20) <<" ----------------------- "                                                                                                                                                                           <<G4endl;
if (NUMBER_OF_POSITRONS > 0)          Summary_file<< setw(20) <<" POSITRONS MAX ENERGY:   "                << POSITRON_ENERGY_MAX                                   <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_POSITRONS > 0)          Summary_file<< setw(20) <<" POSITRONS MIN ENERGY:   "                << POSITRON_ENERGY_MIN                                   <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_POSITRONS > 0)          Summary_file<< setw(20) <<" POSITRONS MEAN ENERGY:  "                << POSITRON_ENERGY_MEAN /NUMBER_OF_POSITRONS             <<" MeV"                                                                                          <<G4endl;
                                      Summary_file<< setw(20) <<" ----------------------- "                                                                                                                                                                           <<G4endl;
if (NUMBER_OF_PROTONS >   0)          Summary_file<< setw(20) <<" PROTONS   MAX ENERGY:   "                << PROTON_ENERGY_MAX                                     <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_PROTONS >   0)          Summary_file<< setw(20) <<" PROTONS   MIN ENERGY:   "                << PROTON_ENERGY_MIN                                     <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_PROTONS >   0)          Summary_file<< setw(20) <<" PROTONS   MEAN ENERGY:  "                << PROTON_ENERGY_MEAN/NUMBER_OF_PROTONS                  <<" MeV"                                                                                          <<G4endl;
                                      Summary_file<< setw(20) <<" ----------------------- "                                                                                                                                                                           <<G4endl;
if (NUMBER_OF_NEUTRONS > 0)           Summary_file<< setw(20) <<" NEUTRONS  MAX ENERGY:   "                << NEUTRON_ENERGY_MAX                                    <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_NEUTRONS > 0)           Summary_file<< setw(20) <<" NEUTRONS  MIN ENERGY:   "                << NEUTRON_ENERGY_MIN                                    <<" MeV"                                                                                          <<G4endl;
if (NUMBER_OF_NEUTRONS > 0)           Summary_file<< setw(20) <<" NEUTRONS  MEAN ENERGY:  "                << NEUTRON_ENERGY_MEAN/NUMBER_OF_NEUTRONS                <<" MeV"                                                                                          <<G4endl;


Summary_file.close();

}

void H5PhaseSpaceWriter::SET_PHASE_SPACE_FILE_NAME( G4String _FILE_NAME){

this->FILE_NAME=_FILE_NAME;
}
