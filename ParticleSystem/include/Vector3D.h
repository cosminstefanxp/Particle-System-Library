/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/


#ifndef Vector3D_H
#define Vector3D_H

#include "glut.h"
#include <math.h>

class Vector3D
{
public:
	// Data
	GLfloat x, y, z;

	// Constructors
	Vector3D( GLfloat _x, GLfloat _y, GLfloat _z ) : x( _x ), y( _y ), z( _z )
		{
		}
	Vector3D( ) : x(0), y(0), z(0)
		{
		}

	// Operator Overloads
	inline bool operator== (const Vector3D& V2) const 
		{
		return (x == V2.x && y == V2.y && z == V2.z);
		}

	inline Vector3D operator+ (const Vector3D& V2) const 
		{
		return Vector3D( x + V2.x,  y + V2.y,  z + V2.z);
		}
	inline Vector3D operator- (const Vector3D& V2) const
		{
		return Vector3D( x - V2.x,  y - V2.y,  z - V2.z);
		}
	inline Vector3D operator- ( ) const
		{
		return Vector3D(-x, -y, -z);
		}

	inline Vector3D operator/ (GLfloat S ) const
		{
		GLfloat fInv = 1.0f / S;
		return Vector3D (x * fInv , y * fInv, z * fInv);
		}
	inline Vector3D operator/ (const Vector3D& V2) const
		{
		return Vector3D (x / V2.x,  y / V2.y,  z / V2.z);
		}
	inline Vector3D operator* (Vector3D V2)
		{
		return Vector3D (x * V2.x,  y * V2.y,  z * V2.z);
		}
	inline Vector3D operator* (GLfloat S) const
		{
		return Vector3D (x * S,  y * S,  z * S);
		}

	inline void operator+= ( const Vector3D& V2 )
		{
		x += V2.x;
		y += V2.y;
		z += V2.z;
		}
	inline void operator-= ( const Vector3D& V2 )
		{
		x -= V2.x;
		y -= V2.y;
		z -= V2.z;
		}

	inline GLfloat operator[] ( int i )
		{
		if ( i == 0 ) return x;
		if ( i == 1 ) return y;
		return z;
		}

	// Functions
	inline GLfloat Dot( const Vector3D &V1 ) const
		{
		return V1.x*x + V1.y*y + V1.z*z;
		}

	inline Vector3D CrossProduct( const Vector3D &V2 ) const
		{
		return Vector3D(
			y * V2.z  -  z * V2.y,
			z * V2.x  -  x * V2.z,
			x * V2.y  -  y * V2.x 	);
		}

	// Vector Length
	GLfloat Length( ) const
		{
		return sqrtf( x*x + y*y + z*z );
		}

	GLfloat Distance( const Vector3D &V1 ) const
		{
		return ( *this - V1 ).Length();	
		}

	inline Vector3D Normalize()
		{
		GLfloat fMag = ( x*x + y*y + z*z );
		if (fMag == 0) {return Vector3D(0,0,0);}

		GLfloat fMult = 1.0f/sqrtf(fMag);            
		x *= fMult;
		y *= fMult;
		z *= fMult;
		return Vector3D(x,y,z);
		}

	inline float X()
	{
		return x;
	}

	inline float Y()
	{
		return y;
	}

	inline float Z()
	{
		return z;
	}

	static float *arr;


	float *Array()
	{
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		return arr;
	}


};

#endif