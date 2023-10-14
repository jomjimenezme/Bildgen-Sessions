// Datei:  modellierung.pov
// Autor:  Sebastian Birk
// Datum:  2014-01-14
// Loesung zu Blatt 11, Aufgabe 19
//   verwendet Teile des Beispielskripts scenes/advanced/blocks/stackerday.pov
//   von Greg M. Johnson und Gilles Tran

#include "colors.inc"
#include "stones.inc"
#include "textures.inc"
#include "shapes.inc"
#include "glass.inc"
#include "metals.inc"
#include "woods.inc"




// 
// Standard-Reflektionseinstellungen
// 
#default {finish { ambient 0.1 diffuse 1 }}

//
// Hintergrund
//
sky_sphere{  // white sky sphere with a blue far end
  pigment{
    gradient z
    poly_wave 2
    color_map{
      [0  White*1.5]            
      [1 rgb <116,138,176>/255]            
    }
    scale 2
    translate -z
  }
}                         

//
// Kamera
//
camera {
  location <-4, 8, -15>
  look_at  <0, 1,  0>
}

//
// Licht
//
light_source {
  <-2, 10, -5>
  color <.3,.3,.3>
}
light_source {
  <2, 6, -10>
  color White
  spotlight
  radius 90
  falloff 3
  tightness 5
  point_at <0,0,0>
}


//
// Objektdefinitionen
//
#declare P_A1 = < 0.00, 0.00, 0>;
#declare P_A2 = < 1.00, 3.00, 0>;
#declare P_A3 = < 2.00, 0.00, 0>;
#declare P_A4 = < 0.50, 1.30, 0>;
#declare P_A5 = < 1.50, 1.30, 0>;

#declare P_i1 = < 2.00, 3.00, 0>;
#declare P_i2 = < 2.25, 2.25, 0>;
#declare P_i3 = < 3.00, 0.00, 0>;

#declare P_V1 = < 3.00, 3.00, 0>;
#declare P_V2 = < 4.00, 0.00, 0>;
#declare P_V3 = < 5.00, 3.00, 0>;
#declare P_V4 = < 3.50, 2.00, 0>;
#declare P_V5 = < 4.50, 2.00, 0>;

#declare logo_thickness = 0.2;

#declare AiA_A = union { // letter A
  sphere_sweep { //  / part of A
    linear_spline
    2,
    P_A1, logo_thickness,
    P_A2, logo_thickness
  }
  sphere_sweep { //  \ part of A
    linear_spline
    2,
    P_A2, logo_thickness,
    P_A3, logo_thickness
  }
  sphere_sweep { //  - part of A
    linear_spline
    2,
    P_A4, logo_thickness,
    P_A5, logo_thickness
  }
} // end letter A

#declare AiA_i = union { // letter i
  sphere_sweep { //  dot of i
    linear_spline
    2,
    P_i1, logo_thickness,
    P_i1, logo_thickness
  }
  sphere_sweep { //  \ part of i
    linear_spline
    2,
    P_i2, logo_thickness,
    P_i3, logo_thickness
  }
} // end letter i

#declare AiA_V = union { // letter upside down A
  sphere_sweep { //  / part of A
    linear_spline
    2,
    P_V1, logo_thickness,
    P_V2, logo_thickness
  }
  sphere_sweep { //  \ part of A
    linear_spline
    2,
    P_V2, logo_thickness,
    P_V3, logo_thickness
  }
  sphere_sweep { //  - part of A
    linear_spline
    2,
    P_V4, logo_thickness,
    P_V5, logo_thickness
  }
} // end letter upside down A


#declare AiA = union {
  object {
    AiA_A
    texture { pigment {color rgb <.5,.5,1>} }
  }
  object {
    AiA_i
    texture { pigment {color rgb <.5,.5,1>} }
  }
  object {
    AiA_V
    texture { pigment {color rgb <.0,.0,1>} }
  }
}


//
// Objekte zeichnen
//

// Bildgenerierung-Wuerfel
#macro centertext(Text)
        #local MinText=min_extent(Text);
        #local MaxText=max_extent(Text);
        translate -(MinText+(MaxText-MinText)/2)
#end

