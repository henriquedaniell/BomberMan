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
uniform_int_distribution<> ranX(1, 17); // Sorteador da casa X que o inimigo vai spawnar
uniform_int_distribution<> ranY(1, 11); // Sorteador da casa Y que o inimigo vai spawnar
uniform_int_distribution<> distrib(1, 100); // Sorteador da parede quebrável


// ========================================VARIAVEIS GLOBAIS=================================================

int score = 0; // pontuação do jogo
int difficulty = 0;
bool alreadyPlayed = false; // controla o 'Jogar Novamente'


// ========================================INICIO DAS STRUCTS===============================================

// STRUCT DE BOMBAS
struct Bomb {
    int x, y, texture = 0;
    bool placed = false;

    // Variavel para o timer da explosao
    bool isExploding = false;

    // Inicialização variavel que armazena o timer da bomba
    chrono::steady_clock::time_point bombTimer = chrono::steady_clock::now();

    // Inicialização variavel que armazena o timer da explosao
    chrono::steady_clock::time_point explosionTimer = chrono::steady_clock::now();
};


// STRUCT DE PERSONAGEM JOGADOR
struct Character {
    int x=1, y=1, maxConcurrentBombs, totalBombsPlaced = 0;       // Variáveis para posição do player e máximo de bombas que ele pode colocar
    bool alive = true;

