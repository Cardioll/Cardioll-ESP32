# include <Cardioll.h>

float Cardioll::findExtremum(int start_index, int end_index, bool find_maximum) {
    float extremum = buffer_[start_index]; 
    int extremum_index = start_index; 

    for (int i = start_index; i <= end_index; ++i) {
        if ((find_maximum && buffer_[i] > extremum) || (!find_maximum && buffer_[i] < extremum)) {
            extremum = buffer_[i];
            extremum_index = i;
        }
        
        yield();
    }

    return extremum_index;
}

#if defined(DEBUG_TELEPLOT) || defined(DEBUG_SERIAL)
void Cardioll::debugPrint(const char* label, float value) {
    Serial.print(label);
    Serial.println(value);
}
#endif

#ifdef DEBUG_TELEPLOT
void Cardioll::telePlotSignal(const char* name, float value, int index) {
    Serial.print(">");
    Serial.print(name);
    Serial.print(":");
    Serial.println(value);
}

void Cardioll::telePlotMarker(const char* name, int index) {
    Serial.print(">");
    Serial.print(name);
    Serial.print("!");
    Serial.println(index);
}
#endif