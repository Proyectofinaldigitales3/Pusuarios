/**
 * @file s_luminosa.c
 * @brief Señales luminosas para varios estados .
 * 
 * contiene las funciones para inicializar y controlar los LEDs conectados a los pines GPIO de la Raspberry Pi Pico.
 * Los LEDs se utilizan para indicar varios estados en el sistema, como acceso concedido, denegado y estado de parpadeo.
 */
#include "s_luminosa.h"

// Definición de la variable global
bool should_blink = false;   /**< Variable global para indicar si el LED debe parpadear */
uint32_t last_blink_time = 0;  /**< Tiempo en el que el LED cambia de estado por última vez */
bool led_state = false;        /**< Estado actual del LED (encendido o apagado) */

/**
 * @brief Inicializa los LEDs.
 * 
 * Configura los pines GPIO 10, 11 y 12 como salidas, preparándolos para controlar los LEDs.
 */
void inicialization() {
    // Inicializar el GPIO
    gpio_init(LED_PIN_10);
    gpio_init(LED_PIN_11);
    gpio_init(LED_PIN_12);

    // Configurar los pines como salida
    gpio_set_dir(LED_PIN_10, GPIO_OUT);
    gpio_set_dir(LED_PIN_11, GPIO_OUT);
    gpio_set_dir(LED_PIN_12, GPIO_OUT);
}

/**
 * @brief Enciende el LED en el pin GPIO 10 durante 5 segundos lED verde.
 * 
 * Esta función enciende el LED verde conectado al GPIO 10, lo mantiene encendido durante 5 segundos y luego lo apaga.
 * Este led enciende cuando la concesion del acceso se da.
 */
void led_on_gpio10_5_seconds() {
    gpio_put(LED_PIN_10, 1);  // Encender LED verde
    sleep_ms(5000);           // Esperar 5 segundos
    gpio_put(LED_PIN_10, 0);  // Apagar LED
}

/**
 * @brief Enciende el LED en el pin GPIO 11 durante 2 segundos LED rojo.
 * 
 * Esta función enciende el LED rojo conectado al GPIO 11, lo mantiene encendido durante 2 segundos y luego lo apaga.
 * enciende cuando id incorrecto, clave incorrecta, tiempo excedido.
 */
void led_on_gpio11_2_seconds() {
    gpio_put(LED_PIN_11, 1);  // Encender LED rojo
    sleep_ms(2000);           // Esperar 2 segundos
    gpio_put(LED_PIN_11, 0);  // Apagar LED
}

/**
 * @brief Enciende permanentemente el LED en el pin GPIO 12 LED amarillo.
 * 
 * Esta función enciende el LED amarillo conectado al GPIO 12 y lo mantiene encendido de forma permanente.
 * Enciende cuando se inicializa el sistema estando listo para ingresar el id.
 */
void led_on_gpio12_permanently() {
    gpio_put(LED_PIN_12, 1);  // Encender LED permanentemente Amarillo
}

/**
 * @brief Apaga el LED en el pin GPIO 12 LED amarillo.
 * 
 * Esta función apaga el LED amarillo conectado al GPIO 12.
 * Apaga cuando se presiona una tecla.
 */
void led_off_gpio12() {
    gpio_put(LED_PIN_12, 0);  // Apagar LED Amarillo
}

/**
 * @brief Inicia el parpadeo del LED en el pin GPIO 12 Amarillo.
 * 
 * Esta función activa el parpadeo del LED amarillo conectado al GPIO 12 cuando se esta ingresando la clave y almacena el tiempo actual.
 */
void start_blink() {
    should_blink = true;
    last_blink_time = time_us_32(); /**< Almacena el tiempo actual en microsegundos */
    led_state = true;
    gpio_put(LED_PIN_12, led_state);
}

/**
 * @brief Detiene el parpadeo del LED en el pin GPIO 12 Amarillo.
 * 
 * Esta función desactiva el parpadeo del LED amarillo y apaga el LED.
 */
void stop_blink() {
    should_blink = false;
    gpio_put(LED_PIN_12, 0);
}

/**
 * @brief Actualiza el estado de parpadeo del LED.
 * 
 * Esta función debe ser llamada regularmente en el bucle principal para verificar si el LED debe cambiar de estado
 * (encendido o apagado) basado en un intervalo de 1 segundo. Solo funciona si el parpadeo está activado.
 */
void update_blink() {
    if (should_blink) {
        uint32_t current_time = time_us_32();  /**< Obtiene el tiempo actual en microsegundos */
        if (current_time - last_blink_time >= 1000000) {  // 1 segundo en microsegundos
            led_state = !led_state;                       /**< Cambia el estado del LED */
            gpio_put(LED_PIN_12, led_state);         /**< Actualiza el estado del LED */
            last_blink_time = current_time;         /**< Actualiza el tiempo del último cambio */
        }
    }
}
 


