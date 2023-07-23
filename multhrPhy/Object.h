#ifndef OBJECT_H
#define OBJECT_H

#include<vector>
#include<iostream>

#include"CollisionFunction.h"

class Object//���Ǹ������࣬�����ṩͳһָ����������̳е�
{
protected:
	float mass;
	sf::Vector2f velocity;
public:
	static std::vector<Object*>objects;//���������ɵ�ͼ�ζ��ᱻ���������̬������������

	Object(const float& mass, const sf::Vector2f velocity);
	~Object();

	const float& getMass();
	const sf::Vector2f& getVelocity();
	void appendVelocity(const sf::Vector2f& append_velocity);//ԭ�ٶȵ������ٶ�
	void setVelocity(const sf::Vector2f& set_velocity);//���ٶȸ���ԭ�ٶ�

	virtual const std::string shapeType() = 0;//���ش���ͼ��������ַ�������ͬ���෵�ز�ͬ�ַ���

	void update(const float& dt);

	virtual void updateMovement(const float& dt) = 0;//���ݵ�ǰ�ٶȺ�����ˢ������
	virtual void updateCollision() = 0;//�ж���ײ����ʱ��ûд
	virtual void updateLogic() = 0;//һЩ�Ų���ǰ���������Ĺ��ܿ���д������

	virtual void render(sf::RenderTarget& target) = 0;
};

#endif