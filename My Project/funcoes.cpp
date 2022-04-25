#define MAZE_SIZE 100  // número de quadrantes em cada direção

#define FORWARD_DELAY  1188
#define FORWARD_STEPS  2200
#define ROTATION_DELAY 1500

#define FORWARD_VELOCITY 0.4

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
typedef struct Posicao {
	int x, y;
} POSICAO;

typedef struct Robo {
	POSICAO pos;
	int dir;
	}ROBO;
	
	
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
	
}