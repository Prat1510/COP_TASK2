#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_net.h>
#include <string.h>
using namespace std;

int main(int argc, char const *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	IPaddress ip;
	SDLNet_ResolveHost(&ip, "127.0.0.1", 1234);
	TCPsocket connection = SDLNet_TCP_Open(&ip);

	if (connection == NULL){
		cout<<"Failed connection"<<endl;
	}
	SDLNet_SocketSet server = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(server, connection);

	SDL_Event event;
	string input;
	bool running = true;
	char tmp[1400];	
	while (running){
		while(SDL_PollEvent(&event)){
			cout<<"here"<<endl;
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
				running = false;
				break;
			}
		}
		if (running)
		{
			cin>>input;
			sprintf(tmp, "%s", input.c_str());
			SDLNet_TCP_Send(connection, tmp, strlen(tmp)+1); 
			SDLNet_TCP_Recv(connection, tmp, 1400);
			cout<<tmp<<endl;
			// SDL_Delay(1);
		}
	}
	// SDLNet_TCP_Recv(client, text, 100);
	// cout<<text

	SDLNet_TCP_Close(connection);
	SDLNet_FreeSocketSet(server);
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}