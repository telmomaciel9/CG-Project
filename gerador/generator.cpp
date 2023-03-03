#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "Forma.h"
#include "Ponto.h"
using namespace std;

// A coordenada y é sempre 0
// O plano tem que estar centrado na origem
void geraPlano(float len, float div, char *file){
    Forma* forma = new Forma();

    float ref = len / 2;
    float x = -ref, y = 0, z = -ref;
    float ladoQua = len/div;

    for (; z < ref; z += ladoQua)
    {
        x = -ref;
        for (; x < ref; x += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x + ladoQua, y, z + ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
        }
    }

    forma->escreveFicheiro(file);
}

// Um cubo é composto por 6 faces
void geraCubo(float len, float div, char *file){
    Forma* forma = new Forma();

    float ref = len / 2;
    float x = -ref, y = -ref, z = -ref;
    float ladoQua = len/div;

    for (; z < ref; z += ladoQua)
    {
        x = -ref;
        for (; x < ref; x += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
            forma->adicionarPonto(Ponto(x + ladoQua, y, z + ladoQua));
        }
    }

    x = -ref;
    y = ref;
    z = -ref;

    for (; z < ref; z += ladoQua)
    {
        x = -ref;
        for (; x < ref; x += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x + ladoQua, y, z + ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
        }
    }

    x = -ref;
    y = -ref;
    z = -ref;

    for (; z < ref; z += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z + ladoQua));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));
        }
    }

    x = ref;
    y = -ref;
    z = -ref;

    for (; z < ref; z += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z+ladoQua));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z + ladoQua));
        }
    }

    x = -ref;
    y = -ref;
    z = -ref;

    for (; x < ref; x += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
        }
    }

    x = -ref;
    y = -ref;
    z = ref;

    for (; x < ref; x += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));
        }
    }


    forma->escreveFicheiro(file);
}

// A base do cone tem que estar contida no plano XZ
void geraCone(float radius, float height, int slices, int stacks, char * file){
    Forma* forma = new Forma();

    float alfa = 2 * M_PI/slices;
	float alfainc = alfa;
    float alt = height / stacks;
    float altinc = alt;
    float raio = radius / stacks;
    float r = radius - raio;

    for(int i = 0; i < slices; i++){
        // Base
        forma->adicionarPonto(Ponto(0,0,0));
        forma->adicionarPonto(Ponto(radius * sin(alfainc-alfa),0,radius * cos(alfainc-alfa)));
        forma->adicionarPonto(Ponto(radius * sin(alfainc),0, radius * cos(alfainc)));

        // Lados
        for(int j = 0; j < stacks; j++){
            forma->adicionarPonto(Ponto((r + raio) * sin(alfainc-alfa),altinc-alt,(r + raio) * cos(alfainc-alfa)));
            forma->adicionarPonto(Ponto(r * sin(alfainc),altinc,r * cos(alfainc)));
            forma->adicionarPonto(Ponto(r * sin(alfainc-alfa),altinc,r * cos(alfainc-alfa)));
            
            forma->adicionarPonto(Ponto((r + raio) * sin(alfainc-alfa),altinc-alt,(r + raio) * cos(alfainc-alfa)));
            forma->adicionarPonto(Ponto((r + raio) * sin(alfainc),altinc-alt,(r + raio) * cos(alfainc)));
            forma->adicionarPonto(Ponto(r * sin(alfainc),altinc,r * cos(alfainc)));

            altinc += alt;
            r -= raio;
        }

        alfainc += alfa;
        altinc = alt;
        r = radius - raio;
    }

    forma->escreveFicheiro(file);
}


int main(int argc, char *argv[]){

    if(strcmp(argv[1],"plane") == 0){
        geraPlano(stof(argv[2]), stof(argv[3]), argv[4]);
    }else if(strcmp(argv[1],"box") == 0){
        geraCubo(stof(argv[2]),stof(argv[3]),argv[4]);
    }else if(strcmp(argv[1],"cone") == 0){
        geraCone(stof(argv[2]),stof(argv[3]),stoi(argv[4]),stoi(argv[5]),argv[6]);
    }

    return 1;
}