CC = g++
CXXFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

TARGET = projekt

$(TARGET): $(TARGET).cpp
	$(CC)  -o $(TARGET) $(TARGET).cpp Spawn.cpp Rules.cpp Wall.cpp Region.cpp  $(CXXFLAGS)

clean: 
	$(RM) $(TARGET)