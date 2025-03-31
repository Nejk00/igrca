#include<iostream>
#include<vector>
#include<memory>
#include<algorithm>
#include<bitset>
#include<array>

#ifndef ESC_HPP
#define ESC_HPP

class Component;
class Entity;
class Manager;

using ComponentID = std :: size_t;
using Group = std :: size_t;

inline ComponentID GetNewComponentTypeID() {
    static ComponentID lastID = 0u;
    return lastID++;
}

template<typename T> inline ComponentID getComponentTypeID()noexcept {
    static ComponentID typeID = GetNewComponentTypeID();
    return typeID;
}

constexpr std :: size_t maxComponents = 32;
constexpr std :: size_t maxGroups = 32;

using ComponentBtSet = std :: bitset<maxComponents>;
using GroupBtSet = std :: bitset<maxGroups>;
using ComponentArray = std :: array<Component*, maxComponents>;

class Component {
    public:
    Entity* entity;

    virtual void init() {

    }
    virtual void update() {

    }
    virtual void draw() {

    }
    virtual ~Component() {

    }
};

class Entity {
    private:
    Manager& manager;
    bool active = true;
    bool pooled = false;
    std :: vector<std :: unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBtSet componentBtSet;
    GroupBtSet groupBtSet;
    public:
    Entity(Manager& mManager)  : manager(mManager) {}


    void update() {
        for (auto& c : components)c -> update();

    }
    void draw() {
        for (auto& c : components)c -> draw();

    }
    bool isActive() const {return active;}
    bool isPooled() const {return pooled;}

    void deactivate() {
        active = false;
        pooled = true;
    }
    void reactivate() {
        active = true;
        pooled = false;
    }
    bool hasGroup(Group mGroup) {return groupBtSet[mGroup];}
    void addGroup(Group mGroup);
    void delGroup(Group mGroup){groupBtSet[mGroup]=false;}

    template<typename T> bool hasComponent() const {
        return componentBtSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c(new T(std :: forward<TArgs>(mArgs)...));
        c->entity = this;
        std :: unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBtSet[getComponentTypeID<T>()] = true;

        c -> init();
        return *c;

    }

    template <typename T> T& getComponent() const {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

};

class Manager {
    private:
    std :: vector<std :: unique_ptr<Entity>> entities;
    std :: array<std::vector<Entity*>, maxGroups> groupedEntities;
    public:
    void update() {
        for (auto& e : entities) e->update();
    }
    void draw() {
        for (auto& e : entities) e->draw();
    }

    void refresh() {
    for (auto i(0u); i < maxGroups; i++) {
        auto& v(groupedEntities[i]);
        v.erase(
            std::remove_if(std :: begin(v),std :: end(v),
                [i](Entity* mEntity) {
                    return !mEntity->isActive() || !mEntity->hasGroup(i)/* && !mEntity->isPooled()*/;
        }),std::end(v));
    }

        entities.erase(std::remove_if(std :: begin(entities), std :: end(entities), [](const std :: unique_ptr<Entity> &mEntity) {
            return !mEntity -> isActive() /*&& !mEntity -> isPooled()*/;
        }),
        std :: end(entities));
    }

    void AddToGroup(Entity* mEntity, Group mGroup) {
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    std::vector<Entity*>& getGroup(Group mGroup) {
        return groupedEntities[mGroup];
    }

    Entity& addEntity() {
        for (auto& e : entities) {
            if (e->isPooled()) {
                e->reactivate();
                return *e;
            }
        }
        Entity* e = new Entity(*this);
        std :: unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};

#endif //ESC_HPP

