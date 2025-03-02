#include"Vector2D.hpp"

Vector2D::Vector2D() {
    x=0.0f;
    y=0.0f;
}

Vector2D::Vector2D(float x, float y) {
    this->x=x;
    this->y=y;
}

Vector2D& Vector2D :: Add(const Vector2D &v) {
    this->x+=v.x;
    this->y+=v.y;
    return *this;
}

Vector2D& Vector2D :: Sub(const Vector2D &v) {
    this->x-=v.x;
    this->y-=v.y;
    return *this;
}

Vector2D& Vector2D :: Mult(const Vector2D &v) {
    this->x*=v.x;
    this->y*=v.y;
    return *this;
}

Vector2D& Vector2D::Dev(const Vector2D &v) {
    if (v.x == 0 || v.y == 0) {
        // Return a default vector or leave the current vector unchanged
        this->x = 0;
        this->y = 0;
        return *this;
    }
    this->x /= v.x;
    this->y /= v.y;
    return *this;
}



Vector2D&  operator+(Vector2D &v1, const Vector2D &v2) {
    return v1.Add(v2);
}

Vector2D&  operator-(Vector2D &v1, const Vector2D &v2) {
    return v1.Sub(v2);
}

Vector2D&  operator*(Vector2D &v1, const Vector2D &v2) {
    return v1.Mult(v2);
}

Vector2D&  operator/(Vector2D &v1, const Vector2D &v2) {
    return v1.Dev(v2);
}


Vector2D& Vector2D :: operator+=(const Vector2D &v) {
    return this->Add(v);
}
Vector2D& Vector2D :: operator-=(const Vector2D &v) {
    return this->Sub(v);
}

Vector2D& Vector2D :: operator*=(const Vector2D &v) {
    return this->Mult(v);
}

Vector2D& Vector2D :: operator/=(const Vector2D &v) {
    return this->Dev(v);
}

Vector2D& Vector2D :: operator*(const int& i) {
    this->x*=i;
    this->y*=i;
    return *this;
}
bool Vector2D :: operator==(const Vector2D &v) {
    return (this->x==v.x && this->y==v.y);
}

Vector2D &Vector2D::Zero() {
    this->x=0;
    this->y=0;
    return *this;
}



