#include "texture.h"

//tell opengl sa elibereze toate resursele necesare pentru textura cu id-ul texture
void FreeTexture( GLuint texture_id )
{
  glDeleteTextures( 1, &texture_id );  
}

//standard
struct header{
	unsigned char type[2];
	int f_leght;
	short rezerved1;
	short rezerved2;
	int offBits;
};

struct header_info{
	int size;
	int width;
	int height;
	short planes;
	short bitCount;
	int compresion;
	int sizeImage;
	int xPelsPerMeter;
	int yPelsPerMeter;
	int clrUsed;
	int clrImportant;
};

GLuint LoadTextureBMP(const char* filename, int optiune_filtrare){
		GLuint texture_id;

		
		unsigned char *data;
		FILE *file;
		header h; header_info h_info;
	    
		fopen_s(&file, filename, "rb" );
		if ( file == NULL ) return 0;

		//read header 1
		fread( &(h.type), 2, 1, file); 
		fread( &(h.f_leght), 4, 1, file); 
		fread( &(h.rezerved1), 2, 1, file); 
		fread( &(h.rezerved2), 2, 1, file); 
		fread( &(h.offBits), 4, 1, file);
		//read header 2
		fread(&(h_info), sizeof(header_info) , 1, file); 

		//aloca memorie
		data = (unsigned char *)malloc( h_info.width * h_info.height * 3 );


		// verific daca exista grupuri de 4 octeti pe linie
		long padd=0;
		if ((h_info.width * 3) % 4 != 0) padd = 4 - (h_info.width * 3) % 4;

		//save height &width
		long width=h_info.width;
		long height=h_info.height;

		long pointer;
		unsigned char r, g, b;
		//citesc matricea
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				fread(&b, 1, 1, file);
				fread(&g, 1, 1, file);
				fread(&r, 1, 1, file);
				
				pointer=(i*height+j)*3;
				data[pointer]=r; data[pointer+1]=g; data[pointer+2]=b;
			}

			fseek(file, padd, SEEK_CUR);
		}

		fclose(file);
		
		//generare textura
		glGenTextures( 1, &texture_id );
		glBindTexture( GL_TEXTURE_2D, texture_id );

		//propietati invariante
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		
		//filtrare in functie de optiuni
		if(optiune_filtrare == TEXTURA_FILTRARE_NEAREST){
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, data);
		}
		if(optiune_filtrare == TEXTURA_FILTRARE_BILINEAR){
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, data);
		}
		if(optiune_filtrare == TEXTURA_FILTRARE_TRILINEAR){
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
		}
		if(optiune_filtrare == TEXTURA_FILTRARE_TRILINEAR_ANISOTROPIC){
			//TODO [DONE]
			GLfloat fLargest; 
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
			gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
		}
    
		free( data );//free memory
		return texture_id;
}




//recreeaza textura exact de dimensiunea ecranului, pentru render to texture 
void CreeazaTexturaEcran(GLuint *texture_id, int width, int height){

	//sterge textura precedenta
	glDeleteTextures(1,texture_id);

	//creeaza o textura noua de dimensiune corecta
	glGenTextures(1, texture_id);

	//bind
	glBindTexture(GL_TEXTURE_2D, (*texture_id));

	//filtrare biliniara, mai mult de atat nu are sens
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	//creeaza textura empty cu dimensiunea corecta
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width, height,0, GL_RGBA,GL_UNSIGNED_BYTE, NULL);
}