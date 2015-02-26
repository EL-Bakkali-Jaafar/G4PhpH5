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


#include "G4UserH5PhaseSpaceReader.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "Randomize.hh"
#include "Randomize.hh"
#include "Randomize.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#ifndef H5_NO_NAMESPACE
#ifndef H5_NO_STD
    using std::cout;
    using std::endl;
#endif  // H5_NO_STD
#endif

#include "H5Cpp.h"

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif
 static int   incr=-1,
              PHASE_SPACE_TIME_REUSED=1,
              h=1,
              NUMBER_OF_TIME_OF_REUSE_PHASE_SPACE_FILE=0,
              ID_OF_NEXT_EVENT=0;
        int   INDEX_OF_CURRENT_DATA=0,
              event_data_length=0;

 G4UserH5PhaseSpaceReader::PhspData  *  dump_PhspData ;
 G4UserH5PhaseSpaceReader::EventData *  dump_EventData ;
 bool a=true;


G4UserH5PhaseSpaceReader::G4UserH5PhaseSpaceReader()
: G4VPrimaryGenerator()

{

    this->apply=false;


}

 void G4UserH5PhaseSpaceReader::INITIALIZE()
{
    READ_BEAM_DATA();
    this-> NUMBER_OF_SIMULATED_HISTORIES=myBeamData[0].NUMBER_OF_HISORIES;
    this-> Z_STOP*=mm;
    this-> Z_STOP= myBeamData[0].Z_STOP;
    G4cout <<" PLANE Z STOP IS :" <<  this-> Z_STOP<< " mm"<<G4endl;

    READ_EVENT_DATA();
    READ_PHASE_SPACE_FILE();
}


G4UserH5PhaseSpaceReader::~G4UserH5PhaseSpaceReader()
{

}

