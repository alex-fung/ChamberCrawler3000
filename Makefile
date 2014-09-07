CXX = g++
CXXFLAGS = -Wall -MMD -g
EXEC = cc3k
OBJECTS = harness.o character.o treasure.o potion.o variable.o floor.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
