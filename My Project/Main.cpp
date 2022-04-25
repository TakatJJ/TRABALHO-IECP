#include <stdio.h>
#include <iostream>
#include "libs/EdubotLib.hpp"

using namespace std;

/* FUNÇÕES INTERESSANTES: 
 *   edubotLib->getSonar(i);

	edubotLib->getBumper(i)
	
	edubotLib->getEncoderCountLeft();
	edubotLib->getEncoderCountRight();
	edubotLib->getEncoderCountDT();
	
	edubotLib->getX();
	edubotLib->getY();
	edubotLib->getTheta();
	
	edubotLib->move(0.3);
	edubotLib->rotate(90);
	edubotLib->sleepMilliseconds(2000);
	edubotLib->stop();
 */

#define MAZE_SIZE 100  // número de quadrantes em cada direção

#define FORWARD_DELAY  1175
#define FORWARD_STEPS  2200
#define ROTATION_DELAY 1500

#define FORWARD_VELOCITY 0.4

typedef struct Posicao {
	int x, y;
} POSICAO;

typedef struct Robo {
	POSICAO pos;
	int dir;
}ROBO;

/* MATRIZ DE MOVIMENTAÇÃO: recebe um id (0-3, que representa
   as direções esquerda, frente e direita) e retorna a "movimentação"
   que é feita naquela direção.
   Ex: matMov[1][2] = -1, já que, no eixo y (1), apontado na direção Sul
   (2), o robo sobe (foi adotado o ponto inferior esquerdo como refe-
   rencial)
*/
const int matMov[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

const int sensorId[3] = {0, 3, 6};

float max(float a, float b) {

	float res = a;
	if(b > a) res = b;

	return res;
	
}

int intDir(char direcao) {

	int res;

	switch(direcao) {
		case 'N': res = 0;  break;
		case 'L': res = 1;  break;
		case 'S': res = 2;  break;
		case 'O': res = -1; break;
	}

	return res;
	
}
void ajuste(EdubotLib *edubotLib){
	
}
void orienta(EdubotLib *edubotLib, ROBO *robo, char direcao) {

	int dir_int, goto_dir;

	dir_int = intDir(direcao);
	goto_dir = (dir_int - robo->dir)*90;
	
	if(goto_dir == (-3)*90) {
		goto_dir = (1)*90;
	} else if(goto_dir == (3)*90) {
		goto_dir = (-1)*90;
	}
	
	if(goto_dir != 0) {
		edubotLib->rotate(goto_dir);
		edubotLib->sleepMilliseconds(ROTATION_DELAY);
		edubotLib->stop();
	}
	ajuste(edubotLib);
	
}

void move_frente(EdubotLib *edubotLib, ROBO *robo) {

	// MÉTODO 1 - DELAY
	// - Principal problema: deve ser reajustado toda
	//   vez que a velocidade for reajustada.
	edubotLib->move(FORWARD_VELOCITY);
	edubotLib->sleepMilliseconds(FORWARD_DELAY);
	edubotLib->stop();
	ajuste(edubotLib);

	/*// MÉTODO 2 - ENCODER
	// - Resolve problemas do delay
	// - OBS: parece com o millis() :)
	// - Principal problema: é bugado quando usa com o simulador
	
	long startLeft = edubotLib->getEncoderCountLeft(),
		startRight = edubotLib->getEncoderCountRight();

	long dL, dR;
		
	do {
		
		edubotLib->move(FORWARD_VELOCITY);

		dL = edubotLib->getEncoderCountLeft() - startLeft;
		dR = edubotLib->getEncoderCountRight() - startRight;
		
		//printf("Left = %d, Right = %d\n", dL, dR);
		edubotLib->sleepMilliseconds(50);
		
	} while(dL < FORWARD_STEPS || dR < FORWARD_STEPS);  
	// A redundância confere segurança ao sistema
	// (como se eu não estivesse sendo puxa-saco do professor...)

	edubotLib->stop();*/

	//========================================//

	// CONCLUSÃO:
	// Usar delay parece ser a melhor opção
	
}

void move_to(EdubotLib *edubotLib, ROBO *robo, int toX, int toY) {

	if(toX == robo->pos.x && toY == robo->pos.y+1) {
		// Move no mundo real
		orienta(edubotLib, robo, 'N');
		move_frente(edubotLib, robo);
		// Move no mundo virtual
		robo->dir = intDir('N');
		robo->pos.y++;
	} else if(toX == robo->pos.x && toY == robo->pos.y-1) {
		// Move no mundo real
		orienta(edubotLib, robo, 'S');
		move_frente(edubotLib, robo);
		// Move no mundo virtual
		robo->dir = intDir('S');
		robo->pos.y--;
	} else if(toX == robo->pos.x+1 && toY == robo->pos.y) {
		// Move no mundo real
		printf("HELLO");
		orienta(edubotLib, robo, 'L');
		move_frente(edubotLib, robo);
		// Move no mundo virtual
		robo->dir = intDir('L');
		robo->pos.y--;
		
	} else if(toX == robo->pos.x-1 && toY == robo->pos.y) {
		// Move no mundo real
		orienta(edubotLib, robo, 'O');
		move_frente(edubotLib, robo);
		// Move no mundo virtual
		robo->dir = intDir('O');
		robo->pos.y--;
	} else {
		printf("Muito longe, me ajuda!\n");
	}
	
}

int checkVisitado(int visitado[][MAZE_SIZE], ROBO robo, int lado) {

	int direcao = robo.dir + lado;
	if(direcao == -1) direcao = 3;
	else if(direcao == -2) direcao = 2;

	cout << direcao << endl;

	int x = robo.pos.x + matMov[direcao][0];
	int y = robo.pos.y + matMov[direcao][1];
	
	return visitado[x][y];
	
}

int main(){

	printf("oi");

	EdubotLib *edubotLib = new EdubotLib();

	if(edubotLib->connect()) {

		ROBO robo;
		int visitado[MAZE_SIZE][MAZE_SIZE];
		POSICAO pai[MAZE_SIZE][MAZE_SIZE];

		for(int i = 0; i < MAZE_SIZE; i++) {
			for(int j = 0; j < MAZE_SIZE; j++) {
				visitado[i][j] = 0;
			}
		}
		
		robo.pos.x = MAZE_SIZE/2;
		robo.pos.y = MAZE_SIZE/2;
		robo.dir = intDir('L'); 

		pai[robo.pos.x][robo.pos.y] = (POSICAO){robo.pos.x, robo.pos.y};
		// O pai da casa inicial é a própria casa

		/* SCRIPT 1: Movimento cardinal*/
		move_to(edubotLib, &robo,robo.pos.x+1, robo.pos.y);
		move_to(edubotLib, &robo,robo.pos.x+1, robo.pos.y);
		move_to(edubotLib, &robo,robo.pos.x, robo.pos.y-1);
		move_to(edubotLib, &robo,robo.pos.x-1, robo.pos.y);
		move_to(edubotLib, &robo,robo.pos.x-1, robo.pos.y);
		move_to(edubotLib, &robo,robo.pos.x-1, robo.pos.y);
		move_to(edubotLib, &robo,robo.pos.x, robo.pos.y+1);
		move_to(edubotLib, &robo,robo.pos.x-1, robo.pos.y);
		move_to(edubotLib, &robo,robo.pos.x-1, robo.pos.y);
		move_to(edubotLib, &robo,robo.pos.x-1, robo.pos.y);
		
		//orienta(edubotLib,&robo,'O');
		for( int i = 0; i <= 6; i+=3){
			edubotLib->getSonar(i);
			printf("%f\n",edubotLib->getSonar(i));}
		

		/*float a = -1, b = -1;
		//cout << "ok\n";
		while(true) {
			if(i == 0) {
				a = edubotLib->getSonar(0);
				i++;
			} else {
				a = edubotLib->getSonar(0);
			}
			printf("%f\n", a);
			edubotLib->sleepMilliseconds(1000);
			//cout << "a: " << a << " b: " << b << endl;
		}*/

		//===============================================================//
		
		// PROGRAMA
		/*int leLixo = 1;
		while(true) {
			
			// Marca quadrante atual como visitado
			visitado[robo.pos.x][robo.pos.y] = 1;
			
			int maxId = -1;
			float dist[3], maxDist = 0;

			//printf("Entrei\n");

			if(leLixo) {
				for(int i = 0; i < 3; i++) {
					dist[i] = edubotLib->getSonar(sensorId[i]);
					printf("%f ", dist[i]);
				}
				printf("\n");
				leLixo = 0;
				continue;
			}
			
			for(int i = 0; i < 3; i++) {

				if(checkVisitado(visitado, robo, i-1)) {
					//   i:  0, 1, 2
					// i-1: -1, 0, 1 -> esquerda, frente, direita!
					continue;
				}
				
				dist[i] = edubotLib->getSonar(sensorId[i]);
				printf("dist[%d]: %.2f (sensor %d)\n", i, dist[i], sensorId[i]);
				if(dist[i] > maxDist) {
					maxDist = dist[i];
					maxId = i;
				}
			
			}

			if(maxId == -1) {
				printf("Voltei\n");
				move_to(edubotLib, &robo, pai[robo.pos.x][robo.pos.y].x, pai[robo.pos.x][robo.pos.y].y);
			} else {
				maxId -= 1;  // (0, 1, 2) -> (-1, 0, 1)
				int x = robo.pos.x;
				int y = robo.pos.y;
				printf("x:%d y:%d\n", x + matMov[maxId][0], y + matMov[maxId][1]);
				move_to(edubotLib, &robo, x + matMov[maxId][0], y + matMov[maxId][1]);
				pai[robo.pos.x][robo.pos.y] = (POSICAO){x, y};
			}

			edubotLib->sleepMilliseconds(5000);*/
					
		//}
		
		edubotLib->disconnect();
	
	} else {
		printf("Could not connect to the robot!\n");
	}

	return 0;
}
