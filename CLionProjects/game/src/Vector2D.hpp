

#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP


class Vector2D {
public:
    float x, y;

    Vector2D();
    Vector2D(float x, float y);

    Vector2D& Add(const Vector2D &v);
    Vector2D& Sub(const Vector2D &v);
    Vector2D& Mult(const Vector2D &v);
    Vector2D& Dev(const Vector2D &v);

    friend Vector2D& operator+(Vector2D &v1, const Vector2D &v2);
    friend Vector2D& operator-(Vector2D &v1, const Vector2D &v2);
    friend Vector2D& operator*(Vector2D &v1, const Vector2D &v2);
    friend Vector2D& operator/(Vector2D &v1, const Vector2D &v2);

    Vector2D& operator+=(const Vector2D &v);
    Vector2D& operator-=(const Vector2D &v);
    Vector2D& operator*=(const Vector2D &v);
    Vector2D& operator/=(const Vector2D &v);

    Vector2D& operator*(const int& i);
    Vector2D& Zero();
};

#endif //VECTOR2D_HPP
