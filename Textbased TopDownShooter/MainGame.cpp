#include <iostream>
#include <Windows.h>
#include <thread>
#include <stdlib.h> 
#include "LinkedList.h"
#include "Node.h"
#include "GameObject.h"
#include "SEnemy.h"
#include "DEnemy.h"
#include "Bullet.h"
#include "Player.h"
#include "PowerUp.h"
#include <queue>
using namespace std;

const int nScreenWidth  = 120;
const int nScreenHeight = 40;

//check if the provided pointer is an instance of provided class
template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}


int main()
{
    bool game_running = true;
    LinkedList obj_list;

    //used to track any screen cleanup jobs
    queue<int> cleaning_x_queue;
    queue<int> cleaning_y_queue;

    //Create screen buffer
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];

    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        screen[i] = L' ';
    screen[nScreenWidth * nScreenHeight - 1] = '\0';    //Tells where to stop

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    bool bKey[6];

    long int nSpeedCount = 0;
    GameObject* player = new Player(15, 15);
    obj_list.Insert(player);
    //game loop
    while (game_running)
    {
        this_thread::sleep_for(50ms);
        nSpeedCount++;
        
        //Enemy Generation at random y location
        if (nSpeedCount % 50 == 0)
        {
            //randomly generate a shooting or a dodging enemy
            int random = rand() % 36;
            int random1 = rand() % 2;
            GameObject* enemy;
            if (random1 == 0)
            {
                enemy = new SEnemy(116, random);
            }
            else
            {
                enemy = new DEnemy(116, random);
            }
            obj_list.Insert(enemy);
        }

        //Enemy Shooting
        for (int i = 0; i < obj_list.Size(); i++)
        {
            GameObject* temp = obj_list.At(i);
            if (instanceof<SEnemy>(temp))
            {
                if (!((SEnemy*)temp)->OnCoolDown())
                    obj_list.Insert(((SEnemy*)temp)->Shoot());
            }
        }

        //INPUT
        for (int k = 0; k < 6; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26\x20\x1B"[k]))) != 0;

        if (bKey[0])//Right
        {
            ((Player*)player)->Translate(3, 0);
        }
        if (bKey[1])//Left
        {
            ((Player*)player)->Translate(-3, 0);
        }
        if (bKey[2])//Down
        {
            ((Player*)player)->Translate(0, 2);
        }
        if (bKey[3])//Up
        {
            ((Player*)player)->Translate(0, -2);
        }
        if (bKey[4])//Space
        {
            if(!((Player*)player)->OnCoolDown())
                obj_list.Insert(((Player*)player)->Shoot());
        }
        if (bKey[5])//Escape
        {
            game_running = false;
        }

        //RENDER
        if (nSpeedCount % 2 == 0)
        {
            //completing all overdue screen cleaning
            while (!cleaning_x_queue.empty())
            {
                int cealnX = cleaning_x_queue.front();
                int cleanY = cleaning_y_queue.front();
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        screen[cealnX + i + (cleanY + j) * nScreenWidth] = L' ';
                        screen[cealnX + i + 1 + (cleanY + j) * nScreenWidth] = L' ';
                    }
                }
                cleaning_x_queue.pop();
                cleaning_y_queue.pop();
            }
            
            Node* temp = obj_list.Begin();
            while (temp != nullptr)
            { 
                //give momentum to enemy aircrafts
                if (instanceof<SEnemy>(temp->data))
                {
                    ((SEnemy*)temp->data)->Translate(-2, 0);
                }
                else if (instanceof<DEnemy>(temp->data))
                {
                    ((DEnemy*)temp->data)->Translate(-2, 0);
                }

                //cleaning object from current location
                // 
                //bullet is exception as it is a 1x1 sprite 
                if (instanceof<Bullet>(temp->data)) 
                {
                    screen[temp->data->x + (temp->data->y) * nScreenWidth] = L' ';
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            screen[temp->data->x + i + (temp->data->y + j) * nScreenWidth] = L' ';
                            screen[temp->data->x + i + 1 + (temp->data->y + j) * nScreenWidth] = L' ';
                        }
                    }
                }

                //moving object to new location
                temp->data->Move();

                //drawing object in new location
                // 
                //bullet is exception as it is a 1x1 sprite 
                if (instanceof<Bullet>(temp->data))
                {
                    if (temp->data->Alive() == true)
                        screen[temp->data->x + (temp->data->y ) * nScreenWidth] = temp->data->Art()[0];
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (temp->data->Alive() == true)
                                screen[temp->data->x + i + (temp->data->y + j) * nScreenWidth] = temp->data->Art()[i + j * 4];
                        }
                    }
                }
                temp = temp->next;
            }
        }

        //nested loop for collision, homing range and dodging range detection
        for (int i = 0; i < obj_list.Size(); i++)
        {
            for (int j = 0; j < obj_list.Size(); j++)
            {
                GameObject* temp1 = obj_list.At(i);
                GameObject* temp2 = obj_list.At(j);
                if (temp1 != temp2 && temp1->Alive()  && temp2->Alive())
                {
                    bool collision = false;

                    //check if two objects overlap
                    if ((temp1->x < (temp2->x + temp2->Width()) && (temp1->x + temp1->Width()) > temp2->x) &&
                        (temp1->y < (temp2->y + temp2->Height()) && (temp1->y + temp1->Height()) > temp2->y)) collision = true;

                    if (collision)
                    {
                        //if bullet collides with enemy
                        if ((instanceof<Bullet>(temp1) && (instanceof<DEnemy>(temp2) || instanceof<SEnemy>(temp2)))
                            || (instanceof<Bullet>(temp2) && (instanceof<DEnemy>(temp1) || instanceof<SEnemy>(temp1))))
                        {
                            //saving the object location for explosion and cleaning
                            int explosionX, explosionY;
                            //saving bullet hostile status
                            bool hostile;
                            if (instanceof<Bullet>(temp1))
                            {
                                hostile = ((Bullet*)temp1)->IsHostile();
                                explosionX = temp2->x;
                                explosionY = temp2->y;
                            }
                            else
                            {
                                hostile = ((Bullet*)temp2)->IsHostile();
                                explosionX = temp1->x;
                                explosionY = temp1->y;
                            }
                            //avoiding enemy friendly fire
                            if (!hostile)
                            {
                                //explosion showcase
                                for (int i = 0; i < 4; i++)
                                {
                                    for (int j = 0; j < 4; j++)
                                    {
                                        screen[explosionX + i + (explosionY + j) * nScreenWidth] = L'*';
                                        screen[explosionX + i + 1 + (explosionY + j) * nScreenWidth] = L'*';
                                    }
                                }
                                //30% chance of powerup
                                int powChance = rand() % 100;
                                if (powChance <= 30)
                                    obj_list.Insert(new PowerUp(explosionX, explosionY));

                                cleaning_x_queue.push(explosionX);
                                cleaning_y_queue.push(explosionY);
                                obj_list.At(i)->Kill();
                                obj_list.At(j)->Kill();
                            }
                        }
                        //if two enemies collide
                        else if ((instanceof<DEnemy>(temp1) || instanceof<SEnemy>(temp1)) && (instanceof<DEnemy>(temp2) || instanceof<SEnemy>(temp2)))
                        {
                            //saving object locations for explosion and cleaning
                            int explosionX1 = temp1->x;
                            int explosionX2 = temp2->x;
                            int explosionY1 = temp1->y;
                            int explosionY2 = temp2->y;

                            //explosion
                            for (int i = 0; i < 4; i++)
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    screen[explosionX1 + i + (explosionY1 + j) * nScreenWidth] = L'*';
                                    screen[explosionX1 + i + 1 + (explosionY1 + j) * nScreenWidth] = L'*';
                                    screen[explosionX2 + i + (explosionY2 + j) * nScreenWidth] = L'*';
                                    screen[explosionX2 + i + 1 + (explosionY2 + j) * nScreenWidth] = L'*';
                                }
                            }
                            cleaning_x_queue.push(explosionX1);
                            cleaning_x_queue.push(explosionX2);
                            cleaning_y_queue.push(explosionY1);
                            cleaning_y_queue.push(explosionY2);
                            obj_list.At(i)->Kill();
                            obj_list.At(j)->Kill();
                        }
                        //if player collides with enemy -> game over
                        else if ((instanceof<Player>(temp1) && (instanceof<DEnemy>(temp2) || instanceof<SEnemy>(temp2))) || (instanceof<Player>(temp2) && (instanceof<DEnemy>(temp1) || instanceof<SEnemy>(temp1))))
                        {
                            game_running = false;
                        }
                        //if player collides with bullet
                        else if ((instanceof<Player>(temp1) && instanceof<Bullet>(temp2)) || (instanceof<Player>(temp2) && instanceof<Bullet>(temp1)))
                        {
                            //check bullet hostile status
                            bool hostile;
                            if (instanceof<Bullet>(temp1))
                                hostile = ((Bullet*)temp1)->IsHostile();
                            else
                                hostile = ((Bullet*)temp2)->IsHostile();
                            //if bullet is hostile -> game over
                            if (hostile)
                            {
                                game_running = false;
                            }
                        }
                        //if player collides with powerup
                        else if ((instanceof<Player>(temp1) && instanceof<PowerUp>(temp2)) || (instanceof<Player>(temp2) && instanceof<PowerUp>(temp1)))
                        {
                            if (instanceof<Player>(temp1)) 
                            {
                                //if no prior powerups
                                if (!((Player*)temp1)->PowerUpStatus())
                                    ((Player*)temp1)->PowerUp();
                                //if already have powerup
                                else
                                    ((Player*)temp1)->IncreasePowerUpCoolDown();

                                //tracking coordinates for cleaning
                                cleaning_x_queue.push(temp2->x);
                                cleaning_y_queue.push(temp2->y);
                                temp2->Kill();
                            } 
                            else 
                            {
                                //if no prior powerups
                                if (!((Player*)temp2)->PowerUpStatus())
                                    ((Player*)temp2)->PowerUp();

                                //if already have powerup
                                else
                                    ((Player*)temp2)->IncreasePowerUpCoolDown();

                                //tracking coordinates for cleaning
                                cleaning_x_queue.push(temp1->x);
                                cleaning_y_queue.push(temp1->y);
                                temp1->Kill();
                            } 
                        }
                    }
                    //no collision
                    else
                    {
                        //if dodging enemy is near a bullet
                        if ((instanceof<Bullet>(temp1) && instanceof<DEnemy>(temp2))
                            || (instanceof<Bullet>(temp2) && instanceof<DEnemy>(temp1)))
                        {
                            //distance between bullet and dodging enemy 
                            int c = sqrt(((temp2->x + 1 - temp1->x) * (temp2->x + 1 - temp1->x) + (temp2->y - 1 - temp1->y) * (temp2->y - 1 - temp1->y)));
                            if (c < 6)
                            {
                                if (instanceof<DEnemy>(temp1)) 
                                {
                                    //if bullet is hostile to enemy
                                    if (!((Bullet*)temp2)->IsHostile())
                                    {
                                        if (!((DEnemy*)temp1)->dodged)
                                        {
                                            //tracking coordinates for cleaning
                                            cleaning_x_queue.push(temp1->x);
                                            cleaning_y_queue.push(temp1->y);

                                            //dodging up or down based on 50% chance
                                            int random = rand() % 2;
                                            if (random == 0) ((DEnemy*)temp1)->Translate(0, 3);
                                            else ((DEnemy*)temp1)->Translate(0, -3);
                                            ((DEnemy*)temp1)->dodged = true;
                                        }
                                    }
                                } 
                                else
                                {
                                    //if bullet is hostile to enemy
                                    if (!((Bullet*)temp1)->IsHostile())
                                    {
                                        if (!((DEnemy*)temp2)->dodged)
                                        {
                                            //tracking coordinates for cleaning
                                            cleaning_x_queue.push(temp2->x);
                                            cleaning_y_queue.push(temp2->y);

                                            //dodging up or down based on 50% chance
                                            int random = rand() % 2;
                                            if (random == 0) ((DEnemy*)temp2)->Translate(0, 2);
                                            else ((DEnemy*)temp2)->Translate(0, -2);
                                            ((DEnemy*)temp2)->dodged = true;
                                        }
                                    }
                                } 
                            }
                        }

                        //if homing bullet is close to an enemy
                        if ((instanceof<Bullet>(temp1) && (instanceof<DEnemy>(temp2) || instanceof<SEnemy>(temp2))
                            || (instanceof<Bullet>(temp2) && (instanceof<DEnemy>(temp1)|| instanceof<SEnemy>(temp1)))))
                        {
                            //distance between bullet and enemy 
                            int c = sqrt(((temp2->x + 1 - temp1->x) * (temp2->x + 1 - temp1->x) + (temp2->y - 1 - temp1->y) * (temp2->y - 1 - temp1->y)));
                            if (c < 10)
                            {
                                if (instanceof<Bullet>(temp1))
                                    //check if bullet can home
                                    if (((Bullet*)temp1)->Home())
                                        //check if bullet doesn't already have a target set
                                        if (((Bullet*)temp1)->GetTarget() == nullptr)
                                            ((Bullet*)temp1)->SetTarget(temp2);
                                else
                                    //check if bullet can home
                                    if (((Bullet*)temp2)->Home())
                                        //check if bullet doesn't already have a target set
                                        if(((Bullet*)temp2)->GetTarget()==nullptr)
                                            ((Bullet*)temp2)->SetTarget(temp1);
                            }
                        }

                    }
                }
            }
        }
        
        //MEMORY CLEANUP
        
        for (int i = 0; i< obj_list.Size(); i++)
        {
            if (obj_list.At(i)->Alive() == false)
            {
                obj_list.Erase(i);
                i--;
                if (i < 0)i = 0;
            }
        }
        //Writing output to console
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    
    return 0;
}
