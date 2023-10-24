// Datei:  roller.inc
// Autor:  Sebastian Birk, Holger Arndt
// Datum:  08.01.2016

#include "colors.inc"
#include "shapes.inc"



// #######################
// Makros
#macro Roller( TexturGummi,        //
               TexturChrom,        //
               TexturRahmen,       //
               TexturTrittbrett,   //
               LenkerDrehung       // Lenker Drehung
             )
#local Speiche = cylinder {
  -14*y,
  14*y,
  .2
  texture {TexturChrom}
}

#local Achse = cylinder {
  -5*z,
  5*z,
  1
  texture {TexturChrom}
}

#local Felge = difference {
  cylinder { -1*z, 1*z, 11.5 }
  cylinder { -2*z, 2*z, 10 }
  texture {TexturChrom}
}

#local AnzSpeichen = 10;
#local Speichen = union {
  #local CountSpeiche = 0;
  #while (CountSpeiche < AnzSpeichen)
    object { Speiche
      rotate (CountSpeiche*(180/AnzSpeichen))*z
    }
    #local CountSpeiche = CountSpeiche + 1;
  #end
}

#local Reifen = torus {
  14, 3
  texture { TexturGummi }
  rotate 90*x
}

#local Rad = union {
  object { Speichen }
  object { Achse }
  object { Reifen }
  object { Felge }
}

#local Biegung = difference {
  torus {
    5, 1
    rotate 90*x
  }
  box { <-7,-7,-2>, <0,7,2> }
  box { <-7,-7,-2>, <7,0,2> }
  texture { TexturRahmen }
}

#local HalbGabel = union {
  object { Biegung }
  cylinder {
    <5,0,0>, <5,-15,0>, 1
  }
  sphere { <5,-15,0>, 1 }
  texture { TexturRahmen }
}

#local Gabel = union {
  object { HalbGabel }
  object {
    HalbGabel
    rotate 180*y
  }
}

#local Griff = union {
  cylinder { <0,0,0>, <0,0,.5>, 2 }
  cylinder { <0,0,.5>, <0,0,10.5>, 1.5 }
  cylinder { <0,0,10.5>, <0,0,11>, 2 }
 texture { TexturGummi }
}

#local Lenker = union {
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  object {
    Gabel
    translate 15*y
    rotate 90*y
  }
  object {
    Rad
  }
  cylinder { // Lenkerstange
    <0,18,0>, <0,64,0>, 3
    texture { TexturRahmen }
  }
  cylinder { // Griffstange
    <0,64,-15>, <0,64,15>, 3
    texture { TexturRahmen }
  }
  object { // Griff rechts
    Griff
    translate 15*z
    translate 64*y
  }
  object { // Griff links
    Griff
    translate 15*z
    rotate 180*x
    translate 64*y
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
// Verbindung Lenker-Trittbrett
#local Verbindung = difference {
  torus {
    24, 2
    rotate 90*x
    texture { TexturRahmen }
  }
  box {
    <-30,-30,-3>, <30,0,3>
  }
  box {
    <-30,-30,-3>, <0,30,3>
    rotate -20*z
  }
}

// Trittbrett
//#local Trittbrett =

// HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// Rahmen 
//#local Rahmen =

// HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

union { // Roller

   object {
    Lenker
    rotate LenkerDrehung*y
  }
  object {
    Verbindung
  }
  translate 17*y
}

#end //------------------- end of macro








