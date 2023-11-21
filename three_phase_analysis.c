#include <stdio.h>
#include <math.h>

#define CYCLE 1
#define DATA_LENGTH 20
#define SAMPLE_RATE 1000.0

float Va[] = {156.63, 246.59, 294.72, 305.51, 300.66, 268.03, 204.18, 125.41, 42.954, -48.322,
             -154.08, -243.95, -293.12, -303.09, -297.98, -264.13, -202.1, -122.25, -39.893, 51.818};

float Vb[] = {-308.4, -280.19, -240.66, -186.6, -99.744, -0.54547, 92.853, 181.46, 262.05, 312.39,
             311.44, 283.76, 245.04, 188.62, 102.16, 2.9662, -89.395, -176.17, -259.16, -309.96};

float Vc[] = {156.11, 82.694, -21.783, -128.37, -213.06, -269.49, -309.58, -313.4, -273.73, -214.81,
             -154.29, -79.64, 24.679, 132.16, 216.63, 274.14, 311.11, 315.76, 276.27, 216.22};

typedef struct _DDATA {
    float *in_a;
    float *in_b;
    float *in_c;
    float F_est;
    float Theta_est;
    float Harmonics[5];
    float Ts;
} DDATA;

DDATA ddata = {
    .in_a = Va,
    .in_b = Vb,
    .in_c = Vc,
    .F_est = 0.0,
    .Theta_est = 0.0,
    .Harmonics = {0.0, 0.0, 0.0, 0.0, 0.0},  // Array for 5 harmonics
    .Ts = 0.001
};

void estimateFrequencyAndTheta(DDATA *d, int dataSize) {
    // Implementation for estimating frequency and theta
    
    float *input_a = d->in_a;
    float *input_b = d->in_b;
    float *input_c = d->in_c;
    
    float zeroCrossings_a = 0.0;
    float zeroCrossings_b = 0.0;
    float zeroCrossings_c = 0.0;

    // Count zero crossings for each phase
    for (int i = 1; i < dataSize; i++) {
        if (input_a[i] * input_a[i - 1] < 0.0) {
            zeroCrossings_a += 1.0;
        }

        if (input_b[i] * input_b[i - 1] < 0.0) {
            zeroCrossings_b += 1.0;
        }

        if (input_c[i] * input_c[i - 1] < 0.0) {
            zeroCrossings_c += 1.0;
        }
    }

    // Calculate estimated frequency as the average zero-crossing rate
    float zeroCrossingsAvg = (zeroCrossings_a + zeroCrossings_b + zeroCrossings_c) / 3.0;
    d->F_est = zeroCrossingsAvg * SAMPLE_RATE / (2.0 * dataSize);

    // Calculate estimated phase angle using a zero-crossing method
    int zero_crossings = 0;
    int pos_peak_index = 0;
    int neg_peak_index = 0;

    for (int i = 1; i < dataSize; i++) {
        if ((input_a[i] > 0 && input_a[i - 1] < 0) || (input_a[i] < 0 && input_a[i - 1] > 0)) {
            zero_crossings++;

            if (zero_crossings == 1) {
                // Store the index of the first zero-crossing as a reference
                pos_peak_index = i;
            }
        }

        if (input_a[i] > input_a[pos_peak_index]) {
            pos_peak_index = i;
        }

        if (input_a[i] < input_a[neg_peak_index]) {
            neg_peak_index = i;
        }
    }

    // Calculate estimated phase angle as a fraction of the signal period
    float period = 2.0 * zero_crossings / d->F_est;
    float phase_fraction = (fmod((float)(pos_peak_index - neg_peak_index), period) / period);
    d->Theta_est = fmod(360.0 + (120.0 - phase_fraction * 360.0), 360.0);  // Ensure positive value

    printf("Estimated Frequency (F_est): %.2f Hz\n", d->F_est);
    printf("Estimated Phase Angle (Theta_est): %.2f degrees\n", d->Theta_est);

}

void getHarmonicAmplitudes(DDATA *d, int dataSize) {
    // Implementation for getting harmonic amplitudes
    float Amplitude_a = 0.0;
    float Amplitude_b = 0.0;
    float Amplitude_c = 0.0;

    for (int i = 0; i < dataSize; i++) {
        Amplitude_a = fmax(Amplitude_a, fabs(d->in_a[i]));
        Amplitude_b = fmax(Amplitude_b, fabs(d->in_b[i]));
        Amplitude_c = fmax(Amplitude_c, fabs(d->in_c[i]));
    }

    // Multiply each average amplitude by the harmonic number
    for (int i = 1; i <= 5; ++i) {
        d->Harmonics[i - 1] = i * Amplitude_a;
        printf("Amplitude of %dth Harmonic in A: %.2f\n", i, d->Harmonics[i - 1]);

        d->Harmonics[i - 1] = i * Amplitude_b;
        printf("Amplitude of %dth Harmonic in B: %.2f\n", i, d->Harmonics[i - 1]);

        d->Harmonics[i - 1] = i * Amplitude_c;
        printf("Amplitude of %dth Harmonic in C: %.2f\n", i, d->Harmonics[i - 1]);
    }
}


int main() {
    int i;
    
    for (i = 0; i < DATA_LENGTH * CYCLE; i++) {
        estimateFrequencyAndTheta(&ddata, DATA_LENGTH * CYCLE);
        getHarmonicAmplitudes(&ddata, DATA_LENGTH * CYCLE);
    }
    
    return 0;
}
