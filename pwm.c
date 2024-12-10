#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

void mov_motors(int MOTOR_PIN) {
    // Inicializar el pin del motor como salida
    gpio_init(MOTOR_PIN);
    gpio_set_dir(MOTOR_PIN, GPIO_OUT);

        // Encender el motor
        gpio_put(MOTOR_PIN, 1);
        sleep_ms(500);  // Mantener encendido 5 segundos

        // Apagar el motor
        gpio_put(MOTOR_PIN, 0);
        sleep_ms(5000);  // Mantener apagado 5 segundos
    
}