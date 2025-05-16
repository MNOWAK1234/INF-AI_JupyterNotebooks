/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "objects/myCube.h"
#include "objects/myTeapot.h"
#include "objects/szachownica.h"
#include "objects/king.h"
#include "objects/queen.h"
#include "objects/rook.h"
#include "objects/bishop.h"
#include "objects/knight.h"
#include "objects/pawn.h"

float speed_x=0;
float speed_y=0;
float aspectRatio=1;

// Camera variables
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -120.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

ShaderProgram *sp;

//chessboard
float* vertices = TestVertices;
float* texCoords = tex;
float* normals = VertexNormals;
int vertexCount = TestVertexCount;

GLuint tex0;
GLuint tex1;
GLuint tex2;

std::vector < glm::mat4 > M;
std::vector < glm::vec3 > T;

int pieces[10][10];

std::vector <std::string> moves;
int c1, c2, c3, c4;

std::vector<int>whiteQueens;
std::vector<int>blackQueens;
std::vector<int>whiteRooks;
std::vector<int>blackRooks;
std::vector<int>whiteBishops;
std::vector<int>blackBishops;
std::vector<int>whiteKnights;
std::vector<int>blackKnights;

std::vector<int> letter1;
std::vector<int> number1;
std::vector<int> letter2;
std::vector<int> number2;
std::vector<int> special;

std::fstream file;

int count = 0;
int whitePiecesTaken = 0;
int blackPiecesTaken = 0;

void nextmove(int letter1, int number1, int letter2, int number2, int special)
{
    if (special == 1)
    {
        //krotka roszada biale
        nextmove(5, 1, 7, 1, 0);
        nextmove(8, 1, 6, 1, 0);
    }
    else if (special == 2)
    {
        //krotka roszada czarne
        nextmove(5, 8, 7, 8, 0);
        nextmove(8, 8, 6, 8, 0);
    }
    else if (special == 3)
    {
        //dluga roszada biale
        nextmove(5, 1, 3, 1, 0);
        nextmove(1, 1, 4, 1, 0);
    }
    else if (special == 4)
    {
        //dluga roszada czarne
        nextmove(5, 8, 3, 8, 0);
        nextmove(1, 8, 4, 8, 0);
    }
    else
    {
        //oblicz roznice miedzy polami do wykonania ruchu
        int LettersDiff = letter2 - letter1;
        int NumbersDiff = number2 - number1;
        int PositiveLettersDiff=LettersDiff;
        int PositiveNumbersDiff=NumbersDiff;
        if (LettersDiff < 0) PositiveLettersDiff = -1 * LettersDiff;
        if (NumbersDiff < 0) PositiveNumbersDiff = -1 * NumbersDiff;
        //znajdz figure
        int pieceNumber = pieces[number1][letter1];
        //nic nie zostaje na polu
        pieces[number1][letter1] = 0;
        
        int frames, takenPiece, TakeLettersDiff, TakeNumbersDiff, PositiveLetterDiff1, PositiveNumberDiff1;
        float moveLetter, moveNumber;
        
        if (pieces[number2][letter2] != 0) //cos stoi na polu - bicie
        {
            takenPiece = pieces[number2][letter2];
            if (takenPiece <= 16) //zbita figura jest biala
            {
                TakeLettersDiff = 10 - letter2 - whitePiecesTaken; 
                TakeNumbersDiff = 10 - number2; 
                whitePiecesTaken++;

                PositiveLetterDiff1 = TakeLettersDiff;
                PositiveNumberDiff1 = TakeNumbersDiff;
            }
            else //zbita figura jest czarna
            {
                TakeLettersDiff = -letter2 - 1 + blackPiecesTaken;
                TakeNumbersDiff = -number2 - 1;
                blackPiecesTaken++;

                PositiveLetterDiff1 = -1 * TakeLettersDiff;
                PositiveNumberDiff1 = -1 * TakeNumbersDiff;
            }
            
            frames = std::max(PositiveLetterDiff1, PositiveNumberDiff1) * 500000;
            moveLetter = (float)TakeLettersDiff * 16/ (float)frames;
            moveNumber = (float)TakeNumbersDiff * 16/ (float)frames;
            for (int currFrame = 0; currFrame < frames; currFrame++)
            {
                T[takenPiece] += glm::vec3(-1 * moveNumber, 0.0f, -1 * moveLetter);
            }
            std::cout << T[takenPiece].x << " " << T[takenPiece].z << std::endl;
        }
        //nowe pole figury
        pieces[number2][letter2] = pieceNumber;

        //oblicz ile klatek ma wykonac
        frames = std::max(PositiveLettersDiff, PositiveNumbersDiff) * 500000;
        moveLetter = (float)LettersDiff * 16 / (float)frames;
        moveNumber = (float)NumbersDiff * 16 / (float)frames;
        //wykonaj ruch
        for (int currFrame = 0; currFrame < frames; currFrame++)
        {
            T[pieceNumber] += glm::vec3(-1 * moveNumber, 0.0f, -1 * moveLetter);
        }
    }


}

