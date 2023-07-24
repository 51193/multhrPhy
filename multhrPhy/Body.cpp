#include "Body.h"

void Body::init()
{
	this->initWindow();
	this->initFont();
	this->initStates();
}

void Body::initWindow()
{
	std::string title = "Game";
	sf::VideoMode window_bounds;
	sf::ContextSettings window_settings;

	window_bounds.width = sf::VideoMode::getDesktopMode().width;
	window_bounds.height = sf::VideoMode::getDesktopMode().height;
	this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Close, window_settings);
	this->window->setFramerateLimit(120);
	this->window->setVerticalSyncEnabled(false);
	window_settings.antialiasingLevel = 0;
}

void Body::initFont()
{
	std::ifstream input("Font/Font.ini", std::ios::in);
	std::string font_path;
	char buffer;

	if (input.is_open()) {

		buffer = input.get();
		while (buffer != EOF && buffer != '\n') {
			font_path += buffer;
			buffer = input.get();
		}
		this->font.loadFromFile(font_path);
	}

	input.close();
}

void Body::initStates()
{
	state = new State{};
}

void Body::updateDt()
{
	this->dt = this->dt_clock.restart().asSeconds();
}

void Body::updateSFMLEvent()
{
	while (this->window->pollEvent(this->event)) {
		if (this->event.type == sf::Event::Closed) {
			this->window->close();
		}
	}
}

Body::Body()
{
	this->init();
}

Body::~Body()
{
	delete this->window;
	delete this->state;
}

void Body::update()
{
	this->updateDt();
	this->updateSFMLEvent();

	this->state->update(this->dt);
}

void Body::render()
{
	this->window->clear();

	this->state->render(*this->window);

	this->window->display();
}

void Body::run()
{
	while (this->window->isOpen()) {
		this->update();
		this->render();
	}
}
