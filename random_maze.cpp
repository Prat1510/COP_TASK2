#include <iostream>
using namespace std;

#define MAZE_WIDTH 10
#define MAZE_HEIGHT 10

//----GLOBAL VARIABLES------------------------------------------------
char maze[MAZE_WIDTH*MAZE_HEIGHT];
//----FUNCTION PROTOTYPES---------------------------------------------
void ResetMaze();
int XYToIndex( int x, int y );
void Visit( int x, int y );
void PrintMaze();
//----FUNCTIONS-------------------------------------------------------
int main()
{
    // Starting point and top-level control.
    srand( time(0) ); // seed random number generator.
    ResetMaze();
    Visit(1,1);
    PrintMaze();
    return 0;
}
void ResetMaze()
{
    // Fills the maze with walls ('#' characters).
    for (int i=0; i<MAZE_WIDTH*MAZE_HEIGHT; ++i)
    {
        maze[i] = '#';
    }
}
int XYToIndex( int x, int y )
{
    // Converts the two-dimensional index pair (x,y) into a
    // single-dimensional index. The result is y * ROW_WIDTH + x.
    return y * MAZE_WIDTH + x;
}

// This is the recursive function we will code in the next project
void Visit( int x, int y )
{
    // Starting at the given index, recursively visits every direction in a
    // randomized order.
    // Set my current location to be an empty passage.
    cout<<"Visiting"<<x<<", "<<y<<endl;
    maze[ XYToIndex(x,y) ] = ' ';
    // Create an local array containing the 4 directions and shuffle their order.
    int dirs[4];
    dirs[0] = 0;
    dirs[1] = 1;
    dirs[2] = 2;
    dirs[3] = 3;
    for (int i=0; i<4; ++i)
    {
        int r = rand() & 3;
        int temp = dirs[r];
        dirs[r] = dirs[i];
        dirs[i] = temp;
    }
    // Loop through every direction and attempt to Visit that direction.
    for (int i=0; i<4; ++i)
    {
        // dx,dy are offsets from current location. Set them based
        // on the next direction I wish to try.
        int dx=0, dy=0;
        switch (dirs[i])
        {
            case 0: dy = -1; break;
            case 2: dy = 1; break;
            case 1: dx = 1; break;
            case 3: dx = -1; break;
        }
        // Find the (x,y) coordinates of the maze cell 2 spots
        // away in the given direction.
        
        int x2 = x + 2*dx;
        int y2 = y + 2*dy;
//        cout<<x<<", "<<x+dx<<", "<<x2<<endl;
//        cout<<y<<", "<<y+dy<<", "<<y2<<endl;
        if (x2 >= 0 && x2 < MAZE_WIDTH && y2 >= 0 && y2 < MAZE_HEIGHT)
        {
            if (maze[ XYToIndex(x2,y2) ] == '#')
            {
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                maze[ XYToIndex(x2-dx,y2-dy) ] = ' ';
                // Recursively Visit (x2,y2)
                Visit(x2,y2);
            }
        }
    }
    
}
void PrintMaze()
{
    // Displays the finished maze to the screen.
    for (int y=0; y<MAZE_HEIGHT; ++y)
    {
        for (int x=0; x<MAZE_WIDTH; ++x)
        {
            cout << maze[XYToIndex(x,y)];
        }
        cout << endl;
    }
}
