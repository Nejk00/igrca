
#ifndef HPCOMPONENT_HPP
#define HPCOMPONENT_HPP



class HPComponent : public Component {
public:
    int healthPoints = 1;
    int maxHealthPoints = 3;
    HPComponent(int n) {
        healthPoints = n;
    }

    void init() override {

    }
    void update() override {
        if (healthPoints <= 0) {
            entity->deactivate();
        }
    }
};

#endif //HPCOMPONENT_HPP
