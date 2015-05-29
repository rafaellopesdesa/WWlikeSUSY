CC = g++
INCLUDE = -I./ -I./include/ -I./mt2/include/oxbridgekinetics-1.0/ -I./dataMCplotMaker/include/ -I./inih/include/
CFLAGS = -Wall -g $(shell root-config --cflags) $(INCLUDE)
LINKER = g++

LINKERFLAGS = -undefined dynamic_lookup -headerpad_max_install_names $(shell root-config --libs) -lEG -lGenVector -lMinuit -lMinuit2 -lTreePlayer

SRCDIR = src
OBJDIR = obj
BINDIR = bin
LNKDIR = links

SOURCES = $(wildcard $(SRCDIR)/*.cxx) $(patsubst $(LNKDIR)/%_linkdef.h,$(SRCDIR)/%_dict.cxx,$(wildcard $(LNKDIR)/*.h))
OBJECTS = $(patsubst %,$(OBJDIR)/%.o,$(basename $(notdir $(SOURCES)))) 

LIBS = -L./mt2/lib/ -loxbridgekinetics-1.0.1 -L./dataMCplotMaker/lib -ldataMCplotMaker -L./inih/lib -linih
EXE = analysis.exe mttester.exe

.PRECIOUS: $(OBJDIR)/%.o $(SRCDIR)/%_dict.cxx

.PHONY: all

all: $(EXE)

# General rule for making object files
$(SRCDIR)/%_dict.cxx: $(LNKDIR)/%_linkdef.h
	@rootcling -f $@ -c $^
	@cp $(SRCDIR)/$^_dict_rdict.pcm .

$(OBJDIR)/%.o: $(BINDIR)/%.cc 
	$(CC) $(CFLAGS) $< -c -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx
	$(CC) $(CFLAGS) $< -c -o $@

%.exe: $(OBJECTS) $(OBJDIR)/%.o
	$(CC) -Wall -o $@ $(LINKERFLAGS) $(LIBS) $^

clean:  
	rm -v -f \
        $(OBJDIR)/*.o *.exe \
	$(SRCDIR)/*_dict.cxx \
	*.pcm
	@echo "Done"
