# Cruise-Control-DC-Motor-LabVIEW
# 🚗 Adaptive Cruise Control & Speed Limit Assist using LabVIEW

A LabVIEW-based simulation of an **Adaptive Cruise Control (ACC)** and **Speed Limit Assist (SLA)** system for a DC motor platform, inspired by the driving assistance features found in the **Hyundai Ioniq 5 Smart Cruise Control**.

> **Originally developed as a Classic Cruise Control project, this system has been redesigned into a complete Adaptive Cruise Control simulation with automatic distance-based speed regulation and integrated Speed Limit Assist.**

---

## ✨ Overview

This project demonstrates how modern driver assistance systems can be implemented on a low-cost embedded platform using **ESP32**, **LabVIEW**, and **PID control**.

The **ESP32** performs real-time motor control, while **LabVIEW** provides an interactive HMI for monitoring, parameter tuning, and supervising the entire control system.

---

## 🚦 Key Features

* 🎯 **Adaptive Cruise Control (ACC)**

  * Maintain a preset cruising speed
  * Configure multiple following distance levels
  * Automatically reduce speed when approaching a lead vehicle
  * Smoothly resume the preset speed once the road is clear

* 🚧 **Speed Limit Assist (SLA)**

  * Prevent vehicle speed from exceeding a user-defined limit
  * Driver can continue pressing the accelerator without exceeding the configured speed limit

* ⚙️ **Real-Time Motor Control**

  * PID-based speed controller
  * Real-time communication between ESP32 and LabVIEW
  * Live visualization of vehicle states and control parameters

---

## 🔄 From Cruise Control to Adaptive Cruise Control

Unlike a conventional **Cruise Control** system that only maintains a constant speed, this project extends the functionality by continuously monitoring the distance to the vehicle ahead and dynamically adjusting the target speed.

| Classic Cruise Control         | Adaptive Cruise Control                        |
| ------------------------------ | ---------------------------------------------- |
| Maintain fixed speed           | Maintain speed **and** safe following distance |
| No obstacle awareness          | Automatic speed adaptation                     |
| Driver responsible for braking | Automatic deceleration and acceleration        |

---

## 🛠️ Technologies

* **ESP32**
* **Embedded C/C++**
* **Arduino Framework**
* **LabVIEW**
* **PID Controller**
* **UART Communication**

---

## 📊 Project Highlights

* Adaptive Cruise Control inspired by commercial EV systems
* Integrated Speed Limit Assist
* Real-time HMI built with LabVIEW
* Modular embedded software architecture
* Interactive simulation for control algorithm development and demonstration

---

## 🎯 Project Goal

This project aims to provide a practical demonstration of how modern vehicle speed assistance systems can be implemented on an embedded platform, serving as a learning and development platform for **Automotive Embedded Systems**, **Control Engineering**, and **LabVIEW-based HMI**.
