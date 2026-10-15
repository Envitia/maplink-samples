# *****************************************************************************
#		Copyright (c) 2010 by Envitia Ltd
# =============================================================================
# PROJECT      : TRASH
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
# -- Include the platform stuff

include Makefile.$(MAPLARCH)

# =============================================================================
# -- Now some local and common stuff 

INCDIR= $(MAPL_HOME)/include
LIBDIR= $(MAPL_HOME)/lib

CC = $(CCEXEC) -DTSL_DEFINE_BOOL $(CCFLAGS) $(INCS) -I$(INCDIR) -O -G

CCXX = $(CCXXEXEC) $(CCXXFLAGS) $(INCS) -I$(INCDIR) -g

.SUFFIXES:
.SUFFIXES: .cpp .c .o $(SUFFIXES)

.c.o:   ; $(CC) -o $@ $<

.cpp.o:   ; $(CCXX) $< -o $@

LDFLAGS    = $(OPT_DBG)
LINK       = $(PRELINK) $(CCXXEXEC) $(LDFLAGS)
MV         = \mv -f
CP         = \cp
RM         = \rm -f
CAT        = cat
SED        = sed
AWK        = awk

MAPL_LIBS  = -L$(LIBDIR) -lddo_$(MAPLARCH) \
		$(MAPLINK_ADDITIONAL_LIBS) -lmaplinks_$(MAPLARCH) -lc

THISBIN   = example_$(MAPLARCH)

MOTIFLIBS = -lXm -lXt -lX11

VXWORKS_LIBS = -L$(LIBDIR) -lmaplinks_$(MAPLARCH) $(MAPLINK_ADDITIONAL_LIBS)

# =============================================================================
# -- Define the objects making up the executable.

OBJS = createmapcrcs.o crc_32.o

default: binary

remove:
	$(RM) *.o

build:	clean objs

objs:	$(OBJS)

binary:	$(THISBIN)
	cp $(THISBIN) createCRCs

clean:
	touch fred.o
	$(RM) *.o

# =============================================================================
# -- Build rules for exe's

include Makefile.exes

