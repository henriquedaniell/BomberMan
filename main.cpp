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
#include <cstdlib>

using namespace std;

// Variavel que diz se uma bomba foi colocada, e outra para o timer da explosao
bool bombPlaced = false, isExploding = false;
// Variavel que seta se o player esta vivo

int escolha = 0;
int pontos = 0;

struct bomba {
    int x, y, texture = 0;
};

struct Inimigo {
    int x, y, direction=0;
    bool alive = true;
};

struct Personagem {
    int x=1, y=1;
    bool alive = true;
};

void cruzBomba(int &posMatriz) {
	if (posMatriz == 0 || posMatriz == 2 || posMatriz == 5)
		posMatriz = 4;
}

string corBomba(int segundosRestantes) {
    switch(segundosRestantes) {
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

    while(escolha!=1){
        cout << "\033[2J\033[H";

        cout << " ||===\\\\  ||==== ||\\   /||      \\\\      // || ||\\   || ||==\\\\   //==\\\\      " << endl;
        cout << " ||   //  ||     ||\\\\ //||       \\\\    //  || ||\\\\  || ||   \\\\ ||    ||       " << endl;
        cout << " ||===|   ||==   || \\// || ====   \\\\  //   || || \\\\ || ||   || ||    ||          " << endl;
        cout << " ||   \\\\  ||     ||     ||         \\\\//    || ||  \\\\|| ||   // ||    ||         " << endl;
        cout << " ||===//  ||==== ||     ||          \\/     || ||   \\|| ||==//   \\\\==//            " << endl;

        this_thread::sleep_for(std::chrono::seconds(2));
        cout << "\033[2J\033[H";

        cout << "         BOMBERMAN          " << endl;
        cout << " ========================== " << endl;
        cout << "||                        ||" << endl;
        cout << "||       1- Jogar         ||" << endl;
        cout << "||       2- Sair          ||" << endl;
        cout << "||                        ||" << endl;
        cout << " ========================== " << endl;
        cout << "     ESCOLHA UMA OPCAO: ";
        cin >> escolha;
        if(escolha==2)
            return 0;
    }

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
    inimigos[1].y=9;

    inimigos[2].x=11;
    inimigos[2].y=17;

    inimigos[3].x=1;
    inimigos[3].y=17;

    // Inicialização da struct Personagem (player)
    Personagem player;

    // Inicialização da variável que armazena o timer do movimento dos inimigos.
    auto tempoInimigos = chrono::steady_clock::now();

    // Inicialização da struct bomba (b1)
    bomba b1;

    // Inicialização variavel que armazena o timer da bomba
    auto tempoBomba = chrono::steady_clock::now();

	// Inicialização variavel que armazena o timer da explosao
    auto tempoExplosao = chrono::steady_clock::now();

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

    //Começa o jogo
    while(player.alive){
        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<13;i++){
            for(int j=0;j<19;j++){
                if(i==player.x && j==player.y){
                    cout<< "🤠 "; //personagem
                } else {
                    switch (m[i][j]){
                        case 0: cout<<"   "; break; //caminho
                        case 1: cout << "\033[90m" << "███" << "\033[0m"; break;
                        case 2: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break;
                        case 3: cout << corBomba(b1.texture) << "💣ʔ" << "\033[0m"; break;
                        case 4:
							if (isExploding)
								cout << "\033[33m" << "💥 " << "\033[0m";
							else {
								m[i][j] = 0;
								cout<<"   ";
							}
						break;

                        case 5: cout << "👾 "; break;
                        //default: cout<<"-"; //erro
                    } //fim switch
                }
            }
            cout<<"\n";
        } //fim for mapa
        cout << endl;
        cout << "    Cima ⬆/W | Direita ➡ /D | Baixo ⬇/S | Esquerda ⬅/A" << endl;
        cout << "                   Colocar Bomba - F" << endl;
        cout << "                   PONTOS: " << pontos << endl;

        ///executa os movimentos
        if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    if (m[player.x-1][player.y] == 0 || m[player.x-1][player.y] == 3)
                        player.x--;
                    else if(m[player.x-1][player.y] == 5 || m[player.x-1][player.y] == 4)
                        player.alive = false;
                break;
                case 80: case 's': ///baixo
                    if (m[player.x+1][player.y] == 0 || m[player.x+1][player.y] == 3)
                        player.x++;
                    else if(m[player.x+1][player.y] == 5 || m[player.x+1][player.y] == 4)
                        player.alive = false;
                break;
                case 75:case 'a': ///esquerda
                    if (m[player.x][player.y-1] == 0 || m[player.x][player.y-1] == 3)
                        player.y--;
                    else if(m[player.x][player.y-1] == 5 || m[player.x][player.y-1] == 4)
                        player.alive = false;
                break;
                case 77: case 'd': ///direita
                    if (m[player.x][player.y+1] == 0 || m[player.x][player.y+1] == 3)
                        player.y++;
                    else if(m[player.x][player.y+1] == 5 || m[player.x][player.y+1] == 4)
                        player.alive = false;
                break;
                case 'f':
                    // Se não há bomba colocada, o F posiciona uma bomba na posição atual do jogador
                    if (!bombPlaced) {
                        m[player.x][player.y] = 3;

                        b1.x = player.x;
                        b1.y = player.y;

                        bombPlaced = true;

                        tempoBomba = chrono::steady_clock::now(); // define o tempo em que a bomba foi posicionada
                    }
                break;
            }

        }


        auto agoraInimigos = chrono::steady_clock::now();
        auto tempoPassadoInimigos = chrono::duration_cast<chrono::milliseconds>(
            agoraInimigos - tempoInimigos
        ).count();

        // Movimento dos inimigos
        if(tempoPassadoInimigos >= 1000){ // If de confirmação se passou um segundo
            for(int i=0; i<4; i++){

                if (m[inimigos[i].x][inimigos[i].y] == 4){
                    inimigos[i].alive = false;
                    m[inimigos[i].x][inimigos[i].y] = 0;
                    inimigos[i].x = 0;
                    inimigos[i].y = 0;
                    pontos+=250;
                }

                bool mexeu = false;
                int tentativas = 0;
                int velhoX = inimigos[i].x; // Guarda a posição antiga do inimigo.
                int velhoY = inimigos[i].y;

                while(!mexeu && tentativas < 10 && inimigos[i].alive){
                    inimigos[i].direction = ranDir(gen);
                    switch(inimigos[i].direction){
                        case 0:
                            if (m[inimigos[i].x-1][inimigos[i].y] == 0){ // Para cima
                                inimigos[i].x-=1;
                                mexeu = true;
                            } else if(m[player.x-1][player.y] == 5)
                                player.alive = false;
                            else if(m[player.x-1][player.y] == 4){
                                inimigos[i].alive = false;
                                m[inimigos[i].x][inimigos[i].y] = 0;
                                inimigos[i].x = 0;
                                inimigos[i].y = 0;
                                pontos+=250;
                            }
                            break;
                        case 1:
                            if (m[inimigos[i].x+1][inimigos[i].y] == 0){ // Para baixo
                                inimigos[i].x+=1;
                                mexeu = true;
                            } else if(m[player.x+1][player.y] == 5)
                                player.alive = false;
                            else if(m[player.x+1][player.y] == 4){
                                inimigos[i].alive = false;
                                m[inimigos[i].x][inimigos[i].y] = 0;
                                inimigos[i].x = 0;
                                inimigos[i].y = 0;
                                pontos+=250;
                            }
                            break;
                        case 2:
                            if (m[inimigos[i].x][inimigos[i].y-1] == 0){ // Para esquerda
                                inimigos[i].y-=1;
                                mexeu = true;
                            } else if(m[player.x][player.y-1] == 5)
                                player.alive = false;
                            else if(m[player.x][player.y-1] == 4){
                                inimigos[i].alive = false;
                                m[inimigos[i].x][inimigos[i].y] = 0;
                                inimigos[i].x = 0;
                                inimigos[i].y = 0;
                                pontos+=250;
                            }
                            break;
                        case 3:
                            if (m[inimigos[i].x][inimigos[i].y+1] == 0){ // Para direita
                                inimigos[i].y+=1;
                                mexeu = true;
                            } else if(m[player.x][player.y+1] == 5)
                                player.alive = false;
                            else if(m[player.x][player.y+1] == 4){
                                inimigos[i].alive = false;
                                m[inimigos[i].x][inimigos[i].y] = 0;
                                inimigos[i].x = 0;
                                inimigos[i].y = 0;
                                pontos+=250;
                            }
                            break;
                    }
                    tentativas++;
                }
                if (mexeu) {
                    m[velhoX][velhoY] = 0;                     // Excluí o desenho do inimigo que estava na posição anterior.
                    m[inimigos[i].x][inimigos[i].y] = 5;       // Desenha o inimigo na posição nova.
                }
            }
            tempoInimigos = agoraInimigos; // Reinicia o cronômetro
        }

         // timer da bomba depois de posicionada:
         if (bombPlaced) {
            auto agoraBomba = chrono::steady_clock::now(); // define o tempo de AGORA

            auto tempoPassadoBomba = chrono::duration_cast<chrono::seconds>(
                agoraBomba - tempoBomba
            ).count(); // conta quanto tempo já passou desde o posicionamento da bomba em SEGUNDOS, de acordo com o "AGORA"

            b1.texture = 3 - tempoPassadoBomba; // atualiza a textura da bomba

            if (tempoPassadoBomba >= 3) { // se passarem os 3 segundos do timer:
                // EXPLODE
                m[b1.x][b1.y] = 4;
				isExploding = true;
				cruzBomba(m[b1.x - 1][b1.y]);
				cruzBomba(m[b1.x + 1][b1.y]);
				cruzBomba(m[b1.x][b1.y - 1]);
				cruzBomba(m[b1.x][b1.y + 1]);

                bombPlaced = false;

				tempoExplosao = chrono::steady_clock::now(); // define o tempo em que a bomba explodiu
                cout << "\a"; // som de EXPLOSAO
            }
        }

		if (isExploding) {
			if (m[player.x][player.y] == 4)
				player.alive = false;
			auto agora = chrono::steady_clock::now(); // define o tempo de AGORA

            auto tempoPassado = chrono::duration_cast<chrono::seconds>(
                agora - tempoExplosao
            ).count(); // conta quanto tempo já passou desde a explosao da bomba em SEGUNDOS, de acordo com o "AGORA"


            if (tempoPassado >= 1) { // depois de 1seg de timer:
				isExploding = false; // a explosao acaba
			}
		}

        if(player.alive == false)
            cout << "Você morreu! Aperte QUALQUER BOTÃO para continuar.";
        else if(pontos == 1000)
            cout << "Parabéns! Você matou todos os INIMIGOS! 🏅";


    } //fim do laco do jogo



    return 0;
} //fim main
