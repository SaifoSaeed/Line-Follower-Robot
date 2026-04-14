# Autonomous Multi-Zone Mobile Robot (PIC16F877A)

An embedded C project implementing an autonomous mobile robot capable of navigating a complex, multi-zone track. The system relies on a PIC16F877A microcontroller, dealing with strict low-level constraints, hardware interrupts, and peripheral multiplexing.

## Hardware Architecture

* **Microcontroller:** PIC16F877A.
* **Motor Control:** L298 Dual Full-Bridge Driver controlling two DC motors in a differential drive setup. The L298 utilizes both available hardware CCP (Capture/Compare/PWM) modules on the PIC for speed modulation.
* **Power Supply:** Three 3.7V batteries routed through a voltage regulator for stable Vcc.
* **Sensors:** * 4x Infrared (IR) Sensors (digital tracking & zone detection).
  * 1x Light Dependent Resistor (LDR) tied to an ADC channel.
  * 1x Ultrasonic Sensor for distance measuring.
* **Actuators:** DC Motors (wheels) and a Servo Motor (parking flag). Due to CCP port exhaustion, the servo is driven via a software workaround on a standard digital I/O pin.

## Firmware Features & State Machine

The software architecture operates on a continuous main loop with interrupt-driven events to handle concurrent tasks. 

### 1. Line Following & Intersection Logic
The robot uses left and right front-mounted IR sensors to modulate motor speeds for line tracking. A rear-mounted IR sensor is dedicated to detecting specific zones. When front sensors trigger simultaneously, the firmware detects an intersection and executes a lightweight, hard-coded left turn to save processing overhead.

### 2. Tunnel Navigation & ADC
An LDR connected to an analog port continuously feeds the ADC module. When the voltage crosses a calibrated low-light threshold, the robot triggers a buzzer. Exiting the tunnel sets the `ULTRA_ON` global flag, enabling the ultrasonic sensor for the next zone.

### 3. Obstacle Avoidance
In the obstacle zone, the ultrasonic sensor measures forward distance while a side-mounted IR sensor differentiates between white track walls and black obstacles. 
* **Distances < 10cm:** Triggers an emergency "Reverse & Turn" sequence.
* **Wall Detected:** Executes a hard right turn.
* **Obstacle Detected:** Executes an avoidance maneuver via hard-coded delays.

### 4. Incline (Bump) & Automated Parking
An "All Sensors Black" condition signifies the track incline, prompting the MCU to inject a short speed burst to clear the bump. Concurrently, a background timer interrupt polls for the final parking line. Upon detection, it sets a `FINISHED` flag, overriding the main loop to kill the DC motors and send maximum voltage to the servo pin to raise the parking flag.

## Usage
1. Clone the repository and configure your toolchain for the PIC16F877A.
2. Verify pinout mappings in the source headers to match your specific breadboard/chassis wiring.
3. Flash the compiled `.hex` binary to the microcontroller.
