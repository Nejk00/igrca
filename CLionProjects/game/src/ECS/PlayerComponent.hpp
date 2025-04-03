

#ifndef PLAYERCOMPONENT_HPP
#define PLAYERCOMPONENT_HPP


class PlayerComponent : public Component {
    public:
    int ammo_count = 5;
    int max_ammo = 5;
    int animals_saved = 0;

    void init() override {

    }
    void update() override {

    }
};




#endif //PLAYERCOMPONENT_HPP
