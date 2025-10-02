// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#ifndef RECT2_H_
#define RECT2_H_

#include "Vector2.h"
#include "Vector3.h"
#include <vector>

class Rect2
{
public:
    int x1;
    int y1;
    int x2;
    int y2;

    Rect2() : x1(), y1(), x2(), y2(), sizetemp_() {};
    ~Rect2() = default;

    inline Rect2(const Vector2& _lt, const Vector2& _rb)
    {
        x1 = static_cast<int>(_lt.x);
        y1 = static_cast<int>(_lt.y);
        x2 = static_cast<int>(_rb.x);
        y2 = static_cast<int>(_rb.y);
        sizetemp_ = 0;
        return;
    }

    Rect2(const Vector3&) = delete;

    Vector2 LeftTop()       { return Vector2(x1, y1); }
    Vector2 RightTop()      { return Vector2(x2, y1); }
    Vector2 RightBottom()   { return Vector2(x2, y2); }
    Vector2 LeftBottom()    { return Vector2(x1, y2); }

    std::vector<Vector2> GetVertices();

    void MakeSquare(int _size, bool _centerMode = true);
    void MakeRectangle(int _width, int _height, bool _centerMode = true);
    int GetSize() const { return sizetemp_; }

    Rect2 operator+(const Vector2& _pos);

private:
    int sizetemp_;
};

#endif // RECT2_H