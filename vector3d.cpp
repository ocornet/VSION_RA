//
//  vector3d.cpp
//  testCamera
//
//  Created by Jean-Marie Normand on 29/09/2014.
//  Copyright (c) 2014 Jean-Marie Normand. All rights reserved.
//


#include "vector3d.h"
#include <cmath>
#define PI 3.14159

Vector3D::Vector3D()
{
    vX = 0;
    vY = 0;
    vZ = 0;
}

Vector3D::Vector3D(double x,double y,double z=0)
{
    vX = x;
    vY = y;
    vZ = z;
}

Vector3D::Vector3D(const Vector3D & v)
{
    vX = v.vX;
    vY = v.vY;
    vZ = v.vZ;
}

Vector3D::Vector3D(const Vector3D & from,const Vector3D & to)
{
    vX = to.vX - from.vX;
    vY = to.vY - from.vY;
    vZ = to.vZ - from.vZ;
}

Vector3D & Vector3D::operator= (const Vector3D & v)
{
    vX = v.vX;
    vY = v.vY;
    vZ = v.vZ;
    return *this;
}

Vector3D & Vector3D::operator+= (const Vector3D & v)
{
    vX += v.vX;
    vY += v.vY;
    vZ += v.vZ;
    return *this;
}

Vector3D Vector3D::operator+ (const Vector3D & v) const
{
    Vector3D t = *this;
    t += v;
    return t;
}

Vector3D & Vector3D::operator-= (const Vector3D & v)
{
    vX -= v.vX;
    vY -= v.vY;
    vZ -= v.vZ;
    return *this;
}

Vector3D Vector3D::operator- (const Vector3D & v) const
{
    Vector3D t = *this;
    t -= v;
    return t;
}

Vector3D & Vector3D::operator*= (const double a)
{
    vX *= a;
    vY *= a;
    vZ *= a;
    return *this;
}

Vector3D Vector3D::operator* (const double a)const
{
    Vector3D t = *this;
    t *= a;
    return t;
}

Vector3D operator* (const double a,const Vector3D & v)
{
    return Vector3D(v.vX*a,v.vY*a,v.vZ*a);
}

Vector3D & Vector3D::operator/= (const double a)
{
    vX /= a;
    vY /= a;
    vZ /= a;
    return *this;
}

Vector3D Vector3D::operator/ (const double a)const
{
    Vector3D t = *this;
    t /= a;
    return t;
}

// Setters/Getters
void Vector3D::setX(double x) {
   vX = x;
}

double Vector3D::getX() const{
   return vX;
}

void Vector3D::setY(double y) {
   vY = y;
}

double Vector3D::getY() const{
   return vY;
}

void Vector3D::setZ(double z) {
   vZ = z;
}

double Vector3D::getZ() const{
   return vZ;
}

Vector3D Vector3D::crossProduct(const Vector3D & v)const
{
    Vector3D t;
    t.setX(vY*v.getZ() - vZ*v.getY());
    t.setY(vZ*v.getX() - vX*v.getZ());
    t.setZ(vX*v.getY() - vY*v.getX());
    return t;
}

double Vector3D::length()const
{
    return sqrt( vX*vX + vY*vY + vZ*vZ);
}

Vector3D  Vector3D::normalized() const
{
    return (*this) / length();
  
}

void Vector3D::translate(Vector3D v){
	this->operator+=(v);
}

float Vector3D::angle(Vector3D v1, Vector3D v2){
	return atan2(v2.vZ-v1.vZ,v2.vX - v1.vX);
}


float Vector3D::random(float min, float max){
	long r = (long)(1000*min) + 10*rand()%(long)((max-min)*1000);
	return (float)r/1000;
}


