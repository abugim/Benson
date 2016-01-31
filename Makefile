LIBS := -lm -lpthread
CXX := g++

all: benson

%.o: %.cpp
		$(CXX) -c $< -o $@

%.o: %.hpp
		$(CXX) -c $< -o $@

benson: main.o
		@echo "** Building the Benson"
		$(CXX) -o benson main.o $(LIBS)

clean:
		@echo "** Removing object files and executable..."
		rm -f benson *.o

# install:
#         @echo '** Installing...'
#         cp benson /usr/bin/

# uninstall:
#         @echo '** Uninstalling...'
#         $(RM) /usr/bin/thegame