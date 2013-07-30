/*

	Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MATH_3D_H
#define	MATH_3D_H

#include <stdio.h>
#ifdef WIN32
#define _USE_MATH_DEFINES 
#include <cmath>
#else
#include <math.h>
#endif

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

struct Vector2i
{
    int x;
    int y;
};

struct Vector2f
{
    float x;
    float y;

    Vector2f()
    {
    }

    Vector2f(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    Vector2f operator-() const
    {
        return Vector2f(-x, -y);
    }

    Vector2f operator+(const Vector2f &r) const
    {
        Vector2f temp = Vector2f(x, y);
        temp += r;
        return temp;
    }

    Vector2f operator-(const Vector2f &r) const
    {
        Vector2f temp = Vector2f(x, y);
        temp -= r;
        return temp;
    }

    Vector2f operator+=(const Vector2f &r)
    {
        x += r.x;
        y += r.y;
        return *this;
    }

    Vector2f operator-=(const Vector2f &r)
    {
        x -= r.x;
        y -= r.y;
        return *this;
    }
};


struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f()
    {
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f operator + ( const Vector3f &r )
    {
        const float _x = x + r.x;
        const float _y = y + r.y;
        const float _z = z + r.z;

        return Vector3f(_x, _y, _z);
    }

    Vector3f operator + ( void ) const
    {
        return *this;
    }

    Vector3f operator - ( const Vector3f &r )
    {
        const float _x = x - r.x;
        const float _y = y - r.y;
        const float _z = z - r.z;

        return Vector3f(_x, _y, _z);
    }

    Vector3f operator - ( void ) const
    {
        return Vector3f(-x, -y, -z);
    }

    Vector3f operator * ( const Vector3f &r )
    {
        const float _x = x * r.x;
        const float _y = y * r.y;
        const float _z = z * r.z;

        return Vector3f(_x, _y, _z);
    }

    Vector3f operator * ( const float scalar )
    {
        const float _x = x * scalar;
        const float _y = y * scalar;
        const float _z = z * scalar;

        return Vector3f(_x, _y, _z);
    }

    Vector3f operator / ( const Vector3f &r )
    {
        const float _x = x / r.x;
        const float _y = y / r.y;
        const float _z = z / r.z;

        return Vector3f(_x, _y, _z);
    }

    Vector3f operator+=(const Vector3f& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

    Vector3f operator-=(const Vector3f& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

    Vector3f operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    Vector3f Cross(const Vector3f& v) const;

    Vector3f& Normalize();

    void Rotate(float Angle, const Vector3f& Axis);

    void Print() const
    {
        printf("(%.02f, %.02f, %.02f)", x, y, z);
    }
};


struct Vector4f
{
    float x;
    float y;
    float z;
    float w;

    Vector4f()
    {        
    }
    
    Vector4f(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    
    void Print() const
    {
        printf("(%.02f, %.02f, %.02f, %.02f)", x, y, z, w);
    }       
};

inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z);

    return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z);

    return Ret;
}

inline Vector3f operator*(const Vector3f& l, float f)
{
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
}

struct PersProjInfo
{
    float FOV;
    float Width;
    float Height;
    float zNear;
    float zFar;
};

class Matrix4f
{
public:
    float m_matrix[4][4];

    Matrix4f()
    {        
    }

    Matrix4f(
        const float &aw, const float &ax, const float &ay, const float &az,
        const float &bw, const float &bx, const float &by, const float &bz,
        const float &cw, const float &cx, const float &cy, const float &cz,
        const float &dw, const float &dx, const float &dy, const float &dz)
    {
        m_matrix[0][0] = aw;
        m_matrix[1][0] = ax;
        m_matrix[2][0] = ay;
        m_matrix[3][0] = az;

        m_matrix[0][1] = bw;
        m_matrix[1][1] = bx;
        m_matrix[2][1] = by;
        m_matrix[3][1] = bz;

        m_matrix[0][2] = cw;
        m_matrix[1][2] = cx;
        m_matrix[2][2] = cy;
        m_matrix[3][2] = cz;

        m_matrix[0][3] = dw;
        m_matrix[1][3] = dx;
        m_matrix[2][3] = dy;
        m_matrix[3][3] = dz;
    }


    Matrix4f Transpose() const
    {
        Matrix4f n;
        
        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                n.m_matrix[i][j] = m_matrix[j][i];
            }
        }
        
        return n;
    }


    inline void InitIdentity()
    {
        m_matrix[0][0] = 1.0f; m_matrix[0][1] = 0.0f; m_matrix[0][2] = 0.0f; m_matrix[0][3] = 0.0f;
        m_matrix[1][0] = 0.0f; m_matrix[1][1] = 1.0f; m_matrix[1][2] = 0.0f; m_matrix[1][3] = 0.0f;
        m_matrix[2][0] = 0.0f; m_matrix[2][1] = 0.0f; m_matrix[2][2] = 1.0f; m_matrix[2][3] = 0.0f;
        m_matrix[3][0] = 0.0f; m_matrix[3][1] = 0.0f; m_matrix[3][2] = 0.0f; m_matrix[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
        Matrix4f Ret;

        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                Ret.m_matrix[i][j] = m_matrix[i][0] * Right.m_matrix[0][j] +
                              m_matrix[i][1] * Right.m_matrix[1][j] +
                              m_matrix[i][2] * Right.m_matrix[2][j] +
                              m_matrix[i][3] * Right.m_matrix[3][j];
            }
        }

        return Ret;
    }

    Vector4f operator*(const Vector4f& v) const
    {
        Vector4f r;
        
        r.x = m_matrix[0][0]* v.x + m_matrix[0][1]* v.y + m_matrix[0][2]* v.z + m_matrix[0][3]* v.w;
        r.y = m_matrix[1][0]* v.x + m_matrix[1][1]* v.y + m_matrix[1][2]* v.z + m_matrix[1][3]* v.w;
        r.z = m_matrix[2][0]* v.x + m_matrix[2][1]* v.y + m_matrix[2][2]* v.z + m_matrix[2][3]* v.w;
        r.w = m_matrix[3][0]* v.x + m_matrix[3][1]* v.y + m_matrix[3][2]* v.z + m_matrix[3][3]* v.w;
        
        return r;
    }

    float determinant() const
    {
        #define m(i,j) m_matrix[j][i]
        #define term(a,b,c,d) m(0,a)*m(1,b)*m(2,c)*m(3,d)

        return
        + term(0,1,2,3) - term(0,1,3,2)
        - term(0,2,1,3) + term(0,2,3,1)
        + term(0,3,1,2) + term(0,3,2,1)

        - term(1,0,2,3) + term(1,0,3,2)
        + term(1,2,0,3) - term(1,2,3,0)
        - term(1,3,0,2) + term(1,3,2,0)

        + term(2,0,1,3) - term(2,0,3,1)
        - term(2,1,0,3) + term(2,1,3,0)
        + term(2,3,0,1) - term(2,3,1,0)

        - term(3,0,1,2) + term(3,0,2,1)
        + term(3,1,0,2) - term(3,1,2,0)
        - term(3,2,0,1) + term(3,2,1,0);

        #undef term
        #undef m
    }

   Matrix4f inverted() const
   {
     const float det_M = determinant();

 #define m(i,j) (m_matrix[j][i])

     const float m00 = (m(1,2)*m(2,3)*m(3,1) - m(1,3)*m(2,2)*m(3,1) + m(1,3)*m(2,1)*m(3,2) - m(1,1)*m(2,3)*m(3,2) - m(1,2)*m(2,1)*m(3,3) + m(1,1)*m(2,2)*m(3,3)) / det_M;
     const float m01 = (m(0,3)*m(2,2)*m(3,1) - m(0,2)*m(2,3)*m(3,1) - m(0,3)*m(2,1)*m(3,2) + m(0,1)*m(2,3)*m(3,2) + m(0,2)*m(2,1)*m(3,3) - m(0,1)*m(2,2)*m(3,3)) / det_M;
     const float m02 = (m(0,2)*m(1,3)*m(3,1) - m(0,3)*m(1,2)*m(3,1) + m(0,3)*m(1,1)*m(3,2) - m(0,1)*m(1,3)*m(3,2) - m(0,2)*m(1,1)*m(3,3) + m(0,1)*m(1,2)*m(3,3)) / det_M;
     const float m03 = (m(0,3)*m(1,2)*m(2,1) - m(0,2)*m(1,3)*m(2,1) - m(0,3)*m(1,1)*m(2,2) + m(0,1)*m(1,3)*m(2,2) + m(0,2)*m(1,1)*m(2,3) - m(0,1)*m(1,2)*m(2,3)) / det_M;
     const float m10 = (m(1,3)*m(2,2)*m(3,0) - m(1,2)*m(2,3)*m(3,0) - m(1,3)*m(2,0)*m(3,2) + m(1,0)*m(2,3)*m(3,2) + m(1,2)*m(2,0)*m(3,3) - m(1,0)*m(2,2)*m(3,3)) / det_M;
     const float m11 = (m(0,2)*m(2,3)*m(3,0) - m(0,3)*m(2,2)*m(3,0) + m(0,3)*m(2,0)*m(3,2) - m(0,0)*m(2,3)*m(3,2) - m(0,2)*m(2,0)*m(3,3) + m(0,0)*m(2,2)*m(3,3)) / det_M;
     const float m12 = (m(0,3)*m(1,2)*m(3,0) - m(0,2)*m(1,3)*m(3,0) - m(0,3)*m(1,0)*m(3,2) + m(0,0)*m(1,3)*m(3,2) + m(0,2)*m(1,0)*m(3,3) - m(0,0)*m(1,2)*m(3,3)) / det_M;
     const float m13 = (m(0,2)*m(1,3)*m(2,0) - m(0,3)*m(1,2)*m(2,0) + m(0,3)*m(1,0)*m(2,2) - m(0,0)*m(1,3)*m(2,2) - m(0,2)*m(1,0)*m(2,3) + m(0,0)*m(1,2)*m(2,3)) / det_M;
     const float m20 = (m(1,1)*m(2,3)*m(3,0) - m(1,3)*m(2,1)*m(3,0) + m(1,3)*m(2,0)*m(3,1) - m(1,0)*m(2,3)*m(3,1) - m(1,1)*m(2,0)*m(3,3) + m(1,0)*m(2,1)*m(3,3)) / det_M;
     const float m21 = (m(0,3)*m(2,1)*m(3,0) - m(0,1)*m(2,3)*m(3,0) - m(0,3)*m(2,0)*m(3,1) + m(0,0)*m(2,3)*m(3,1) + m(0,1)*m(2,0)*m(3,3) - m(0,0)*m(2,1)*m(3,3)) / det_M;
     const float m22 = (m(0,1)*m(1,3)*m(3,0) - m(0,3)*m(1,1)*m(3,0) + m(0,3)*m(1,0)*m(3,1) - m(0,0)*m(1,3)*m(3,1) - m(0,1)*m(1,0)*m(3,3) + m(0,0)*m(1,1)*m(3,3)) / det_M;
     const float m23 = (m(0,3)*m(1,1)*m(2,0) - m(0,1)*m(1,3)*m(2,0) - m(0,3)*m(1,0)*m(2,1) + m(0,0)*m(1,3)*m(2,1) + m(0,1)*m(1,0)*m(2,3) - m(0,0)*m(1,1)*m(2,3)) / det_M;
     const float m30 = (m(1,2)*m(2,1)*m(3,0) - m(1,1)*m(2,2)*m(3,0) - m(1,2)*m(2,0)*m(3,1) + m(1,0)*m(2,2)*m(3,1) + m(1,1)*m(2,0)*m(3,2) - m(1,0)*m(2,1)*m(3,2)) / det_M;
     const float m31 = (m(0,1)*m(2,2)*m(3,0) - m(0,2)*m(2,1)*m(3,0) + m(0,2)*m(2,0)*m(3,1) - m(0,0)*m(2,2)*m(3,1) - m(0,1)*m(2,0)*m(3,2) + m(0,0)*m(2,1)*m(3,2)) / det_M;
     const float m32 = (m(0,2)*m(1,1)*m(3,0) - m(0,1)*m(1,2)*m(3,0) - m(0,2)*m(1,0)*m(3,1) + m(0,0)*m(1,2)*m(3,1) + m(0,1)*m(1,0)*m(3,2) - m(0,0)*m(1,1)*m(3,2)) / det_M;
     const float m33 = (m(0,1)*m(1,2)*m(2,0) - m(0,2)*m(1,1)*m(2,0) + m(0,2)*m(1,0)*m(2,1) - m(0,0)*m(1,2)*m(2,1) - m(0,1)*m(1,0)*m(2,2) + m(0,0)*m(1,1)*m(2,2)) / det_M;

 #undef m

     return Matrix4f(
       m00, m01, m02, m03,
       m10, m11, m12, m13,
       m20, m21, m22, m23,
       m30, m31, m32, m33);
    }

    Matrix4f & invert()
    {
        return *this = inverted();
    }

    Matrix4f Perspective(const float &fov_rad_y, const float &aspect, const float &near_, const float &far_)
    {
#define cotangent(angle) (1.0f / tanf(angle))
        const float f = float(cotangent(0.5f * fov_rad_y));
#undef cotangent
        const float denom = near_ - far_;

        return Matrix4f(f / aspect, 0.0f, 0.0f, 0.0f,
                        0.0f, f, 0.0f, 0.0f,
                        0.0f, 0.0f, (far_ + near_) / denom, 2.0f * far_ * near_ / denom,
                        0.0f, 0.0f, -1.0f, 0.0f);
    }

    void Print() const
    {
        for (int i = 0 ; i < 4 ; i++) {
            printf("%f %f %f %f\n", m_matrix[i][0], m_matrix[i][1], m_matrix[i][2], m_matrix[i][3]);
        }       
    }

    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
    void InitPersProjTransform(const PersProjInfo& p);
    void InitOrthoTransform(float left, float right, float bottom, float top, float near, float far);
};


struct Quaternion
{
    float x, y, z, w;

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate();  
 };

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);

struct Ray
{
    Vector3f Origin;
    Vector3f Destination;
    bool Hit;
    Vector2f HitCoords;
    float HitMult;
    float Threshold;

    // 3D Equasion
    inline Ray(Vector3f origin, Vector3f dest)
    {
        Origin = origin;
        Destination = dest;
        Hit = false;
        Threshold = 0.001f;
    }

    // 2D Equasion
    inline Ray(Vector2f origin, Vector2f dest)
    {
        Origin = Vector3f(origin.x, origin.y ,0.0f);
        Destination = Vector3f(dest.x, dest.y ,0.0f);
        Hit = false;
        Threshold = 0.001f;
    }

    inline Ray()
    {
        Threshold = 0.001f;
        Hit = false;
    }

    inline Vector3f HitPos()
    {
        return Origin * (1 - HitMult) + Destination * HitMult;
    }
};

#endif	/* MATH_3D_H */