bool isBishopMovePossible(int column1, int row1, int column2, int row2)
{
	if (abs(column1 - column2) == abs(row1 - row2)) return true;
	else return false;
}

bool isRookMovePossible(int column1, int row1, int column2, int row2)
{
	// Check if the squares are in the same row or column
	if ((column1 == column2) || (row1 == row2))return true;
	else return false;
}

bool isQueenMovePossible(int column1, int row1, int column2, int row2)
{
	if (isBishopMovePossible(column1, row1, column2, row2)
		|| isRookMovePossible(column1, row1, column2, row2)) return true;
	else return false;
}

bool isKingMovePossible(int column1, int row1, int column2, int row2)
{
	// Check if the squares are adjacent horizontally, vertically, or diagonally
	if ((abs(column1 - column2) <= 1) && (abs(row1 - row2) <= 1))return true;
	else return false;
}

bool isKnightMovePossible(int column1, int row1, int column2, int row2)
{
	// Check if the squares are a valid knight's move away
	int fileDiff = std::abs(column1 - column2);
	int rankDiff = std::abs(row1 - row2);
	if ((fileDiff == 1 && rankDiff == 2) || (fileDiff == 2 && rankDiff == 1))return true;
	else return false;
}

void findPiece(int k, bool (*pieceFunction)(int, int, int, int), std::vector<int>white, std::vector<int>black)
{
	if (moves[k].length() > 3 && moves[k][1] != 'x') //two pieces can go on that square
	{
		if (moves[k][4] == 'x' || moves[k].size() == 5) //we know both
		{
			c3 = int(moves[k][1] - 'a') + 1;
			c4 = int(moves[k][1] - '1') + 1;
		}
		if (int(moves[k][1]) >= 48 && int(moves[k][1] <= 57)) //we know the number
		{
			c4 = int(moves[k][1] - '1') + 1;
		}
		else if (int(moves[k][1]) >= 97 && int(moves[k][1] <= 122)) //we know the letter
		{
			c3 = int(moves[k][1] - 'a') + 1;
		}
	}
	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 9; j++)
		{
			if (pieceFunction(i, j, c1, c2))
			{
				if (c4 != 0)
				{
					//skip if wrong rank
					if (i != c4)continue;
				}
				if (c3 != 0)
				{
					//skip if wrong file
					if (j != c3)continue;
				}
				if (k % 2 == 0)//white piece
				{
					for (int q = 0; q < white.size(); q++)
					{
						if (pieces[i][j] == white[q])
						{
							number1.push_back(i);
							letter1.push_back(j);
						}
					}
				}
				else if (k % 2 == 1)//black piece
				{
					for (int q = 0; q < black.size(); q++)
					{
						if (pieces[i][j] == black[q])
						{
							number1.push_back(i);
							letter1.push_back(j);
						}
					}
				}
			}
		}
	}
}

