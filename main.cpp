/**
    Estrutura inicial para um jogo
    versão: 0.1 (Prof. Alex,  Adaptado Prof. Felski)
*/
/// BomberMan (Alunos: Henrique Daniel e Eduardo Ramos
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> ranDir(0, 3); // Sorteador da direção que o personagem vai
uniform_int_distribution<> ranX(1, 11); // Sorteador da casa X que o inimigo vai spawnar
uniform_int_distribution<> ranY(1, 17); // Sorteador da casa Y que o inimigo vai spawnar

// Variavel que diz se uma bomba foi colocada, e outra para o timer da explosao
bool bombPlaced = false, isExploding = false;


// ========================================VARIAVEIS GLOBAIS=================================================

// Variavel para o timer da explosao
bool isExploding = false;

int menuChoice = 0; // menu
int score = 0; // pontuação do jogo
bool alreadyPlayed = false; // controla o 'Jogar Novamente'

struct bomb {
    int x, y, texture = 0;
};

struct Enemy {
    int x, y, direction=0;
    bool alive = true;
    chrono::steady_clock::time_point deathTimer = chrono::steady_clock::now();
};

struct Character {
    int x=1, y=1;
    bool alive = true;
};

void bombCross(int &gridPos) { //Função da área de explosão da bomba


// ========================================INICIO DAS STRUCTS===============================================

// STRUCT DE BOMBAS
struct Bomb {
    int x, y, texture = 0;
};


// STRUCT DE PERSONAGEM JOGADOR
struct Character {
    int x=1, y=1;
    bool alive = true, bombsPlaced = false;

    void movement(int gameMap[13][19], int newX = 0, int newY = 0) {
        if (gameMap[x + newX][y + newY] == 0) {
            x += newX;           // Move o player caso haja caminho livre
            y += newY;
        }
        else if(gameMap[x + newX][y + newY] == 5 || gameMap[x + newX][y + newY] == 4) {
            alive = false; // Player morre se der de cara com um inimigo ou com uma casa de explosão da bomba
        }
    }
};


// STRUCT DOS INIMIGOS
struct Enemy {
    int x, y, direction=0;
    bool alive = true, deathAnimation = false;
    chrono::steady_clock::time_point deathTimer = chrono::steady_clock::now();

	void movement(int (&gameMap)[13][19], Character &player, bool &hasMoved, int newX = 0, int newY = 0) {
		if (x + newX == player.x && y + newY == player.y) 	// Mata o player se o inimigo encostar nele
		    player.alive = false;
		else if (gameMap[x + newX][y + newY] == 0){ 				// Move o inimigo para o novo local caso seja caminho livre
		    x += newX;
			y += newY;
		    hasMoved = true;
		} else if(gameMap[x + newX][y + newY] == 4){ // Se for explosão
		    gameMap[x][y] = 0;    // Limpa a posição antiga (sem caveira, pois o inimigo nem chegou lá)
		    x += newX;                     // Inimigo anda
			y += newY;
		    death(gameMap[x][y]);
		}
	}

	void death (int &gridPos) {
		alive = false;
		deathTimer = chrono::steady_clock::now();
		score += 250;
		gridPos = 6;
	}
};
// ========================================FIM DAS STRUCTS===============================================



// ========================================INICIO DAS FUNÇÕES===============================================

// Função da área de explosão da bomba
void bombCross(int &gridPos) {
	if (gridPos == 0 || gridPos == 2 || gridPos == 5)
		gridPos = 4;
}


// Função para pintar o pavio da bomba
string bombColor(int secondsRemaining) {
    switch(secondsRemaining) {
        case 3: return "\033[38;5;226m";
        case 2: return "\033[38;5;208m";
        case 1: return "\033[38;5;196m";
    }

    return "\033[0m";
}

void killEnemy(Enemy &enemy, int &score, int &gridPos) {
    enemy.alive = false;
    enemy.deathTimer = chrono::steady_clock::now();
    score += 250;
    gridPos = 6;
}

void enemiesSpawn (int enemiesAmount, Enemy &enemy, Character player, int (&mapGrid)[13][19] ){
    bool enemyInPosition = false;
    while (enemyInPosition == false){
        int randomX = ranX(gen);
        int randomY = ranY(gen);

        if((abs(randomX - player.x) <= 4 || abs(randomY - player.y) <=4) || mapGrid[randomX][randomY]!=0)
            continue;
        else {
            enemy.x = randomX;
            enemy.y = randomY;
            mapGrid[randomX][randomY] = 5;

            if(mapGrid[randomX+1][randomY] != 1 && mapGrid[randomX+1][randomY] != 5)
                mapGrid[randomX+1][randomY] = 0;
            if(mapGrid[randomX-1][randomY] != 1 && mapGrid[randomX-1][randomY] != 5)
                mapGrid[randomX-1][randomY] = 0;
            if(mapGrid[randomX][randomY+1] != 1 && mapGrid[randomX][randomY+1] != 5)
                mapGrid[randomX][randomY+1] = 0;
            if(mapGrid[randomX][randomY-1] != 1 && mapGrid[randomX][randomY-1] != 5)
                mapGrid[randomX][randomY-1] = 0;
        }
        enemyInPosition = true;
    }
}

    int main()


// Função de timer global
int globalTimer(std::chrono::steady_clock::time_point startTime, float timerDuration, bool &timerTrigger, bool needTexture = false) {
	auto nowTime = chrono::steady_clock::now(); // define o tempo de AGORA

	// Define a duração diretamente como float em segundos
    chrono::duration<float> elapsedTimeDuration = nowTime - startTime;
    auto elapsedTime = elapsedTimeDuration.count(); // RETORNA FLOAT


	if (elapsedTime >= timerDuration) {		// ativa o gatilho do final do timer, e o desliga
	    timerTrigger = false;
	}

	if (needTexture)
        return timerDuration - elapsedTime + 1;
    else
        return 0;
}

void drawMap(int (&gameMap)[13][19], Character player, Bomb bomb, bool explosionVerifier){
	///Imprime o jogo: mapa, personagem e inimigos.
	for(int i=0;i<13;i++){
	    for(int j=0;j<19;j++){
	        if(i==player.x && j==player.y){
	            cout<< "🤠 "; //Personagem
	        } else {
	            switch (gameMap[i][j]){
	                case 0: cout<<"   "; break; //Caminho
	                case 1: cout << "\033[90m" << "███" << "\033[0m"; break; // Parede INDESTRUTÍVEL
	                case 2: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break; // Parede DESTRUTÍVEL
	                case 3: cout << bombColor(bomb.texture) << "💣ʔ" << "\033[0m"; break; // Bomba
	                case 4:
	                    if (explosionVerifier)
	                        cout << "\033[33m" << "💥 " << "\033[0m"; // Explosão
	                    else {
	                        gameMap[i][j] = 0; // Acabou a explosão, volta a ser caminho
	                        cout<<"   ";
	                    }
	                break;

	                case 5: cout << "👾 "; break; // Inimigo
	                case 6: cout << "💀 "; break; // Morte do inimigo
	                //default: cout<<"-"; //erro
	            } //Fim switch
	        }
	    }
	    cout<<"\n";
	} //Fim for mapa
}
// ========================================FIM DAS FUNÇÕES=================================================


// ===============================================INICIO DO MAIN=================================================


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

    ///TRECHO PARA ACEITAR CÓDIGOS DE CORES ANSI E CARACTERES UTF-8 (EMOJIS)
        // 1. Configura o terminal para aceitar caracteres Unicode (como a bola '●')
        SetConsoleOutputCP(CP_UTF8);

        // 2. Configura o terminal para aceitar códigos de cores ANSI
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    ///FIM DO TRECHO

    cout << "\033[2J\033[H" << endl;
    cout << endl;
    cout << " ||===\\\\  ||==== ||\\   /||      \\\\      // || ||\\   || ||==\\\\   //==\\\\      " << endl;
    cout << " ||   //  ||     ||\\\\ //||       \\\\    //  || ||\\\\  || ||   \\\\ ||    ||       " << endl;
    cout << " ||===|   ||==   || \\// || ====   \\\\  //   || || \\\\ || ||   || ||    ||          " << endl;
    cout << " ||   \\\\  ||     ||     ||         \\\\//    || ||  \\\\|| ||   // ||    ||         " << endl;
    cout << " ||===//  ||==== ||     ||          \\/     || ||   \\|| ||==//   \\\\==//            " << endl;

    this_thread::sleep_for(chrono::seconds(2)); // Pausa de 2 segundos para então mostrar o menu


	// Inicialização da struct Character (player)
    Character player;


    while(menuChoice!=2){
        cout << "\033[2J\033[H"; // Apaga tudo que está no console e move o cursor para o topo
        cout << endl;
        cout << "         BOMBERMAN          " << endl;
        cout << " ========================== " << endl;
        cout << "||                        ||" << endl;

        if(alreadyPlayed == false){
            cout << "||       1- Jogar         ||" << endl;
        } else {
            cout << "||   1- Jogar Novamente   ||" << endl;
        }

        cout << "||       2- Sair          ||" << endl;
        cout << "||                        ||" << endl;
        cout << " ========================== " << endl;
        cout << "     ESCOLHA UMA OPCAO: ";
        cin >> menuChoice;

        if(menuChoice!=1) //Enquanto não escolher 1, fica pedindo para escolher uma opção, caso escolha 2, irá encerrar.
            continue;

        player.x = 1, player.y = 1, player.alive = true, player.bombsPlaced = false;
        score = 0;
        isExploding = false;
        alreadyPlayed = true; // Marca que entrou no jogo uma vez

        ///Mapa do Jogo: 0- Caminho livre    1- Parede Indestrutível  2- Parede destrutível   3- Bomba   4- Explosão   5- Inimigo   6- Inimigo morto
        int mapGrid[13][19]=  { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
                                1,0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
                                1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
                                1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
                                1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
                                1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
                                1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
                                1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
                                1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
                                1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
                                1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

        // Inicialização da variável que armazena o timer do movimento dos inimigos.
        auto enemiesMoveTimer = chrono::steady_clock::now();

        // Inicialização da struct bomba (b1)
        Bomb b1;

        // Inicialização variavel que armazena o timer da bomba
        auto bombTimer = chrono::steady_clock::now();

        // Inicialização variavel que armazena o timer da explosao
        auto explosionTimer = chrono::steady_clock::now();

        ///Sorteio das paredes que serão destrutíveis
        int randomWallGen = 0;
        for(int i=0; i<13; i++){
            for(int j=0; j<19; j++){
                if(mapGrid[i][j]==2){
                    randomWallGen = distrib(gen);
                    if(randomWallGen>6){
                        mapGrid[i][j]=0;
                    }
                }
            }
        }
              
        ///Inicialização dos inimigos e suas respectivas posições
        Enemy enemies[enemiesAmount];
        for(int i=0; i<enemiesAmount; i++){
            enemiesSpawn(enemiesAmount, enemies[i], player, mapGrid);
        }

        //Variavel para tecla pressionada
        char keyPressed;

        //Começa o jogo de verdade
        while(player.alive && score<1000){ //Condição de VITÓRIA: 1000 PONTOS. Condição de DERROTA: MORRER
            ///Posiciona a escrita no início do console
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

            // Imprime o jogo: mapa, personagem e inimigos.
            drawMap(mapGrid, player, b1, isExploding);


            cout << endl;
            cout << "    Cima ⬆/W | Direita ➡ /D | Baixo ⬇/S | Esquerda ⬅/A" << endl; // Tutorial
            cout << "                   Colocar Bomba - F" << endl;
            cout << "                   PONTOS: " << score << endl; // Pontuação

            ///Executa os movimentos
            if ( _kbhit() ){
                keyPressed = getch();
                switch(keyPressed)
                {
                    case 72: case 'w': /// Cima
                        player.movement(mapGrid, -1);
                    break;
                    case 80: case 's': /// Baixo
                        player.movement(mapGrid, 1);
                    break;
                    case 75:case 'a': /// Esquerda
                        player.movement(mapGrid, 0, -1);
                    break;
                    case 77: case 'd': /// Direita
                        player.movement(mapGrid, 0, 1);
                    break;
                    case 'f':
                        // Se não há bomba colocada, o F posiciona uma bomba na posição atual do jogador
                        if (!player.bombsPlaced) {
                            mapGrid[player.x][player.y] = 3;

                            b1.x = player.x;
                            b1.y = player.y;

                            player.bombsPlaced = true;

                            bombTimer = chrono::steady_clock::now(); // Define o tempo em que a bomba foi posicionada
                        }
                    break;
                }

            }


			// MOVIMENTO DOS INIMIGOS:
            {
                bool cannotMoveYet = true;
                globalTimer(enemiesMoveTimer, 1, cannotMoveYet);

                if (!cannotMoveYet) {
                    for(int i=0; i<enemiesAmount; i++){ // Processamento dos 4 inimigos

					    if (enemies[i].alive) {
					        if (mapGrid[enemies[i].x][enemies[i].y] == 4){ // Se está na explosão, ele morre
					            enemies[i].death(mapGrid[enemies[i].x][enemies[i].y]);
					        }

					        bool hasMoved = false;
					        int attempts = 0;
					        int oldX = enemies[i].x; // Guarda a posição antiga do inimigo.
					        int oldY = enemies[i].y;

					        while(!hasMoved && attempts < 10 && enemies[i].alive && player.alive){ // Se ainda não se mexeu, não tentou se mexer 10 vezes e ainda está vivo...
					            enemies[i].direction = ranDir(gen); // Sorteia uma direção
					            switch(enemies[i].direction){
					                case 0: // Para cima
					                    enemies[i].movement(mapGrid, player, hasMoved, -1);
					                    break;
					                case 1: // Para baixo
					                    enemies[i].movement(mapGrid, player, hasMoved, 1);
					                    break;
					                case 2: // Para esquerda
					                    enemies[i].movement(mapGrid, player, hasMoved, 0, -1);
					                    break;
					                case 3: // Para direita
					                    enemies[i].movement(mapGrid, player, hasMoved, 0, 1);
					                    break;
					            }
					            attempts++; // Aumenta o contador de tentativas
					        }
					        if (hasMoved) {
					            mapGrid[oldX][oldY] = 0;                     // Exclui o desenho do inimigo que estava na posição anterior.
					            mapGrid[enemies[i].x][enemies[i].y] = 5;       // Desenha o inimigo na posição nova.
					        }
					    }
					}
					enemiesMoveTimer = chrono::steady_clock::now(); // Reinicia o cronômetro
                }
            }
			// FIM DO MOVIMENTO DOS INIMIGOS



             // timer da bomba depois de posicionada:
            if (player.bombsPlaced) {
                b1.texture = globalTimer(bombTimer, 3, player.bombsPlaced, true);

                if (!player.bombsPlaced) {
                    mapGrid[b1.x][b1.y] = 4;
                    isExploding = true;
                    bombCross(mapGrid[b1.x - 1][b1.y]);
                    bombCross(mapGrid[b1.x + 1][b1.y]);
                    bombCross(mapGrid[b1.x][b1.y - 1]);
                    bombCross(mapGrid[b1.x][b1.y + 1]);


                    explosionTimer = chrono::steady_clock::now(); // define o tempo em que a bomba explodiu
                    cout << "\a"; // Som de EXPLOSAO (beep)
                }

            }

            if (isExploding) { //Duração da explosão
                if (mapGrid[player.x][player.y] == 4) // Mata o jogador se ele andar enquanto a explosão está ativa
                    player.alive = false;

                globalTimer(explosionTimer, 1, isExploding);
            }

            for (int i = 0; i < enemiesAmount; i++) {
                if (!enemies[i].alive && mapGrid[enemies[i].x][enemies[i].y] == 6) {
                    enemies[i].deathAnimation = true;

                    globalTimer(enemies[i].deathTimer, 1, enemies[i].deathAnimation);

                    if (!enemies[i].deathAnimation) {
                        mapGrid[enemies[i].x][enemies[i].y] = 0;
                    }
                }
            }

        } //Fim do laço do jogo

        ///Mensagem de fim de jogo
        if(player.alive == false)
            cout << "       Você morreu..." << endl;
        else if(score == 1000)
            cout << "       Parabéns! Você matou todos os INIMIGOS! 🏅" << endl;

        cout << "       Aperte uma tecla para voltar ao Menu.";
        getch();
    }


    return 0;
} //Fim main