/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
/*<><>           implementation of GENERATE_PRIMARY_VERTEX method                      <><>*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
void G4UserH5PhaseSpaceReader::GeneratePrimaryVertex(G4Event*anEvent)
{
    if (a==true) incr++ ;

    Event_ID                =    dump_EventData[incr]. EVENT_ID;

    Number_of_Entries       =    dump_EventData[incr].NUMBER_OF_ENTRIES;

    if (anEvent->GetEventID()< this-> NUMBER_OF_SIMULATED_HISTORIES*h )

    { //  first if

        if (anEvent->GetEventID()!=(this-> NUMBER_OF_SIMULATED_HISTORIES)*NUMBER_OF_TIME_OF_REUSE_PHASE_SPACE_FILE+Event_ID)

            { //  second if

            a=false;

            } // second if
            else
            { // second else
                  a=true;

                  switch (this->EVENT_GENERATION_FLAG)
                   {
                   case 0:
                                   GENERATE_NEW_PARTICLE(incr,anEvent);
                       break;
                   case 1:
                                   GENERATE_NEW_PART_ROT_SPLITTING(incr,anEvent);
                       break;
                   case 2:
                                   GENERATE_NEW_PART_SPLITTING(incr,anEvent);

                       break;

                   }


            }// second else
    }// first if

    else {// first else
        incr=0;
        Event_ID                =   dump_EventData[incr]. EVENT_ID;

        Number_of_Entries       =    dump_EventData[incr].NUMBER_OF_ENTRIES;
       G4cout<<" Simulation ID, local ID "<< anEvent->GetEventID()<<","<<Event_ID<<G4endl;

        if (anEvent->GetEventID()!=(this-> NUMBER_OF_SIMULATED_HISTORIES)*h+Event_ID)

            { //  second if

            a=false;

            } // second if
            else
            { // second else
                  a=true;
                  switch (this->EVENT_GENERATION_FLAG)
                   {
                   case 0:
                                   GENERATE_NEW_PARTICLE(incr,anEvent);
                       break;
                   case 1:
                                   GENERATE_NEW_PART_ROT_SPLITTING(incr,anEvent);
                       break;
                   case 2:
                                   GENERATE_NEW_PART_SPLITTING(incr,anEvent);

                       break;

                   }



            }// second else

     G4cout<<"Phase space recycled "<< h<<" times"<<G4endl;
     h++;
     NUMBER_OF_TIME_OF_REUSE_PHASE_SPACE_FILE++;

    }// second else

}


/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
/*<><>           implementation of GENERATE_NEW_PART_SPLITTING method              <><>*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/


void G4UserH5PhaseSpaceReader:: GENERATE_NEW_PART_SPLITTING(int incr,G4Event*& anEvent){

    this->energy           *=MeV;
    this->pos_x            *=mm;
    this->pos_y            *=mm;

    this->dir_x            *=mm;
    this->dir_y            *=mm;
    this->dir_z            *=mm;

    for (INDEX_OF_CURRENT_DATA=0+incr; INDEX_OF_CURRENT_DATA<Number_of_Entries+incr;INDEX_OF_CURRENT_DATA++){

         this->particle_pdge   = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_PDGE;
         this->particle_weight = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_WEIGHT;
         this->energy          = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_KINETIC;
         this->pos_x           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_POS_X;
         this->pos_y           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_POS_Y;
         this->pos_z           = this-> Z_STOP;
         this->dir_x           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_X;
         this->dir_y           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_Y;
         this->dir_z           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_Z;

        G4ParticleDefinition*  partDef=GET_PARTICLE_DEFINITION( this->particle_pdge);
        G4float   partMass = partDef->GetPDGMass();
        G4float   partEnergy = this->energy + partMass;
        G4float   partMom = std::sqrt( partEnergy*partEnergy - partMass*partMass );
        SET_Z_PLANE( this-> Z_plane, this->apply );

        G4float  particle_time =0;

        for ( int k=0; k< SPLITTING_FACTOR; k++){

     G4ThreeVector particle_position(this->pos_x,this->pos_y,this->pos_z);
     G4ThreeVector particle_momentum(partMom*this->dir_x,partMom*this->dir_y,partMom*this->dir_z);


     G4PrimaryParticle * particle = new G4PrimaryParticle(GET_PARTICLE_DEFINITION( this->particle_pdge),particle_momentum.x() , particle_momentum.y(), particle_momentum.z());

     G4double  new_weight=this->particle_weight/ (double)SPLITTING_FACTOR;
     particle->SetWeight( new_weight);

     // Create the new primary vertex and set the primary to it
     G4PrimaryVertex * vertex = new G4PrimaryVertex(particle_position, particle_time);
     vertex->SetPrimary(particle);
     // And finally set the vertex to this event
     anEvent->AddPrimaryVertex(vertex);
    }// end of for boucle

    }// end for


    ID_OF_NEXT_EVENT=ID_OF_NEXT_EVENT+Number_of_Entries+1;


}

/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
/*<><>           implementation of GENERATE_NEW_PART_ROT_SPLITTING              <><>*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
void G4UserH5PhaseSpaceReader:: GENERATE_NEW_PART_ROT_SPLITTING(int incr,G4Event*& anEvent){
    this->energy           *=MeV;
    this->pos_x            *=mm;
    this->pos_y            *=mm;

    this->dir_x            *=mm;
    this->dir_y            *=mm;
    this->dir_z            *=mm;
    for (INDEX_OF_CURRENT_DATA=0+incr; INDEX_OF_CURRENT_DATA<Number_of_Entries+incr;INDEX_OF_CURRENT_DATA++){

         this->particle_pdge   = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_PDGE;
         this->particle_weight = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_WEIGHT;
         this->energy          = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_KINETIC;
         this->pos_x           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_POS_X;
         this->pos_y           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_POS_Y;
         this->pos_z           = this-> Z_STOP;
         this->dir_x           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_X;
         this->dir_y           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_Y;
         this->dir_z           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_Z;

        G4ParticleDefinition*  partDef=GET_PARTICLE_DEFINITION( this->particle_pdge);
        G4float   partMass = partDef->GetPDGMass();
        G4float   partEnergy = this->energy+ partMass;
        G4float   partMom = std::sqrt( partEnergy*partEnergy - partMass*partMass );
       SET_Z_PLANE( this-> Z_plane, this->apply );

        G4float  particle_time =0;

        for ( int k=0; k< SPLITTING_FACTOR; k++){


       G4ThreeVector particle_position(this->pos_x,this->pos_y,this->pos_z);

     G4float angle;

     //
     angle*=deg;

     angle = k*360.00/(double)SPLITTING_FACTOR;

     G4ThreeVector particle_momentum(partMom*this->dir_x,partMom*this->dir_y,partMom*this->dir_z);

     particle_position.rotateZ(angle);

     particle_momentum.rotateZ(angle);

     G4PrimaryParticle * particle = new G4PrimaryParticle(GET_PARTICLE_DEFINITION( this->particle_pdge),particle_momentum.x() , particle_momentum.y(), particle_momentum.z());

     G4double  new_weight=this->particle_weight/ (double)SPLITTING_FACTOR;
     particle->SetWeight( new_weight);

     // Create the new primary vertex and set the primary to it
     G4PrimaryVertex * vertex = new G4PrimaryVertex(particle_position, particle_time);
     vertex->SetPrimary(particle);
     // And finally set the vertex to this event
     anEvent->AddPrimaryVertex(vertex);
    }// end of for boucle

    }// end for
    ID_OF_NEXT_EVENT=ID_OF_NEXT_EVENT+Number_of_Entries+1;
}

/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
/*<><>           implementation of GENERATE_NEW_PARTICLE method                        <><>*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
void G4UserH5PhaseSpaceReader:: GENERATE_NEW_PARTICLE(int incr,G4Event*& anEvent){
    this->energy           *=MeV;
    this->pos_x            *=mm;
    this->pos_y            *=mm;

    this->dir_x            *=mm;
    this->dir_y            *=mm;
    this->dir_z            *=mm;
for ( INDEX_OF_CURRENT_DATA=0+incr; INDEX_OF_CURRENT_DATA<Number_of_Entries+incr;INDEX_OF_CURRENT_DATA++){

     this->particle_pdge   = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_PDGE;
     this->particle_weight = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_WEIGHT;
     this->energy          = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_KINETIC;
     this->pos_x           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_POS_X;
     this->pos_y           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_POS_Y;
     this->pos_z           = this-> Z_STOP;
     this->dir_x           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_X;
     this->dir_y           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_Y;
     this->dir_z           = dump_PhspData[INDEX_OF_CURRENT_DATA]. PART_DIR_Z;

    G4ParticleDefinition*  partDef=GET_PARTICLE_DEFINITION( this->particle_pdge);
    G4float   partMass = partDef->GetPDGMass();
    G4float partEnergy = this->energy+ partMass;
    G4float  partMom = std::sqrt( partEnergy*partEnergy - partMass*partMass );
    G4float  particle_time =0;

    SET_Z_PLANE( this-> Z_plane, this->apply );

    G4ThreeVector particle_position(this->pos_x,this->pos_y,this->pos_z);

    G4PrimaryParticle * particle = new G4PrimaryParticle(GET_PARTICLE_DEFINITION( this->particle_pdge),partMom*this->dir_x,partMom*this->dir_y,
                                                         partMom*this->dir_z);
    particle->SetWeight(this->particle_weight);
     // Create the new primary vertex and set the primary to it
    G4PrimaryVertex * vertex = new G4PrimaryVertex(particle_position, particle_time);
    vertex->SetPrimary(particle);
    // And finally set the vertex to this event
    anEvent->AddPrimaryVertex(vertex);
}// end for

ID_OF_NEXT_EVENT=ID_OF_NEXT_EVENT+Number_of_Entries+1;


}


/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
/*<><>           implementation of GET_PARTICLE_DEFINITION method                      <><>*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/

 G4ParticleDefinition*  G4UserH5PhaseSpaceReader::GET_PARTICLE_DEFINITION(int& PDGE)
{
G4String particleName               =     "";

G4ParticleTable* particleTable      =    G4ParticleTable::GetParticleTable();

G4ParticleDefinition* particle_def  =    particleTable->FindParticle(particleName="gamma");

    switch (PDGE)
     {
     case -11:
                   particle_def     =    particleTable->FindParticle(particleName="e+");;
         break;
     case 11:
                   particle_def     =    particleTable->FindParticle(particleName="e-");
         break;
     case 22:
                   particle_def     =    particleTable->FindParticle(particleName="gamma");
         break;
    case 2112:
                   particle_def     =    particleTable->FindParticle(particleName="neutron");
        break;
    case 2212:
                   particle_def     =    particleTable->FindParticle(particleName="proton");
        break;
     }

return particle_def;
}


 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
 /*<><>           implementation of READ_EVENT_DATA method                               <><>*/
 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
 void G4UserH5PhaseSpaceReader::READ_EVENT_DATA()
 {
      // DECLARATION OF STRINGS OF MEMBERS OF DATASET NAMED "EventData"
      const std::string DatasetName_event("EventData");
      const std::string MEMBER_ENTERIES("_ENTERIES");
      const std::string MEMBER_EVENTID("_EVENTID");
      H5File file(  this->FileName, H5F_ACC_RDONLY );
      DataSet dataset_event = file.openDataSet( DatasetName_event );
      H5::CompType mtype_event( sizeof(EventData) );
      // GET NUMBER OF DATA IN DATASET NAMED EventData
     G4int data_event_size=dataset_event.getSpace().getSimpleExtentNpoints();
     dump_EventData = new EventData[data_event_size];
     event_data_length=data_event_size;
     mtype_event.insertMember(MEMBER_ENTERIES,     HOFFSET(EventData, NUMBER_OF_ENTRIES),      H5::PredType::NATIVE_INT);
     mtype_event.insertMember(MEMBER_EVENTID,      HOFFSET(EventData, EVENT_ID),               H5::PredType::NATIVE_INT);
     dataset_event.read( dump_EventData, mtype_event );

 }

 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
 /*<><>           implementation of :SET_PHASE_SPACE_FILE_NAME method                   <><>*/
 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
