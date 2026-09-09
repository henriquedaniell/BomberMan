/**
    Estrutura inicial para um jogo
    versão: 0.1 (Prof. Alex,  Adaptado Prof. Felski)
*/
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <random>

using namespace std;


int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    ///TRECHO PARA ACEITAR CÓDIGOS DE CORES ANSI E CARACTERES UNICODE
        // 1. Configura o terminal para aceitar caracteres Unicode (como a bola '●')
        SetConsoleOutputCP(CP_UTF8);

        // 2. Configura o terminal para aceitar códigos de cores ANSI
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    ///FIM DO TRECHO

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 10);

    int m[13][19]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                    1,0,0,0,2,0,2,2,2,2,2,0,2,2,2,2,0,0,1,
                    1,0,1,2,1,2,1,0,1,0,1,2,1,0,1,2,1,0,1,
                    1,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,1,
                    1,0,1,2,1,0,1,2,1,2,1,2,1,0,1,0,1,2,1,
                    1,2,2,0,2,2,2,2,2,0,2,0,2,2,0,2,2,0,1,
                    1,0,1,2,1,2,1,0,1,0,1,2,1,0,1,2,1,2,1,
                    1,2,2,2,2,0,2,0,2,0,2,2,0,2,2,2,2,2,1,
                    1,0,1,2,1,2,1,2,1,2,1,2,1,0,1,0,1,2,1,
                    1,2,2,0,2,2,0,2,0,2,2,0,2,2,0,0,2,2,1,
                    1,0,1,2,1,0,1,0,1,2,1,2,1,2,1,2,1,0,1,
                    1,0,0,2,0,2,2,2,2,0,2,2,2,2,2,2,0,0,1,
                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    int sorteioParede = 0;
    for(int i=0; i<13; i++){
        for(int j=0; j<19; j++){
            if(m[i][j]==2){
                sorteioParede = distrib(gen);
                if(sorteioParede>6){
                    m[i][j]=0;
                }
            }
        }
    }
    //Posicao inicial do personagem no console
    int x=1, y=1;
    //Variavel para tecla precionada
    char tecla;

    while(true){
        ///Posiciona a escrita no iicio do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<13;i++){
            for(int j=0;j<19;j++){
                if(i==x && j==y){
                    cout<< "🤠 "; //personagem
                } else {
                    switch (m[i][j]){
                        case 0: cout<<"   "; break; //caminho
                        case 1: cout << "\033[90m" << "███" << "\033[0m"; break;
                        case 2: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break;
                        case 3: cout << "\033[31m" << " ● " << "\033[0m"; break;
                        //default: cout<<"-"; //erro
                    } //fim switch
                }
            }
            cout<<"\n";
        } //fim for mapa

        ///executa os movimentos
         if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    if (m[x-1][y] == 0)
                        x--;
                break;
                case 80: case 's': ///baixo
                    if (m[x+1][y] == 0)
                        x++;
                break;
                case 75:case 'a': ///esquerda
                    if (m[x][y-1] == 0)
                        y--;
                break;
                case 77: case 'd': ///direita
                    if (m[x][y+1] == 0)
                        y++;
                break;
                case 'f': ///Colocar bomba
                    m[x][y]=3;
                break;
            }
         }


    } //fim do laco do jogo

    return 0;
} //fim main