#macro unitbox(let1, let2, let3, let4, let5, let6)
  #local COL=<rand(rd),rand(rd),rand(rd)>;
  union{
    difference{
      box{<-0.5,-0.5,-0.5>,<0.5,0.5,0.5>}
      box{<-0.45,-0.45,-1>,<0.45,0.45,1>}
      box{<-1,-0.45,-0.45>,<1,0.45,0.45>}
      box{<-0.45,-1,-0.45>,<0.45,1,0.45>}
      txtBox(COL)
    }                
    box{-0.45,0.45 texture{pigment{White*1.4}}}                                
    #declare Font="cyrvetic"
    #declare sFont=<0.76,0.76,0.05>;
    #local L1=text { ttf Font let1 1, 0 scale sFont}
    #local L2=text { ttf Font let2 1, 0 scale sFont}
    #local L3=text { ttf Font let3 1, 0 scale sFont}
    #local L4=text { ttf Font let4 1, 0 scale sFont}
    #local L5=text { ttf Font let5 1, 0 scale sFont}
    #local L6=text { ttf Font let6 1, 0 scale sFont}
    union{
      object{L1 centertext(L1) translate -0.5*z rotate y*90 txtBox(COL)}
      object{L2 centertext(L2) translate -0.5*z  txtBox(COL)}                        
      object{L3 centertext(L3) translate -0.5*z rotate -90*y txtBox(COL)}   
      object{L4 centertext(L4) translate -0.5*z rotate 180*y txtBox(COL)}
      object{L5 centertext(L5) translate -0.5*z rotate 90*x txtBox(COL)}  
      object{L6 centertext(L6) translate -0.5*z rotate -90*x txtBox(COL)}  
    }
  }
#end

#declare rd=seed(0); // color random stream
#declare sc1=0.5;
#declare sc2=1;
#macro txtBox(COL)
  texture{
    pigment { rgb <COL.x+rand(rd)*sc1,COL.y+rand(rd)*sc1,COL.z+rand(rd)*sc1>*sc2}
    finish{ambient 0 diffuse 1}
  }                   
#end                                                    

#declare abst=1.3;
#declare wink=30;
union{
  object{ unitbox("B","E","F","I","I","O") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-2*abst,1.5,-3> }
  object{ unitbox("I","W","F","M","M","Z") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-1*abst,1.5,-3> }
  object{ unitbox("L","Q","H","O","L","E") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 0*abst,1.5,-3> }
  object{ unitbox("D","R","Z","P","A","Y") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 1*abst,1.5,-3> }
  object{ unitbox("-","T","B","K","L","R") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 2*abst,1.5,-3> }

  object{ unitbox("G","D","O","L","K","T") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-5*abst,0.5,-5> }
  object{ unitbox("E","S","L","R","J","Z") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-4*abst,0.5,-5> }
  object{ unitbox("N","W","L","T","G","R") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-3*abst,0.5,-5> }
  object{ unitbox("E","I","K","G","E","E") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-2*abst,0.5,-5> }
  object{ unitbox("R","O","M","F","R","G") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate <-1*abst,0.5,-5> }
  object{ unitbox("I","P","N","B","T","I") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 0*abst,0.5,-5> }
  object{ unitbox("E","G","B","V","R","J") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 1*abst,0.5,-5> }
  object{ unitbox("R","H","V","W","W","L") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 2*abst,0.5,-5> }
  object{ unitbox("U","N","S","E","A","K") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 3*abst,0.5,-5> }
  object{ unitbox("N","B","E","Q","S","R") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 4*abst,0.5,-5> }
  object{ unitbox("G","C","D","Y","Y","N") rotate -90*y rotate (rand(rd)-0.5)*wink*<0,1,0>  translate < 5*abst,0.5,-5> }
}


// Podest
box {
  <-10,0,-4> <10,1,0> 
  pigment { rgb <1,1,1> }
  finish { ambient .3 diffuse 1 }
}
box {
  <-10,0,0> <10,2,4> 
  pigment { rgb <1,1,1> }
  finish { ambient .3 diffuse 1 }
}

// Spiegel
box {
  <-10,0,4> <10,8,5> 
  texture { Polished_Chrome scale 1 }
}

// AiA Logo
object {
  AiA
  translate <-2.5,2+logo_thickness/2,1>
}

// Boden
plane{
  y, 0
  texture{
    pigment{White}
    finish{
      ambient 0.1
      diffuse 1
    }
  }
}

// Schriftzug WS ...
text {
  ttf "cyrvetic" "WS 20XX/YY" 1, 0
  scale <1.0,1.0,0.2>
  rotate x*90
  rotate y*20
  translate <-1.5,0.1,-7.5>
  texture { Aluminum }
}
