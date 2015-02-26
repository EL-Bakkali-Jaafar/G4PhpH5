G4PhspH5 is a new  opensource tool for writting and reading phase-space file based on HDF5  file format originally developed by Jaafar EL Bakkali,  it can be easy incoporated into existing Geant4 application, since  only two C++ classes needed to be added  into an existing code in order to  perform  phase-space writting task and only one for  phase-space reading task. It should be noted that for correct use of this tool, the user must ensure that the HDF5  C++ Library is installed, which it can  be obtained from HDF5 Group website, it strongly recommended that the user install this library into the " /opt" UNIX folder.

In order to writing a new phase-space file, the user must perform the following steps:
1 - change the content of the GNUmakefile to be like this:

name :=example
G4TARGET := $(name)
G4EXLIB := true
EXTRALIBS  +=  -L/opt/hdf5/lib -lhdf5_cpp -lhdf5
ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk


2 - add the following headers and source files:

H5PhaseSpaceWriter(.hh, and .cc)
G4UserH5PhaseSpaceWriter(.hh, and .cc)

3 - include the G4UserH5PhaseSpaceWriter.hh into Geant4  the header of User VolumeConstruction Class, and add the following line to this class:
G4UserH5PhaseSpaceWriter::GetInstance‭()‬-‭>‬SET_PARAMETERS(FILE_NAME,‭ ‬Z_STOP,‭ ‬PLANE_HALF_X,‭ ‬PLANE_HALF_Y,‭ ‬LOGICAL_WORLD‭);


For phase-space reading task the user must do the following things:

1 - change the content of the GNUmakefile as decribed early

2 - add the following header and source files:

G4UserH5PhaseSpaceReader.hh, and   G4UserH5PhaseSpaceReader.cc

3 -  include the G4UserH5PhaseSpaceReader.hh into the header of User PrimaryGenerator Class, and add the following line to this class:
theG4UserH5PhaseSpaceReader = new G4UserH5PhaseSpaceReader();
theG4UserH5PhaseSpaceReader->SET_PARAMETERS(
"PHASE_SPACE.h5", // name of phase-space file.
false, // if true the user must indicate the new Z_STOP parameter.
0, // new Z_STOP parameter.
0, // particle generation mode.
0); // splitting parameter.
theG4UserH5PhaseSpaceReader->INITIALIZE();
 
theG4UserH5PhaseSpaceReader->GeneratePrimaryVertex(anEvent);

REMARK IMPORTANT: It should be noted here, that for the reading task, all contents of  phase-space HDF5 file are dumped to the RAM memory at begin of
 simulation, so a high RAM space will be reserved to this phase-space file, so the user must be care when he try to reading a phase-space file which it size must be much less than available RAM espace, in order to start correctelly the simulation.


The tool has been tested in Ubuntu distribution, for a very basic example(see writer-example and reader-example examples) using  Geant4.10 code.

There is first release of this tool, for more information contact me at the following  adress email:  bahmedj@gmail.com
Jaafar EL Bakkali, PhD in Radiation and Nuclear Systems
Membre of  RNSG (Radiation and Nuclear Systems Group)
University Abdelmalk Essaadi, Faculty of Science Tetouan, Morocco.
