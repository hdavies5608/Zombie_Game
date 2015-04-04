#pragma once
#include "Human.h"

#include <IceEngine/InputManager.h>
#include <IceEngine/Camera2D.h>
class Gun;
class Bullet;
class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, IceEngine::InputManager* inputManager, IceEngine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string> &levelData,
		std::vector<Human*> &humans,
		std::vector<Zombie*> &zombies,
		float deltaTime) override;
    float getSpeed() { return _speed; };
    void setSpeed(float speed) { _speed = speed; };
private:
	IceEngine::InputManager* _inputManager;
	IceEngine::Camera2D* _camera;
	std::vector<Gun*>  _guns;
	std::vector<Bullet>* _bullets;
	int _currentGunIndex;
};

