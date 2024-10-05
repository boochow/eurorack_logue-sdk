# #############################################################################
# Project Customization
# #############################################################################

EURORACKDIR = ../eurorack
BRAIDSDIR   = $(EURORACKDIR)/braids
STMLIBDIR   = $(EURORACKDIR)/stmlib
SRCDIR      = ../common

PROJECT = lily-nz

UCSRC =

UCXXSRC = $(SRCDIR)/unit.cc $(SRCDIR)/digital_oscillator.cc $(SRCDIR)/resources.cc $(BRAIDSDIR)/quantizer.cc $(SRCDIR)/macro_oscillator.cc $(SRCDIR)/analog_oscillator.cc $(STMLIBDIR)/utils/random.cc

UINCDIR = $(EURORACKDIR) $(STMLIBDIR) $(STMLIBDIR)/third_party/STM $(STMLIBDIR)/third_party/STM/STM32F10x_StdPeriph_Driver/inc/ $(STMLIBDIR)/third_party/STM/CMSIS/CM3_f10x

UDEFS = -DLILYNZ -fno-use-cxa-atexit

ULIB = 

ULIBDIR =