    void movement(int gameMap[13][19], int newX = 0, int newY = 0) {
        if (gameMap[y + newY][x + newX] == 0) {
            x += newX;           // Move o player caso haja caminho livre
            y += newY;
        }
        else if(gameMap[y + newY][x + newX] == 5 || gameMap[y + newY][x + newX] == 4) {
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
		else if (gameMap[y + newY][x + newX] == 0){ 				// Move o inimigo para o novo local caso seja caminho livre
		    x += newX;
			y += newY;
		    hasMoved = true;
		} else if(gameMap[y + newY][x + newX] == 4){ // Se for explosão
		    gameMap[y][x] = 0;    // Limpa a posição antiga (sem caveira, pois o inimigo nem chegou lá)
		    x += newX;                     // Inimigo anda
			y += newY;
		    death(gameMap[y][x]);
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

void enemiesSpawn (int enemiesAmount, Enemy &enemy, Character player, int (&mapGrid)[13][19] ){
    bool enemyInPosition = false;
    while (enemyInPosition == false){
        int randomX = ranX(gen);
        int randomY = ranY(gen);

        if((abs(randomX - player.x) + abs(randomY - player.y) <=4) || mapGrid[randomY][randomX]!=9)
            continue;
        else {
            enemy.x = randomX;
            enemy.y = randomY;
            mapGrid[randomY][randomX] = 5;

            if(mapGrid[randomY+1][randomX] == 9)
                mapGrid[randomY+1][randomX] = 0;
            if(mapGrid[randomY-1][randomX] == 9)
                mapGrid[randomY-1][randomX] = 0;
            if(mapGrid[randomY][randomX+1] == 9)
                mapGrid[randomY][randomX+1] = 0;
            if(mapGrid[randomY][randomX-1] == 9)
                mapGrid[randomY][randomX-1] = 0;
        }
        enemyInPosition = true;
    }
}

///Sorteio das paredes que serão destrutíveis
void wallsGeneration(int (&gameMap)[13][19]) {
	int randomWallGen = 0;
	for(int i=0; i<13; i++){
		for(int j=0; j<19; j++){
			if(gameMap[i][j]==9){
				randomWallGen = distrib(gen);
				if(randomWallGen>35){
					gameMap[i][j] = 0;
				} else {
					gameMap[i][j] = 2;
				}
			}
		}
	}
}


void mapGeneration(int enemiesAmount, Enemy enemies[], Character player, int (&gameMap)[13][19]) {
	for(int i=0; i<enemiesAmount; i++){
	    enemiesSpawn(enemiesAmount, enemies[i], player, gameMap);
	}

	wallsGeneration(gameMap);
}


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

void drawMap(int (&gameMap)[13][19], Character player, Bomb bomb[]){
	///Imprime o jogo: mapa, personagem e inimigos.
	for(int i=0;i<13;i++){
	    for(int j=0;j<19;j++){
	        if(i==player.y && j==player.x){
	            cout<< "🤠 "; //Personagem
	        } else {
	            switch (gameMap[i][j]){
	                case 0: cout<<"   "; break; //Caminho
	                case 1: cout << "\033[90m" << "███" << "\033[0m"; break; // Parede INDESTRUTÍVEL
	                case 2: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break; // Parede DESTRUTÍVEL
	                case 3: {
						int tex = 0;
	                    for (int k = 0; k < player.maxConcurrentBombs; k++) {
                            if (bomb[k].placed && i == bomb[k].y && j == bomb[k].x)
                                tex = bomb[k].texture;
	                    }
						cout << bombColor(tex) << "💣ʔ" << "\033[0m";
                    break;
                    }
	                case 4: cout << "\033[33m" << "💥 " << "\033[0m"; break; // Explosão
	                case 5: cout << "👾 "; break; // Inimigo
	                case 6: cout << "💀 "; break; // Morte do inimigo
	                //default: cout<<"-"; //erro
	            } //Fim switch
	        }
	    }
	    cout << "\033[K\n";
	} //Fim for mapa
}

bool menu(){
    while (true) {
        int menuChoice = 0; // menu
        cout << "\033[2J\033[H"; // Apaga tudo que está no console e move o cursor para o topo
        cout << endl;
        cout <<     "         BOMBERMAN          " << endl;
        cout <<     " ========================== " << endl;
        cout <<     "||                        ||" << endl;

        if(alreadyPlayed == false){
            cout << "||       1- Jogar         ||" << endl;
        } else {
            cout << "||   1- Jogar Novamente   ||" << endl;
        }
        cout <<     "||     2- Instruções      ||" << endl;
        cout <<     "||      3- Créditos       ||" << endl;
        cout <<     "||       4- Sair          ||" << endl;
        cout <<     "||                        ||" << endl;
        cout <<     " ========================== " << endl;
        cout <<     "     ESCOLHA UMA OPCAO: ";
        cin >> menuChoice;

        if (cin.fail()) {          // se usuário digitou letra, ignora e tenta dnv
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch(menuChoice){
        case 1: return true;
        case 2: /*instructions(); */    break;
        case 3: /*credits(); */         break;
        case 4: return false;
        }
    }
}
/*
void instructions(){
    cout << "\033[2J\033[H"; // Apaga tudo que está no console e move o cursor para o topo
    cout << endl;
    cout <<     "         BOMBERMAN          " << endl;
    cout <<     " ========================== " << endl;
    cout <<     "||  Bem-vindo ao BomberMan!                      ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     "||                        ||" << endl;
    cout <<     " ========================== " << endl;
    cout <<     "     ESCOLHA UMA OPCAO: ";
    getch();
    menu();
}

void credits(){
    cout << "\033[2J\033[H"; // Apaga tudo que está no console e move o cursor para o topo
    cout << endl;
    cout <<     "             CRÉDITOS          " << endl;
    cout <<     " ================================" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     " ================================ " << endl;
    cout <<     "Aperte ENTER para voltar ao menu!";
    getch();
    menu();
} */
// ========================================FIM DAS FUNÇÕES=================================================


// ===============================================INICIO DO MAIN=================================================


int main() {

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

    system("mode con: cols=70 lines=30");

	// Inicialização da struct Character (player)
    Character player{.maxConcurrentBombs = 1};

    while(menu()){
        cout << "\033[2J\033[H";
        player.x = 1, player.y = 1, player.alive = true, player.totalBombsPlaced = 0;
        score = 0;
        alreadyPlayed = true; // Marca que entrou no jogo uma vez
        int enemiesAmount = 5;

        ///Mapa do Jogo: 0- Caminho livre    1- Parede Indestrutível  2- Parede destrutível   3- Bomba   4- Explosão   5- Inimigo   6- Inimigo morto
        int mapGrid[13][19]=  { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,0,0,2,9,9,9,2,9,9,9,2,9,9,9,9,9,9,1,
                                1,0,1,9,1,2,1,9,1,9,1,9,1,9,1,9,1,9,1,
                                1,2,9,2,9,9,9,2,9,9,2,9,9,9,9,9,9,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
                                1,2,9,9,9,2,9,9,2,9,9,2,9,9,9,9,2,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,2,1,9,1,9,1,
                                1,9,9,9,9,2,9,9,9,9,9,9,9,9,9,9,9,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,2,1,9,1,9,1,
                                1,9,9,2,9,9,9,9,9,2,9,9,9,9,2,9,9,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
                                1,9,9,2,9,9,9,9,9,9,2,9,9,9,9,9,9,9,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

        // Inicialização da variável que armazena o timer do movimento dos inimigos.
        auto enemiesMoveTimer = chrono::steady_clock::now();

        // Inicialização da struct de bombas
        Bomb bombs[5];

        ///Inicialização dos inimigos e suas respectivas posições
        Enemy enemies[enemiesAmount];

        mapGeneration(enemiesAmount, enemies, player, mapGrid);

        //Variavel para tecla pressionada
        char keyPressed;

        //Começa o jogo de verdade
        while(player.alive && score < enemiesAmount * 250){ //Condição de VITÓRIA: Matar todos inimigos. Condição de DERROTA: MORRER
            ///Posiciona a escrita no início do console
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

            // Imprime o jogo: mapa, personagem e inimigos.
            drawMap(mapGrid, player, bombs);


            cout << "    Cima ⬆/W | Direita ➡ /D | Baixo ⬇/S | Esquerda ⬅/A\033[K" << endl;
            cout << "                   Colocar Bomba - F\033[K" << endl;
            cout << "                   PONTOS: " << score << "\033[K" << endl;

            ///Executa os movimentos
            if ( _kbhit() ){
                keyPressed = getch();
                switch(keyPressed)
                {
                    case 72: case 'w': /// Cima
                        player.movement(mapGrid, 0, -1);
                    break;
                    case 80: case 's': /// Baixo
                        player.movement(mapGrid, 0, 1);
                    break;
                    case 75:case 'a': /// Esquerda
                        player.movement(mapGrid, -1);
                    break;
                    case 77: case 'd': /// Direita
                        player.movement(mapGrid, 1);
                    break;
                    case 'f':
                        // Se não há bomba colocada, o F posiciona uma bomba na posição atual do jogador
                        if (player.totalBombsPlaced < player.maxConcurrentBombs && mapGrid[player.y][player.x] != 3) {
                            mapGrid[player.y][player.x] = 3;

                            for (int k = 0; k < player.maxConcurrentBombs; k++) {
                                if (!bombs[k].placed && !bombs[k].isExploding) {
                                    bombs[k].x = player.x;
                                    bombs[k].y = player.y;
                                    bombs[k].placed = true;
                                    bombs[k].bombTimer = chrono::steady_clock::now(); // Define o tempo em que a bomba foi posicionada
                                    break;
                                }
                            }

                            player.totalBombsPlaced++;
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
					        if (mapGrid[enemies[i].y][enemies[i].x] == 4){ // Se está na explosão, ele morre
					            enemies[i].death(mapGrid[enemies[i].y][enemies[i].x]);
					        }

					        bool hasMoved = false;
					        int attempts = 0;
					        int oldX = enemies[i].x; // Guarda a posição antiga do inimigo.
					        int oldY = enemies[i].y;

					        while(!hasMoved && attempts < 10 && enemies[i].alive && player.alive){ // Se ainda não se mexeu, não tentou se mexer 10 vezes e ainda está vivo...
					            enemies[i].direction = ranDir(gen); // Sorteia uma direção
					            switch(enemies[i].direction){
					                case 0: // Para cima
					                    enemies[i].movement(mapGrid, player, hasMoved, 0, -1);
					                    break;
					                case 1: // Para baixo
					                    enemies[i].movement(mapGrid, player, hasMoved, 0, 1);
					                    break;
					                case 2: // Para esquerda
					                    enemies[i].movement(mapGrid, player, hasMoved, -1);
					                    break;
					                case 3: // Para direita
					                    enemies[i].movement(mapGrid, player, hasMoved, 1);
					                    break;
					            }
					            attempts++; // Aumenta o contador de tentativas
					        }
					        if (hasMoved) {
					            mapGrid[oldY][oldX] = 0;                     // Exclui o desenho do inimigo que estava na posição anterior.
					            mapGrid[enemies[i].y][enemies[i].x] = 5;       // Desenha o inimigo na posição nova.
					        }
					    }
					}
					enemiesMoveTimer = chrono::steady_clock::now(); // Reinicia o cronômetro
                }
            }
			// FIM DO MOVIMENTO DOS INIMIGOS



             // timer da bomba depois de posicionada:
             for (int k = 0; k < player.maxConcurrentBombs; k++) {
                if (bombs[k].placed) {
                    bombs[k].texture = globalTimer(bombs[k].bombTimer, 3, bombs[k].placed, true);

                    if (!bombs[k].placed) {
                        mapGrid[bombs[k].y][bombs[k].x] = 4;
                        bombs[k].isExploding = true;
                        bombCross(mapGrid[bombs[k].y - 1][bombs[k].x]);
                        bombCross(mapGrid[bombs[k].y + 1][bombs[k].x]);
                        bombCross(mapGrid[bombs[k].y][bombs[k].x - 1]);
                        bombCross(mapGrid[bombs[k].y][bombs[k].x + 1]);

                        for (int i = 0; i < enemiesAmount; i++) {
                            if (enemies[i].alive && mapGrid[enemies[i].y][enemies[i].x] == 4)
                                enemies[i].death(mapGrid[enemies[i].y][enemies[i].x]);
                        }

                        bombs[k].explosionTimer = chrono::steady_clock::now(); // define o tempo em que a bomba explodiu
                        cout << "\a"; // Som de EXPLOSAO (beep)
                    }
                }
            }

            for (int k = 0; k < player.maxConcurrentBombs; k++) {
                if (bombs[k].isExploding) { //Duração da explosão
                    if (mapGrid[player.y][player.x] == 4) // Mata o jogador se ele andar enquanto a explosão está ativa
                        player.alive = false;

                    globalTimer(bombs[k].explosionTimer, 1, bombs[k].isExploding);

                    if (!bombs[k].isExploding) { // acabou a explosão DESTA bomba
                        int bx = bombs[k].x, by = bombs[k].y;
                        if (mapGrid[by][bx]     == 4) mapGrid[by][bx]     = 0;
                        if (mapGrid[by-1][bx]   == 4) mapGrid[by-1][bx]   = 0;
                        if (mapGrid[by+1][bx]   == 4) mapGrid[by+1][bx]   = 0;
                        if (mapGrid[by][bx-1]   == 4) mapGrid[by][bx-1]   = 0;
                        if (mapGrid[by][bx+1]   == 4) mapGrid[by][bx+1]   = 0;

						if (player.totalBombsPlaced > 0) {player.totalBombsPlaced--;}
                    }
                }
            }

            for (int i = 0; i < enemiesAmount; i++) {
                if (!enemies[i].alive && mapGrid[enemies[i].y][enemies[i].x] == 6) {
                    enemies[i].deathAnimation = true;

                    globalTimer(enemies[i].deathTimer, 1, enemies[i].deathAnimation);

                    if (!enemies[i].deathAnimation) {
                        mapGrid[enemies[i].y][enemies[i].x] = 0;
                    }
                }
            }

        } //Fim do laço do jogo

        ///Mensagem de fim de jogo
        if(player.alive == false)
            cout << "       Você morreu..." << endl;
        else if(score >= enemiesAmount * 250)
            cout << "       Parabéns! Você matou todos os INIMIGOS! 🏅" << endl;

        cout << "       Aperte uma tecla para voltar ao Menu.";
        getch();
    }


    return 0;
} //Fim main
