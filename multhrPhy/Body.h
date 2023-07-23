#ifndef BODY_H
#define BODY_H

#include<fstream>
#include<stack>

#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

#include"State.h"

class Body
{
private:

	//������ر���

	sf::RenderWindow* window;

	//�¼�����

	sf::Event event;

	//����ˢ����ر���

	sf::Clock dt_clock;
	float dt;

	//�������

	sf::Font font;

	//State����

	State* state;

	//��ʼ��

	void init();

	void initWindow();
	void initFont();
	void initStates();

	//��ɢupdate����

	void updateDt();
	void updateSFMLEvent();

public:
	//���캯������������

	Body();
	~Body();

	//���ܺ���

	void update();
	void render();
	void run();

};

#endif GAME_H