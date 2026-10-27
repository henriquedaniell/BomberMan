/// BomberMan (Alunos: Henrique Daniel e Eduardo Ramos

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <mmsystem.h>

void ranking();
void career();
#include "menu.cpp"

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> ranDir(0, 3); // Sorteador da direção que o personagem vai
uniform_int_distribution<> ranIntelligentMove(0, 3); // Sorteador para ver se o inimigo vai na direção certa para alcançar o personagem ou não
uniform_int_distribution<> ranX(1, 17); // Sorteador da casa X que o inimigo vai spawnar
uniform_int_distribution<> ranY(1, 11); // Sorteador da casa Y que o inimigo vai spawnar
uniform_int_distribution<> distrib(1, 100); // Sorteador da parede quebrável
uniform_int_distribution<> ranPortalX(9, 17); // Sorteador da casa X que o portal vai spawnar
uniform_int_distribution<> ranPortalY(6, 11); // Sorteador da casa Y que o portal vai spawnar




/*
PARA FAZER (EDUARDO):
    - adicionar caixas e powerups
    - limpar código criando funções
*/



// ========================================VARIAVEIS GLOBAIS=================================================

int score = 0; // Pontuação do jogo
int currentLevel = 4; // Controla a fase
bool enteredPortal = false; // Confirma se entrou no portal
bool alreadyPlayed = false; // controla o 'Jogar Novamente'

enum PowerUpType {NOT_PLACED, MORE_EXPLOSION_RANGE, EXTRA_BOMB, EXPLOSION_IMMNUNITY, NOCLIP, EXTREME_EXPLOSION};


// =================FUNCAO DE DIRECOES================
int& fourDirectionsValueReturner(int dir, int range, int gameMap[13][19], int x, int y, bool explosion = false, int* newX = nullptr, int* newY = nullptr) {
	int posRange = abs(range);
	int negRange = posRange * (-1);

	switch (dir) {
		case 0:
			if (explosion)
				*newY = y + negRange;
			return gameMap[y + negRange][x];
		break;
		case 1:
			if (explosion)
				*newX = x + posRange;
			return gameMap[y][x + posRange];
		break;
		case 2:
			if (explosion)
				*newY = y + posRange;
			return gameMap[y + posRange][x];
		break;
		case 3:
			if (explosion)
				*newX = x + negRange;
			return gameMap[y][x + negRange];
		break;
	}

	return gameMap[0][0];
}

// =====FUNCAO DE REDUZIR CONTAGENS POSITIVAS======
void reducePositiveCount(int &count, int &gridPos, bool resetGridValue = false) {
	if (count <= 1) {
		count = 0;
		if (resetGridValue)
			gridPos = 0;
	} else
		count--;
}

// ========================================INICIO DAS STRUCTS===============================================

// STRUCT JOGO
struct GeneralGame {
	int 			explosionCountingGrid[13][19] = {0};
	PowerUpType		powerUpGrid[13][19] = {static_cast<PowerUpType>(0)};
	int 			maxPowerUps = 15, powerUpsNumber = 0;
};
GeneralGame GAME;


struct PowerUp {int x, y; PowerUpType type; bool collected = false;};

// STRUCT DE PERSONAGEM JOGADOR
struct Character {
    int x=1, y=1, maxConcurrentBombs, totalBombsPlaced = 0, bombsPlaced = 0, enemiesKilled = 0, totalMovement = 0, totalWallsBroken = 0, range = 1, lastPowerUpPicked;       // Variáveis para posição do player e máximo de bombas que ele pode colocar
    string nick;
    bool alive = true, immuneToExplosion = false;

    void movement(int gameMap[13][19], bool &enteredPortal, int &powerUpsNumber, int newX = 0, int newY = 0) {
        if (gameMap[y + newY][x + newX] == 0 || (gameMap[y + newY][x + newX] == 4 && immuneToExplosion)) {
            x += newX;           // Move o player caso haja caminho livre
            y += newY;
        }
        else if(gameMap[y + newY][x + newX] == 5 || (gameMap[y + newY][x + newX] == 4 && !immuneToExplosion) || gameMap[y + newY][x + newX] == 10) {
            alive = false; // Player morre se der de cara com um inimigo ou com uma casa de explosão da bomba
        }
		else if (gameMap[y + newY][x + newX] == 9) {
			x += newX;
            y += newY;
			powerUpsNumber--;
			gameMap[y][x] = 0;

			switch (GAME.powerUpGrid[y][x]) {
                case MORE_EXPLOSION_RANGE:
                    if (range < 10)
                        range++;
                    break;
                case EXTRA_BOMB:
                    if (maxConcurrentBombs < 5)
                        maxConcurrentBombs++;
                    break;
				case EXPLOSION_IMMNUNITY:
					immuneToExplosion = true;
					break;
			}

			GAME.powerUpGrid[y][x] = static_cast<PowerUpType>(0);
		}
        else if(gameMap[y + newY][x + newX] == 8){
            enteredPortal = true;
		}
    }
};

// STRUCT DOS INIMIGOS
struct Enemy {
    int x = 9, y = 9, direction=0, lifes = 1;
    bool deathAnimation = false;
    chrono::steady_clock::time_point deathTimer = chrono::steady_clock::now();


