# 👨‍🦯 Smart Assistive Stick for Visually Impaired People

A standalone, sensor-integrated assistive device designed to enhance indoor mobility and safety for visually impaired individuals through real-time hazard detection and buzzer-based feedback.

---

## 🎯 Objective

To provide a **low-cost, intuitive, and real-time hazard detection system** for visually impaired users that works independently—without smartphones or external apps—by sensing and alerting for:
- Obstacles (at different heights and directions)
- Gas leaks
- Water spills
- Fire presence
- Improper stick orientation (fall or mishandling)

---

## ✨ Key Features

- **Multi-hazard Detection**: Uses 4 ultrasonic sensors + gas, flame, water, and tilt sensors.
- **Real-time Feedback**: Buzzer emits unique beep patterns for each hazard.
- **No Smartphone Needed**: Fully standalone operation using Arduino Uno R3.
- **Obstacle Navigation**: Suggests safe direction (left/right) if path is blocked.
- **Power Efficient**: Operates on USB power (via power bank).

---

## 🧰 Tools & Technologies Used

- **Microcontroller**: Arduino Uno R3
- **Programming Language**: C/C++ (Arduino IDE)
- **Sensors**:
  - HC-SR04 (Ultrasonic) ×4
  - MQ-2 (Gas)
  - KY-026 (Flame)
  - Analog Water Sensor
  - Digital Tilt Sensor
- **Feedback**: Active Buzzer (Low-level trigger)
- **Power Supply**: USB power bank (stable current)

---

## ⚙️ Methodology

1. **Sensor Configuration**  
   Sensors are connected to Arduino pins and calibrated using analog/digital thresholds.  
   - Front-bottom & middle sensors detect obstacles.
   - Side sensors help determine clear direction.
   - Gas, water, flame, and tilt sensors detect environmental hazards.

2. **Priority-Based Alert Logic**  
   The system evaluates hazards in the following order:  
   `Gas > Flame > Water > Tilt > Obstacle`.  
   Only the highest priority alert is activated per cycle to avoid confusion.

3. **Buzzer Feedback Patterns**  
   - 🔊 Gas: Two triple quick beeps  
   - 🔊 Flame: Long-short-long  
   - 🔊 Water: Two wave-timed beeps  
   - 🔊 Tilt: Four quick beeps  
   - 🔊 Obstacle: Long beep, then left/right suggestion

4. **Real-Time Loop & Debouncing**  
   - Loop runs continuously with a short delay for responsiveness.
   - Flags and timing (using `millis()`) prevent false or repeated triggers.

5. **Testing & Refinement**  
   - Simulated hazards were tested in indoor settings.
   - Adjustments made for power stability, false positives, and multi-sensor conflict.

---

## 🧪 Summary

This project provides a **reliable and affordable** solution for visually impaired users navigating indoor spaces. Its modular design allows future upgrades (like voice alerts or vibration feedback) while maintaining a simple, effective core functionality.

