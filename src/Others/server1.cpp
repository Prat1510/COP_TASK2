#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_net.h>
#include <vector> 
#include <cstdio>
#include <string>
using namespace std;

struct data{
	TCPsocket socket;
	int timeout;
	int id;

	data(TCPsocket sock, int t, int i): socket(sock), timeout(t), id (i){} 
};

int main(int argc, char const *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	int curid = 0;
	int playernum = 0;
	SDL_Event event;
	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 1234);
	
	vector<data> socketvector;
	char tmp[1400];
	bool running = true;
	SDLNet_SocketSet sockets = SDLNet_AllocSocketSet(2);

	TCPsocket server = SDLNet_TCP_Open(&ip);

	while(running){
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
				running = false;
			}	
		}
		TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
		if (tmpsocket){
			// cout<<"here"<<endl;
			if (playernum < 2){
				SDLNet_TCP_AddSocket(sockets, tmpsocket);
				socketvector.push_back(data(tmpsocket, SDL_GetTicks(), curid));
				playernum++;
				// sprintf(tmp, "0 %d \n", curid);
				curid++;
				cout<<"New connect: "<<curid<<'\n';
			}
			else{
				sprintf(tmp, "3 \n");
			}
			// SDLNet_TCP_Send(tmpsocket, tmp, strlen(tmp)+1);
		}
		// data in
		if (SDLNet_CheckSockets(sockets,0) > 0){
			// cout<<"here59"<<endl;
			for (int i = 0; i < socketvector.size(); i++)
			{
				// cout<<i<<endl;
				if( SDLNet_SocketReady(socketvector[i].socket)){
					// socketvector[i].timeout = SDL_GetTicks();
					int rec = SDLNet_TCP_Recv(socketvector[i].socket, tmp, 1400);
					// cout<<rec<<endl;
					// if (rec > 0)
					// {
					// 	for (int j = 0; j < socketvector.size(); j++)
					// 	{
					// 		if (i!=j){
					// 			cout<<tmp<<endl; 
					// 			SDLNet_TCP_Send(socketvector[j].socket, tmp, strlen(tmp)+1);
					// 		}
					// 	}
					// }
					cout<<tmp<<endl; 
					for (int j = 0; j < socketvector.size(); j++)
					{
						if (i==j){
							continue;
						}
						SDLNet_TCP_Send(socketvector[j].socket, tmp, strlen(tmp)+1);
					}
				} 
			}
		}	
		// SDL_Delay(1);
	}

	for (int i = 0; i < socketvector.size();i++)
	{
		SDLNet_TCP_Close(socketvector[i].socket); 
	}
	SDLNet_FreeSocketSet(sockets);
	SDLNet_TCP_Close(server);
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}