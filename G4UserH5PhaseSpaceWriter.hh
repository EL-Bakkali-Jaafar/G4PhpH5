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


#ifndef G4UserH5PhaseSpaceWriter_hh
#define G4UserH5PhaseSpaceWriter_hh 1

#include "G4ThreeVector.hh"
#include "globals.hh"

#include <set>
#include <vector>

class G4Event;
class G4Run;
class G4Step;
class G4LogicalVolume;
class H5PhaseSpaceWriter;

class G4UserH5PhaseSpaceWriter
{
private:
  static G4UserH5PhaseSpaceWriter *theInstance;
  G4UserH5PhaseSpaceWriter();

public:
  ~G4UserH5PhaseSpaceWriter();

public:
  static G4UserH5PhaseSpaceWriter * GetInstance();

  void SET_PARAMETERS( G4String _FILE_NAME,G4double zstop, G4double X_PLANE_HALF_SIZE,G4double Y_PLANE_HALF_SIZE,  G4LogicalVolume*& logicWorld);

private:
H5PhaseSpaceWriter  *mSD;



};

#endif
