## -*- Makefile -*-
##
## User: Jasper van Herpt
## Time: 2014-02-16 15:30
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g -fpermissive
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
BASEDIR_Application=.
SOURCEDIR_Application=$(BASEDIR_Application)/src/jaztec
TARGETDIR_Application=$(BASEDIR_Application)/build
OBJDIR_Application=$(TARGETDIR_Application)/obj
BINDIR_Application=$(TARGETDIR_Application)/bin


all: $(BINDIR_Application)/Warmonger

## Target: Herder
CPPFLAGS_Application = -Isrc/


OBJS_Application =  \
	$(OBJDIR_Application)/main.o \
	$(OBJDIR_Application)/TileManager.o \
	$(OBJDIR_Application)/JEngine.o \
	$(OBJDIR_Application)/JRoute.o \
	$(OBJDIR_Application)/JlibSDL.o \
	$(OBJDIR_Application)/WMMainState.o \
	$(OBJDIR_Application)/WMPlayState.o \
	$(OBJDIR_Application)/WMCamera.o
SYSLIBS_Application = -ldl 
USERLIBS_Application = -Bstatic -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL `sdl-config --libs` $(SYSLIBS_Application) 
DEPLIBS_Application =      
LDLIBS_Application = $(USERLIBS_Application)


# Link or archive
$(BINDIR_Application)/Warmonger: $(TARGETDIR_Application) $(OBJS_Application) $(DEPLIBS_Application)
	$(LINK.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ $(OBJS_Application) $(LDLIBS_Application)


# Compile source files into .o files
$(OBJDIR_Application)/main.o: $(BASEDIR_Application) main.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ main.cpp

$(OBJDIR_Application)/TileManager.o: $(SOURCEDIR_Application) src/jaztec/TileManager.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ src/jaztec/TileManager.cpp

$(OBJDIR_Application)/JEngine.o: $(SOURCEDIR_Application) src/jaztec/JEngine.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ src/jaztec/JEngine.cpp

$(OBJDIR_Application)/JRoute.o: $(SOURCEDIR_Application) src/jaztec/JRoute.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ src/jaztec/JRoute.cpp

$(OBJDIR_Application)/JlibSDL.o: $(SOURCEDIR_Application) src/jaztec/JlibSDL.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ src/jaztec/JlibSDL.cpp

$(OBJDIR_Application)/WMMainState.o: $(SOURCEDIR_Application) src/warmonger/WMMainState.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ src/warmonger/WMMainState.cpp

$(OBJDIR_Application)/WMPlayState.o: $(SOURCEDIR_Application) src/warmonger/WMPlayState.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ src/warmonger/WMPlayState.cpp
	
$(OBJDIR_Application)/WMCamera.o: $(SOURCEDIR_Application) src/warmonger/WMCamera.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Application) -o $@ src/warmonger/WMCamera.cpp

#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(BINDIR_Application)/Warmonger \
		$(OBJDIR_Application)/main.o \
		$(OBJDIR_Application)/TileManager.o \
		$(OBJDIR_Application)/JEngine.o \
		$(OBJDIR_Application)/JRoute.o \
		$(OBJDIR_Application)/JlibSDL.o \
		$(OBJDIR_Application)/WMMainState.o \
		$(OBJDIR_Application)/WMCamera.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

