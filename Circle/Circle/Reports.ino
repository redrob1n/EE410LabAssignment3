

void Print() {
    Serial.print(state);
    Serial.write(9);
    Serial.print(LtCount);
    Serial.write(9);
    Serial.print(RtCount);
    Serial.write(9);        
    Serial.print(LtCountDel);
    Serial.write(9);
    Serial.print(RtCountDel);
    Serial.write(9);
    Serial.println(rightDutyCycle);  
}

void LogoPrint() {
    Serial1.print(LtCount);
    Serial1.write(9);
    Serial1.print(RtCount);
    Serial1.write(9);        
    Serial1.print(LtCountDel);
    Serial1.write(9);
    Serial1.print(RtCountDel);
    Serial1.write(9);
    Serial1.println(rightDutyCycle);    
}
