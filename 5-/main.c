#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>

char * NOMBRE_VENTANA = "Sprite";

IplImage* mezclarImagenesSSE(IplImage* background, IplImage* obj, int x, int y){
    IplImage* resultado = cvCloneImage(background);

    //definimos los punteros
    uchar* pBg,
        * pRes,
        * pObj;


    __m128i rObjeto,
            rRes,
            rFondo,
            fceros = _mm_set1_epi32(0xFF000000),
            ceros = _mm_set1_epi32(0x00000000),
            mascara,
            temp1, temp2;


    //fila y columna respecto a la imagen de fondo (background)
    int fila, columna, component;

    for (fila = y; fila < (y + obj -> height); fila ++){
        pBg = (uchar*) background -> imageData + background -> widthStep * fila + x * 4;
        pRes = (uchar*) resultado -> imageData + resultado -> widthStep * fila + x * 4;
        pObj = (uchar*) obj -> imageData + obj -> widthStep * (fila - y);

        for (columna = x; columna < (x + obj -> width); columna += 4){

            rObjeto = _mm_loadu_si128((__m128i*) pObj);
            rFondo = _mm_loadu_si128((__m128i*) pBg);
            rRes = _mm_loadu_si128((__m128i*) pRes);

            temp1 = _mm_and_si128(rObjeto, fceros);
            mascara = _mm_cmpeq_epi32(temp1, ceros);

            rRes = _mm_or_si128(_mm_and_si128(rFondo, mascara), _mm_andnot_si128(mascara , rObjeto));
            _mm_storeu_si128((__m128i*) pRes, rRes);
            pRes += 16;
            pBg += 16;
            pObj += 16;
        }
    }

    return resultado;
}

IplImage* extraerSubzona(IplImage* original, int xInicial, int yInicial, int xFinal, int yFinal){
    CvSize tam = cvSize(xFinal - xInicial, yFinal - yInicial);
    IplImage* res = cvCreateImage(tam, IPL_DEPTH_8U, 4);

    __m128i temp1;

    unsigned char* pOr,
                * pRes;
    int fila,
        columna;

    for (fila = yInicial; fila < yFinal; fila ++){
        pOr = (unsigned char*) original -> imageData + fila * original -> widthStep + xInicial * 4;
        pRes = (unsigned char*) res -> imageData + (fila - yInicial) * res -> widthStep;

        for (columna = xInicial; columna < xFinal; columna+= 4){
            temp1 = _mm_loadu_si128((__m128i *) pOr);
            _mm_storeu_si128((__m128i *) pRes, temp1);
            pOr += 16;
            pRes += 16;
        }
    }

    return res;
}

int main(int argc, char** argv){
    if (argc != 3){
        printf("Error nº argumentos");
        exit(EXIT_FAILURE);
    }

    IplImage* background = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    if (!background){
        printf("Error cargando background");
        exit(EXIT_FAILURE);
    }

    IplImage* sprite = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    if (!sprite){
        printf("Error cargando sprite");
        exit(EXIT_FAILURE);
    }

    cvNamedWindow(NOMBRE_VENTANA, 1);

    IplImage* porcion = extraerSubzona(sprite, 780, 908, 780 + 56, 908 + 56);
    IplImage* mezcla = mezclarImagenesSSE(background, porcion, 100, 100);
    cvShowImage(NOMBRE_VENTANA, mezcla);
    cvWaitKey(0);

    cvReleaseImage(&background);
    cvReleaseImage(&sprite);
    cvDestroyWindow(NOMBRE_VENTANA);

}