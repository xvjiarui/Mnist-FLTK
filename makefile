CC=gcc
CXX=g++
CXXFLAGS=-I. -I/usr/local/include -O2 -std=c++11 -Wall
LDFLAGS=$(shell fltk-config --ldflags --use-images)
RM=rm -f
MKDIR=mkdir -p
SDIR=. ui util model
ODIR=obj
OUT=mnist-demo

SRC=$(foreach dir, $(SDIR), $(wildcard $(dir)/*.cpp))
OBJ=$(patsubst %.cpp, $(ODIR)/%.o, $(SRC))
DEP=$(OBJ:.o=.d)
SOBJ=$(foreach dir, $(SDIR), $(ODIR)/$(dir))

.PHONY: directories

all: debug all_directories $(OUT)

debug:
	$(info $$SRC=$(SRC))
	$(info $$OBJ=$(OBJ))
	$(info $$SOBJ=$(SOBJ))
$(OUT): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

-include $(DEP)
$(ODIR)/%.d: %.cpp
	$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) > $@
$(ODIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUT)
	$(RM) -r $(ODIR)
all_directories: directories $(SOBJ)
$(SOBJ): $(ODIR)/%:
	$(MKDIR) $@
directories: $(ODIR)

$(ODIR):
	$(MKDIR) $(ODIR)