    void spawn (Character player, int (&gameMap)[13][19]) {
        bool inPosition = false;
        while (!inPosition) {
            int randomX = ranX(gen);
            int randomY = ranY(gen);

			if ((abs(randomX - player.x) + abs(randomY - player.y) <= 4) || gameMap[randomY][randomX] != 9)
				continue;
			else {
				x = randomX;
				y = randomY;
				gameMap[y][x] = 5;

				for (int dir = 0; dir < 4; dir++) {
					int &gridPos = fourDirectionsValueReturner(dir, 1, gameMap, x, y);

					if (gridPos == 9)
						gridPos = 0;
				}
			}

			inPosition = true;
        }
    }


	void movement(int (&gameMap)[13][19], Character &player, bool &hasMoved, int i, int newX = 0, int newY = 0) {
		if (x + newX == player.x && y + newY == player.y) 	// Mata o player se o inimigo encostar nele
		    player.alive = false;
		else if (gameMap[y + newY][x + newX] == 0){ 				// Move o inimigo para o novo local caso seja caminho livre
		    x += newX;
			y += newY;
		    hasMoved = true;
		} else if(gameMap[y + newY][x + newX] == 4){ // Se for explosão
		    if(i>-1){
                if (gameMap[y][x] == 5)
                    gameMap[y][x] = 0;    // Limpa a posição antiga (sem caveira, pois o inimigo nem chegou lá)
                x += newX;                     // Inimigo anda
                y += newY;
                death(gameMap[y][x], i, player);
		    }
		}
	}

	void death (int &gridPos, int i, Character &player) {
        if(lifes <=0)
            return;

		if(i>-1)
            lifes--;
		deathTimer = chrono::steady_clock::now();
		score += 250;
		gridPos = 6;
		player.enemiesKilled += 1;
	}

	void damage(int &gridPos) {
	    if(lifes <=0)
            return;

		lifes--;
		if(lifes == 0){
            deathTimer = chrono::steady_clock::now();
            deathAnimation = true;
            score += 250;
            gridPos = 6;
		}
	}
};

// STRUCT DE BOMBAS
struct Bomb {
    int x, y, texture = 0;
    bool placed = false;


    // Inicialização variavel que armazena o timer da bomba
    chrono::steady_clock::time_point bombTimer = chrono::steady_clock::now();


	struct Explosion {
		int x, y, range = 1;

		// (só grava nas casas que implementa:)
		int reachedRange[4] = {0}; // Distância que a explosão de fato realmente chegou nas 4 (QUATRO) direções

		bool isExploding = false; // Variavel para o timer da explosao

		// Inicialização variavel que armazena o timer da explosao
		chrono::steady_clock::time_point explosionTimer = chrono::steady_clock::now();
	};

	Explosion explosion;

    // Função da área de explosão da bomba
    void bombCross(int (&gameMap)[13][19], int &score, Character &player, Bomb (&bombs)[], Enemy (&enemies)[], Enemy &boss, int enemiesAmount) {
        for (int dir = 0; dir < 4; dir++) {		// testa todas as 4 direções para explosão
			bool hitWall = false;
			int explosionRange = explosion.range;
			explosion.reachedRange[dir] = 0;
			Explosion origin = explosion;

			for (int range = 1; range <= explosionRange; range++) {
                int newX = origin.x, newY = origin.y;
				int &gridPos = fourDirectionsValueReturner(dir, range, gameMap, origin.x, origin.y, true, &newX, &newY);
				switch (gridPos) {
				    case 1: case 8:
						hitWall = true;
						break;
					case 2:
                        GAME.explosionCountingGrid[newY][newX]++;
                        gridPos = 4;
                        player.totalWallsBroken++;
                        explosion.reachedRange[dir] = range;
						hitWall = true;
                        score += 10;
						break;
                    case 3:
                        for (int k = 0; k < player.maxConcurrentBombs; k++) {
                            if (bombs[k].x == newX && bombs[k].y == newY && bombs[k].placed) {
                                bombs[k].placed = false;
                                bombs[k].explode(gameMap, score, player, bombs, enemies, boss, enemiesAmount);
                                GAME.explosionCountingGrid[newY][newX]++;
                                break;
                            }
                        }
                        hitWall = true;
                        explosion.reachedRange[dir] = range;
                        break;
					case 4: case 6:
                        GAME.explosionCountingGrid[newY][newX]++;
                        explosion.reachedRange[dir] = range;
						break;
					case 0: case 5: case 9:
                        if (gridPos == 9) {
                            GAME.powerUpsNumber--;
                            GAME.powerUpGrid[newY][newX] = static_cast<PowerUpType>(0);
                        }
                        GAME.explosionCountingGrid[newY][newX]++;
                        gridPos = 4;
                        explosion.reachedRange[dir] = range;
						break;
					case 7:
                        gridPos = 9;
                        player.totalWallsBroken++;
                        GAME.explosionCountingGrid[newY][newX]++;
						hitWall = true;
						explosion.reachedRange[dir] = range;
                        score += 10;
						break;
                    case 10:
                        if (boss.lifes > 0) {
                            boss.damage(gridPos);
                        }
                        hitWall = true;
                        break;
				}

				if (hitWall)
					break;
			}
		}
    }


