# Configure compiler settings
CXX=clang++
CXXFLAGS=-gdwarf-4 -std=c++11 -Werror -D_GLIBCXX_DEBUG
# The object files for the program.
OFILES = keywordSearcher.o hashFunctions.o
# The header files for the program
HFILES = $(shell ls -1 *.h)

# UnitTest++ keeps its object files in this library.
UNITTEST_LIB = -lUnitTest++

all: keywordSearch tests manualTests

# This target builds your main program.
keywordSearch: $(HFILES) $(OFILES) main.o
	$(CXX) $(CXXFLAGS) -o $@ main.o $(OFILES)

# This target builds your tests.
tests: tests.o $(HFILES) $(OFILES)
	$(CXX) $(CXXFLAGS) -o $@ tests.o $(OFILES) $(UNITTEST_LIB)

# This target builds your manual testing program.
manualTests: manualTests.o $(HFILES) $(OFILES)
	$(CXX) $(CXXFLAGS) -o $@ manualTests.o $(OFILES)

# This target describes how to compile a .o file from a .cpp file.
%.o: %.cpp $(HFILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# This target deletes the temporary files we have built.
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.aux *.log
	rm -f keywordSearch
	rm -f tests
	rm -f manualTests
	rm -f experiment
