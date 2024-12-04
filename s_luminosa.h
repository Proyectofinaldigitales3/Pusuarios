/**
 * @file s_luminosa.h
 * @brief Declaraciones de funciones y variables para el control de LEDs en el sistema de acceso.
 * 
 *Contiene las definiciones de pines, variables globales y los prototipos de las funciones para controlar los LEDs
 *Los LEDs se utilizan para indicar diferentes estados del sistema.
 */
#ifndef S_LUMINOSA_H
#define S_LUMINOSA_H

#include "pico/stdlib.h"

/**
 * @brief Definición del pin del LED verde.
 */
#define LED_PIN_10 10

/**
 * @brief Definición del pin del LED rojo.
 */
#define LED_PIN_11 11

/**
 * @brief Definición del pin del LED amarillo.
 */
#define LED_PIN_12 12

/**
 * @brief Variable global para controlar si el LED debe parpadear.
 */
extern bool should_blink;

/**
 * @brief Tiempo en microsegundos del último cambio de estado del LED parpadeante.
 */
extern uint32_t last_blink_time;

/**
 * @brief Estado actual del LED (encendido o apagado).
 */
extern bool led_state;

/**
 * @brief Inicializa los pines GPIO asociados a los LEDs.
 * 
 * Esta función configura los pines GPIO como salidas para controlar los LEDs.
 */
void inicialization(void);

/**
 * @brief Enciende el LED verde (GPIO 10) durante 5 segundos.
 * 
 * Esta función enciende el LED conectado al GPIO 10, lo mantiene encendido por 5 segundos y luego lo apaga.
 */
void led_on_gpio10_5_seconds(void);

/**
 * @brief Enciende el LED rojo (GPIO 11) durante 2 segundos.
 * 
 * Esta función enciende el LED conectado al GPIO 11, lo mantiene encendido por 2 segundos y luego lo apaga.
 */
void led_on_gpio11_2_seconds(void);

/**
 * @brief Enciende el LED amarillo (GPIO 12) de manera permanente.
 * 
 * Esta función enciende el LED conectado al GPIO 12 y lo mantiene encendido de forma continua.
 */
void led_on_gpio12_permanently(void);

/**
 * @brief Apaga el LED amarillo (GPIO 12).
 * 
 * Esta función apaga el LED conectado al GPIO 12.
 */
void led_off_gpio12(void);

/**
 * @brief Inicia el parpadeo del LED amarillo (GPIO 12).
 * 
 * Activa el parpadeo del LED amarillo, cambiando su estado cada 1 segundo.
 */
void start_blink(void);

/**
 * @brief Detiene el parpadeo del LED amarillo (GPIO 12).
 * 
 * Desactiva el parpadeo y apaga el LED.
 */
void stop_blink(void);

/**
 * @brief Actualiza el estado del parpadeo del LED amarillo (GPIO 12).
 * 
 * Esta función debe ser llamada regularmente para verificar si el LED debe cambiar de estado 
 * basándose en un intervalo de tiempo de 1 segundo.
 */
void update_blink(void);

#endif // S_LUMINOSA_H
