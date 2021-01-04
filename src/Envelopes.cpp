#include "Envelopes.h"
#include <cmath>

Envelopes::Envelopes()
{
}

Envelopes::~Envelopes()
{
}

double Envelopes::arLin(double input, int trigger)
{
    switch (currentEnvState) {
    case ATTACK:
        amplitude += (1 * attack);
        if (amplitude >= 1) {
            amplitude = 1;
            currentEnvState = HOLD;
        }
        break;
    case HOLD:
        amplitude = 1;
        if (trigger != 1) {
            currentEnvState = RELEASE;
        }
        break;
    case RELEASE:
        amplitude -= (1 * release);
        if (amplitude <= 0) {
            amplitude = 0;
            currentEnvState = STOP;
        }
        break;
    case STOP:
        amplitude = 0.0;
        if (trigger == 1) {
            currentEnvState = ATTACK;
        }
        break;
    }

    output = input * amplitude;
    return output;
}

double Envelopes::arExp(double input, int trigger)
{
    switch (currentEnvState) {
    case ATTACK:
        amplitude *= attack;
        if (amplitude >= 1) {
            amplitude = 1;
            currentEnvState = HOLD;
        }
        break;
    case HOLD:
        amplitude = 1;
        if (trigger != 1) {
            currentEnvState = RELEASE;
        }
        break;
    case RELEASE:
        amplitude *= (2 - release); // releasevalue is calculated as attackvalue in setADSRValue, to convert to release subtract the value of 2
        if (amplitude <= amplitudeStartValue) {
            amplitude = 0;
            currentEnvState = STOP;
        }
        break;
    case STOP:
        amplitude = 0.0;
        if (trigger == 1) {
            amplitude = amplitudeStartValue;
            currentEnvState = ATTACK;
        }
        break;
    }

    output = input * amplitude;
    return output;
}

void Envelopes::setARLinear(double attackMs, double releaseMs) 
{
    attack = (1.0 / updateRate) * (1 / (attackMs / 1000.0));
    release = (1.0 / updateRate) * (1 / (releaseMs / 1000.0));
}

void Envelopes::setARExp(double attackMs, double releaseMs)
{
    attack = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * (attackMs / 1000.0)));
    release = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * (releaseMs / 1000.0)));
}