#include <iostream>
#include <conio.h>
#include <limits>
#include <string>

using namespace std;

string difficultyString = "Facil";
int difficulty = 1;
bool IAPlay = false;
bool twoPlayers = false; // Modo de dois jogadores (exclusivo com o IAPlay)

// Espera o ENTER para passar de pagina
void waitEnter() {
    while (_getch() != 13) {
        // Aguarda o ENTER
    }
}

void instructions() {
    // ---------------- PAGINA 1: CONTROLES ----------------
    system("cls");

    cout << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |            INSTRUCOES - 1/3 CONTROLES          |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  UM JOGADOR                                    |" << endl;
    cout << "  |  W A S D ou SETAS       - Mover                |" << endl;
    cout << "  |  F, ESPACO ou ENTER     - Colocar bomba        |" << endl;
    cout << "  |  R                      - Detonar bomba remota |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  DOIS JOGADORES                                |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  Jogador 1                                     |" << endl;
    cout << "  |  W A S D                - Mover                |" << endl;
    cout << "  |  ESPACO (ou F)          - Colocar bomba        |" << endl;
    cout << "  |  R                      - Detonar bomba remota |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  Jogador 2                                     |" << endl;
    cout << "  |  SETAS DO TECLADO       - Mover                |" << endl;
    cout << "  |  ENTER                  - Colocar bomba        |" << endl;
    cout << "  |  P                      - Detonar bomba remota |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << endl;
    cout << "       Pressione ENTER para continuar.";

    waitEnter();

    // ---------------- PAGINA 2: POWER-UPS ----------------
    system("cls");

    cout << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |            INSTRUCOES - 2/3 POWER-UPS          |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  Os power-ups ficam escondidos dentro das      |" << endl;
    cout << "  |  paredes amarelas. Quebre a parede para que    |" << endl;
    cout << "  |  o item apareca e passe por cima dele.         |" << endl;
    cout << "  |  Cada um vale 20 pontos.                       |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  🔥+ ALCANCE                                   |" << endl;
    cout << "  |  Aumenta em 1 o alcance das suas explosoes.    |" << endl;
    cout << "  |  Permanente, ate o limite de 10.               |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  🧨+ BOMBA EXTRA                               |" << endl;
    cout << "  |  Mais uma bomba ao mesmo tempo no mapa.        |" << endl;
    cout << "  |  Permanente, ate o limite de 5.                |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  🛡️+ IMUNIDADE (10s)                           |" << endl;
    cout << "  |  Andar sobre explosoes nao mata.               |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  👻+ NOCLIP (10s)                              |" << endl;
    cout << "  |  Atravessa paredes amarelas. Se o tempo        |" << endl;
    cout << "  |  acabar com voce dentro de uma, ela quebra     |" << endl;
    cout << "  |  sem dar pontos. Nao da pra soltar bomba       |" << endl;
    cout << "  |  de dentro da parede.                          |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  💥+ EXPLOSAO EXTREMA (10s)                    |" << endl;
    cout << "  |  Toda bomba colocada durante esse tempo sai    |" << endl;
    cout << "  |  com alcance maximo. Quando acaba, o alcance   |" << endl;
    cout << "  |  volta ao normal.                              |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  📱+ BOMBA REMOTA (3 bombas)                   |" << endl;
    cout << "  |  As 3 proximas bombas so explodem quando       |" << endl;
    cout << "  |  voce mandar (R ou P). Se esquecer, elas tem   |" << endl;
    cout << "  |  um pavio de seguranca de 15 segundos.         |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << endl;
    cout << "       Pressione ENTER para continuar.";

    waitEnter();

    // ---------------- PAGINA 3: ELEMENTOS E OBJETIVO ----------------
    system("cls");

    cout << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |         INSTRUCOES - 3/3 ELEMENTOS DO JOGO     |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  OBJETIVO                                      |" << endl;
    cout << "  |  Elimine todos os inimigos e entre no portal   |" << endl;
    cout << "  |  para avancar de fase. Na fase 4 e preciso     |" << endl;
    cout << "  |  derrotar tambem o boss.                       |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  MAPA                                          |" << endl;
    cout << "  |  Paredes cinzas: indestrutiveis                |" << endl;
    cout << "  |  Paredes amarelas: podem ser destruidas        |" << endl;
    cout << "  |  🌀 Portal: leva para a proxima fase           |" << endl;
    cout << "  |  👾 Inimigo: encostar nele causa derrota       |" << endl;
    cout << "  |  😈 Boss: aguenta 3 explosoes                  |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  JOGADORES                                     |" << endl;
    cout << "  |  🤠 Jogador 1    🥷 Jogador 2                  |" << endl;
    cout << "  |  😇 com imunidade   🫥 com noclip              |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  DOIS JOGADORES                                |" << endl;
    cout << "  |  O modo e cooperativo, mas a bomba de um       |" << endl;
    cout << "  |  mata o outro! Se um morrer, o outro continua  |" << endl;
    cout << "  |  sozinho e o que morreu volta na proxima       |" << endl;
    cout << "  |  fase. Basta um entrar no portal.              |" << endl;
    cout << "  |  Os power-ups sao independentes.               |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  PONTOS                                        |" << endl;
    cout << "  |  Inimigo 250   Parede 10   Power-up 20         |" << endl;
    cout << "  |  Cada 2 movimentos custam 1 ponto no final.    |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << endl;
    cout << "       Pressione ENTER para voltar ao menu.";

    waitEnter();
}

