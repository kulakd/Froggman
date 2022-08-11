//#pragma warning(disable:4996)
//#pragma warning(disable:4244)
//#pragma warning(disable:6386)
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "funkcje.h"

using namespace sf;
class Doggo
{
private:
    int x, y;
    int dx, dy;
public:
    Doggo();
    void move();
    friend void update(int y, int x);
    friend void game(Doggo a[]);
    ~Doggo();
};

Doggo::Doggo()
{
    //rozmiar
    x = y = 80;
    dx = 4 - rand() % 8;
    dy = 4 - rand() % 8;
}

void Doggo::move()
{
    //poziom
    x += dx;
    if (map[y / 18][x / 18] == 1)
    {
        dx = -dx;
        x += dx;
    }
    //pion
    y += dy;
    if (map[y / 18][x / 18] == 1)
    {
        dy = -dy;
        y += dy;
    }
}

void update(int y, int x)
{
    if (map[y][x] == 0)
        map[y][x] = -1;
    if (map[y - 1][x] == 0)
        update(y - 1, x);
    if (map[y + 1][x] == 0)
        update(y + 1, x);
    if (map[y][x - 1] == 0)
        update(y, x - 1);
    if (map[y][x + 1] == 0)
        update(y, x + 1);
}

Doggo::~Doggo()
{}