void initBoard()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            pieces[i][j] = 0;
        }
    }
    for (int i = 0; i < 8; i++)
    {
        pieces[1][i + 1] = (8 * 0) + i + 1;
        pieces[2][i + 1] = (8 * 1) + i + 1;
        pieces[7][i + 1] = (8 * 2) + i + 1;
        pieces[8][i + 1] = (8 * 3) + i + 1;
    }
    //wyczysc liste figur
    whiteBishops.clear();
    whiteKnights.clear();
    whiteRooks.clear();
    whiteQueens.clear();

    blackBishops.clear();
    blackKnights.clear();
    blackRooks.clear();
    blackQueens.clear();

    //ustaw poczatkowe
    whiteBishops.push_back(3);
    whiteBishops.push_back(6);
    whiteKnights.push_back(2);
    whiteKnights.push_back(7);
    whiteRooks.push_back(1);
    whiteRooks.push_back(8);
    whiteQueens.push_back(4);

    blackBishops.push_back(27);
    blackBishops.push_back(30);
    blackKnights.push_back(26);
    blackKnights.push_back(31);
    blackRooks.push_back(25);
    blackRooks.push_back(32);
    blackQueens.push_back(28);
}

void readFile()
{
    file.open("PGN/example2.pgn");

    std::string input = "";
    std::string newmove;

    //read file and create vector moves
    while (!file.eof())
    {
        getline(file, input);

        int i = 0;
        while (i < input.length())
        {
            if ((int(input[i]) >= 65 && int(input[i]) <= 90) || (int(input[i]) >= 97 && int(input[i]) <= 122))
            {
                newmove = "";
                while (i < input.length() && input[i] != ' ')
                {
                    newmove += input[i];
                    i++;
                }
                moves.push_back(newmove);
                i++;
            }
            else i++;
        }
    }
}

