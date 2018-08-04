#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

struct Warrior {
    string weapon;
    int power;
};

//Feel free to modify the parameters if you need other values
bool skirmish(Warrior*** protectors, Warrior*** invaders, int skirmish_row,
		      int rows, int columns, int &reserves, ofstream &output){
    //returns true if the invaders breach the walls.
    if(skirmish_row > columns)
    {//if i is out of bounds, move onto next skirmish
        return false;
    }
    for(int i = 0; i < rows; i++)
    {
        Warrior* protector = protectors[i][skirmish_row];
        Warrior* invader = invaders[skirmish_row][i];
        if(invader->power != -1 && protector->power != -1)
        {
            if((protector->weapon == "axe" && invader->weapon != "axe") ||
            (((protector->weapon == "axe" && invader->weapon == "axe")||
             (protector->weapon == "sword" && invader->weapon == "sword"))&&
                 (protector->power > invader->power)))
            {
                //PROTECTOR WINS
                invaders[skirmish_row][i]->power = -1;//-1 power means r ded
                output << "Invader killed\n";
            }
            else if((protector->weapon != "axe" && invader->weapon == "axe") ||
                  (((protector->weapon == "axe" && invader->weapon == "axe")||
                (protector->weapon == "sword" && invader->weapon == "sword"))&&
                 (protector->power < invader->power)))
            {
                //INVADER WINS
                bool defected = false;//used 2 break loop if protector defects
                for(int a = 0; a < columns; a++)
                {
                    for(int b = 0; b < rows; b++)
                    {
                        if(invaders[a][b]->power == -1)
                        {//chk 4 open spot in invader
                            invaders[a][b]->power = protector->power;//defected
                            invaders[a][b]->weapon = protector->weapon;
                            output << "Protector defected\n";
                            defected = true; 
                            protector->power = -1;
                            goto finish;
                        }
                    }
                }
                finish:
                if(defected != true)
                {
                    protector->power = -1;//protector spot now open
                    output << "Protector killed\n";
                }
                if(reserves > 0)//reserve protector take splace
                {
                    protector->weapon = "axe";
                    protector->power = 100;
                    reserves--;
                }
                else
                {
                   protector->power = -1;//protector spot now open
                }
            }
            else
            {
                //DRAW
                output << "Duel ends in draw\n";
            }
        }
        else if(invader->power == -1)
        {
            output << "No assault\n";
        }
        else if(invader->power != -1 && protector->power == -1)
        {//walls are breached
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
	   cerr << "Please provide an input and output file" << endl;
	   return -1;
    }

    ifstream input(argv[1]);
    ofstream output(argv[2]);

    int rows;
    int cols;
    int reserve;
    int skirmishes;

    //read the input file and initialize the values here.
    input >> rows >> cols >> reserve >> skirmishes;
    Warrior ***protectors;
    Warrior ***invaders;
    //initialize the protectors and invaders here.
    //protectors
    protectors = new Warrior**[rows];
    for(int i = 0; i < rows; i++)
    {
        protectors[i] = new Warrior*[cols];
    }
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            Warrior* protector = new Warrior();
            if(i % 2 == 0)
            {
                protector->weapon = "axe";
            }
            else
            {
                protector->weapon = "sword";
            }
            protector->power = i*10+(j+1)*10;
            protectors[i][j] = protector;
        }
    }

    //invaders
    invaders = new Warrior**[cols];
    for(int i = 0; i < cols; i++)
    {
        invaders[i] = new Warrior*[rows];
    }
    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            Warrior* invader = new Warrior();
            if(i % 2 == 0)
            {
                invader->weapon = "sword";
            }
            else
            {
                invader->weapon = "axe";
            }
            invader->power = i*10+(j+1)*10;
            invaders[i][j] = invader;
        }
    }

    bool breached = false;//bool to determine if breach has happened
    for (int i=1; i <= skirmishes; i++)
    {
        int skirmish_row;
        
        //read the input file to find the value for skirmish row
        input >> skirmish_row;
        
    //In general, it is bad style to throw everything into your main function
        bool end = skirmish(protectors, invaders, skirmish_row, rows, 
                           cols, reserve, output);
        if(end == true)
        {
            breached = true;
        }
    }
    //output the winner and deallocate your memory.
    if(breached == true)
    {
        output << "Winner: invaders\n";
    }
    else
    {
        output << "Winner: protectors\n";
    }
    //DEALLOCATE
    
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            delete protectors[i][j];
        }
        delete[] protectors[i];
    }
    delete[] protectors;
    
    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            delete invaders[i][j];
        }
        delete[] invaders[i];
    }
    delete[] invaders;
    input.close();
    output.close();
    
    return 0;
}
