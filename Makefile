CXX = g++
CXXFLAGS = -std=c++17 -Wall

FLEX = flex
BISON = bison

TARGET = analisador

SOURCES = main.cpp parser.tab.cpp lexer.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = ast.hpp lexer.hpp parser.tab.hpp

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) -lfl

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

parser.tab.cpp parser.tab.hpp: parser.y
	$(BISON) -d --language=c++ -o parser.tab.cpp parser.y

lexer.cpp: lexer.l parser.tab.hpp
	$(FLEX) -o lexer.cpp lexer.l

lexer.o: lexer.cpp

clean:
	rm -f $(TARGET) $(OBJECTS) parser.tab.cpp parser.tab.hpp lexer.cpp \
	      position.hh location.hh stack.hh