	// Função para limpar a área de explosão da bomba
	void clearBombCross(int (&gameMap)[13][19]){
		for (int dir = 0; dir < 4; dir++) {		// testa todas as 4 direções para explosão
			int explosionRange = explosion.reachedRange[dir];
			Explosion origin = explosion;

			for (int range = 0; range <= explosionRange; range++) {
                int newX = origin.x, newY = origin.y;
				int &gridPos = fourDirectionsValueReturner(dir, range, gameMap, origin.x, origin.y, true, &newX, &newY);
				switch (gridPos) {
					case 4:
                        reducePositiveCount(GAME.explosionCountingGrid[newY][newX], gridPos, true);
						break;
                    default:
                        reducePositiveCount(GAME.explosionCountingGrid[newY][newX], gridPos);
                        break;
				}
			}
		}


	}

	void explode(int (&gameMap)[13][19], int &score, Character &player, Bomb (&bombs)[], Enemy (&enemies)[], Enemy &boss, int enemiesAmount) {
		gameMap[y][x] = 4;
		GAME.explosionCountingGrid[y][x]++;
		explosion.isExploding = true;
		explosion.x = x;
		explosion.y = y;
		bombCross(gameMap, score, player, bombs, enemies, boss, enemiesAmount);

		for (int i = 0; i < enemiesAmount; i++) {
		    if (enemies[i].lifes > 0 && gameMap[enemies[i].y][enemies[i].x] == 4)
		        enemies[i].death(gameMap[enemies[i].y][enemies[i].x], i, player);
		}


		explosion.explosionTimer = chrono::steady_clock::now(); // define o tempo em que a bomba explodiu
		if (player.totalBombsPlaced > 0) {player.totalBombsPlaced--;}
	}
};

struct Ranking {
    string nick;
    string dataHour;
    int totalScore;
    int levelReached;
    int totalKills;
    int totalWallsBreaked;
    int totalBombsPlaces;
    int totalMoves;
};
// ========================================FIM DAS STRUCTS===============================================



// ========================================INICIO DAS FUNÇÕES===============================================

