// Datei:  modellierung.pov
// Autor:  Sebastian Birk, Martin Galgon, Holger Arndt
// Datum:  07.01.2016
//   verwendet Teile des Beispielskripts scenes/advanced/blocks/stackerday.pov
//   von Greg M. Johnson und Gilles Tran

#version 3.7;

#include "colors.inc" // 100 vordefinierte Farben
#include "textures.inc" // Polished_Chrome
#include "skies.inc"

//======================================================================
// Grundeinstellungen
//======================================================================

#default {finish { ambient 0.1 diffuse 1 }}
global_settings { assumed_gamma 1 } // Gammakorrektur

//----------------------------------------------------------------------
// Makro um Text zu zentrieren
//----------------------------------------------------------------------
// Einige Buchstaben ragen unter die Text-Basislinie hinaus (alle
// Buchstaben mit rundem Unterteil, wie U, O, G, J, etc.; zusätzlich
// natürlich Q), einige ragen darüber hinaus (wie der Bindestrich).
// Der Grund dafür ist einfacher Textsatz entlang einer festen linie
// ohne dabei individuelle Verschiebungen für jeden Buchstaben
// berücksichtigen zu müssen.
// Um den Text mittels Bounding-Box (min_extent, max_extent) zu
// zentrieren, muss diese Verschiebung zunächst rückgängig gemacht
// werden, so dass min_extent auch tatsächlich im Ursprung liegt.
// Deshalb die Verschiebung um -min_extent zu Beginn.
#macro centertext(Text)
   #local MinText = min_extent(Text);
   #local MaxText = max_extent(Text);
   translate -(MinText + (MaxText - MinText) / 2)
#end

//======================================================================
// Hintergrund
//======================================================================

sky_sphere {  // white sky sphere with a blue far end
  pigment { P_Cloud4 }
}

//======================================================================
// Kamera
//======================================================================

camera {
  location <-4, 8, -15>
  look_at  <0, 1,  0>
}

//======================================================================
// Lichter
//======================================================================

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

light_source {
  <-2, 10, -5>
  color <.3,.3,.3>
}

light_source {
  <2, 6, -10>
  color White
//---
  spotlight
  radius 90
  falloff 3
  tightness 5
  point_at <0,0,0>
//---
  // vector vector num_lights num_lights
  area_light <2, 0, 0>, <0, 0, 2>, 5, 5
  adaptive 1
  jitter
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//======================================================================
// Objektdefinitionen
//======================================================================

#declare AI_green = < 89, 174, 32 > / 255;

#declare AI_bar = box {
  < -0.05, -3, -0.05 > < 0.05, 3, 0.05>
}

#declare AI_node = box {
  < -0.5, -0.5, -0.2 > < 0.5, 0.5, 0.2>
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#declare AI_bars = union {
  object { AI_bar translate < 2, 0, 0 > }
  object { AI_bar }
  object { AI_bar translate < -2, 0, 0 > }
}

#declare AI_grid = union {
  object { AI_bars }
  object { AI_bars rotate 90*z }
}

#declare AI_array = union {
  object { AI_node translate < -2, 0, 0 > }
  object { AI_node }
  object { AI_node translate < 2, 0, 0 > }
}

#declare AI_lattice = union {
  object { AI_grid }
  object { AI_array translate < 0, -2, 0 > }
  object { AI_array }
  object { AI_array translate < 0, 2, 0 > }
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#declare AI_positions = array[6]{ <0,0,0>, <0.375,0.925,0>, <1.1,1.725,0>, <1.763,2.487,0>, <2.837,3.212,0>, <4,4,0> }
#declare AI_rotations = array[6]{ 0, -26.15, -12.01, 9.77, -51.01, 0 }
#declare AI_scales = array[6]{ 1.0001, 1.098, 1.207, 1.225, 1.36, 1.693 }

#declare AI_logo = union {

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  object {
    AI_lattice
    pigment { rgb AI_green }
    finish { reflection {0.25} ambient .3 diffuse 1 }
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  #for (i, 0, 5)
    object {
      AI_node
      scale < AI_scales[i], AI_scales[i], 1.05+i/2 >
      rotate < 0, 0, AI_rotations[i] >
      translate < -2, -2, 0 > + AI_positions[i]
      pigment { rgb < 134, 130, 134 > / 255 }
      finish { reflection {0.5} ambient 0.3 diffuse 1 }
    }
  #end

  #local AI_text = text { ttf "cyrvetic" "Ai" 1, 0 scale < 1.6, 1.6, 0.05 > }

  text {
    AI_text
    translate < 2, 2, -0.4*AI_scales[5]-0.1 >
    centertext(AI_text)
    pigment { rgb < 1, 1, 1 > }
    finish { reflection {1.0} ambient 0.3 diffuse 1 }
  }
}

//======================================================================
// Objekte zeichnen
//======================================================================

#declare rd = seed(123);
#declare Font = "cyrvetic"
#declare sFont = < 0.76, 0.76, 0.05 >;

