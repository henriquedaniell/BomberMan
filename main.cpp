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

// Variavel que diz se uma bomba foi colocada, e outra para o timer da explosao
bool bombPlaced = false, isExploding = false;

int escolha = 0; // menu
int pontos = 0; // pontuação do jogo
bool alreadyPlayed = false; // controla o 'Jogar Novamente'

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

void cruzBomba(int &posMatriz) { //Função da área de explosão da bomba
	if (posMatriz == 0 || posMatriz == 2 || posMatriz == 5)
		posMatriz = 4;
}

string corBomba(int segundosRestantes) { //Função para pintar o pavio da bomba
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

    while(escolha!=2){
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
        cin >> escolha;

        if(escolha!=1) //Enquanto não escolher 1, fica pedindo para escolher uma opção, caso escolha 2, irá encerrar.
            continue;

        alreadyPlayed = true; // Marca que entrou no jogo uma vez

        ///Mapa do Jogo: 0- Caminho livre    1- Parede Indestrutível  2- Parede destrutível   3- Bomba   4- Explosão   5- Inimigo   6- Inimigo morto
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
        
        ///Inicialização dos inimigos e suas respectivas posições
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

        // Inicialização variavel que armazena o timer da morte de cada inimigo
        chrono::steady_clock::time_point tempoMorte[4];

        ///Sorteio das paredes que serão destrutíveis
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

        //Começa o jogo de verdade
        while(player.alive && pontos<1000){ //Condição de VITÓRIA: 1000 PONTOS. Condição de DERROTA: MORRER
            ///Posiciona a escrita no início do console
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

            ///Imprime o jogo: mapa, personagem e inimigos.
            for(int i=0;i<13;i++){
                for(int j=0;j<19;j++){
                    if(i==player.x && j==player.y){
                        cout<< "🤠 "; //Personagem
                    } else {
                        switch (m[i][j]){
                            case 0: cout<<"   "; break; //Caminho
                            case 1: cout << "\033[90m" << "███" << "\033[0m"; break; // Parede INDESTRUTÍVEL
                            case 2: cout << "\033[33m" << "▓▓▓" << "\033[0m"; break; // Parede DESTRUTÍVEL
                            case 3: cout << corBomba(b1.texture) << "💣ʔ" << "\033[0m"; break; // Bomba
                            case 4:
                                if (isExploding)
                                    cout << "\033[33m" << "💥 " << "\033[0m"; // Explosão
                                else {
                                    m[i][j] = 0; // Acabou a explosão, volta a ser caminho
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
            cout << "                   PONTOS: " << pontos << endl; // Pontuação

            ///Executa os movimentos
            if ( _kbhit() ){
                tecla = getch();
                switch(tecla)
                {
                    case 72: case 'w': /// Cima
                        if (m[player.x-1][player.y] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.x--;
                        else if(m[player.x-1][player.y] == 5 || m[player.x-1][player.y] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 80: case 's': /// Baixo
                        if (m[player.x+1][player.y] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.x++;
                        else if(m[player.x+1][player.y] == 5 || m[player.x+1][player.y] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 75:case 'a': /// Esquerda
                        if (m[player.x][player.y-1] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.y--;
                        else if(m[player.x][player.y-1] == 5 || m[player.x][player.y-1] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 77: case 'd': /// Direita
                        if (m[player.x][player.y+1] == 0) // Confere se a direção que deseja ir é um caminho livre
                            player.y++;
                        else if(m[player.x][player.y+1] == 5 || m[player.x][player.y+1] == 4) // Se for dar de cara com o inimigo ou com o raio de explosão de bomba
                            player.alive = false; // Morte
                    break;
                    case 'f': 
                        // Se não há bomba colocada, o F posiciona uma bomba na posição atual do jogador
                        if (!bombPlaced) {
                            m[player.x][player.y] = 3;

                            b1.x = player.x;
                            b1.y = player.y;

                            bombPlaced = true;

                            tempoBomba = chrono::steady_clock::now(); // Define o tempo em que a bomba foi posicionada
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
                for(int i=0; i<4; i++){ // Processamento dos 4 inimigos

                    if (inimigos[i].alive) {
                        if (m[inimigos[i].x][inimigos[i].y] == 4){ // Se está na explosão, ele morre
                            inimigos[i].alive = false;
                            pontos+=250;

                            m[inimigos[i].x][inimigos[i].y] = 6;

                            auto agoraMorte = chrono::steady_clock::now(); // define o tempo de AGORA
                            tempoMorte[i] = chrono::steady_clock::now();
                        }

                        bool mexeu = false;
                        int tentativas = 0;
                        int velhoX = inimigos[i].x; // Guarda a posição antiga do inimigo.
                        int velhoY = inimigos[i].y;

                        while(!mexeu && tentativas < 10 && inimigos[i].alive && player.alive){ // Se ainda não se mexeu, não tentou se mexer 10 vezes e ainda está vivo...
                            inimigos[i].direction = ranDir(gen); // Sorteia uma direção
                            switch(inimigos[i].direction){
                                case 0: // Para cima
                                    if (inimigos[i].x-1 == player.x && inimigos[i].y == player.y) // Se a direção que o inimigo quer ir foi o Player
                                        player.alive = false;
                                    else if (m[inimigos[i].x-1][inimigos[i].y] == 0){ // Se for caminho livre
                                        inimigos[i].x-=1;
                                        mexeu = true;
                                    } else if(m[inimigos[i].x-1][inimigos[i].y] == 4){ // Se for explosão
                                        inimigos[i].alive = false;
                                        m[inimigos[i].x][inimigos[i].y] = 0;      // Limpa a posição antiga (sem caveira, pois o inimigo nem chegou lá)
                                        inimigos[i].x -= 1;                       // Inimigo anda
                                        m[inimigos[i].x-1][inimigos[i].y] = 6;    // Desenha a caveira na célula de destino
                                        tempoMorte[i] = chrono::steady_clock::now();
                                        pontos+=250;
                                    }
                                    break;
                                case 1: // Para baixo
                                    if (inimigos[i].x+1 == player.x && inimigos[i].y == player.y) // Se a direção que o inimigo quer ir foi o Player
                                        player.alive = false;
                                    else if (m[inimigos[i].x+1][inimigos[i].y] == 0){ // Se for caminho livre
                                        inimigos[i].x+=1;
                                        mexeu = true;
                                    } else if(m[inimigos[i].x+1][inimigos[i].y] == 4){ // Se for explosão
                                        inimigos[i].alive = false;
                                        m[inimigos[i].x][inimigos[i].y] = 0;      // Limpa a posição antiga (sem caveira, pois o inimigo nem chegou lá)
                                        inimigos[i].x+=1;                         // Inimigo anda
                                        m[inimigos[i].x+1][inimigos[i].y] = 6;    // Desenha a caveira na célula de destino
                                        tempoMorte[i] = chrono::steady_clock::now();
                                        pontos+=250;
                                    }
                                    break;
                                case 2: // Para esquerda
                                    if (inimigos[i].x == player.x && inimigos[i].y-1 == player.y) 
                                        player.alive = false;
                                    else if (m[inimigos[i].x][inimigos[i].y-1] == 0){
                                        inimigos[i].y-=1;
                                        mexeu = true;
                                    } else if(m[inimigos[i].x][inimigos[i].y-1] == 4){
                                        inimigos[i].alive = false;
                                        m[inimigos[i].x][inimigos[i].y] = 0;     
                                        inimigos[i].y-=1;
                                        m[inimigos[i].x][inimigos[i].y-1] = 6;    
                                        tempoMorte[i] = chrono::steady_clock::now();
                                        pontos+=250;
                                    }
                                    break;
                                case 3: // Para direita
                                    if (inimigos[i].x == player.x && inimigos[i].y+1 == player.y) 
                                        player.alive = false;
                                    else if (m[inimigos[i].x][inimigos[i].y+1] == 0){
                                        inimigos[i].y+=1;
                                        mexeu = true;
                                    } else if(m[inimigos[i].x][inimigos[i].y+1] == 4){
                                        inimigos[i].alive = false;
                                        m[inimigos[i].x][inimigos[i].y] = 0;      
                                        inimigos[i].y+=1;
                                        m[inimigos[i].x][inimigos[i].y+1] = 6;    
                                        tempoMorte[i] = chrono::steady_clock::now();
                                        pontos+=250;
                                    }
                                    break;
                            }
                            tentativas++; // Aumenta o contador de tentativas
                        }
                        if (mexeu) {
                            m[velhoX][velhoY] = 0;                     // Exclui o desenho do inimigo que estava na posição anterior.
                            m[inimigos[i].x][inimigos[i].y] = 5;       // Desenha o inimigo na posição nova.
                        }
                    }
                    else if (m[inimigos[i].x][inimigos[i].y] == 5) { //Inimigo morto que ainda estava desenhado
                        m[inimigos[i].x][inimigos[i].y] = 0;
                    }
                    else if (m[inimigos[i].x][inimigos[i].y] == 6) { //Caveira fica visível por 1 segundo, depois some
                        auto agoraMorte = chrono::steady_clock::now();
                        auto tempoPassadoMorte = chrono::duration_cast<chrono::seconds>(
                            agoraMorte - tempoMorte[i]
                        ).count();

                        if (tempoPassadoMorte >= 1) // 1 segundo de caveira visível
                            m[inimigos[i].x][inimigos[i].y] = 0;
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
                    cout << "\a"; // Som de EXPLOSAO (beep)
                }
            }

            if (isExploding) { //Duração da explosão
                if (m[player.x][player.y] == 4) // Mata o jogador se ele andar enquanto a explosão está ativa
                    player.alive = false;
                auto agora = chrono::steady_clock::now(); // define o tempo de AGORA

                auto tempoPassado = chrono::duration_cast<chrono::seconds>(
                    agora - tempoExplosao
                ).count(); // Conta quanto tempo já passou desde a explosão da bomba em SEGUNDOS, de acordo com o "AGORA"


                if (tempoPassado >= 1) { // Depois de 1 seg de timer:
                    isExploding = false; // A explosao acaba
                }
            }

        } //Fim do laço do jogo
    
        ///Mensagem de fim de jogo
        if(player.alive == false) 
            cout << "       Você morreu..." << endl;
        else if(pontos == 1000)
            cout << "       Parabéns! Você matou todos os INIMIGOS! 🏅" << endl;

        cout << "       Aperte uma tecla para voltar ao Menu.";
        getch();
    }


    return 0;
} //Fim main
