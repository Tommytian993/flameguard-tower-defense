#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

/**
 * @brief 二维向量类，用于表示二维空间中的点和向量
 *
 * 该类提供了二维向量的基本运算，包括加减乘除、长度计算、归一化等操作。
 * 主要用于游戏中的位置、速度、加速度等二维向量的表示和计算。
 */
class Vector2
{
public:
     double x = 0; // x坐标或分量
     double y = 0; // y坐标或分量

public:
     Vector2() = default;
     ~Vector2() = default;

     /**
      * @brief 构造函数
      * @param x x坐标或分量
      * @param y y坐标或分量
      */
     Vector2(double x, double y)
         : x(x), y(y) {}

     /**
      * @brief 向量加法
      * @param vec 要加的向量
      * @return 两个向量的和
      */
     Vector2 operator+(const Vector2 &vec) const
     {
          return Vector2(x + vec.x, y + vec.y);
     }

     /**
      * @brief 向量加法赋值
      * @param vec 要加的向量
      */
     void operator+=(const Vector2 &vec)
     {
          x += vec.x, y += vec.y;
     }

     /**
      * @brief 向量减法
      * @param vec 要减的向量
      * @return 两个向量的差
      */
     Vector2 operator-(const Vector2 &vec) const
     {
          return Vector2(x - vec.x, y - vec.y);
     }

     /**
      * @brief 向量减法赋值
      * @param vec 要减的向量
      */
     void operator-=(const Vector2 &vec)
     {
          x -= vec.x, y -= vec.y;
     }

     /**
      * @brief 向量点乘
      * @param vec 要点乘的向量
      * @return 两个向量的点积
      */
     double operator*(const Vector2 &vec) const
     {
          return x * vec.x + y * vec.y;
     }

     /**
      * @brief 向量数乘
      * @param val 要乘的标量
      * @return 向量与标量的乘积
      */
     Vector2 operator*(double val) const
     {
          return Vector2(x * val, y * val);
     }

     /**
      * @brief 向量数乘赋值
      * @param val 要乘的标量
      */
     void operator*=(double val)
     {
          x *= val, y *= val;
     }

     /**
      * @brief 向量相等比较
      * @param vec 要比较的向量
      * @return 两个向量是否相等
      */
     bool operator==(const Vector2 &vec) const
     {
          return x == vec.x && y == vec.y;
     }

     /**
      * @brief 向量长度比较（大于）
      * @param vec 要比较的向量
      * @return 当前向量长度是否大于目标向量长度
      */
     bool operator>(const Vector2 &vec) const
     {
          return length() > vec.length();
     }

     /**
      * @brief 向量长度比较（小于）
      * @param vec 要比较的向量
      * @return 当前向量长度是否小于目标向量长度
      */
     bool operator<(const Vector2 &vec) const
     {
          return length() < vec.length();
     }

     /**
      * @brief 计算向量长度
      * @return 向量的模长
      */
     double length() const
     {
          return sqrt(x * x + y * y);
     }

     /**
      * @brief 向量归一化
      * @return 归一化后的单位向量
      *
      * 如果向量长度接近零，则返回零向量
      */
     Vector2 normalize() const
     {
          double len = length();

          if (len == 0)
               return Vector2(0, 0);

          return Vector2(x / len, y / len);
     }

     /**
      * @brief 判断向量是否接近零向量
      * @return 向量长度是否小于阈值（0.0001）
      */
     bool approx_zero() const
     {
          return length() < 0.0001;
     }
};

#endif // !_VECTOR2_H_
