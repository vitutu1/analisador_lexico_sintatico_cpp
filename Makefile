CXX = g++
CXXFLAGS = -std=c++17 -Wall

BISON = bison
FLEX = flex

TARGET = analisador

PARSER_CPP = parser.tab.cpp
PARSER_HPP = parser.tab.hpp
LEXER_CPP = lex.yy.cc


OBJS = main.o $(PARSER_CPP:.cpp=.o) $(LEXER_CPP:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(PARSER_CPP) $(PARSER_HPP): parser.y
	$(BISON) -d --language=c++ -o $(PARSER_CPP) parser.y


$(LEXER_CPP): lexer.l $(PARSER_HPP)
	$(FLEX) --c++ -o $(LEXER_CPP) lexer.l


main.o: main.cpp lexer.hpp ast.hpp $(PARSER_HPP)
parser.tab.o: $(PARSER_CPP) lexer.hpp ast.hpp
lex.yy.o: $(LEXER_CPP) lexer.hpp $(PARSER_HPP)

clean:
	rm -f $(TARGET) $(OBJS) $(PARSER_CPP) $(PARSER_HPP) $(LEXER_CPP) location.hh position.hh stack.hh
