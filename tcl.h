/**
 * @file tcl.h
 * @brief Declaraciones y definiciones para el control de acceso con teclado matricial 4x4.
 * 
 * Contiene las definiciones de constantes, tipos de datos y funciones para la implementación
 * de un sistema de control de acceso basado en ID de usuarios y contraseñas, tambien realiza el cambio de
 * contraseñas y bloqueo de usuarios después de múltiples intentos fallidos.
 * 
 */
#ifndef TCL_H
#define TCL_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

/**
 * @brief Tiempo de retardo para el debounce de los botones, en microsegundos.
 */
#define DEBOUNCE_DELAY 200000

/**
 * @brief Número máximo de usuarios permitidos en el sistema de datos.
 */
#define NUM_USERS 5

/**
 * @brief Longitud del ID de usuario.
 */
#define ID_LENGTH 6

/**
 * @brief Longitud de la contraseña.
 */
#define PASSWORD_LENGTH 4

/**
 * @brief Tiempo máximo permitido para ingresar el ID y la contraseña, en milisegundos, osea 20 seg .
 */
#define MAX_INPUT_TIME_MS 20000

/**
 * @brief Número máximo de intentos fallidos antes de bloquear a un usuario.
 */
#define MAX_FAILED_ATTEMPTS 3

/**
 * @brief Pines GPIO utilizados para las filas del teclado matricial.
 */
extern const uint8_t ROW_PINS[4];

/**
 * @brief Pines GPIO utilizados para las columnas del teclado matricial.
 */
extern const uint8_t COL_PINS[4];

/**
 * @brief Mapeo de teclas del teclado matricial 4x4, organizado en filas y columnas.
 */
extern const char KEYPAD[4][4];

/**
 * @brief Estados posibles del sistema.
 */
typedef enum {
    STATE_ENTER_ID,          /**< Estado para ingresar el ID de usuario */
    STATE_ENTER_PASSWORD,    /**< Estado para ingresar la contraseña */
    STATE_LOGGED_IN,         /**< Estado cuando el usuario ha iniciado sesión correctamente */
    STATE_CHECK_BALANCE,
    STATE_WITHDRAW_MONEY,
    STATE_CHANGE_PASSWORD,   /**< Estado para cambiar la contraseña */
    STATE_CONFIRM_PASSWORD   /**< Estado para confirmar el cambio de contraseña */
} SystemState;

/**
 * @brief Estructura que representa a un usuario en el sistema.
 */
typedef struct {
    char id[ID_LENGTH + 1];                 /**< ID del usuario */
    char password[PASSWORD_LENGTH + 1];     /**< Contraseña del usuario */
    char name[20];                          /**< Nombre del usuario */
     double balance;
    uint8_t failed_attempts;                /**< Número de intentos fallidos del usuario */
    bool is_blocked;                        /**< Indica si el usuario está bloqueado */
} User;

/**
 * @brief Indica si se ha presionado una tecla.
 */
extern volatile bool key_pressed;

/**
 * @brief Fila actual escaneada en el teclado matricial.
 */
extern volatile uint8_t current_row;

/**
 * @brief Última tecla presionada en el teclado matricial.
 */
extern volatile char last_key;

/**
 * @brief Tiempo en el que se presionó la última tecla.
 */
extern volatile absolute_time_t last_key_time;

/**
 * @brief Buffer para almacenar el ID de usuario ingresado.
 */
extern char input_id[ID_LENGTH + 1];

/**
 * @brief Buffer para almacenar la contraseña ingresada.
 */
extern char input_password[PASSWORD_LENGTH + 1];

/**
 * @brief Buffer para almacenar la nueva contraseña al cambiarla.
 */
extern char new_password[PASSWORD_LENGTH + 1];

/**
 * @brief Índice actual del input ingresado.
 */
extern int input_index;

/**
 * @brief Estado actual del sistema.
 */
extern SystemState current_state;

/**
 * @brief Tiempo de inicio del input actual.
 */
extern absolute_time_t input_start_time;

/**
 * @brief Puntero al usuario actual que está interactuando con el sistema.
 */
extern User* current_user;

/**
 * @brief timer_Callback que se ejecuta cuando se cumple el tiempo de un temporizador.
 * 
 * @param alarm_num Número del temporizador que ha expirado.
 */
void timer_callback(uint alarm_num);

/**
 * @brief gpio_Callback que se ejecuta cuando ocurre un evento en el GPIO.
 * 
 * @param gpio Número del pin GPIO que generó el evento.
 * @param events Tipos de eventos que ocurrieron subida del flanco.
 */
void gpio_callback(uint gpio, uint32_t events);

/**
 * @brief Inicializa el teclado matricial y configura los pines GPIO correspondientes.
 */
void init_keypad(void);

/**
 * @brief Busca un usuario en la base de datos de usuarios según su ID.
 * 
 * @param id El ID del usuario que se busca.
 * @return Puntero al usuario encontrado, o NULL si no se encuentra.
 */
User* find_user(const char* id);

/**
 * @brief Reinicia el estado del sistema para un nuevo intento de inicio de sesión.
 */
void reset_state(void);

/**
 * @brief Maneja el caso en que el tiempo para ingresar el ID o la contraseña ha sido excedido.
 */
void handle_timeout(void);

/**
 * @brief Muestra el menú de opciones del usuario una vez que ha iniciado sesión.
 */
void show_menu(void);
void amount_menu(void);
/**
 * @brief Procesa las acciones del usuario cuando ha iniciado sesión.
 * 
 * @param key Tecla presionada por el usuario.
 */
void process_logged_in_state(char key);
void amount_selection(char key);

/**
 * @brief Procesa la tecla presionada por el usuario según el estado actual del sistema.
 * 
 * @param key Tecla presionada por el usuario.
 */
void process_key(char key);
void withdraw_money();
void check_balance();

#endif // TCL_H