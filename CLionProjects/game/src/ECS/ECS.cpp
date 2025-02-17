#include"ECS.hpp"
void Entity::addGroup(Group mGroup) {
    groupBtSet[mGroup] = true;
    manager.AddToGroup(this, mGroup);
}

