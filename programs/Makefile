# ==============================================================================
# Build Recipes for Hardware Drivers
# ==============================================================================
#
# Host Info: 
#	* (run cat /proc/cpuinfo/)
#	Hardware        : BCM2835
#	Revision        : c03112
#	Model           : Raspberry Pi 4 Model B Rev 1.2
#
# Notes:
#	This make-config targets dependencies by pattern-matching. New drivers
#	written and tossed into DRIVERS_PATH/ don't need to be appended to the build
#	inscructions below, but should be suitable candidates for pattern-matching.
#	Specifically, if my_target.cpp #includes foobar.h, with methods defined in
#	foobar.cpp, both should be present in /DRIVERS_PATH/ during the build.
#
#	For these simple drivers and unit tests, deviating from this would be a 
#	strange and deliberate effort involving unique paths + intermediate object 
#	linking, and I can't imagine a sane reason for doing this (knock on wood).
#
#	I haven't invoked a specific -std=c++xx because I'm not using any new STL 
#	entries, and my use of compile-time directives/ templates is pretty thin.

#
CXX = g++
CXXFLAGS = -Wall -std=c++11

#
LINKED = -lwiringPi

#
DRIVERS_PATH = ../drivers
TARGETS_PATH = ../programs

#
SOURCES = $(wildcard $(DRIVERS_PATH)/*.cpp)

#
TARGETS = $(wildcard $(TARGETS_PATH)/*.cpp)

# Executables should have same name as build targets (minus file suffix)
EXECS = $(basename $(TARGETS))

# Compile everything (very handy for package tests)
all: $(EXECS)

list:
	$(info $(EXECS))

% : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(SOURCES) $(LINKED)

clean:
	rm -f *.o $(EXECS)
