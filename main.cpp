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

// Variavel que diz se uma bomba foi colocada
bool bombPlaced = false;

struct bomba {
    int x, y, texture = 0;
};

struct Inimigo {
    int x, y, direction=0;
};

struct Personagem {
    int x=1, y=1;
};

string corBomba(int segundosRestantes)
{
    switch(segundosRestantes)
    {
        case 3: return "\033[38;5;226m";
        case 2: return "\033[38;5;208m";
        case 1: return "\033[38;5;196m";
    }

    return "\033[0m";
}

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
    uniform_int_distribution<> ranDir(0, 3);

    int m[13][19]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                    1,0,0,0,2,0,2,2,2,2,2,0,2,2,2,2,0,5,1,
                    1,0,1,2,1,2,1,0,1,0,1,2,1,0,1,2,1,0,1,
                    1,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,1,
                    1,0,1,2,1,0,1,2,1,2,1,2,1,0,1,0,1,2,1,
                    1,2,2,0,2,2,2,2,2,0,2,0,2,2,0,2,2,0,1,
                    1,0,1,2,1,2,1,0,1,5,1,2,1,0,1,2,1,2,1,
                    1,2,2,2,2,0,2,0,2,0,2,2,0,2,2,2,2,2,1,
                    1,0,1,2,1,2,1,2,1,2,1,2,1,0,1,0,1,2,1,
                    1,2,2,0,2,2,0,2,0,2,2,0,2,2,0,0,2,2,1,
                    1,0,1,2,1,0,1,0,1,2,1,2,1,2,1,2,1,0,1,
                    1,5,0,2,0,2,2,2,2,0,2,2,2,2,2,2,0,5,1,
                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    Inimigo inimigos[4];
    inimigos[0].x=11;
    inimigos[0].y=1;

    inimigos[1].x=6;
    inimigos[1].y=6;

    inimigos[2].x=17;
    inimigos[2].y=17;

    inimigos[3].x=1;
    inimigos[3].y=17;

    for(int i=0; i<4; i++){
        while(true){
            inimigos[i].direction = ranDir(gen);
            switch(inimigos[i].direction):
            case 1:
                if(inimigos[i].x+1==0){
                    inimigos[i].x+=1;
                } else if(inimigos[i].x+1==)
        }
    }

    // Inicialização da struct bomba (b1)
    bomba b1;

    // Inicialização variavel que armazena o timer da bomba
    auto tempoBomba = chrono::steady_clock::now();

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

    //Variavel para tecla precionada
    char tecla;

    while(true){
        ///Posiciona a escrita no iicio do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<13;i++){
            for(int j=0;j<19;j++){
                if(i==Personagem.x && j==Personagem.y){
                    cout<< "🤠 "; //personagem
                } else {
                    switch (m[i][j]){
                        case 0: cout<<"   "; break; //caminho
                        case 1: cout << "\033[90m" << "███" << "\033[0m"; break;
                        case 2: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break;
                        case 3: cout << corBomba(b1.texture) << " ● " << "\033[0m"; break;

                        case 5: cout << "👾 " << break;
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
                    if (m[Personagem.x-1][Personagem.y] == 0)
                        x--;
                break;
                case 80: case 's': ///baixo
                    if (m[Personagem.x+1][Personagem.y] == 0)
                        x++;
                break;
                case 75:case 'a': ///esquerda
                    if (m[Personagem.x][Personagem.y-1] == 0)
                        y--;
                break;
                case 77: case 'd': ///direita
                    if (m[Personagem.x][Personagem.y+1] == 0)
                        y++;
                break;
                case 'f':
                    // Se não há bomba colocada, o F posiciona uma bomba na posição atual do jogador
                    if (!bombPlaced) {
                        m[Personagem.x][Personagem.y] = 3;

                        b1.x = Personagem.x;
                        b1.y = Personagem.y;

                        bombPlaced = true;

                        tempoBomba = chrono::steady_clock::now(); // define o tempo em que a bomba foi posicionada
                    }
                break;
            }
         }

         // timer da bomba depois de posicionada:
         if (bombPlaced) {
            auto agora = chrono::steady_clock::now(); // define o tempo de AGORA

            auto tempoPassado = chrono::duration_cast<chrono::seconds>(
                agora - tempoBomba
            ).count(); // conta quanto tempo já passou desde o posicionamento da bomba em SEGUNDOS, de acordo com o "AGORA"

            b1.texture = 3 - tempoPassado; // atualiza a textura da bomba

            if (tempoPassado >= 3) { // se passarem os 3 segundos do timer:
                // EXPLODE
                m[b1.x][b1.y] = 0;

                bombPlaced = false;

                cout << "\a"; // som de EXPLOSAO
            }
        }


    } //fim do laco do jogo

    return 0;
} //fim main
