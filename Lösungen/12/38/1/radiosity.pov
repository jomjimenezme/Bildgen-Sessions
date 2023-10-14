// Datei:  radiosity.pov
// Autor:  Sebastian Birk, Holger Arndt
// Datum:  13.01.2016

#include "metals.inc"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#declare useRadiosity=1;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#if (useRadiosity)
  global_settings{
    assumed_gamma 1.0
    radiosity{
      pretrace_start 0.04
      pretrace_end 0.01
      count 400
      recursion_limit 2
      nearest_count 5
      error_bound 0.5
    }
  }
  #declare amb=0.0;
#else
  #declare amb=0.1;
#end


#declare Finish = finish{diffuse 0.95 ambient amb}

#declare White  = texture{pigment{rgb<1,1,1>} finish{Finish}}
#declare Red    = texture{pigment{rgb<0.97,0.025,0.025>} finish{Finish}}
#declare Green  = texture{pigment{rgb<0.325,0.736,0.025>} finish{Finish}}
#declare Blue   = texture{pigment{rgb<0.225,0.236,0.925>} finish{Finish}}


// lights

#declare LightColor=<2,2,2>;

light_source{
  <7,8,2>
  color rgb LightColor
  area_light y*.5, z*3, 5, 5 jitter adaptive 2
  spotlight
  radius 75
  falloff 10
  tightness 10
  point_at <-7, 0, 2>
}


// camera

camera{
  location  <0,10,-10>
  look_at   <0, 0, 0>
  angle 90
}

// objects

// umschliessende Box
box {
  <-1,-1,-1> <1,1,1>
  scale <25,25,25>
  texture {White}
  hollow
}

// farbige Boxen
box {
  <-1,-1,-1> <1,1,1>
  scale <2,2,2>
  translate <-4,0,-2>
  texture {Red}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
box {
  <-1,-1,-1> <1,1,1>
  scale <2,2,2>
  translate <-4,0,2>
  texture {Green}
}

box {
  <-1,-1,-1> <1,1,1>
  scale <2,2,2>
  translate <-4,0,6>
  texture {Blue}
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// weisse Box
box {
  <-1,-1,-1> <1,1,1>
  scale <2,2,6>
  translate <4,0,2>
  texture {White}
}

// Boden
plane {
  <0,1,0>, -2
  texture {White}
}

