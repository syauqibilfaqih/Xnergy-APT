# Xenergy-APT
A repository of a code to estimate a three-phase system frequency and phase angle and determine amplitude of the harmonic series.

## Overview

The code consists of three main functions:

1. `estimateFrequencyAndTheta`: Estimates the frequency (F_est) and phase angle (Theta_est) of the three-phase voltage signal using a zero-crossing method.

2. `getHarmonicAmplitudes`: Calculates the amplitude of the harmonic series up to the 5th harmonic for each phase.

3. `main`: The main function calls the above functions for the given three-phase voltage signal data.

## How to Use

1. Open the code file (`three_phase_analysis.c`) in a C compiler or IDE.

2. Compile and run the code.

3. The output will display the estimated frequency, phase angle, and harmonic amplitudes for each phase.

## Important Notes

- Adjust the `CYCLE` and `DATA_LENGTH` constants in the code to control the number of cycles and data length.

- Ensure that the input data arrays (`Va`, `Vb`, `Vc`) represent the three-phase voltage samples.

- Modify the controller gains (`Kc1`, `Kc2`, `Kc3`) as needed for your specific application.

## Example Data

The code includes example three-phase voltage signal data (`Va`, `Vb`, `Vc`) sampled at a frequency of 1 kHz. You can replace this data with your own three-phase voltage samples.

