#ifndef OBJECT_H
#define OBJECT_H

#include<vector>
#include<utility>
#include<iostream>

#include"MathFunction.h"

class Object//这是个纯虚类，纯粹提供统一指针和用来被继承的
{
protected:
	static int No;

	float mass;
	sf::Vector2f velocity;
public:
	int serial_number;
	//static std::vector<Object*>objects;//所有新生成的图形都会被存在这个静态变量里，方便调用
	static std::vector<std::pair<Object*, Object*>>collision_pairs;//储存所有可能的碰撞对

	Object(const float& mass, const sf::Vector2f& velocity);
	~Object();

	const float& getMass();
	const sf::Vector2f& getVelocity();
	void appendVelocity(const sf::Vector2f& append_velocity);//原速度叠加新速度
	void setVelocity(const sf::Vector2f& set_velocity);//新速度覆盖原速度

	virtual const std::string shapeType() = 0;//返回代表图形种类的字符串，不同子类返回不同字符串

	void update(const float& dt, const sf::Vector2f& rendering_size, const sf::Vector2f& logical_size, const sf::Vector2f& offset);

	virtual void updateMovement(const float& dt) = 0;//根据当前速度和坐标刷新坐标
	virtual void updateShape(const sf::Vector2f& rendering_size, const sf::Vector2f& logical_size, const sf::Vector2f& offset) = 0;//根据当前坐标与缩放比例修改显示图像位置和大小
	//virtual void updateCollision() = 0;//判断碰撞，暂时还没写
	virtual void updateLogic() = 0;//一些放不进前两个函数的功能可以写进这里

	virtual void render(sf::RenderTarget& target) = 0;
};

#endif