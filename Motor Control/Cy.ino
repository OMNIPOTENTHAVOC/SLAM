// Cytron Motor Driver Pins
const int MOTOR_FW = 9;   // Forward
const int MOTOR_BW = 10;  // Backward

// Actuator Pins (optional)
const int ACTUATOR_DIR = 4;
const int ACTUATOR_PWM = 2;

// Current coordinates of the rover
float currentX = 0.0;
float currentY = 0.0;

void setup() {
    Serial.begin(9600);
    
    // Set up motor driver pins
    pinMode(MOTOR_FW, OUTPUT);
    pinMode(MOTOR_BW, OUTPUT);
    
    // Set up actuator pins
    pinMode(ACTUATOR_DIR, OUTPUT);
    pinMode(ACTUATOR_PWM, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        String coords = Serial.readStringUntil('\n');
        int commaIndex = coords.indexOf(',');
        if (commaIndex > 0) {
            String xStr = coords.substring(0, commaIndex);
            String yStr = coords.substring(commaIndex + 1);
            float x = xStr.toFloat();
            float y = yStr.toFloat();
            
            move_rover_to_target(x, y);
            
            if (reached_target(x, y)) {
                collect_soil();
            }
        }
    }
}

void move_rover_to_target(float targetX, float targetY) {
    while (true) {
        float deltaX = targetX - currentX;
        float deltaY = targetY - currentY;
        
        float threshold = 0.1;

        if (abs(deltaX) < threshold && abs(deltaY) < threshold) {
            stop_motors();
            break;
        }

        if (abs(deltaX) > abs(deltaY)) {
            if (deltaX > 0) {
                forward();
            } else {
                backward();
            }
        } else {
            if (deltaY > 0) {
                right();
            } else {
                left();
            }
        }
    }
}

void forward() {   
    digitalWrite(MOTOR_FW, HIGH);
    digitalWrite(MOTOR_BW, LOW);
    delay(1000);
    currentX += 1.0;  // Update position as needed based on movement
}

void backward() {    
    digitalWrite(MOTOR_FW, LOW);
    digitalWrite(MOTOR_BW, HIGH);
    delay(1000);
    currentX -= 1.0;  // Update position as needed based on movement
}

void left() {
    digitalWrite(MOTOR_FW, LOW);
    digitalWrite(MOTOR_BW, HIGH);
    delay(500);  
    currentY -= 1.0;  // Adjust as needed based on turning
}

void right() {
    digitalWrite(MOTOR_FW, HIGH);
    digitalWrite(MOTOR_BW, LOW);
    delay(500);  
    currentY += 1.0;  // Adjust as needed based on turning
}

void stop_motors() {
    digitalWrite(MOTOR_FW, LOW);
    digitalWrite(MOTOR_BW, LOW);
}

bool reached_target(float targetX, float targetY) {
    return (abs(targetX - currentX) < 0.1 && abs(targetY - currentY) < 0.1);
}

void collect_soil() {
    extend();
    delay(3000); 
    pullback();
    delay(3000);
}

void extend() {
    digitalWrite(ACTUATOR_DIR, HIGH);
    analogWrite(ACTUATOR_PWM, 255);  
}

void pullback() {
    digitalWrite(ACTUATOR_DIR, LOW);
    analogWrite(ACTUATOR_PWM, 255);  
}
