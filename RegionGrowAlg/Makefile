#This is a general make file for every opencv project in c++ which has the scope to facilitate the compilation of .cpp files
#that necessitate a lot of external library to include.
prog="RegionGrow.cpp"
CXX=g++
CXXFLAGS += -c -std=c++11 -Wall $(shell pkg-config --cflags opencv)
LDFLAGS += $(shell pkg-config --libs --static	opencv)

#to generate the name of the executable, we use the name of the .ccp file of input and strip it the extension;
stripped=$(shell echo $(prog) | cut -f1 -d ".")

#Dependent from the function name after the stripped variable and the clean to execute after;
all: $(stripped) 

#check if the name of the program is set by the user defined as: prog=program_name;	
ifndef prog 
	$(error Usage: make prog=example.cpp)
endif

$(stripped):$(stripped).o; $(CXX) $< -o $@ $(LDFLAGS)

%.o: %.cpp; $(CXX) $< -o $@ $(CXXFLAGS)

clean:; find . -type f  ! \( -name "*.*" -o -name "Makefile" \) -delete 