void playMusic(){
    PlaySound(TEXT("bombermanMusica.wav"), NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
}

 // Retorna a data e a hora atuais
string dataHourNow() {
    time_t now = time(nullptr);
    tm* dataNow = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", dataNow);

    return string(buffer);
}

// Carrega todos os saves do arquivo organizado por data
vector<Ranking> loadSaves(const string& fileName = "saves_data.txt") {
    vector<Ranking> saves;
    ifstream file(fileName);

    string row;

    while (getline(file, row)) {
        stringstream ss(row);
        string field;
        Ranking save;

        try {
            getline(ss, save.dataHour, ';');
            getline(ss, save.nick, ';');

            getline(ss, field, ';');
            save.totalScore = stoi(field);

            getline(ss, field, ';');
            save.levelReached = stoi(field);

            getline(ss, field, ';');
            save.totalKills = stoi(field);

            getline(ss, field, ';');
            save.totalWallsBreaked = stoi(field);

            getline(ss, field, ';');
            save.totalBombsPlaces = stoi(field);

            getline(ss, field, ';');
            save.totalMoves = stoi(field);

            saves.push_back(save);
        }
        catch (...) {
            // Ignora uma linha que esteja incompleta ou inválida
        }
    }

    file.close();
    return saves;
}

// Escreve todos os saves no arquivo informado
void typeSaves(const string& fileName, const vector<Ranking>& saves) {
    ofstream file(fileName);

    for (const Ranking& save : saves) {
        file << save.dataHour << ';'
             << save.nick << ';'
             << save.totalScore << ';'
             << save.levelReached << ';'
             << save.totalKills << ';'
             << save.totalWallsBreaked << ';'
             << save.totalBombsPlaces << ';'
             << save.totalMoves << '\n';
    }

    file.close();
}

// Registra uma partida nos dois arquivos
void registerSave(Ranking newSave) {
    vector<Ranking> savesData = loadSaves();

    // Adiciona a nova partida sem apagar as anteriores
    savesData.push_back(newSave);

    // Ordena da partida mais recente para a mais antiga
    sort(savesData.begin(), savesData.end(),
         [](const Ranking& a, const Ranking& b) {
             return a.dataHour > b.dataHour;
         });

    typeSaves("saves_data.txt", savesData);

    // Cria uma cópia para ordenar pela pontuação
    vector<Ranking> savesScore = savesData;

    sort(savesScore.begin(), savesScore.end(),
         [](const Ranking& a, const Ranking& b) {
             if (a.totalScore != b.totalScore) {
                 return a.totalScore > b.totalScore;
             }

             // Em caso de empate, a partida mais recente vem primeiro
             return a.dataHour > b.dataHour;
         });

    typeSaves("saves_score.txt", savesScore);
}

void ranking() {
    vector<Ranking> saves = loadSaves("saves_score.txt");

    system("cls");

    cout << "\n========== RANKING ==========\n\n";

    if (saves.empty()) {
        cout << "Nenhuma partida registrada.\n";
    } else {
        for (size_t i = 0; i < saves.size(); i++) {
            cout << i + 1 << " - " << saves[i].nick
                 << " | Pontuacao: " << saves[i].totalScore
                 << " | Fase: " << saves[i].levelReached
                 << " | Inimigos eliminados: " << saves[i].totalKills
                 << "\n";
        }
    }

    cout << "\nPressione ENTER para voltar ao menu.";

    while (_getch() != 13) {
    }
}

void career() {
    vector<Ranking> saves = loadSaves();

    system("cls");

    cout << "\n========== CARREIRA ==========\n\n";

    if (saves.empty()) {
        cout << "Nenhuma partida registrada.\n";
    } else {
        for (size_t i = 0; i < saves.size(); i++) {
            cout << "Partida " << i + 1
                 << " - " << saves[i].nick << "\n";

            cout << "Data: " << saves[i].dataHour << "\n";
            cout << "Pontuacao: " << saves[i].totalScore << "\n";
            cout << "Fase alcancada: " << saves[i].levelReached << "\n";
            cout << "Inimigos eliminados: " << saves[i].totalKills << "\n";
            cout << "Paredes destruidas: "
                 << saves[i].totalWallsBreaked << "\n";
            cout << "Bombas colocadas: "
                 << saves[i].totalBombsPlaces << "\n";
            cout << "Movimentos: " << saves[i].totalMoves << "\n";

            cout << "------------------------------\n";
        }
    }

    cout << "\nPressione ENTER para voltar ao menu.";

    while (_getch() != 13) {
    }
}

// Criação do Boss
void spawnBoss(Enemy &boss, int (&mapGrid)[13][19]){
    boss.lifes = 3;
    boss.x = 17;
    boss.y = 11;
    boss.deathAnimation = false;

    mapGrid[boss.y][boss.x] = 10;
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


// FUNÇÕES PARA GERAÇÃO INICIAL DO MAPA ANTES DE CADA FASE
// Sorteio das paredes que serão destrutíveis
void wallsGeneration(int (&gameMap)[13][19], int &powerUpsNumber, PowerUp (&powerUps)[]) {
	int randomWallGen = 0;
	for(int i=0; i<13; i++){
		for(int j=0; j<19; j++){
			if(gameMap[i][j]==9){
				randomWallGen = distrib(gen);
				if (randomWallGen <= 35){
					gameMap[i][j] = 2;
				} else if (randomWallGen <= 50 && powerUpsNumber < GAME.maxPowerUps) {
					gameMap[i][j] = 7;
					powerUps[powerUpsNumber].x = j;
					powerUps[powerUpsNumber].y = i;

					uniform_int_distribution<> randomPowerUpType(1, 3);
					int randIndex = randomPowerUpType(gen);
					PowerUpType powerUpType = static_cast<PowerUpType>(randIndex);

					powerUps[powerUpsNumber].type = powerUpType;
					GAME.powerUpGrid[i][j] = powerUpType;

					powerUpsNumber++;
				} else {
					gameMap[i][j] = 0;
				}
			}
		}
	}
}

// Geração do portal de passagem de fase
void portalGeneration(int (&gameMap)[13][19]) {
    int randomPortalX = 0;
    int randomPortalY = 0;
    while (gameMap[randomPortalY][randomPortalX] != 2){
        randomPortalX = ranPortalX(gen);
        randomPortalY = ranPortalY(gen);
    }
    gameMap[randomPortalY][randomPortalX] = 8;
}

void mapGeneration(int enemiesAmount, Enemy enemies[], Character player, int (&gameMap)[13][19], int &powerUpsNumber, PowerUp (&powerUps)[], Enemy &boss) {
	if(currentLevel == 4)
        spawnBoss(boss, gameMap);

	for(int i=0; i<enemiesAmount; i++){
	    enemies[i].spawn(player, gameMap);
	}

	wallsGeneration(gameMap, powerUpsNumber, powerUps);
	portalGeneration(gameMap);
}
// FIM DAS FUNÇÕES PARA GERAÇÃO INICIAL DO MAPA ANTES DE CADA FASE



// Função de timer global
int globalTimer(std::chrono::steady_clock::time_point startTime, float timerDuration, bool &timerTrigger, bool needTexture = false) {
	auto nowTime = chrono::steady_clock::now(); // define o tempo de AGORA

	// Define a duração diretamente como float em segundos
    chrono::duration<float> elapsedTimeDuration = nowTime - startTime;
    auto elapsedTime = elapsedTimeDuration.count(); // RETORNA FLOAT


	if (elapsedTime >= timerDuration) {		// ativa o gatilho do final do timer, e o desliga
	    timerTrigger = false;
	}

	if (needTexture) {
        return timerDuration - elapsedTime + 1;
	} else {
        return 0;
    }
}


// FUNÇÃO DE DESENHAR O MAPA
void drawMap(int (&gameMap)[13][19], Character player, Bomb bomb[], int score){
	///Imprime o jogo: mapa, personagem e inimigos.
	for(int i=0;i<13;i++){
	    for(int j=0;j<19;j++){
	        if(i==player.y && j==player.x){
                if (player.immuneToExplosion)
                    cout<< "😇 "; //Personagem
                else
                    cout<< "🤠 "; //Personagem
	        } else {
	            switch (gameMap[i][j]){
	                case 0: cout<<"   "; break; //Caminho
	                case 1: cout << "\033[90m" << "███" << "\033[0m"; break; // Parede INDESTRUTÍVEL
	                case 2: case 7: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break; // Parede DESTRUTÍVEL OU Parede com POWERUP BOX
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
                    // case 7: cout << "\033[33m" << "▓*▓" << "\033[0m"; break; // parede com powerup box
	                case 8: cout << "🌀 "; break; // Portal para próxima fase
                    case 9: {
					    switch (GAME.powerUpGrid[i][j]) {
                            case MORE_EXPLOSION_RANGE:
                                cout << "\033[92m" << "🔥+" << "\033[0m"; break; // powerup
                            case EXTRA_BOMB:
                                cout << "\033[92m" << "🧨+" << "\033[0m"; break; // powerup
                            case EXPLOSION_IMMNUNITY:
                                cout << "\033[92m" << "🛡️+" << "\033[0m"; break; // powerup
                            default:
                                cout << "\033[92m" << "🌟 " << "\033[0m"; break; // powerup
                        }
                        break;
                    }
	               case 10: cout << "😈 "; break; // Boss
	               //default: cout<<"-"; //erro
	            } //Fim switch
	        }
	    }
	    cout << "\033[K\n";
	} //Fim for mapa

	cout << "    Cima ⬆/W | Direita ➡ /D | Baixo ⬇/S | Esquerda ⬅/A\033[K" << endl;
	cout << "                   Colocar Bomba - F\033[K" << endl;
	cout << "                   PONTOS: " << score << "\033[K" << endl;
}
// FIM DA FUNÇÃO DE DESENHAR O MAPA


void randomEnemyMovement(bool &hasMoved, int &attempts, Enemy &enemies, int (&mapGrid)[13][19], Character &player, int i){
    while(!hasMoved && attempts < 10 && enemies.lifes && player.alive){ // Se ainda não se mexeu, não tentou se mexer 10 vezes e ainda está vivo...
        enemies.direction = ranDir(gen); // Sorteia uma direção
        switch(enemies.direction){
            case 0: // Para cima
                enemies.movement(mapGrid, player, hasMoved, i, 0, -1);
                break;
            case 1: // Para direita
                enemies.movement(mapGrid, player, hasMoved, i, 1);
                break;
            case 2: // Para baixo
                enemies.movement(mapGrid, player, hasMoved, i, 0, 1);
                break;
            case 3: // Para esquerda
                enemies.movement(mapGrid, player, hasMoved, i, -1);
                break;
        }
        attempts++; // Aumenta o contador de tentativas
    }
}

void intelligentEnemyMovement(bool &hasMoved, int &attempts, Enemy &enemies, int (&mapGrid)[13][19], Character &player, int i){
    while(!hasMoved && attempts < 10 && enemies.lifes > 0 && player.alive){

        if(player.x > enemies.x)
            enemies.movement(mapGrid, player, hasMoved, i, 1);
        else if(player.x < enemies.x)
            enemies.movement(mapGrid, player, hasMoved, i, -1);

        if(!hasMoved){
            if(player.y > enemies.y)
                enemies.movement(mapGrid, player, hasMoved, i, 0, 1);
            else if(player.y < enemies.y)
                enemies.movement(mapGrid, player, hasMoved, i, 0, -1);
        }

        attempts++;
    }
}
// FIM DAS FUNÇÕES DE MOVIMENTOS DOS INIMIGOS



// FUNÇÃO DE PASSAR PARA A PROX FASE
void nextLevel (int (&mapGrid)[13][19], int enemiesAmount, Character &player, Enemy enemies[], int &powerUpsNumber, PowerUp (&powerUps)[], Enemy &boss, Bomb bombs[]){
    int newMap[13][19]=  { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
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

    for(int i=0; i< 13; i++){
        for(int j=0; j<19; j++){
            mapGrid[i][j]=newMap[i][j];
        }
    }

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 19; j++) {
            GAME.explosionCountingGrid[i][j] = 0;
        }
    }

    for (int k = 0; k < player.maxConcurrentBombs; k++) {
        bombs[k].placed = false;
        bombs[k].texture = 0;
        bombs[k].explosion.isExploding = false;

        bombs[k].bombTimer = chrono::steady_clock::now();
        bombs[k].explosion.explosionTimer = chrono::steady_clock::now();
    }

    player.totalBombsPlaced = 0;
    player.x = 1;
    player.y = 1;
    player.alive = true;
    enteredPortal = false;

    for (int i = 0; i < enemiesAmount; i++) {
        enemies[i].lifes = 1;
        enemies[i].deathAnimation = false;
    }

    mapGeneration(enemiesAmount, enemies, player, mapGrid, powerUpsNumber, powerUps, boss);
}
// FIM DA FUNÇÃO DE PASSAR PARA A PROX FASE



// Função de configurar o console pro jogo no início do código
void configurarConsole(COORD &coord) {
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
}

void drawBossBar(Enemy &boss) {
    cout << endl;
    cout << "\033[91m";
    cout << "       ========================================\n";
    cout << "                      O Professor\n";
    cout << "       ========================================\n";
    cout << "\033[0m";
    cout << "                     [";

    for (int i = 0; i < 3; i++) {
        if (i < boss.lifes)
            cout << "\033[91m ❤️ \033[0m";
        else
            cout << "\033[90m -- \033[0m";
    }
    cout << "]";
}

// ========================================FIM DAS FUNÇÕES=================================================


// ===============================================INICIO DO MAIN=================================================


int main() {

    playMusic();

	COORD coord;
    configurarConsole(coord);

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
    Character player;

    while(menu(alreadyPlayed)){
        cout << "\033[2J\033[H";
		player = {.x = 1, .y = 1, .maxConcurrentBombs = 1, .totalBombsPlaced = 0, .range = 1, .alive = true, .immuneToExplosion = false};
        score = 0;
        alreadyPlayed = true; // Marca que entrou no jogo uma vez
        currentLevel = 4;
        enteredPortal = false;
        int enemiesAmount;
		GAME.powerUpsNumber = 0;


        int enemyRandomMoveChance;
        float enemyMovementSpeed;
        float bossMovementSpeed;

        switch (difficulty) {
            case 1:
                enemyRandomMoveChance = 0;
                enemyMovementSpeed = 1.5;
                bossMovementSpeed = 1;
                enemiesAmount = 3;
            break;
            case 2:
                enemyRandomMoveChance = 1;
                enemyMovementSpeed = 1;
                bossMovementSpeed = .7;
                enemiesAmount = 5;
            break;
            case 3:
                enemyRandomMoveChance = 3;
                enemyMovementSpeed = 0.7;
                bossMovementSpeed = .5;
                enemiesAmount = 7;
            break;

        }

        for (int i = 0; i < 13; i++) {
            for (int j = 0; j < 19; j++) {
                GAME.explosionCountingGrid[i][j] = 0;
            }
        }


        ///Mapa do Jogo: 0- Caminho livre    1- Parede Indestrutível  2- Parede destrutível   3- Bomba   4- Explosão   5- Inimigo   6- Inimigo morto
        int mapGrid[13][19]=  { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,0,0,2,9,9,9,2,9,9,9,2,9,9,9,9,9,9,1,
                                1,0,1,9,1,2,1,9,1,9,1,9,1,9,1,9,1,9,1,
                                1,2,9,9,2,9,9,9,2,9,2,9,9,9,9,9,9,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
                                1,9,2,9,9,2,9,9,2,9,9,2,9,9,9,9,2,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,2,1,9,1,9,1,
                                1,9,9,9,9,9,2,9,9,9,9,9,9,9,9,9,9,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,2,1,9,1,9,1,
                                1,9,9,2,9,9,9,9,9,2,9,9,9,9,2,9,9,9,1,
                                1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
                                1,9,9,9,2,9,9,9,9,9,2,9,9,9,9,9,9,9,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

        // Inicialização da variável que armazena o timer do movimento dos inimigos.
        auto enemiesMoveTimer = chrono::steady_clock::now();
        auto bossMoveTimer = chrono::steady_clock::now();

        // Inicialização da struct de bombas
        Bomb bombs[5];

        //Inicialização dos inimigos e suas respectivas posições
        Enemy enemies[enemiesAmount];

		    //Inicialização do boss
        Enemy boss;

        PowerUp powerUps[GAME.maxPowerUps];

        mapGeneration(enemiesAmount, enemies, player, mapGrid, GAME.powerUpsNumber, powerUps, boss);

        //Variavel para tecla pressionada
        char keyPressed;

        player = {.maxConcurrentBombs = 1, .totalBombsPlaced = 0, .range = 1, .immuneToExplosion = false};


        //Começa o jogo de verdade
        while(player.alive && currentLevel < 5){

            while(player.alive && enteredPortal == false){ //Condição de VITÓRIA: Passar pelo portal. Condição de DERROTA: Morrer

                ///Posiciona a escrita no início do console
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

                // Imprime o jogo: mapa, personagem e inimigos.
                drawMap(mapGrid, player, bombs, score);

                cout << "                   Powerups: " << GAME.powerUpsNumber << "\033[K" << endl;

                if (currentLevel == 4 && boss.lifes > 0) {
                    drawBossBar(boss);
                }


                ///Executa os movimentos
                if ( _kbhit() ){
                    keyPressed = getch();
                    switch(keyPressed)
                    {
                        case 72: case 'w': /// Cima
                            player.movement(mapGrid, enteredPortal, GAME.powerUpsNumber, 0, -1);
                            player.totalMovement +=1;
                        break;
                        case 80: case 's': /// Baixo
                            player.movement(mapGrid, enteredPortal, GAME.powerUpsNumber, 0, 1);
                            player.totalMovement +=1;
                        break;
                        case 75:case 'a': /// Esquerda
                            player.movement(mapGrid, enteredPortal, GAME.powerUpsNumber, -1);
                            player.totalMovement +=1;
                        break;
                        case 77: case 'd': /// Direita
                            player.movement(mapGrid, enteredPortal, GAME.powerUpsNumber, 1);
                            player.totalMovement +=1;
                        break;
                        case 13: case 32: case 'f':
                            // Se há espaço para colocar uma nova bomba, o F posiciona uma bomba na posição atual do jogador
                            if (player.totalBombsPlaced < player.maxConcurrentBombs && mapGrid[player.y][player.x] != 3) {
                                mapGrid[player.y][player.x] = 3;

                                for (int k = 0; k < player.maxConcurrentBombs; k++) {
                                    if (!bombs[k].placed) {
                                        bombs[k].x = player.x;
                                        bombs[k].y = player.y;
                                        bombs[k].placed = true;
                                        bombs[k].explosion.range = player.range;
                                        bombs[k].bombTimer = chrono::steady_clock::now(); // Define o tempo em que a bomba foi posicionada
                                        break;
                                    }
                                }

                                player.totalBombsPlaced++;
                                player.bombsPlaced++;
                            }
                        break;
                    }

                }


                // MOVIMENTO DOS INIMIGOS:
                {
                    bool cannotMoveYet = true;
                    int intelligentMove = 0;
                    globalTimer(enemiesMoveTimer, enemyMovementSpeed, cannotMoveYet);

                    if (!cannotMoveYet) {
                        for(int i=0; i<enemiesAmount; i++){ // Processamento dos 4 inimigos

                            if (enemies[i].lifes) {
                                if (mapGrid[enemies[i].y][enemies[i].x] == 4){ // Se está na explosão, ele morre
                                    enemies[i].death(mapGrid[enemies[i].y][enemies[i].x], i, player);
                                }

                                bool hasMoved = false;
                                int attempts = 0;
                                int oldX = enemies[i].x; // Guarda a posição antiga do inimigo.
                                int oldY = enemies[i].y;


                                intelligentMove = ranIntelligentMove(gen);
                                if(intelligentMove >= enemyRandomMoveChance){
                                    randomEnemyMovement(hasMoved, attempts, enemies[i], mapGrid, player, i);
                                } else{
                                    intelligentEnemyMovement(hasMoved, attempts, enemies[i], mapGrid, player, i);
                                }


                                if (!hasMoved && enemies[i].lifes && player.alive) {
                                    int startDirection = ranDir(gen);

                                    for (int j = 0; j < 4 && !hasMoved && player.alive; j++) {
                                        enemies[i].direction = (startDirection + j) % 4;

                                        switch (enemies[i].direction) {
                                            case 0: // Cima
                                                enemies[i].movement(mapGrid, player, hasMoved, i, 0, -1);
                                                break;

                                        case 1: // Direita
                                            enemies[i].movement(mapGrid, player, hasMoved, i, 1);
                                            break;

                                        case 2: // Baixo
                                            enemies[i].movement(mapGrid, player, hasMoved, i, 0, 1);
                                            break;

                                        case 3: // Esquerda
                                            enemies[i].movement(mapGrid, player, hasMoved, i, -1);
                                            break;
                                        }
                                    }
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

                // MOVIMENTO DO BOSS:
                if(currentLevel == 4 && boss.lifes){
                    bool cannotMoveYet = true;
                    globalTimer(bossMoveTimer, bossMovementSpeed, cannotMoveYet);

                    if (!cannotMoveYet) {
                        if (boss.lifes) {

                            bool hasMoved = false;
                            int attempts = 0;
                            int oldX = boss.x; // Guarda a posição antiga do boss.
                            int oldY = boss.y;
                            int i=-1;

                            intelligentEnemyMovement(hasMoved, attempts, boss, mapGrid, player, i);


                            if (!hasMoved && boss.lifes && player.alive) {
                                int startDirection = ranDir(gen);

                                for (int j = 0; j < 4 && !hasMoved && player.alive; j++) {
                                    boss.direction = (startDirection + j) % 4;

                                    switch (boss.direction) {
                                    case 0: // Cima
                                        boss.movement(mapGrid, player, hasMoved, i, 0, -1);
                                        break;

                                    case 1: // Direita
                                        boss.movement(mapGrid, player, hasMoved, i, 1);
                                        break;

                                    case 2: // Baixo
                                        boss.movement(mapGrid, player, hasMoved, i, 0, 1);
                                        break;

                                    case 3: // Esquerda
                                        boss.movement(mapGrid, player, hasMoved, i, -1);
                                        break;
                                    }
                                }
                            }

                            if (hasMoved) {
                                mapGrid[oldY][oldX] = 0;           // Exclui o desenho do boss que estava na posição anterior.
                                mapGrid[boss.y][boss.x] = 10;       // Desenha o boss na posição nova.
                            }
                        }
                        bossMoveTimer = chrono::steady_clock::now(); // Reinicia o cronômetro
                    }
                }
                // FIM DO MOVIMENTO DO BOSS


                 // timer da bomba depois de posicionada (ANTES DE EXPLODIR):
                for (int k = 0; k < player.maxConcurrentBombs; k++) {
                    if (bombs[k].placed) {
                        bombs[k].texture = globalTimer(bombs[k].bombTimer, 3, bombs[k].placed, true);
                        if (!bombs[k].placed) {
                            bombs[k].explode(mapGrid, score, player, bombs, enemies, boss, enemiesAmount);
                            cout << "\a"; // Som de EXPLOSAO (beep)
                        }
                    }
                }


                // timer da explosão da bomba (DURANTE A EXPLOSÃO)
                for (int k = 0; k < player.maxConcurrentBombs; k++) {
                    if (bombs[k].explosion.isExploding) { //Duração da explosão
                        if (mapGrid[player.y][player.x] == 4 && !player.immuneToExplosion) // Mata o jogador se ele andar enquanto a explosão está ativa
                            player.alive = false;

                        globalTimer(bombs[k].explosion.explosionTimer, 1, bombs[k].explosion.isExploding);
                        if (!bombs[k].explosion.isExploding) { // acabou a explosão DESTA bomba
                            bombs[k].clearBombCross(mapGrid);
                            if (mapGrid[bombs[k].explosion.y][bombs[k].explosion.x] != 3) {
                                reducePositiveCount(GAME.explosionCountingGrid[bombs[k].explosion.y][bombs[k].explosion.x], mapGrid[bombs[k].explosion.y][bombs[k].explosion.x], true);
                            } else {
                                int tempVar = 0;
                                reducePositiveCount(GAME.explosionCountingGrid[bombs[k].explosion.y][bombs[k].explosion.x], tempVar);
                            }
                        }
                    }
                }


                // timer da caveira dos inimigos
                for (int i = 0; i < enemiesAmount; i++) {
                    if (!enemies[i].lifes && mapGrid[enemies[i].y][enemies[i].x] == 6) {
                        enemies[i].deathAnimation = true;

                        globalTimer(enemies[i].deathTimer, 1, enemies[i].deathAnimation);

                        if (!enemies[i].deathAnimation) {
                            if (GAME.explosionCountingGrid[enemies[i].y][enemies[i].x] > 0)
                                mapGrid[enemies[i].y][enemies[i].x] = 4;
                            else
                                mapGrid[enemies[i].y][enemies[i].x] = 0;
                        }
                    }
                }

                // timer da caveira do boss
                if (boss.lifes <= 0 && boss.deathAnimation) {
                    globalTimer(boss.deathTimer, 1, boss.deathAnimation);

                    if (!boss.deathAnimation && mapGrid[boss.y][boss.x] == 6) {
                        if (GAME.explosionCountingGrid[boss.y][boss.x] > 0)
                            mapGrid[boss.y][boss.x] = 4;
                        else
                            mapGrid[boss.y][boss.x] = 0;
                    }
                }

            } //Fim do laço do jogo

            if(enteredPortal){
                bool allEnemiesDead = true;

                for(int i=0; i<enemiesAmount; i++){
                    if(enemies[i].lifes > 0){
                        allEnemiesDead = false;
                    }
                }

                    if (currentLevel == 4 && boss.lifes > 0) {
                        allEnemiesDead = false;
                    }

                    if (allEnemiesDead) {
                        currentLevel++;

                        if (currentLevel < 5) {
                            GAME.powerUpsNumber = 0;
                            nextLevel(mapGrid, enemiesAmount, player, enemies, GAME.powerUpsNumber, powerUps, boss, bombs);
                        }
                    }
                    else {
                        enteredPortal = false;
                    }
            }
        }

        if(player.totalMovement % 2 == 0)
            score -= player.totalMovement / 2;
        else
            score -= (player.totalMovement - 1) / 2;

        cout << "\033[2J\033[H";

        cout << endl;
        cout << "  +------------------------------------------------+" << endl;
        cout << "  |               RESULTADO DA PARTIDA             |" << endl;
        cout << "  +------------------------------------------------+" << endl;
        cout << "  |                                                |" << endl;

        if (player.alive == false) {
            cout << "  |                VOCE MORREU!                    |" << endl;
        }
        else if (currentLevel == 5) {
            cout << "  |             PARABENS, VOCE VENCEU!             |" << endl;
        }

        cout << "  |                                                |" << endl;
        cout << "  +------------------------------------------------+" << endl;
        cout << "  |                  SEUS STATUS                   |" << endl;
        cout << "  +------------------------------------------------+" << endl;
        cout << endl;

        cout << "    Fase alcancada:            "
             << currentLevel << "                            " << endl;

        cout << "    Pontuacao final:           "
             << score << "                         |" << endl;

        cout << "    Inimigos eliminados:       "
             << player.enemiesKilled << "                            " << endl;

        cout << "    Paredes quebradas:         "
             << player.totalWallsBroken << "                            " << endl;

        cout << "    Bombas colocadas:          "
             << player.bombsPlaced << "                            " << endl;

        cout << "    Movimentos realizados:     "
             << player.totalMovement << "                            " << endl;

        cout << "                                                  " << endl;
        cout << "  +------------------------------------------------+" << endl;
        cout << endl;
        cout << "       Digite seu apelido para salvar no ranking." << endl;
        cout<<  "             .. ";
        getline(cin >> ws, player.nick);

        Ranking save;
        save.dataHour = dataHourNow();
        save.levelReached = currentLevel;
        save.nick = player.nick;
        save.totalBombsPlaces = player.totalBombsPlaced;
        save.totalKills = player.enemiesKilled;
        save.totalMoves = player.totalMovement;
        save.totalWallsBreaked = player.totalWallsBroken;
        save.totalScore = score;

        registerSave(save);

    }


    return 0;
} //Fim main