void parse()
{
    for (int k = 0; k < moves.size(); k++)
    {
        if (moves[k] == "O-O")
        {
            //trash but necessary
            letter1.push_back(1000);
            number1.push_back(1000);
            letter2.push_back(1000);
            number2.push_back(1000);
            if (k % 2 == 0)
            {
                special.push_back(1);
                pieces[1][7] = pieces[1][5];
                pieces[1][6] = pieces[1][8];
                pieces[1][5] = 0;
                pieces[1][8] = 0;
            }
            if (k % 2 == 1)
            {
                special.push_back(2);
                pieces[8][7] = pieces[8][5];
                pieces[8][6] = pieces[8][8];
                pieces[8][5] = 0;
                pieces[8][8] = 0;
            }
        }
        else if (moves[k] == "O-O-O")
        {
            //trash but necessary
            letter1.push_back(1000);
            number1.push_back(1000);
            letter2.push_back(1000);
            number2.push_back(1000);
            if (k % 2 == 0)
            {
                special.push_back(3);
                pieces[1][3] = pieces[1][5];
                pieces[1][4] = pieces[1][1];
                pieces[1][5] = 0;
                pieces[1][1] = 0;
            }
            if (k % 2 == 1)
            {
                special.push_back(4);
                pieces[8][3] = pieces[8][5];
                pieces[8][4] = pieces[8][1];
                pieces[8][5] = 0;
                pieces[8][1] = 0;
            }
        }
        else
        {
            special.push_back(0);
            c3 = 0;
            c4 = 0;
            if (moves[k][moves[k].length() - 1] == '+' || moves[k][moves[k].length() - 1] == '#') moves[k].pop_back();
            //c2 -->kolumna docelowa
            //c1 -->wiersz docelowy
            c2 = int(moves[k][moves[k].length() - 2] - 'a') + 1;
            c1 = int(moves[k][moves[k].length() - 1] - '1') + 1;
            letter2.push_back(c2);
            number2.push_back(c1);
            if (moves[k][0] == 'K')
            {
                if (k % 2 == 0)
                {
                    for (int i = 1; i < 9; i++)
                    {
                        for (int j = 1; j < 9; j++)
                        {
                            if (pieces[i][j] == 5)//white king
                            {
                                number1.push_back(i);
                                letter1.push_back(j);
                            }
                        }
                    }
                }
                else if (k % 2 == 1)
                {
                    for (int i = 1; i < 9; i++)
                    {
                        for (int j = 1; j < 9; j++)
                        {
                            if (pieces[i][j] == 29)//black king
                            {
                                number1.push_back(i);
                                letter1.push_back(j);
                            }
                        }
                    }
                }
            }
            else if (moves[k][0] == 'Q')
            {
                findPiece(k, isQueenMovePossible, whiteQueens, blackQueens);
            }
            else if (moves[k][0] == 'R')
            {
                findPiece(k, isRookMovePossible, whiteRooks, blackRooks);
            }
            else if (moves[k][0] == 'B')
            {
                findPiece(k, isBishopMovePossible, whiteBishops, blackBishops);
            }
            else if (moves[k][0] == 'N')
            {
                findPiece(k, isKnightMovePossible, whiteKnights, blackKnights);
            }
            else
            {
                if (k % 2 == 0) //white pawn
                {
                    if (moves[k][1] == 'x') //takes
                    {
                        c3 = int(moves[k][0] - 'a') + 1;
                        c4 = int(moves[k][3] - '1');
                        letter1.push_back(c3);
                        number1.push_back(c4);
                    }
                    else //moves
                    {
                        if (c1 == 4 && pieces[c1 - 1][c2] != 0)//one-square move
                        {
                            letter1.push_back(c2);
                            number1.push_back(c1 - 1);
                        }
                        else if (c1 == 4)//two-square move
                        {
                            letter1.push_back(c2);
                            number1.push_back(c1 - 2);
                        }
                        else//normal move
                        {
                            letter1.push_back(c2);
                            number1.push_back(c1 - 1);
                        }
                    }
                }
                else //black pawn
                {
                    if (moves[k][1] == 'x') //takes
                    {
                        c3 = int(moves[k][0] - 'a') + 1;
                        c4 = int(moves[k][3] - '1') + 2;
                        letter1.push_back(c3);
                        number1.push_back(c4);
                    }
                    else //moves
                    {
                        if (c1 == 5 && pieces[c1 + 1][c2] != 0)//one-square move
                        {
                            letter1.push_back(c2);
                            number1.push_back(c1 + 1);
                        }
                        else if (c1 == 5) //two-square move
                        {
                            letter1.push_back(c2);
                            number1.push_back(c1 + 2);
                        }
                        else //normal move
                        {
                            letter1.push_back(c2);
                            number1.push_back(c1 + 1);
                        }
                    }
                }
            }
            if (moves[k].find('x') != std::string::npos)
            {
                pieces[c1][c2] = 0;
            }
            std::swap(pieces[c1][c2], pieces[number1[number1.size() - 1]][letter1[letter1.size() - 1]]);
        }
    }
    file.close();
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {

    float cameraScrollSpeed = 0.9f;
    glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPosition);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    //scrollem mozna przod-tyl
    if (yOffset > 0) {
        //przyblizam sie
        cameraPosition += cameraFront * cameraScrollSpeed;
        cameraTarget += cameraFront * cameraScrollSpeed;
    }
    else if (yOffset < 0) {
        //oddalam sie
        cameraPosition -= cameraFront * cameraScrollSpeed;
        cameraTarget -= cameraFront * cameraScrollSpeed;
    }
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    float cameraSpeed = 1.0f;
    float cameraRotationSpeed = 0.1f;
    float cameraVerticalSpeed = 1.0f;

    glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPosition);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    
    if (action==GLFW_PRESS || action==GLFW_REPEAT) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
        if (key == GLFW_KEY_SPACE) {
            if (count < letter1.size()) {
                nextmove(letter1[count], number1[count], letter2[count], number2[count], special[count]); //zrob nowy ruch
                count++;
            }
        }
        //WSAD-em lewo-prawo i gora dol
        if (key == GLFW_KEY_A) {
            //lewo
            cameraPosition -= cameraRight * cameraSpeed;
            cameraTarget -= cameraRight * cameraSpeed;
        }
        if (key == GLFW_KEY_D) {
            //prawo
            cameraPosition += cameraRight * cameraSpeed;
            cameraTarget += cameraRight * cameraSpeed;
        }
        if (key == GLFW_KEY_W) {
            //gora
            cameraPosition += cameraUp * cameraVerticalSpeed;
            cameraTarget += cameraUp * cameraVerticalSpeed;
        }
        if (key == GLFW_KEY_S) {
            //dol
            cameraPosition -= cameraUp * cameraVerticalSpeed;
            cameraTarget -= cameraUp * cameraVerticalSpeed;
        }
        //IJKL obracamy sie
        if (key == GLFW_KEY_I) {
            //glowa w gore
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), cameraRotationSpeed, cameraRight);
            cameraFront = glm::vec3(rotationMatrix * glm::vec4(cameraFront, 1.0f));
            cameraFront = glm::normalize(cameraFront);  // Normalize the cameraFront vector to avoid flipping
            cameraTarget = cameraPosition + cameraFront;
            cameraUp = glm::vec3(rotationMatrix * glm::vec4(cameraUp, 1.0f));
            cameraUp = glm::normalize(cameraUp);  // Normalize the cameraUp vector to avoid flipping
        }
        if (key == GLFW_KEY_K) {
            //glowa w dol
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), -cameraRotationSpeed, cameraRight);
            cameraFront = glm::vec3(rotationMatrix * glm::vec4(cameraFront, 1.0f));
            cameraFront = glm::normalize(cameraFront);  // Normalize the cameraFront vector to avoid flipping
            cameraTarget = cameraPosition + cameraFront;
            cameraUp = glm::vec3(rotationMatrix * glm::vec4(cameraUp, 1.0f));
            cameraUp = glm::normalize(cameraUp);  // Normalize the cameraUp vector to avoid flipping
        }
        if (key == GLFW_KEY_J) {
            //glowa w lewo
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), cameraRotationSpeed, cameraUp);
            cameraFront = glm::vec3(rotationMatrix * glm::vec4(cameraFront, 1.0f));
            cameraTarget = cameraPosition + cameraFront;
        }
        if (key == GLFW_KEY_L) {
            //glowa w prawo
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), -cameraRotationSpeed, cameraUp);
            cameraFront = glm::vec3(rotationMatrix * glm::vec4(cameraFront, 1.0f));
            cameraTarget = cameraPosition + cameraFront;
        }
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}


