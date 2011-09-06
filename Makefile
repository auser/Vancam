####################
# VARIABLES
####################
PROG_NAME			:= badge
CXX_SOURCES 	:= $(wildcard $(addsuffix *.cpp, src/))
CXX_OBJECTS 	:= ${CXX_SOURCES:.cpp=.o}
INCLUDE_DIRS 	:= ./include /usr/local/include/opencv
LIBRARY_DIRS 	:= 
LIBRARIES			:= libczmq libzmq opencv
LIBRARY_FLAGS := -lev

####################
# /VARIABLES
####################

CXX_FLAGS 	= -Wall
CXX_FLAGS  += $(foreach include_dir, $(INCLUDE_DIRS), -I$(include_dir))
CXX_FLAGS  += $(foreach pkg, $(LIBRARIES), `pkg-config --cflags $(pkg)`)

LD_FLAGS 		= $(foreach libdir, $(LIBRARY_DIRS), -L$(libdir))
LD_FLAGS	 += $(foreach lib, $(LIBRARIES), `pkg-config --libs $(lib)`)
LD_FLAGS 	 += $(LIBRARY_FLAGS)

.PHONY: all clean

CC = g++

all: $(PROG_NAME)

$(PROG_NAME): $(CXX_OBJECTS)
	$(CC) -o bin/$(PROG_NAME) $(LD_FLAGS) $(CXX_OBJECTS)

deps: $(wildcard lib/**)
	@cd $< && ./configure && make

$(CXX_OBJECTS): %.o : %.cpp
	$(CC) $(CXX_FLAGS) -c -o $@ $<

clean:
	@- $(RM) bin/event
	@- $(RM) $(BUILD_OBS)