// ========================================
// FUNCAO: CREDITOS
// ========================================

void credits() {
    system("cls");

    cout << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |                    CREDITOS                    |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |              DESENVOLVEDORES                   |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  Nome: Eduardo Ramos do Amaral                 |" << endl;
    cout << "  |  Nome: Henrique Daniel da Silva                |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |              SOBRE O PROJETO                   |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  Jogo desenvolvido em C++                      |" << endl;
    cout << "  |  Projeto academico: BomberMan                  |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  DISCIPLINA: Algoritmos e Programação II       |" << endl;
    cout << "  |  PROFESSOR:  Alex Rese                         |" << endl;
    cout << "  |  INSTITUICAO: UNIVALI                          |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << endl;
    cout << "       Pressione ENTER para voltar ao menu.";

    waitEnter();
}

// ========================================
// FUNCAO: ESCOLHER DIFICULDADE
// ========================================

void chooseDifficulty(int &difficulty, string &difficultyString) {
    int escolha;

    while (true) {
        system("cls");

        cout << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |              DIFICULDADE               |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |                                        |" << endl;
        cout << "  |  1 - Facil                             |" << endl;
        cout << "  |  2 - Media                             |" << endl;
        cout << "  |  3 - Dificil                           |" << endl;
        cout << "  |                                        |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |  Atual: ";

        switch (difficulty) {
            case 1:
                cout << "Facil";
                break;
            case 2:
                cout << "Media";
                break;
            case 3:
                cout << "Dificil";
                break;
        }

        cout << "                         |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << endl;
        cout << "  Escolha (1-3) ou 0 para voltar: ";

        cin >> escolha;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (escolha == 0) {
            return;
        }

        if (escolha < 1 || escolha > 3) {
            continue;
        }

        difficulty = escolha;

        switch (difficulty) {
            case 1:
                difficultyString = "Facil";
                break;
            case 2:
                difficultyString = "Media";
                break;
            case 3:
                difficultyString = "Dificil";
                break;
        }

        return;
    }
}

// ========================================
// FUNCAO: MODO DE DOIS JOGADORES
// ========================================

