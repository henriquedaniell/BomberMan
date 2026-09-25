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

//Modularizar os inimigos

using namespace std;

// Variavel que diz se uma bomba foi colocada, e outra para o timer da explosao
bool bombPlaced = false, isExploding = false;

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
	if (gridPos == 0 || gridPos == 2 || gridPos == 5)
		gridPos = 4;
}

string bombColor(int secondsRemaining) { //Função para pintar o pavio da bomba
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

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 10); // Sorteador se a parede é quebrável ou se terá nada
    uniform_int_distribution<> ranDir(0, 3); // Sorteador da direção que o personagem vai

    cout << "\033[2J\033[H" << endl;
    cout << endl;
    cout << " ||===\\\\  ||==== ||\\   /||      \\\\      // || ||\\   || ||==\\\\   //==\\\\      " << endl;
    cout << " ||   //  ||     ||\\\\ //||       \\\\    //  || ||\\\\  || ||   \\\\ ||    ||       " << endl;
    cout << " ||===|   ||==   || \\// || ====   \\\\  //   || || \\\\ || ||   || ||    ||          " << endl;
    cout << " ||   \\\\  ||     ||     ||         \\\\//    || ||  \\\\|| ||   // ||    ||         " << endl;
    cout << " ||===//  ||==== ||     ||          \\/     || ||   \\|| ||==//   \\\\==//            " << endl;

    this_thread::sleep_for(std::chrono::seconds(2)); // Pausa de 2 segundos para então mostrar o menu

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

        score = 0;
        bombPlaced = false;
        isExploding = false;
        alreadyPlayed = true; // Marca que entrou no jogo uma vez

        ///Mapa do Jogo: 0- Caminho livre    1- Parede Indestrutível  2- Parede destrutível   3- Bomba   4- Explosão   5- Inimigo   6- Inimigo morto
        int mapGrid[13][19]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
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

        ///Inicialização dos inimigos e suas respectivas posições
        Enemy enemies[4];
        enemies[0].x=11;
        enemies[0].y=1;

        enemies[1].x=6;
        enemies[1].y=9;

        enemies[2].x=11;
        enemies[2].y=17;

        enemies[3].x=1;
        enemies[3].y=17;

        // Inicialização da struct Character (player)
        Character player;

        // Inicialização da variável que armazena o timer do movimento dos inimigos.
        auto enemiesMoveTimer = chrono::steady_clock::now();

        // Inicialização da struct bomba (b1)
        bomb b1;

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

        //Variavel para tecla pressionada
        char keyPressed;

        //Começa o jogo de verdade
        while(player.alive && score<1000){ //Condição de VITÓRIA: 1000 PONTOS. Condição de DERROTA: MORRER
            ///Posiciona a escrita no início do console
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

            ///Imprime o jogo: mapa, personagem e inimigos.
            for(int i=0;i<13;i++){
                for(int j=0;j<19;j++){
                    if(i==player.x && j==player.y){
                        cout<< "🤠 "; //Personagem
                    } else {
                        switch (mapGrid[i][j]){
                            case 0: cout<<"   "; break; //Caminho
                            case 1: cout << "\033[90m" << "███" << "\033[0m"; break; // Parede INDESTRUTÍVEL
                            case 2: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break; // Parede DESTRUTÍVEL
                            case 3: cout << bombColor(b1.texture) << "💣ʔ" << "\033[0m"; break; // Bomba
                            case 4:
                                if (isExploding)
                                    cout << "\033[33m" << "💥 " << "\033[0m"; // Explosão
                                else {
                                    mapGrid[i][j] = 0; // Acabou a explosão, volta a ser caminho
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
                        if (mapGrid[player.x-1][player.y] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.x--;
                        else if(mapGrid[player.x-1][player.y] == 5 || mapGrid[player.x-1][player.y] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 80: case 's': /// Baixo
                        if (mapGrid[player.x+1][player.y] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.x++;
                        else if(mapGrid[player.x+1][player.y] == 5 || mapGrid[player.x+1][player.y] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 75:case 'a': /// Esquerda
                        if (mapGrid[player.x][player.y-1] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.y--;
                        else if(mapGrid[player.x][player.y-1] == 5 || mapGrid[player.x][player.y-1] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 77: case 'd': /// Direita
                        if (mapGrid[player.x][player.y+1] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.y++;
                        else if(mapGrid[player.x][player.y+1] == 5 || mapGrid[player.x][player.y+1] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 'f':
                        // Se não há bomba colocada, o F posiciona uma bomba na posição atual do jogador
                        if (!bombPlaced) {
                            mapGrid[player.x][player.y] = 3;

                            b1.x = player.x;
                            b1.y = player.y;

                            bombPlaced = true;

                            bombTimer = chrono::steady_clock::now(); // Define o tempo em que a bomba foi posicionada
                        }
                    break;
                }

            }


            auto enemiesNowTime = chrono::steady_clock::now();
            auto enemiesTimePast = chrono::duration_cast<chrono::milliseconds>(
                enemiesNowTime - enemiesMoveTimer
            ).count();

            // Movimento dos inimigos
            if(enemiesTimePast >= 1000){ // If de confirmação se passou um segundo
                for(int i=0; i<4; i++){ // Processamento dos 4 inimigos

                    if (enemies[i].alive) {
                        if (mapGrid[enemies[i].x][enemies[i].y] == 4){ // Se está na explosão, ele morre
                            killEnemy(enemies[i], score, mapGrid[enemies[i].x][enemies[i].y]);
                        }

                        bool hasMoved = false;
                        int attempts = 0;
                        int oldX = enemies[i].x; // Guarda a posição antiga do inimigo.
                        int oldY = enemies[i].y;

                        while(!hasMoved && attempts < 10 && enemies[i].alive && player.alive){ // Se ainda não se mexeu, não tentou se mexer 10 vezes e ainda está vivo...
                            enemies[i].direction = ranDir(gen); // Sorteia uma direção
                            switch(enemies[i].direction){
                                case 0: // Para cima
                                    if (enemies[i].x-1 == player.x && enemies[i].y == player.y) // Se a direção que o inimigo quer ir foi o Player
                                        player.alive = false;
                                    else if (mapGrid[enemies[i].x-1][enemies[i].y] == 0){ // Se for caminho livre
                                        enemies[i].x-=1;
                                        hasMoved = true;
                                    } else if(mapGrid[enemies[i].x-1][enemies[i].y] == 4){ // Se for explosão
                                        mapGrid[enemies[i].x][enemies[i].y] = 0;    // Limpa a posição antiga (sem caveira, pois o inimigo nem chegou lá)
                                        enemies[i].x -= 1;                     // Inimigo anda
                                        killEnemy(enemies[i], score, mapGrid[enemies[i].x][enemies[i].y]);
                                    }
                                    break;
                                case 1: // Para baixo
                                    if (enemies[i].x+1 == player.x && enemies[i].y == player.y) // Se a direção que o inimigo quer ir foi o Player
                                        player.alive = false;
                                    else if (mapGrid[enemies[i].x+1][enemies[i].y] == 0){ // Se for caminho livre
                                        enemies[i].x+=1;
                                        hasMoved = true;
                                    } else if(mapGrid[enemies[i].x+1][enemies[i].y] == 4){ // Se for explosão
                                        mapGrid[enemies[i].x][enemies[i].y] = 0;      // Limpa a posição antiga (sem caveira, pois o inimigo nem chegou lá)
                                        enemies[i].x+=1;                         // Inimigo anda
                                        killEnemy(enemies[i], score, mapGrid[enemies[i].x][enemies[i].y]);
                                    }
                                    break;
                                case 2: // Para esquerda
                                    if (enemies[i].x == player.x && enemies[i].y-1 == player.y)
                                        player.alive = false;
                                    else if (mapGrid[enemies[i].x][enemies[i].y-1] == 0){
                                        enemies[i].y-=1;
                                        hasMoved = true;
                                    } else if(mapGrid[enemies[i].x][enemies[i].y-1] == 4){
                                        mapGrid[enemies[i].x][enemies[i].y] = 0;
                                        enemies[i].y-=1;
                                        killEnemy(enemies[i], score, mapGrid[enemies[i].x][enemies[i].y]);
                                    }
                                    break;
                                case 3: // Para direita
                                    if (enemies[i].x == player.x && enemies[i].y+1 == player.y)
                                        player.alive = false;
                                    else if (mapGrid[enemies[i].x][enemies[i].y+1] == 0){
                                        enemies[i].y+=1;
                                        hasMoved = true;
                                    } else if(mapGrid[enemies[i].x][enemies[i].y+1] == 4){
                                        mapGrid[enemies[i].x][enemies[i].y] = 0;
                                        enemies[i].y+=1;
                                        killEnemy(enemies[i], score, mapGrid[enemies[i].x][enemies[i].y]);
                                    }
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
                enemiesMoveTimer = enemiesNowTime; // Reinicia o cronômetro
            }

             // timer da bomba depois de posicionada:
            if (bombPlaced) {
                auto bombNowTime = chrono::steady_clock::now(); // define o tempo de AGORA

                auto bombTimePast = chrono::duration_cast<chrono::seconds>(
                    bombNowTime - bombTimer
                ).count(); // conta quanto tempo já passou desde o posicionamento da bomba em SEGUNDOS, de acordo com o "AGORA"

                b1.texture = 3 - bombTimePast; // atualiza a textura da bomba

                if (bombTimePast >= 3) { // se passarem os 3 segundos do timer:
                    // EXPLODE
                    mapGrid[b1.x][b1.y] = 4;
                    isExploding = true;
                    bombCross(mapGrid[b1.x - 1][b1.y]);
                    bombCross(mapGrid[b1.x + 1][b1.y]);
                    bombCross(mapGrid[b1.x][b1.y - 1]);
                    bombCross(mapGrid[b1.x][b1.y + 1]);

                    bombPlaced = false;

                    explosionTimer = chrono::steady_clock::now(); // define o tempo em que a bomba explodiu
                    cout << "\a"; // Som de EXPLOSAO (beep)
                }
            }

            if (isExploding) { //Duração da explosão
                if (mapGrid[player.x][player.y] == 4) // Mata o jogador se ele andar enquanto a explosão está ativa
                    player.alive = false;
                auto explosionNowTime = chrono::steady_clock::now(); // define o tempo de AGORA

                auto explosionTimePast = chrono::duration_cast<chrono::seconds>(
                    explosionNowTime - explosionTimer
                ).count(); // Conta quanto tempo já passou desde a explosão da bomba em SEGUNDOS, de acordo com o "AGORA"


                if (explosionTimePast >= 1) { // Depois de 1 seg de timer:
                    isExploding = false; // A explosao acaba
                }
            }

            for (int i = 0; i < 4; i++) {
                if (!enemies[i].alive && mapGrid[enemies[i].x][enemies[i].y] == 6) {
                    auto enemyDeathNowTime = chrono::steady_clock::now();
                    auto enemyDeathTimePast = chrono::duration_cast<chrono::seconds>(
                        enemyDeathNowTime - enemies[i].deathTimer
                    ).count();

                    if (enemyDeathTimePast >= 1) // 1 segundo de caveira visível
                        mapGrid[enemies[i].x][enemies[i].y] = 0;
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