//----------------------------------------------------------------------
// Buchstabenwuerfel
//----------------------------------------------------------------------
#macro unitbox(let1, let2, let3, let4, let5, let6)

  #local COL = < rand(rd), rand(rd), rand(rd) >;

  union{
    // Wuerfelrahmen
    difference{
      // Basis
      box{ < -0.5, -0.5, -0.5 >, < 0.5, 0.5, 0.5 > }
      // Schnittboxen
      box{ < -0.45, -0.45, -1 >, < 0.45, 0.45, 1 > }
      box{ < -1, -0.45, -0.45 >, < 1, 0.45, 0.45 > }
      box{ < -0.45, -1, -0.45 >, < 0.45, 1, 0.45 > }
      // Textur
      txtBox(COL)
    }

    // Innerer Wuerfel
    box{-0.45,0.45 texture{pigment{White*1.4}}}

    // Buchstabenvariablen
    #local L1 = text { ttf Font let1 1, 0 scale sFont}
    #local L2 = text { ttf Font let2 1, 0 scale sFont}
    #local L3 = text { ttf Font let3 1, 0 scale sFont}
    #local L4 = text { ttf Font let4 1, 0 scale sFont}
    #local L5 = text { ttf Font let5 1, 0 scale sFont}
    #local L6 = text { ttf Font let6 1, 0 scale sFont}

    // Buchstabenobjekte auf Wuerfeloberflaeche
    union{
      object{L1 centertext(L1) translate -0.5*z rotate y*90 txtBox(COL)}
      object{L2 centertext(L2) translate -0.5*z txtBox(COL)}
      object{L3 centertext(L3) translate -0.5*z rotate -90*y txtBox(COL)}
      object{L4 centertext(L4) translate -0.5*z rotate 180*y txtBox(COL)}
      object{L5 centertext(L5) translate -0.5*z rotate 90*x txtBox(COL)}
      object{L6 centertext(L6) translate -0.5*z rotate -90*x txtBox(COL)}
    }
  }
#end

//----------------------------------------------------------------------
// Buchstabenwuerfel -- Textur
//----------------------------------------------------------------------
#declare sc1 = 0.5;
#declare sc2 = 1;

#macro txtBox(COL)
  texture{
    pigment { rgb <COL.x+rand(rd)*sc1,COL.y+rand(rd)*sc1,COL.z+rand(rd)*sc1>*sc2}
//     pigment { rgb COL }
    finish{ ambient 0 diffuse 1 }
  }
#end

//----------------------------------------------------------------------
// Alle Wuerfel
//----------------------------------------------------------------------
#declare abst = 1.3;
#declare wink = 30;

union{
  object{ unitbox("B","E","F","I","I","O") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-2*abst,1.5,-3> }
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  object{ unitbox("I","W","F","M","M","Z") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-1*abst,1.5,-3> }
  object{ unitbox("L","Q","H","O","L","E") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 0*abst,1.5,-3> }
  object{ unitbox("D","R","Z","P","A","Y") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 1*abst,1.5,-3> }
  object{ unitbox("-","T","B","K","L","R") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 2*abst,1.5,-3> }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  object{ unitbox("G","D","O","L","K","T") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-5*abst,0.5,-5> }
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  object{ unitbox("E","S","L","R","J","Z") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-4*abst,0.5,-5> }
  object{ unitbox("N","W","L","T","G","R") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-3*abst,0.5,-5> }
  object{ unitbox("E","I","K","G","E","E") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-2*abst,0.5,-5> }
  object{ unitbox("R","O","M","F","R","G") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-1*abst,0.5,-5> }
  object{ unitbox("I","P","N","B","T","I") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 0*abst,0.5,-5> }
  object{ unitbox("E","G","B","V","R","J") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 1*abst,0.5,-5> }
  object{ unitbox("R","H","V","W","W","L") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 2*abst,0.5,-5> }
  object{ unitbox("U","N","S","E","A","K") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 3*abst,0.5,-5> }
  object{ unitbox("N","B","E","Q","S","R") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 4*abst,0.5,-5> }
  object{ unitbox("G","C","D","Y","Y","N") rotate -90*y  rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 5*abst,0.5,-5> }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

//----------------------------------------------------------------------
// Podest
//----------------------------------------------------------------------
box {
  < -10, 0, -4 > < 10, 1, 0 >
  pigment { rgb < 1, 1, 1 > }
  finish { ambient .3 diffuse 1 }
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
box {
  < -10, 0, 0 > < 10, 2, 6 >
  pigment { rgb < 1, 1, 1 > }
  finish { ambient .3 diffuse 1 }
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//----------------------------------------------------------------------
// Spiegel
//----------------------------------------------------------------------
box {
  < -11.5, 0, 7 > < 11.5, 8.5, 8 >
  pigment { rgb < 0, 0, 0 > }
  finish { reflection {1.0} }
}

//----------------------------------------------------------------------
// Ai Logo
//----------------------------------------------------------------------
object {
  AI_logo
  translate < 0, 5, 1 >
}

//----------------------------------------------------------------------
// Boden
//----------------------------------------------------------------------
plane {
  y, 0
  texture{
    pigment{ White }
    finish{ reflection {0.1} ambient 0.1 diffuse 1 }
  }
}

//----------------------------------------------------------------------
// Schriftzug
//----------------------------------------------------------------------

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
text {
  ttf "cyrvetic" "WS 20XX/YY" 1, 0
  scale < 1.0, 1.0, 0.2 >
  rotate x * 90
  rotate y * 20
  translate < -1.5, 0.1, -7.5 >
  pigment { rgb < 1, 1, 1 > * 1.5 }
  finish { ambient 0.3 diffuse 1 }
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