void chooseTwoPlayers() {
    int escolha;
    int intTwoPlayers;

    while (true) {

        if(twoPlayers)
            intTwoPlayers = 2;
        else
            intTwoPlayers = 1;

        system("cls");

        cout << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |            Dois Jogadores              |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |                                        |" << endl;
        cout << "  |  1 - Desativado                        |" << endl;
        cout << "  |  2 - Ativado                           |" << endl;
        cout << "  |                                        |" << endl;
        cout << "  |  P1: WASD, ESPACO para bomba, R        |" << endl;
        cout << "  |  P2: SETAS, ENTER para bomba, P        |" << endl;
        cout << "  |                                        |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |  Atual: ";

        switch (intTwoPlayers) {
            case 1:
                cout << "Desativado";
                break;
            case 2:
                cout << "Ativado   ";
                break;
        }

        cout << "                     |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << endl;
        cout << "  Escolha (1-2) ou 0 para voltar: ";

        cin >> escolha;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (escolha == 0) {
            return;
        }

        if (escolha < 1 || escolha > 2) {
            continue;
        }

        switch (escolha) {
            case 1:
                twoPlayers = false;
                break;
            case 2:
                twoPlayers = true;
                IAPlay = false; // Os dois modos nao podem ficar ligados juntos
                break;
        }

        return;
    }
}

void playAlone() {
    int escolha;
    int intIAPlay;
    while (true) {

        if(IAPlay)
            intIAPlay = 2;
        else
            intIAPlay = 1;

        system("cls");

        cout << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |             Jogar Sozinho              |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |                                        |" << endl;
        cout << "  |  1 - Desativado                        |" << endl;
        cout << "  |  2 - Ativado                           |" << endl;
        cout << "  |                                        |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |  Atual: ";

        switch (intIAPlay) {
            case 1:
                cout << "Desativado";
                break;
            case 2:
                cout << "Ativado   ";
                break;
        }

        cout << "                     |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << endl;
        cout << "  Escolha (1-2) ou 0 para voltar: ";

        cin >> escolha;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (escolha == 0) {
            return;
        }

        if (escolha < 1 || escolha > 2) {
            continue;
        }

        switch (escolha) {
            case 1:
                IAPlay = false;
                break;
            case 2:
                IAPlay = true;
                twoPlayers = false; // Os dois modos nao podem ficar ligados juntos
                break;
        }

        return;
    }
}

// ========================================
// MENU PRINCIPAL
// ========================================

bool menu(bool &alreadyPlayed) {
    while (true) {
        int menuChoice = 0;

        system("cls");

        cout << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |                BOMBERMAN               |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << "  |                                        |" << endl;

        if (!alreadyPlayed) {
            cout << "  |  1 - Jogar                             |" << endl;
        } else {
            cout << "  |  1 - Jogar Novamente                   |" << endl;
        }

        cout << "  |  2 - Dificuldade: ";

        switch (difficulty) {
            case 1:
                cout << "Facil    ";
                break;
            case 2:
                cout << "Media    ";
                break;
            case 3:
                cout << "Dificil  ";
                break;
        }

        cout << "            |" << endl;
        cout << "  |  3 - Instrucoes                        |" << endl;
        cout << "  |  4 - Ranking                           |" << endl;
        cout << "  |  5 - Carreira                          |" << endl;
        cout << "  |  6 - Creditos                          |" << endl;
        cout << "  |  7 - Sair                              |" << endl;
        cout << "  |                                        |" << endl;
        if(!twoPlayers)
            cout << "  |  9 - Dois Jogadores (Desativado)       |" << endl;
        else
            cout << "  |  9 - Dois Jogadores (Ativado)          |" << endl;
        if(!IAPlay)
            cout << "  |  0 - Jogar Sozinho (Desativado)        |" << endl;
        else
            cout << "  |  0 - Jogar Sozinho (Ativado)           |" << endl;
        cout << "  |                                        |" << endl;
        cout << "  +----------------------------------------+" << endl;
        cout << endl;
        cout << "  Escolha uma opcao: ";

        cin >> menuChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (menuChoice) {
            case 1:
                return true;

            case 2:
                chooseDifficulty(difficulty, difficultyString);
                break;

            case 3:
                instructions();
                break;

            case 4:
                ranking();
                break;

            case 5:
                career();
                break;

            case 6:
                credits();
                break;

            case 7:
                return false;

            case 9:
                chooseTwoPlayers();
                break;

            case 0:
                playAlone();
                break;

            default:
                break;
        }
    }
}
