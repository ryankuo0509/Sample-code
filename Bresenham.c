#include "../include/Bresenham.h"

#define DBG_DRAW_LINE	1
#define NO_FLOAT		1

void draw_line(unsigned char **f /*f[x][y]*/, unsigned char foreground, int x1, int y1, int x2, int y2, int nc, int nr)
{
	int x, y;
    int deltaX, deltaY;
    int s1, s2;
    int temp, interchange, fac;
    int i;

    x = x1;
    y = y1;
    deltaX = abs(x2-x1);
    deltaY = abs(y2-y1);

	if(x2-x1 >= 0){
		s1 = 1;
	}else{
		s1 = -1;
	}

    if(y2-y1 >= 0){
		s2 = 1;
	}else{
		s2 = -1;
	}

    /* Interchange deltaX and deltaY depending on the slope of the line*/
    if(deltaY > deltaX){
		temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        interchange = 1;
    }else{
		interchange = 0;
	}

	fac = 2*deltaY-deltaX;

	for(i=1; i<=deltaX; i++){
		if( (x >= 0) && (x < nc) && (y >= 0) && (y < nr)){
			f[x][y] = foreground;
		}

		if(fac >= 0){
			if(interchange == 1){
				x = x + s1;
			}else{
				y = y + s2;
			}
            fac = fac-2*deltaX;
        }

		if(interchange == 1){
				y = y + s2;
		}else{
				x = x + s1;
		}
        fac = fac+2*deltaY;
    }
}

/*
	wiki_line(2, 12, 2, 6);
    Condition: (Dst_X - Src_X) > (Dst_Y - Src_Y),  0 < Slop < 1

*/
void wiki_line_4th_quad(int Src_X, int Dst_X, int Src_Y, int Dst_Y)
{
    SINT32 deltaX, deltaY, x, y;
	float error, Slop, Cal_Y;

	deltaX = Dst_X - Src_X;
	deltaY = Dst_Y - Src_Y;
	Slop = (float)deltaY / (float)deltaX ;

	error = 0;
	y = Src_Y;

	printf("Src   (%2d,%2d)\n", Src_X, Src_Y);
	printf("Dst   (%2d,%2d)\n", Dst_X, Dst_Y);
	printf("Delta (%2d,%2d)\n", deltaX, deltaY);
	printf("Slop:%.2f \n\n", Slop);

	for(x=Src_X; x<=Dst_X; x++){
		if(fabs(error) >= 0.5){
			y = y + 1;
			error = error - 1.0;
		}

		#if DBG_DRAW_LINE
		Cal_Y = Slop * (float)(x-Src_X) + (float)Src_Y;
		printf("Cal_Y:%.2f  error:%5.2f  ", Cal_Y, error);
		printf("(%2d,%2d)\n", x, y);
		#else
		printf("(%2d, %2d)\n", x, y);
		#endif

		error = error + Slop;
	}
}

void wiki_line_entire_quad(int Src_X, int Dst_X, int Src_Y, int Dst_Y)
{
	SINT32 deltaX, deltaY, stepY, x, y;
	UINT8 steep;
	float Slop, Cal_Y;
	#if NO_FLOAT
	SINT32 error;
	#else
	float error;
	#endif // NO_FLOAT

	deltaX = Dst_X - Src_X;
	deltaY = Dst_Y - Src_Y;
	Slop = (float)deltaY / (float)deltaX ;

	/* steep:
	        > 1 : steep over 45 degree
	        < 1 : steep under 45 degree
	        = 1 : steep eual 45 degree
	*/
	steep = abs(deltaY) > abs(deltaX);
	printf("\n\n-------------------");
	printf("\nsteep:%d \n", steep);
	printf("Src   (%2d,%2d)\n", Src_X, Src_Y);
	printf("Dst   (%2d,%2d)\n", Dst_X, Dst_Y);
	printf("Delta (%2d,%2d)\n", deltaX, deltaY);
	printf("Slop:%.2f \n\n", Slop);

	if(steep){
		Swap(&Src_X, &Src_Y);
		Swap(&Dst_X, &Dst_Y);
		printf("Swap X<->Y \n");
		printf("Src  (%2d,%2d)\n", Src_X, Src_Y);
		printf("Dst  (%2d,%2d)\n", Dst_X, Dst_Y);
	}

	if(Src_X > Dst_X){
		Swap(&Src_X, &Dst_X);
		Swap(&Src_Y, &Dst_Y);
		printf("Swap Src<->Dst \n");
		printf("Src  (%2d,%2d)\n", Src_X, Src_Y);
		printf("Dst  (%2d,%2d)\n", Dst_X, Dst_Y);
	}

	deltaX = Dst_X - Src_X;
	deltaY = abs(Dst_Y - Src_Y);
	Slop = (float)deltaY / (float)deltaX ;

	printf("Delta (%2d,%2d)\n", deltaX, deltaY);
	printf("Slop:%.2f \n\n", Slop);

	y = Src_Y;

	if(Src_Y < Dst_Y){
		stepY = 1;
	}else{
		stepY = -1;
	}

	#if NO_FLOAT
	error = deltaX/2;
	#else
	error = 0;
	#endif

	for(x=Src_X; x<=Dst_X; x++){

		#if NO_FLOAT
		if(error < 0){
			y = y + stepY;
			error = error + deltaX;
		}
		#else
		if(fabs(error) >= 0.5){
			y = y + stepY;
			error = error - 1.0;
		}
		#endif // NO_FLOAT

		if(steep){
			printf("(%2d, %2d)\n", y, x);
		}else{
			printf("(%2d, %2d)\n", x, y);
		}

		#if NO_FLOAT
		error = error - deltaY;
		#else
		error = error + Slop;
		#endif // NO_FLOAT
	}
}
