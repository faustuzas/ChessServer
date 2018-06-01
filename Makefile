sdl2 = -F/Library/Frameworks/SDL2.framework/Headers -framework SDL2
sdl2_net = -F/Library/Frameworks/SDL2_net.framework/Headers -framework SDL2_net

main: main.o ClientConnection.o Matchmaker.o NetworkManager.o ServerConnection.o
	g++ main.o ClientConnection.o Matchmaker.o NetworkManager.o ServerConnection.o -o app $(sdl2) $(sdl2_net)

main.o: main.cpp
	g++ main.cpp -c -std=c++11

ClientConnection.o: ClientConnection.cpp
	g++ ClientConnection.cpp -c -std=c++11

Matchmaker.o: Matchmaker.cpp
	g++ Matchmaker.cpp -c -std=c++11

NetworkManager.o: NetworkManager.cpp
	g++ NetworkManager.cpp -c -std=c++11

ServerConnection.o: ServerConnection.cpp
	g++ ServerConnection.cpp -c -std=c++11