/**
 * @file main.c
 * @brief Implementación principal del sistema con teclado matricial 4x4 y señales luminosas.
 * 
 * Contiene la función `main` que inicializa el sistema, maneja la entrada del usuario y controla
 * el flujo del programa, incluyendo la actualización de los estados del sistema y el manejo de tiempo límite.
 * 
 * El sistema utiliza un teclado matricial para ingresar IDs y contraseñas, y controla LEDs para proporcionar
 * retroalimentación visual a los usuarios.
 * 
 * @author: Pablo Francisco Muñoz Burbano
 *          Yeiner Alexander Martinez Barrera
 * @date 07/10/2024
 */
#include "tcl.h"
#include "s_luminosa.h"

/**
 * @brief Función principal del sistema.
 * 
 * Esta función inicializa el sistema, enciende un LED de forma permanente, inicia el teclado matricial y
 * espera la entrada del usuario. El sistema actualiza el estado del LED titilante y maneja el ingreso del ID
 * y la contraseña del usuario, verificando si se ha excedido el tiempo máximo permitido para la entrada.
 * 
 * @return 0 si la ejecución es exitosa.
 */
int main() {
    stdio_init_all();           /**< Inicializa el subsistema */
    inicialization();           /**< Inicializa las señales luminosas */
    led_on_gpio12_permanently();     /**< Enciende el LED amarillo antes de ser presionada alguna tecla */
    init_keypad();                   /**< Inicializa el teclado matricial y configura los pines GPIO correspondientes */
    last_key_time = get_absolute_time();  /**< Registra el tiempo de la última tecla presionada */
    input_start_time = get_absolute_time();   /**< Registra el tiempo de inicio del input */
    
    printf("Sistema de Control de Acceso\n");
    printf("Ingrese ID de 6 dígitos:\n");
    
    while (true) {
        update_blink();   /**< Actualiza el estado del LED titilante (LED amarillo titila ingresando clave) */
        if (key_pressed) {
            process_key(last_key);   /**< Procesa la última tecla presionada */
            key_pressed = false;     /**< Reinicia la bandera de tecla presionada */
        sleep_ms(500);         
    }
    
    return 0;
}
}
