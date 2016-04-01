LIBS := -lm -lpthread -lboost_system -lboost_chrono
CXX := g++
CLASSES := $(wildcard ./classes/*.h)
all: benson

%.o: %.cpp #%.h
		$(CXX) -c $< -o $@

%.o: %.h
		$(CXX) -c $< -o $@

benson: main.cpp $(CLASSES)
		@echo "** Building the Benson"
		cd classes; make
		$(CXX) main.cpp -I ./classes/ -I ./websocketpp/ -L $(LIBS) -o benson

clean:
		@echo "** Removing object files and executable..."
		rm -f benson *.o
