# Makefile für Povray-Modellierung
# Autor:  Sebastian Birk, Holger Arndt
# Datum:  13.01.2016

# povray und seine Optionen
POVRAY = povray

POVOPTS_NICE  = +V +A +R9 +P
POVOPTS_QUICK = +V +Q3 +P

# Dateien
POV_FILES := $(wildcard *.pov)
POV_IMAGES := $(patsubst %.pov,%.png,$(POV_FILES))

# Einstellungen
ifeq ($(MODE),quick)
  override SIZE = vga
  POVOPTS = $(POVOPTS_QUICK)
else
  POVOPTS = $(POVOPTS_NICE)
endif

ifndef SIZE
  ifndef WIDTH
    SIZE = svga
  endif
  ifndef HEIGHT
    SIZE = svga
  endif
endif

ifeq ($(SIZE),full)
  override WIDTH = 1600
  override HEIGHT = 1200
endif
ifeq ($(SIZE),xga)
  override WIDTH = 1024
  override HEIGHT = 768
endif
ifeq ($(SIZE),svga)
  override WIDTH = 800
  override HEIGHT = 600
endif
ifeq ($(SIZE),vga)
  override WIDTH = 640
  override HEIGHT = 480
endif

DIMS = +W$(WIDTH) +H$(HEIGHT)

# Regeln

%.png: %.pov
	$(POVRAY) -I$< +O$@ $(DIMS) $(POVOPTS)

all: $(POV_IMAGES)

.SECONDARY:

.PHONY: all clean

clean:
	rm -f $(POV_IMAGES)