GLuint readTexture(const char* filename) {
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);

    //Wczytanie do pamięci komputera
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
    //Wczytaj obrazek
    unsigned error = lodepng::decode(image, width, height, filename);

    //Import do pamięci karty graficznej
    glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return tex;
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(1,1,1,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	tex0 = readTexture("Textures/boardtexture1.png");
	tex1 = readTexture("Textures/white.png");
	tex2 = readTexture("Textures/black1.png");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
}

void drawBoard(float angle_x, float angle_y) {
	glm::mat4 Board = glm::mat4(1.0f);
	Board = glm::rotate(Board, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
	Board = glm::rotate(Board, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu
	Board = glm::scale(Board, glm::vec3(300.0f, 300.0f, 300.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Board));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Wskaż tablicę z danymi dla atrybutu texCoord

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt
}

void drawPiece(glm::mat4 M, float vertices[], float normals[], float texcoords[], int vertexCount, GLuint tex, float angle_x, float angle_y) {
	
	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texcoords); //Wskaż tablicę z danymi dla atrybutu texCoord

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt
}

void initPieceVector(float angle_x, float angle_y) {
	
	M.push_back(glm::mat4(1.0f));

	for (int i = 1; i <= 32; i++)
	{
		M.push_back(glm::mat4(1.0f));
		M[i] = glm::scale(M[i], glm::vec3(0.5f, 0.5f, 0.5f));
	}

	T.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, 56.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, 40.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, 24.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, 8.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, -8.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, -24.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, -40.0f));
	T.push_back(glm::vec3(56.0f, 0.0f, -56.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, 56.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, 40.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, 24.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, 8.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, -8.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, -24.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, -40.0f));
	T.push_back(glm::vec3(40.0f, 0.0f, -56.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, 56.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, 40.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, 24.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, 8.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, -8.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, -24.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, -40.0f));
	T.push_back(glm::vec3(-40.0f, 0.0f, -56.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, 56.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, 40.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, 24.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, 8.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, -8.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, -24.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, -40.0f));
	T.push_back(glm::vec3(-56.0f, 0.0f, -56.0f));
}

