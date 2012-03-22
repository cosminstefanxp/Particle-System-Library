/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/

#ifndef Vector4D_H
#define Vector4D_H

#include "glut.h"
#include <math.h>

class Vector4D
{
public:
	// Data
	GLfloat x, y, z, a;

	// Constructors
	Vector4D( GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _a) : x( _x ), y( _y ), z( _z ), a( _a)
		{
		}
	Vector4D( ) : x(0), y(0), z(0), a(0)
		{
		}

	// Operator Overloads
	inline bool operator== (const Vector4D& V2) const 
		{
		return (x == V2.x && y == V2.y && z == V2.z && a == V2.a);
		}

	inline Vector4D operator+ (const Vector4D& V2) const 
		{
		return Vector4D( x + V2.x,  y + V2.y,  z + V2.z, a + V2.a);
		}
	inline Vector4D operator- (const Vector4D& V2) const
		{
		return Vector4D( x - V2.x,  y - V2.y,  z - V2.z, a - V2.a);
		}
	inline Vector4D operator- ( ) const
		{
		return Vector4D(-x, -y, -z, -a);
		}

	inline Vector4D operator/ (GLfloat S ) const
		{
		GLfloat fInv = 1.0f / S;
		return Vector4D (x * fInv , y * fInv, z * fInv, a * fInv);
		}
	inline Vector4D operator/ (const Vector4D& V2) const
		{
		return Vector4D (x / V2.x,  y / V2.y,  z / V2.z, a / V2.a);
		}
	inline Vector4D operator* (const Vector4D& V2) const
		{
		return Vector4D (x * V2.x,  y * V2.y,  z * V2.z, a * V2.a);
		}
	inline Vector4D operator* (GLfloat S) const
		{
		return Vector4D (x * S,  y * S,  z * S, a * S);
		}

	inline void operator+= ( const Vector4D& V2 )
		{
		x += V2.x;
		y += V2.y;
		z += V2.z;
		a += V2.a;
		}
	inline void operator-= ( const Vector4D& V2 )
		{
		x -= V2.x;
		y -= V2.y;
		z -= V2.z;
		a -= V2.a;
		}

	inline GLfloat operator[] ( int i )
		{
		if ( i == 0 ) return x;
		if ( i == 1 ) return y;
		if ( i == 2 ) return z;
		return a;
		}

	// Functions
	inline void set( int i, GLfloat newValue)
	{
		if( i==0 ) x=newValue;
		if( i==1 ) y=newValue;
		if( i==2 ) z=newValue;
		if( i==3 ) a=newValue;
	}
	inline GLfloat Dot( const Vector4D &V1 ) const
		{
		return V1.x*x + V1.y*y + V1.z*z + V1.a * a;
		}

	// Vector Length
	GLfloat Length( ) const
		{
		return sqrtf( x*x + y*y + z*z + a*a);
		}

	GLfloat Distance( const Vector4D &V1 ) const
		{
		return ( *this - V1 ).Length();	
		}

	inline Vector4D Normalize()
		{
		GLfloat fMag = ( x*x + y*y + z*z + a*a);
		if (fMag == 0) {return Vector4D(0,0,0,0);}

		GLfloat fMult = 1.0f/sqrtf(fMag);            
		x *= fMult;
		y *= fMult;
		z *= fMult;
		a *= fMult;
		return Vector4D(x,y,z,a);
		}

	static float *arr;

	float *Array()
	{
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		arr[3] = a;
		return arr;
	}

	/* Multiply the vector with the matrix given as 16 values.*/
	Vector4D multiply(const GLfloat *mat)
	{
		Vector4D result;
		result.x=mat[0]*this->x + mat[4]*this->y + mat[8]*this->z + mat[12]*this->a;
		result.y=mat[1]*this->x + mat[5]*this->y + mat[9]*this->z + mat[13]*this->a;
		result.z=mat[2]*this->x + mat[6]*this->y + mat[10]*this->z + mat[14]*this->a;
		result.a=mat[3]*this->x + mat[7]*this->y + mat[11]*this->z + mat[15]*this->a;

		return result;
	}
};


#endif