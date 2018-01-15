#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

struct Bod
{
	int x, y;
} a[4], b[4];


const int X = 20;														// vyska pola
const int Y = 10;														// sirka pola

int pole[X][Y] = { 0 };

int tetromina[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	for (int i = 0; i<4; i++)
		if (a[i].x<0 || a[i].x >= Y || a[i].y >= X) return 0;
		else if (pole[a[i].y][a[i].x]) { 
			return 0; 
		};

		return 1;
};


int main()
{
	srand(time(0));

	RenderWindow okno(VideoMode(320, 480), "Tetrix!");			// otvor okno s rezoluciou a daj mu meno

	Texture tex1, tex2, tex3;
	tex2.loadFromFile("images/background.png");					// textura na sprajta pozadia
	tex3.loadFromFile("images/frame.png");						// textura na sprajta ramu
	tex1.loadFromFile("images/tiles.png");						// textura na sprajta blokov

	Sprite sprajt(tex1), pozadie(tex2), ram(tex3);

	int dx = 2;													// pozicia bloku
	bool rotuj = 0; 
	int NumColor = 1;
	float casovac = 0, 
		  delay = 0.3;

	a[0].x = 0, a[0].y = 1;										// explicitne zobrazenie... -_-
	a[1].x = 1, a[1].y = 1;
	a[2].x = 1, a[2].y = 2;
	a[3].x = 1, a[3].y = 3;

	Clock clock;

	while (okno.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		casovac += time;

		Event e;
		while (okno.pollEvent(e))
		{
			if (e.type == Event::Closed)
				okno.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up) rotuj = true;
				else if (e.key.code == Keyboard::Left) dx = -1;
				else if (e.key.code == Keyboard::Right) dx = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		////// pohyb vlavo/pravo //////
		for (int i = 0; i<4; i++) { b[i] = a[i]; a[i].x += dx; }
		if (!check()) for (int i = 0; i<4; i++) a[i] = b[i];

		///// flipovanie /////
		if (rotuj)
		{
			Bod p = a[1];						//stred rotacie
			for (int i = 0; i<4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()) for (int i = 0; i<4; i++) a[i] = b[i];
		}

		///////Tick//////
		if (casovac>delay)
		{
			for (int i = 0; i<4; i++) { b[i] = a[i]; a[i].y += 1; } // pohyb dole

			if (!check())
			{
				for (int i = 0; i<4; i++) pole[b[i].y][b[i].x] = NumColor;

				NumColor = 1 + rand() % 7;
				int n = rand() % 7;							// ktory z tvarov sa vygeneruje
				for (int i = 0; i<4; i++)
				{					
					a[i].x = tetromina[n][i] % 2;
					a[i].y = tetromina[n][i] / 2;
				}
			}

			casovac = 0;
		}

		///////plna rada//////////
		int k = X - 1;
		for (int i = X - 1; i>0; i--)
		{
			
			int count = 0;
			for (int j = 0; j<Y; j++)
			{
				if (pole[i][j]) count++;
				pole[k][j] = pole[i][j];
			}
			if (count<Y) k--;					// ak je count mensi ako sirka pola vezmi prec row
			
		}

		dx = 0; rotuj = 0; delay = 0.5;

		///// renderovanie //////
		okno.clear(Color::White);
		okno.draw(pozadie);

		for (int i = 0; i<X; i++)
			for (int j = 0; j<Y; j++)				// kontakt so spodom
			{
				if (pole[i][j] == 0) continue;
				sprajt.setTextureRect(IntRect(pole[i][j] * 18, 0, 18, 18));
				sprajt.setPosition(j * 18, i * 18);
				sprajt.move(28, 31); //offset
				okno.draw(sprajt);
			}

		for (int i = 0; i<4; i++)								// tetromina
		{
			sprajt.setTextureRect(IntRect(NumColor * 18, 0, 18, 18));
			sprajt.setPosition(a[i].x * 18, a[i].y * 18);
			sprajt.move(28, 31); //offset
			okno.draw(sprajt);
		}

		okno.draw(ram);
		okno.display();
	}

	return 0;
}