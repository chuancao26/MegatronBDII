megatron: main.o Megatron.o Menu.o
	g++ -g -o megatron main.o Megatron.o Menu.o
main.o Megatron.o Menu.o: main.cpp Megatron.cpp Menu.cpp
	g++ -g -c main.cpp Megatron.cpp Menu.cpp
