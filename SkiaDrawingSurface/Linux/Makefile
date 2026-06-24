# *****************************************************************************
#		Copyright (c) 2000-2026 by Envitia Group PLC.
# =============================================================================
# PROJECT      : Samples
# AUTHOR(s)    : DBLD
# =============================================================================
# DESCRIPTION :  Platform specific makefile
# *****************************************************************************	

VXWORKS_PARTIAL_LINK =
VXWORKS_DUMP =
VXWORKS_LINK =
VXWORKS_CTDT_COMPILE =
VXWORKS_MUNCH =

# =============================================================================
# -- Set MapLink configuration details.
# Dependening on standard release or development build update paths.
# For the standard release the libraries are in the 'redist64' directory,
# for development builds they are installed in 'dist' directory.
MAPL_HOME=$(abspath ${PWD}/../../..)
INSTALL_DIR=$(MAPL_HOME)
MAPL_LIB_DIR=lib64


# =============================================================================
# -- Include the platform stuff

MAPLARCH=linux_gcc
include Makefile.$(MAPLARCH)
$(info Usage: MAPL_HOME=${MAPL_HOME} ./example_$(MAPLARCH) <map-file>)

# =============================================================================
# -- Now some local and common stuff 

INCDIR= $(INSTALL_DIR)/include
LIBDIR= $(INSTALL_DIR)/$(MAPL_LIB_DIR)

CC = $(CCEXEC) -DTSL_DEFINE_BOOL $(CCFLAGS) $(INCS) -I$(INCDIR)

CCXX = -std=c++17 $(CCXXEXEC) $(CCXXFLAGS) $(INCS) -I$(INCDIR)
LDFLAGS += -Wl,-rpath,$(LIBDIR) -Wl,--disable-new-dtags

.SUFFIXES:
.SUFFIXES: .cpp .c .o $(SUFFIXES)

.c.o:   ; $(CC) -o $@ $<

.cpp.o:   ; $(CCXX) $< -o $@

LINK       = $(PRELINK) $(CCXXEXEC) $(LDFLAGS)
MV         = \mv -f
CP         = \cp
RM         = \rm -f
CAT        = cat
SED        = sed
AWK        = awk

THISBIN   = example_$(MAPLARCH)

MAPL_LIBS  = -L$(LIBDIR) -lharfbuzz -lpng  -lMapLink -lc $(MAPLINK_ADDITIONAL_LIBS)

MOTIFLIBS = -lXt -lX11
VXWORKS_LIBS = -L$(LIBDIR) -lXm -lMapLink $(MAPLINK_ADDITIONAL_LIBS)

# =============================================================================
# -- Define the objects making up the executable.

OBJS = example.o vxworks.o 

default: binary

remove:
	$(RM) *.o

build:	clean objs

objs:	$(OBJS)

binary:	$(THISBIN)

clean:
	touch fred.o
	$(RM) *.o

# =============================================================================
# -- Build rules for exe's

include Makefile.exes