void G4UserH5PhaseSpaceReader::SET_PHASE_SPACE_FILE_NAME( G4String _FileName)


 {
 this->FileName=_FileName;

 }

/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
/*<><>           implementation of READ_BEAM_DATA method                               <><>*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/

void G4UserH5PhaseSpaceReader::READ_BEAM_DATA()
 {
     const std::string FileName(this->FileName);
     const std::string DatasetName_histories("BeamData");
     const std::string MEMBER_HISTORIES("_NUMBER_OF_HISTORIES");
     const std::string MEMBER_Z_STOP("_Z_STOP");
     H5File file( FileName, H5F_ACC_RDONLY );
     DataSet dataset_histories = file.openDataSet( DatasetName_histories );
     H5::CompType mtype_histories( sizeof(BeamData) );
     mtype_histories.insertMember(MEMBER_HISTORIES,     HOFFSET(BeamData, NUMBER_OF_HISORIES),      H5::PredType::NATIVE_INT);
     mtype_histories.insertMember(MEMBER_Z_STOP,        HOFFSET(BeamData, Z_STOP),                  H5::PredType::NATIVE_FLOAT);
     dataset_histories.read( myBeamData, mtype_histories );
 }



 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
 /*<><>           implementation of SET_Z_PLANE method                                  <><>*/
 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
 void G4UserH5PhaseSpaceReader::SET_Z_PLANE( G4double Z_plane,  bool apply )
 {
     if ( apply==true){

         this->pos_z           = Z_plane/mm;
     }

     else {
     }

 }


 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
 /*<><>           implementation of READ_PHASE_SPACE_FILE method                        <><>*/
 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>><><><><><><><><>*/
