#include <iostream>

using namespace std;

string difficultyString = "Fácil";
int difficulty = 1;

void instructions(){
    cout << "\033[2J\033[H"; // Apaga tudo que está no console e move o cursor para o topo
    cout << endl;
    cout <<     "            INSTRUÇÕES          " << endl;
    cout <<     " ================================" << endl;
    cout <<     "||    Bem-vindo ao BomberMan!   ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     "||                              ||" << endl;
    cout <<     " =================================" << endl;
    cout <<     " Aperte ENTER para voltar ao menu!";
    getch();
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
    cout <<     " Aperte ENTER para voltar ao menu!";
    getch();
}

void chooseDifficulty(int &difficulty, string &difficultyString){
    cout << "\033[2J\033[H"; // Apaga tudo que está no console e move o cursor para o topo
    cout << endl;
    cout <<     "        DIFICULDADE        "  << endl;
    cout <<     " ========================= " << endl;
    cout <<     "||                       ||" << endl;
    cout <<     "||       1- Fácil        ||" << endl;
    cout <<     "||                       ||" << endl;
    cout <<     "||       2- Média        ||" << endl;
    cout <<     "||                       ||" << endl;
    cout <<     "||      3- Difícil       ||" << endl;
    cout <<     "||                       ||" << endl;
    cout <<     " ================================ " << endl;
    cout <<     "  Escolha a dificuldade: ";
    cin >> difficulty;

    if (cin.fail()) {          // se usuário digitou letra, ignora e tenta dnv
            cin.clear();
            cin.ignore(10000, '\n');
            chooseDifficulty(difficulty, difficultyString);
    } else if(difficulty<1 || difficulty > 3)
        chooseDifficulty(difficulty, difficultyString);

	switch(difficulty){
		case 1:
			difficultyString = "Fácil";
			break;
		case 2:
			difficultyString = "Média";
			break;
		case 3:
			difficultyString = "Difícil";
			break;
	}
}

bool menu(bool &alreadyPlayed){
    while (true) {
        int menuChoice = 0; // menu
        cout << "\033[2J\033[H"; // Apaga tudo que está no console e move o cursor para o topo
        cout << endl;
        cout <<     "         BOMBERMAN          " << endl;
        cout <<     " ========================== " << endl;
        cout <<     "||                         ||" << endl;

        if(alreadyPlayed == false){
            cout << "||        1- Jogar         ||" << endl;
        } else {
            cout << "||   1- Jogar Novamente    ||" << endl;
        }
        cout <<     "||  2- Dificuldade ("<<difficultyString<<")"<<"||" << endl;
        cout <<     "||      3- Instruções      ||" << endl;
		cout <<     "||       4- Créditos       ||" << endl;
		cout <<     "||        5- Sair          ||" << endl;
        cout <<     "||                         ||" << endl;
        cout <<     " =========================== " << endl;
        cout <<     "     ESCOLHA UMA OPCAO: ";
        cin >> menuChoice;

        if (cin.fail()) {          // se usuário digitou letra, ignora e tenta dnv
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch(menuChoice){
        case 1: return true; break;
        case 2: chooseDifficulty(difficulty, difficultyString); break;
        case 3: instructions(); break;
        case 4: credits(); break;
        case 5: return false; break;
        }
    }
}
