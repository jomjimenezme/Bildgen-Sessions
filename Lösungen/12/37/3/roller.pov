// Datei:  roller.pov
// Autor:  Sebastian Birk, Holger Arndt
// Datum:  08.01.2016

#include "colors.inc"
#include "stones.inc"
#include "textures.inc"
#include "shapes.inc"
#include "glass.inc"
#include "metals.inc"
#include "woods.inc"

#include "roller.inc"

// #######################
// Einstellungen
#declare EinfachesMaterial    = 0;
#declare EinfacherHintergrund = 0;
#declare EinfachesLicht       = 0;


// #######################
// Materialien
#if (EinfachesMaterial)
  #declare Gummi = texture {
    pigment {
      Black
    }
    #if (EinfachesLicht)
    finish { ambient 1 }
    #end
  }
  
  #declare Chrom = texture {
    pigment {
      color rgb <.8,.8,.8>
    }
    #if (EinfachesLicht)
    finish { ambient 1 }
    #end
  }
  
  #declare Rahmenfarbe = texture {
    pigment {
      Green
    }
    #if (EinfachesLicht)
    finish { ambient 1 }
    #end
  }
  
  #declare Trittbrettmaterial = texture {
    pigment {
      Blue
    }
    #if (EinfachesLicht)
    finish { ambient 1 }
    #end
  }
#else
  #declare Gummi = texture {
    pigment { color rgb <.05,.05,.05> }
    normal { bumps 1 scale .005 }
    finish { 
      ambient .1
      diffuse .2
      reflection 0
      specular 0
      roughness .1
    }
  }
  
  #declare Chrom = texture {
    Polished_Chrome scale 1
  }
  
  #declare Rahmenfarbe = texture {
    Aluminum
  }
  
  #declare Trittbrettmaterial = texture {
    pigment { Blue }
  }
#end


// #######################
// Hintergrund
#if (EinfacherHintergrund)
  background { color rgb <.3,.3,.5> }
#else
  // Wände und Boden
  plane { // Boden
    <0, 1, 0>, 0
    texture { T_Wood7 scale 20 }
  }
  
  plane { // Wand rechts
    <-.3, 0, -1>, -300
    texture { T_Wood4 scale 30 }
  }
  
  plane { // Wand links
    <1, 0, -.3>, -200
    texture { T_Wood3 scale 20 }
  }
#end


// #######################
// Kamera
camera {
  location <0, 120, -150>
  look_at  <50, 60,  0>
}


// #######################
// Licht
#if (EinfachesLicht)
  global_settings {
    ambient_light rgb <1, 1, 1>
  }
#else
  light_source {
    <190, 220, -180> color White
    spotlight
    area_light 5*y, 5*x, 5, 5
    adaptive 1
    jitter
    point_at <0, 0, 0>
  }
  light_source {
    <90, 150, 280> color White
    spotlight
    area_light 5*x, 5*y, 5, 5
    adaptive 1
    jitter
    point_at <0, 0, 0>
  }
  global_settings {
    ambient_light rgb <1, 1, 1>
  }
#end


// #######################
// Objekte
object {
  Roller(Gummi,Chrom,Rahmenfarbe,Trittbrettmaterial,30)
}


