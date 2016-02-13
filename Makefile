LIBS := -lm -lpthread
CXX := g++

all: benson

%.o: %.cpp #%.h
		$(CXX) -c $< -o $@

%.o: %.hpp
		$(CXX) -c $< -o $@

benson: main.cpp
		@echo "** Building the Benson"
		g++ main.cpp classes/tsunami.cpp classes/controle.cpp classes/malha_fechada.cpp -o benson -lm -lpthread
		#$(CXX) -o benson main.o $(LIBS)

clean:
		@echo "** Removing object files and executable..."
		rm -f benson *.o

# install:
#         @echo '** Installing...'
#         cp benson /usr/bin/

# uninstall:
#         @echo '** Uninstalling...'
#         $(RM) /usr/bin/thegame