void game(Doggo a[])
{
    setlocale(LC_CTYPE, "Polish");
    std::cout << "____________________________________________________________________________________________________________________________________" << std::endl;
    std::cout <<"|                                               Welcome to the Froggman Project!                                                     |" << std::endl;
    std::cout <<"|                        It has been made by kulakd and is available at https://github.com/kulakd.                                   |" << std::endl;
    std::cout <<"|                       Software used to create the game were Aseprite ver 1.2.21 and Visual Studio.                                 |" << std::endl;
    std::cout <<"|                              To move use WSAD keys and to restart a map, use ESC key..                                             |" << std::endl;
    std::cout <<"|                   Your goal is to capture as big of an area as you can, while dodging chasing you doggos.                          |" << std::endl;
    std::cout <<"|                                   Additionally, you must avoid the lines already created.                                          |" << std::endl;
    std::cout <<"|                   The game is considered won after the player has hit 16000 points mark.The points are gained by                   |" << std::endl;
    std::cout <<"|                               creating lines as long as possible and cutting off chunks of a map.                                  |" << std::endl;
    std::cout <<"|                   NOTICE: Because of lagging, score board must have been removed from the interface.                               |" << std::endl;
    std::cout <<"|  To make the game easierand more enjoyable, go to line 258 and remove the comment for score to be visible in the interface again.  |" << std::endl;
    std::cout <<"|                                       Have a good dayand I hope you will enjoy the game!                                           |" << std::endl;
    std::cout << "____________________________________________________________________________________________________________________________________" << std::endl;

    srand(time(0));
    RenderWindow window(VideoMode(40 * 18, 40 * 18), "FROGGMAN!!");
    window.setFramerateLimit(60);

 Texture t1, t2, t3, t4;
    t1.loadFromFile("Grafika/kloc.png");
    t2.loadFromFile("Grafika/lose.png");
    t3.loadFromFile("Grafika/doggo.png");
    t4.loadFromFile("Grafika/win.png");

    Sprite Frog(t1), LOSE(t2), Dogs(t3), WIN(t4);
    LOSE.setPosition(100, 100); // ekran przegranej
    WIN.setPosition(100, 100); // ekran zwyciestwa
    Dogs.setOrigin(20, 20); // miejsce pojawiania sie psow

    float PKT = 0;
    int pack_of_dogs = 10; // ilosc psów na ekranie
   

    bool Game = true;   //ustawienie poczatku gry
    int x = 0, y = 0, dx = 0, dy = 0;
    //ustawienie timera gry
    double timer = 0, delay = 0.07;
    Clock clock;

    // ustawienia mapy
    for (int i = 0; i < 40; i++)
        for (int j = 0; j < 40; j++)
            if (i == 0 || j == 0 || i == 40 - 1 || j == 40 - 1)
                map[i][j] = 1;
    //co sie dzieje gdy otowrzy sie ekran głowny
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            // wylaczenie gry
            if (e.type == Event::Closed)
                window.close();
            //ustawienie restartu
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                {
                    //zerowanie ustawien startowych
                    for (int i = 1; i < 40 - 1; i++)
                        for (int j = 1; j < 40 - 1; j++)
                            map[i][j] = 0;

                    x = 10; y = 0;
                    Game = true;
                    PKT = 0;
                }
        }
        // Ustawienie sterowania
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            dx = -1;
            dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            dx = 1;
            dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            dx = 0;
            dy = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            dx = 0;
            dy = 1;
        }


        //uaktualnianie ekranu ze uwzgledniajac ruch
        if (timer > delay)
        {
            x += dx;
            y += dy;

            if (x < 0)
                x = 0;
            if (x > 40 - 1)
                x = 40 - 1;
            if (y < 0)
                y = 0;
            if (y > 40 - 1)
                y = 40 - 1;
            //najechanie na to samo pole podczas rysowania
            if (map[y][x] == 2)
            {
                Game = false;
                window.draw(LOSE);
            }
            if (map[y][x] == 0)
                map[y][x] = 2;
            timer = 0;
        }
        //poruszanie sie psow tak dlugo jak trwa gra
        for (int i = 0; i < pack_of_dogs; i++)
            a[i].move();

        if (map[y][x] == 1)
        {
            dx = dy = 0;
            //tworzenie pustego srodka
            for (int i = 0; i < pack_of_dogs; i++)
                update(a[i].y / 18, a[i].x / 18);
            // zmiana pól z pustych na zajete
            for (int i = 0; i < 40; i++)
                for (int j = 0; j < 40; j++)
                    if (map[i][j] == -1)
                        map[i][j] = 0;
                    else
                        map[i][j] = 1;
        }
        //pies trafiajacy na rysujaca mape konczy gre
         for (int i = 0; i < pack_of_dogs; i++)
            if (map[a[i].y / 18][a[i].x / 18] == 2)
            {
                window.draw(LOSE);
                Game = false;
            }
            
        window.clear(); // aby animacja nie "śmieciła" ekranu
        for (int i = 0; i < 40; i++)
            for (int j = 0; j < 40; j++)
            {
                if (map[i][j] == 0)
                    continue;
                if (map[i][j] == 1) // robimy obramówke i pola zajęte
                    Frog.setTextureRect(IntRect(0, 0, 18, 18));
                if (map[i][j] == 2) // kolorujemy zakryte pola
                    Frog.setTextureRect(IntRect(54, 0, 18, 18));
                Frog.setPosition(j * 18, i * 18);
                window.draw(Frog);
            }
        //pojawia sie zaba
        Frog.setTextureRect(IntRect(36, 0, 18, 18));
        Frog.setPosition(x * 18, y * 18);
        window.draw(Frog);

        // krecace sie psy( bo to fajne jest) oraz ich rysowanie na ekranie
        Dogs.rotate(12);
        for (int i = 0; i < pack_of_dogs; i++)
        {
            Dogs.setPosition(a[i].x, a[i].y);
            window.draw(Dogs);
        }

        for (int i = 0; i < 40; i++)
            for (int j = 0; j < 40; j++)
            {
                if (map[i][j] == 2)
                {
                    PKT++;
                    //std::cout << "PKT = " << PKT << "Mapa ogolnie = " << sizeof(mapa) * 2.5 << std::endl; //mozna to zakomentowac, to tylko pokazuje zliczanie punktow
                }
                if (PKT > sizeof(map) * 2.5)
                    window.draw(WIN);
            }

        if (!Game)
            if (PKT < sizeof(map) * 2.5)
                window.draw(LOSE);
        window.display();
    }
}

int main()
{
    Doggo a[10];
    game(a);
    return 1;
}