all:
	mkdir o
	g++ -c -o o/config.o config.cpp 
	g++ -c -o o/ini.o ini.c
	g++ -c -o o/INIReader.o INIReader.cpp 
	g++ -c -o o/Part.o Part.cpp 
	g++ -c -o o/distributionLaw.o distributionLaw.cpp 
	g++ -c -o o/StateMachine.o StateMachine.cpp 
	g++ -c -o o/Vect.o Vect.cpp 
	g++ -c -o o/PartArray.o PartArray.cpp 
	g++ -c -o o/StateMachineFree.o StateMachineFree.cpp

	ar -ru libPartsEngine.a o/config.o o/ini.o o/INIReader.o o/Part.o o/distributionLaw.o o/StateMachine.o o/Vect.o o/PartArray.o o/StateMachineFree.o
clean:
	rm -r o
	rm libPartsEngine.a
