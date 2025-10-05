#include "pch.h"
#include "PhysicsSystem.h"
#include "HealthComponent.h"

namespace XYZEngine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
		// std::cout << "PhysicsSystem Update: " << colliders.size() << " colliders" << std::endl;
		
		for (int i = 0; i < colliders.size(); i++)
		{
			auto bodyA = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
			
			for (int j = 0; j < colliders.size(); j++)
			{
				if (j == i)
				{
					continue;
				}
				
				auto bodyB = colliders[j]->GetGameObject()->GetComponent<RigidbodyComponent>();
				
				// Пропускаем коллизии между двумя кинематическими объектами
				if (bodyA && bodyB && bodyA->GetKinematic() && bodyB->GetKinematic())
				{
					continue;
				}

				// Пропускаем коллизии между игроком и врагом (живыми)
				std::string nameA = colliders[i]->GetGameObject()->GetName();
				std::string nameB = colliders[j]->GetGameObject()->GetName();
				bool isPlayerEnemyCollision = (nameA == "Player" && nameB == "Enemy") || (nameA == "Enemy" && nameB == "Player");
				
				if (isPlayerEnemyCollision)
				{
					// Полностью пропускаем коллизии между игроком и врагом
					continue;
				}

				// Проверяем коллизию с расширенными границами для предотвращения туннелирования
				sf::FloatRect boundsA = colliders[i]->bounds;
				sf::FloatRect boundsB = colliders[j]->bounds;
				
				// Расширяем границы на максимальную скорость * время кадра для обнаружения туннелирования
				if (bodyA && !bodyA->GetKinematic())
				{
					Vector2Df velocity = bodyA->GetLinearVelocity();
					float maxSpeed = std::max(abs(velocity.x), abs(velocity.y));
					boundsA.left -= maxSpeed * fixedDeltaTime;
					boundsA.top -= maxSpeed * fixedDeltaTime;
					boundsA.width += 2 * maxSpeed * fixedDeltaTime;
					boundsA.height += 2 * maxSpeed * fixedDeltaTime;
				}

				sf::FloatRect intersection;
				if (boundsA.intersects(boundsB, intersection))
				{
					// Проверяем, не находится ли объект уже внутри другого (туннелирование)
					bool isInside = (colliders[i]->bounds.left >= colliders[j]->bounds.left &&
									 colliders[i]->bounds.top >= colliders[j]->bounds.top &&
									 colliders[i]->bounds.left + colliders[i]->bounds.width <= colliders[j]->bounds.left + colliders[j]->bounds.width &&
									 colliders[i]->bounds.top + colliders[i]->bounds.height <= colliders[j]->bounds.top + colliders[j]->bounds.height);
					
					if (isInside)
					{
						// Если объект внутри стены, выталкиваем его наружу
						auto transform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();
						if (transform && bodyA && !bodyA->GetKinematic())
						{
							Vector2Df playerCenter = { colliders[i]->bounds.left + colliders[i]->bounds.width / 2, 
													   colliders[i]->bounds.top + colliders[i]->bounds.height / 2 };
							Vector2Df wallCenter = { colliders[j]->bounds.left + colliders[j]->bounds.width / 2, 
													colliders[j]->bounds.top + colliders[j]->bounds.height / 2 };
							
							Vector2Df direction = playerCenter - wallCenter;
							if (abs(direction.x) > abs(direction.y))
							{
								// Выталкиваем по X
								float distance = (colliders[i]->bounds.width + colliders[j]->bounds.width) / 2 + 2.0f;
								if (direction.x > 0)
									transform->MoveBy({ distance, 0 });
								else
									transform->MoveBy({ -distance, 0 });
							}
							else
							{
								// Выталкиваем по Y
								float distance = (colliders[i]->bounds.height + colliders[j]->bounds.height) / 2 + 2.0f;
								if (direction.y > 0)
									transform->MoveBy({ 0, distance });
								else
									transform->MoveBy({ 0, -distance });
							}
						}
					}
					
					// Обычная обработка коллизии
					// std::cout << "Collision detected between " << colliders[i]->GetGameObject()->GetName() 
					//		  << " and " << colliders[j]->GetGameObject()->GetName() << std::endl;
					if (colliders[i]->isTrigger != colliders[j]->isTrigger)
					{
						if (triggersEnteredPair.find(colliders[i]) == triggersEnteredPair.end() && triggersEnteredPair.find(colliders[j]) == triggersEnteredPair.end())
						{
							auto trigger = new Trigger(colliders[i], colliders[j]);
							colliders[i]->OnTriggerEnter(*trigger);
							colliders[j]->OnTriggerEnter(*trigger);

							triggersEnteredPair.emplace(colliders[i], colliders[j]);
						}
					}
					else if (!colliders[i]->isTrigger)
					{
						// Обычная обработка коллизии для всех объектов
							float intersectionWidth = intersection.width;
							float intersectionHeight = intersection.height;
							Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth, intersection.top - 0.5f * intersectionHeight };

							// Определяем, какой объект нужно двигать (некинематический)
							TransformComponent* transformToMove = nullptr;
							ColliderComponent* colliderToMove = nullptr;
							
							if (!bodyA || !bodyA->GetKinematic())
							{
								transformToMove = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();
								colliderToMove = colliders[i];
							}
							else if (!bodyB || !bodyB->GetKinematic())
							{
								transformToMove = colliders[j]->GetGameObject()->GetComponent<TransformComponent>();
								colliderToMove = colliders[j];
							}
						
						if (transformToMove && colliderToMove)
						{
							Vector2Df position = { colliderToMove->bounds.left, colliderToMove->bounds.top };
							Vector2Df separation;

							if (intersectionWidth > intersectionHeight)
							{
								if (intersectionPosition.y > position.y)
								{
									separation = { 0, -intersectionHeight - 1.0f }; // Добавляем небольшой зазор
									// std::cout << "Top collision" << std::endl;
								}
								else
								{
									separation = { 0, intersectionHeight + 1.0f };
									// std::cout << "Down collision" << std::endl;
								}
							}
							else
							{
								if (intersectionPosition.x > position.x)
								{
									separation = { -intersectionWidth - 1.0f, 0.f };
									// std::cout << "Right collision" << std::endl;
								}
								else
								{
									separation = { intersectionWidth + 1.0f, 0.f };
									// std::cout << "Left collision" << std::endl;
								}
							}
							
							// Применяем разделение к Transform
							transformToMove->MoveBy(separation);
							
							// Останавливаем движение Rigidbody в направлении коллизии
							auto rigidbody = colliderToMove->GetGameObject()->GetComponent<RigidbodyComponent>();
							if (rigidbody)
							{
								Vector2Df velocity = rigidbody->GetLinearVelocity();
								if (abs(separation.x) > abs(separation.y))
								{
									velocity.x = 0; // Останавливаем горизонтальное движение
								}
								else
								{
									velocity.y = 0; // Останавливаем вертикальное движение
								}
								rigidbody->SetLinearVelocity(velocity);
							}
						}

						auto collision = new Collision(colliders[i], colliders[j], intersection);
						colliders[i]->OnCollision(*collision);
						colliders[j]->OnCollision(*collision);
					}
				}
			}
		}

		for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
			if (!triggeredPair->first->bounds.intersects(triggeredPair->second->bounds))
			{
				auto trigger = new Trigger(triggeredPair->first, triggeredPair->second);
				triggeredPair->first->OnTriggerExit(*trigger);
				triggeredPair->second->OnTriggerExit(*trigger);

				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		std::cout << "Subscribe " << collider << std::endl;
		colliders.push_back(collider);
	}
	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		std::cout << "Unsubscribe " << collider << std::endl;

		colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), colliders.end());
	}
}