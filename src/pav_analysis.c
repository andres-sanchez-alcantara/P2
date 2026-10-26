#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N) {

    float power_sum = 0.0;

    for (unsigned int n = 0; n < N; n++) {
        power_sum = power_sum  + (x[n] * x[n]);
    }

    if (power_sum == 0.0) { 
        return -100.0;   // para que al hacer luego logaritmo de 0 no de -infinito, provocando error en wavesurfer
    }

    float result = 10 * log10(power_sum / N);
    return result;
}

float compute_am(const float *x, unsigned int N) {

    float amp_sum = 0.0;

    for(unsigned int n = 0; n < N; n++){
        amp_sum = amp_sum + fabs(x[n]);
    }

    float amp_avg = amp_sum/N;
    return amp_avg;
}

//compute_zcr cntamos el paso por cero aunque no cambie de signo
float compute_zcr(const float *x, unsigned int N, float fm) {

    float sumatorio = 0.0;
    int sgn_x_prev = 0;
    int en_cero = 0;
    unsigned int n = 0;

    if (N <= 1) {  //si hay 0 o 1 muestras no se puede calcular ningún cruce
        return 0.0;
    }

    while (n < N && x[n] == 0) {  //buscar la primera muestra no nula
        n++;
    }

    if (n == N) {  //si todo el bloque son ceros, no hay cruces
        return 0.0;
    }

    if (x[n] > 0) {  //guardar signo de la primera muestra no nula
        sgn_x_prev = 1;
    } else {
        sgn_x_prev = -1;
    }

    n++;  //empezar en la siguiente muestra

    for (; n < N; n++) {

        if (x[n] == 0) {

            //si entramos en cero, contamos un cruce solo una vez
            if (en_cero == 0) {
                sumatorio = sumatorio + 1;
                en_cero = 1;
            }

        } else {

            int sgn_x;

            //obtener signo de la muestra actual
            if (x[n] > 0) {
                sgn_x = 1;
            } else {
                sgn_x = -1;
            }

            //si no venimos de cero y cambia el signo, contamos cruce
            if (en_cero == 0 && sgn_x != sgn_x_prev) {
                sumatorio = sumatorio + 1;
            }

            sgn_x_prev = sgn_x;  //guardar signo actual para la siguiente iteración
            en_cero = 0;         //ya hemos salido de la zona de cero
        }
    }

    float zcr = (fm / 2) * (sumatorio / (N - 1));
    return zcr;
}