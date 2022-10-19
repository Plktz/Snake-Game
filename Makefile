LIBS = -lsfml-graphics -lsfml-window -lsfml-system 
CXX = g++

%.o : %.cpp
	$(CXX) -c $< -o $@

play : Game.o 
	$(CXX) -o A_Game Game.o $(LIBS)
	.\A_Game.exe