void G4UserH5PhaseSpaceReader::READ_PHASE_SPACE_FILE()
{
     const std::string DatasetName("PhspData");
     const std::string MEMBER_PART_PDGE("_PART_PDGE");
     const std::string MEMBER_PART_WEIGHT("_PART_WEIGHT");
     const std::string MEMBER_PART_POS_X("_PART_POS_X");
     const std::string MEMBER_PART_POS_Y("_PART_POS_Y");
     const std::string MEMBER_PART_POS_Z("_PART_POS_Z");
     const std::string MEMBER_PART_DIR_X("_PART_DIR_X");
     const std::string MEMBER_PART_DIR_Y("_PART_DIR_Y");
     const std::string MEMBER_PART_DIR_Z("_PART_DIR_Z");
     const std::string MEMBER_PART_KINETIC("_PART_KINETIC");

     // LOAD PHASE SPACE HDF5-BASED FORMAT FILE
     H5File file( this->FileName, H5F_ACC_RDONLY );
     DataSet dataset = file.openDataSet( DatasetName );
     H5::CompType mtype2( sizeof(PhspData) );

    // GET NUMBER OF DATA IN DATASET NAMED PhspData
    G4int data_size  = dataset.getSpace().getSimpleExtentNpoints();
    dump_PhspData    = new PhspData[data_size];
    mtype2.insertMember(MEMBER_PART_WEIGHT,  HOFFSET(PhspData, PART_WEIGHT),   H5::PredType::NATIVE_FLOAT);
    mtype2.insertMember(MEMBER_PART_POS_X,   HOFFSET(PhspData, PART_POS_X),    H5::PredType::NATIVE_FLOAT);
    mtype2.insertMember(MEMBER_PART_PDGE,    HOFFSET(PhspData, PART_PDGE),     H5::PredType::NATIVE_INT);
    mtype2.insertMember(MEMBER_PART_POS_Y,   HOFFSET(PhspData, PART_POS_Y),    H5::PredType::NATIVE_FLOAT);
    mtype2.insertMember(MEMBER_PART_POS_Z,   HOFFSET(PhspData, PART_POS_Z),    H5::PredType::NATIVE_FLOAT);
    mtype2.insertMember(MEMBER_PART_DIR_X,   HOFFSET(PhspData, PART_DIR_X),    H5::PredType::NATIVE_FLOAT);
    mtype2.insertMember(MEMBER_PART_DIR_Y,   HOFFSET(PhspData, PART_DIR_Y),    H5::PredType::NATIVE_FLOAT);
    mtype2.insertMember(MEMBER_PART_DIR_Z,   HOFFSET(PhspData, PART_DIR_Z),    H5::PredType::NATIVE_FLOAT);
    mtype2.insertMember(MEMBER_PART_KINETIC, HOFFSET(PhspData, PART_KINETIC),  H5::PredType::NATIVE_FLOAT);
    dataset.read( dump_PhspData, mtype2 );
}

void G4UserH5PhaseSpaceReader::SET_EVENT_GENERATION_FLAG(G4int FLAG){

this->EVENT_GENERATION_FLAG= FLAG;

}

void G4UserH5PhaseSpaceReader::SET_PARAMETERS(G4String _PHASE_SPACE_NAME, bool _CHANGE_Z_STOP_VALUE, G4float Z_STOP, int _EVENT_GENERATION_FLAG, int    _SPLITTING_FACTOR){

    SET_PHASE_SPACE_FILE_NAME(_PHASE_SPACE_NAME);
    this->EVENT_GENERATION_FLAG=_EVENT_GENERATION_FLAG;
    if (_CHANGE_Z_STOP_VALUE==true)  SET_Z_PLANE( Z_STOP,  true ) ;
    SET_EVENT_GENERATION_FLAG( _EVENT_GENERATION_FLAG);
    this-> SPLITTING_FACTOR=_SPLITTING_FACTOR;

}
