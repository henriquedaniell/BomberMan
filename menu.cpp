
#include <iostream>
#include <conio.h>
#include <limits>
#include <string>

using namespace std;

string difficultyString = "Facil";
int difficulty = 1;

void instructions() {
    system("cls");

    cout << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |                  INSTRUCOES                    |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  OBJETIVO                                      |" << endl;
    cout << "  |  Derrote os inimigos e encontre o portal       |" << endl;
    cout << "  |  para avancar para a proxima fase.             |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  CONTROLES                                     |" << endl;
    cout << "  |  W ou SETA PARA CIMA    - Mover para cima      |" << endl;
    cout << "  |  S ou SETA PARA BAIXO   - Mover para baixo     |" << endl;
    cout << "  |  A ou SETA PARA ESQUERDA- Mover para esquerda  |" << endl;
    cout << "  |  D ou SETA PARA DIREITA - Mover para direita   |" << endl;
    cout << "  |  F, ESPACO ou ENTER     - Colocar uma bomba    |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  ELEMENTOS DO JOGO                             |" << endl;
    cout << "  |  Paredes cinzas: indestrutiveis                |" << endl;
    cout << "  |  Paredes amarelas: podem ser destruidas        |" << endl;
    cout << "  |  Inimigos: encostar neles causa derrota        |" << endl;
    cout << "  |  Bombas: cuidado com o alcance da explosao     |" << endl;
    cout << "  |  Portal: permite tentar avancar de fase        |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  POWER-UPS                                     |" << endl;
    cout << "  |  🔥+ Aumenta o alcance da explosao             |" << endl;
    cout << "  |  🧨+ Permite colocar mais bombas ao mesmo tempo|" << endl;
    cout << "  |  🛡️+ Permite andar sobre explosoes sem morrer  |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  |  IMPORTANTE                                    |" << endl;
    cout << "  |  Elimine todos os inimigos antes de usar       |" << endl;
    cout << "  |  o portal. Na fase 4, derrote tambem o boss!   |" << endl;
    cout << "  |                                                |" << endl;
    cout << "  +------------------------------------------------+" << endl;
    cout << endl;
    cout << "       Pressione ENTER para voltar ao menu.";

    while (_getch() != 13) {
        // Aguarda o ENTER
    }
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

    while (_getch() != 13) {
        // Aguarda o ENTER
    }
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

            default:
                break;
        }
    }
}