void boardSetup(float angle_x, float angle_y) {

	for (int i = 1; i <= 32; i++)
	{
		M[i] = glm::mat4(1.0f);
		M[i] = glm::scale(M[i], glm::vec3(0.5f, 0.5f, 0.5f));
		M[i] = glm::rotate(M[i], angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
		M[i] = glm::rotate(M[i], angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu
		M[i] = glm::translate(M[i], T[i]);
	}

	drawPiece(M[1], RookVertices, RookVertexNormals, RookTex, RookVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[2], KnightVertices, KnightVertexNormals, KnightTex, KnightVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[3], BishopVertices, BishopVertexNormals, BishopTex, BishopVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[4], QueenVertices, QueenVertexNormals, QueenTex, QueenVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[5], KingVertices, KingVertexNormals, KingTex, KingVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[6], BishopVertices, BishopVertexNormals, BishopTex, BishopVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[7], KnightVertices, KnightVertexNormals, KnightTex, KnightVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[8], RookVertices, RookVertexNormals, RookTex, RookVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[9], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[10], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[11], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[12], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[13], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[14], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[15], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[16], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex1, angle_x, angle_y);
	drawPiece(M[17], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[18], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[19], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[20], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[21], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[22], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[23], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[24], PawnVertices, PawnVertexNormals, PawnTex, PawnVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[25], RookVertices, RookVertexNormals, RookTex, RookVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[26], KnightVertices, KnightVertexNormals, KnightTex, KnightVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[27], BishopVertices, BishopVertexNormals, BishopTex, BishopVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[28], QueenVertices, QueenVertexNormals, QueenTex, QueenVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[29], KingVertices, KingVertexNormals, KingTex, KingVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[30], BishopVertices, BishopVertexNormals, BishopTex, BishopVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[31], KnightVertices, KnightVertexNormals, KnightTex, KnightVertexCount, tex2, angle_x, angle_y);
	drawPiece(M[32], RookVertices, RookVertexNormals, RookTex, RookVertexCount, tex2, angle_x, angle_y);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 600.0f); //Wylicz macierz rzutowania

	sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord

	drawBoard(angle_x, angle_y); //funkcja rysująca szachownicę
	boardSetup(angle_x, angle_y); //funkcja ustawiająca figury na szachownicy

    glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1980, 1080, "3D CHESS", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

    initBoard();
    readFile();
    parse();
    initBoard();
                               
    //Główna pętla
	float angle_x=0; //Aktualny kąt obrotu obiektu
	float angle_y=0; //Aktualny kąt obrotu obiektu
	initPieceVector(angle_x, angle_y);
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
        angle_x+=speed_x*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        angle_y+=speed_y*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
