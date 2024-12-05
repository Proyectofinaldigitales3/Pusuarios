/**
 * @file tcl.c
 * @brief Implementación de un teclado matricial 4x4 para ingresar ID y clave.
 *
 * Este programa permite el ingreso de usuarios mediante ID y contraseña, con manejo de cambios de contraseñas 
 * y bloqueo de usuarios tras varios intentos fallidos.
 */
#include "tcl.h"
#include "s_luminosa.h"

/**
 * @brief Pines correspondientes a las filas del teclado matricial.
 */
const uint8_t ROW_PINS[4] = {2, 3, 4, 5};

/**
 * @brief Pines correspondientes a las columnas del teclado matricial.
 */
const uint8_t COL_PINS[4] = {6, 7, 8, 9};

/**
 * @brief Mapa de teclas del teclado matricial.
 */
const char KEYPAD[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

/**
 * @brief Estructura que define los usuarios del sistema.
 * 
 * Incluye ID, contraseña, nombre, intentos fallidos y estado de bloqueo.
 */
User users[NUM_USERS] = {
    {"123456", "1234", "Juan Pérez", 220000, 0, false},
    {"234567", "2345", "María García", 350000, 0, false},
    {"345678", "3456", "Carlos López", 10000, 0, false},
    {"456789", "4567", "Ana Martínez", 200000, 0, false},
    {"567890", "5678", "Pedro Sánchez", 100000, 0, false}
};

/**
 * @brief Variable que indica si se ha presionado una tecla.
 */
volatile bool key_pressed = false;

/**
 * @brief Fila actual del teclado que está siendo escaneada.
 */
volatile uint8_t current_row = 0;

/**
 * @brief Última tecla presionada.
 */
volatile char last_key = 0;

/**
 * @brief Último tiempo en que se presionó una tecla.
 */
volatile absolute_time_t last_key_time;

/**
 * @brief Almacena el ID ingresado por el usuario.
 */
char input_id[ID_LENGTH + 1] = {0};

/**
 * @brief Almacena la contraseña ingresada por el usuario.
 */
char input_password[PASSWORD_LENGTH + 1] = {0};

/**
 * @brief Almacena una nueva contraseña cuando el usuario desea cambiarla.
 */
char new_password[PASSWORD_LENGTH + 1] = {0};

/**
 * @brief Índice actual de entrada de datos (ID o contraseña).
 */
int input_index = 0;

/**
 * @brief Estado actual del sistema.
 */
SystemState current_state = STATE_ENTER_ID;

/**
 * @brief Tiempo de inicio de la entrada de datos actual.
 */
absolute_time_t input_start_time;

/**
 * @brief Puntero al usuario actualmente autenticado.
 */
User* current_user = NULL;

double amount= 0;



/**
 * @brief timer_Callback para el temporizador que escanea las filas del teclado matricial.
 * 
 * @param alarm_num Número del temporizador.
 */
void timer_callback(uint alarm_num) {
    gpio_put(ROW_PINS[current_row], 1);
    current_row = (current_row + 1) % 4;
    gpio_put(ROW_PINS[current_row], 0);
    hardware_alarm_set_target(alarm_num, make_timeout_time_ms(5));
}

/**
 * @brief gpio_Callback para manejar la interrupción de un pin de GPIO (tecla presionada).
 * 
 * @param gpio Pin de GPIO que generó la interrupción.
 * @param events Eventos generados por el pin.
 */
void gpio_callback(uint gpio, uint32_t events) {
    led_off_gpio12();                                   //------
    if (!key_pressed) {
        absolute_time_t current_time = get_absolute_time();
        if (absolute_time_diff_us(last_key_time, current_time) > DEBOUNCE_DELAY) {
            for (int col = 0; col < 4; col++) {
                if (gpio == COL_PINS[col]) {
                    last_key = KEYPAD[current_row][col];
                    key_pressed = true;
                    last_key_time = current_time;
                    break;
                }
            }
        }
    }
}

/**
 * @brief Inicializa el teclado matricial configurando los pines de filas y columnas.
 */
void init_keypad() {
    for (int i = 0; i < 4; i++) {
        gpio_init(ROW_PINS[i]);
        gpio_set_dir(ROW_PINS[i], GPIO_OUT);
        gpio_put(ROW_PINS[i], 1);
        
        gpio_init(COL_PINS[i]);
        gpio_set_dir(COL_PINS[i], GPIO_IN);
        gpio_pull_up(COL_PINS[i]);
        gpio_set_irq_enabled_with_callback(COL_PINS[i], GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    }
    
    hardware_alarm_claim(0);
    hardware_alarm_set_callback(0, timer_callback);
    hardware_alarm_set_target(0, make_timeout_time_ms(5));
}

/**
 * @brief Busca un usuario en la lista de usuarios por su ID.
 * 
 * @param id ID del usuario a buscar.
 * @return User* Puntero al usuario encontrado, o NULL si no existe.
 */
User* find_user(const char* id) {     
    for (int i = 0; i < NUM_USERS; i++) {
        if (strcmp(users[i].id, id) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

/**
 * @brief Reinicia el estado del sistema para un nuevo intento de inicio de sesión.
 */
void reset_state() {
    memset(input_id, 0, sizeof(input_id));
    memset(input_password, 0, sizeof(input_password));
    memset(new_password, 0, sizeof(new_password));
    input_index = 0;
    current_state = STATE_ENTER_ID;
    current_user = NULL;
    input_start_time = get_absolute_time();
    led_on_gpio12_permanently();                  //----------
    printf("\nIngrese ID de 6 dígitos:\n");
}

/**
 * @brief Maneja el caso en que el tiempo para ingresar el ID o la contraseña ha sido excedido.
 */
void handle_timeout() {
    printf("\n¡Tiempo excedido! Por favor, intente de nuevo.\n");
    stop_blink();                         // apaga titileo si se demoro mucho ingresando la contraseña
    led_on_gpio11_2_seconds();                                      //------
    reset_state();
}

/**
 * @brief Muestra el menú de opciones del usuario una vez que ha iniciado sesión.
 */
void show_menu() {
    printf("\nMateCash:\n");
    printf("\nMenú de Usuario:\n");
    printf("A - Retirar Dinero\n");
    printf("B - Consultar Saldo\n");
    printf("C - Cambiar Clave\n");
    printf("D - Cerrar sesión\n");
}
void amount_menu() {
    printf("\nMateCash:\n");
    printf("\nCuanto Dinero Desea retirar?:\n");
    printf("A - 10.000\n");
    printf("B - 20.000\n");
    printf("C - 50.000\n");
    printf("D - 100.000\n");
}

/**
 * @brief Procesa las acciones del usuario cuando ha iniciado sesión.
 * 
 * @param key Tecla presionada por el usuario.
 */
void process_logged_in_state(char key) {
    switch (key) {
        case 'A': // Retirar dinero
            amount_menu();
            current_state = STATE_WITHDRAW_MONEY;
            break;

            break;
        case 'B': // Consultar saldo
            printf("\nConsultando saldo...\n");
            current_state = STATE_CHECK_BALANCE;
            check_balance();
            break;
        case 'C':
            printf("\nIngrese nueva contraseña de 4 dígitos:\n");
            current_state = STATE_CHANGE_PASSWORD;
            input_index = 0;
            input_start_time = get_absolute_time();
            break;
        case 'D':
            printf("\nCerrando sesión...\n");
            reset_state();
            break;
        default:
            printf("\nOpción no válida\n");
            led_on_gpio11_2_seconds();                            //--------------
            show_menu();
            break;
    }
}
void amount_selection(char key) {
    switch (key) {
        case 'A': // Retirar dinero
            amount=10000;
            withdraw_money();
            break;
        case 'B': // Consultar saldo

            withdraw_money();
            break;
        case 'C':
             amount=20000;
            withdraw_money();
            break;
        case 'D':
            amount=50000;
            withdraw_money();
            reset_state();
            break;
        case '*':
            amount=100000;
            withdraw_money();
            break;
        default:
            printf("\nOpción no válida\n");
           amount_menu();
            break;
    }
}

void withdraw_money() {
    if (current_user->is_blocked) {
        printf("\nError: Su cuenta está bloqueada.\n");
        return;
    }
    if (amount > current_user->balance) {
        printf("\nError: Fondos insuficientes. Su saldo actual es %.2f\n", current_user->balance);
    } else {
        current_user->balance -= amount;
        printf("\nRetiro exitoso. Su nuevo saldo es: %.2f\n", current_user->balance);
        current_state = STATE_CHECK_BALANCE;
    }
}
// Función para consultar el saldo
void check_balance() {
    if (current_user->is_blocked) {
        printf("\nError: Su cuenta está bloqueada.\n");
        return;
    }

    printf("\nSu saldo actual es: %.2f\n", current_user->balance);
}

/**
 * @brief Procesa la tecla presionada por el usuario según el estado actual del sistema.
 * 
 * @param key Tecla presionada por el usuario.
 */
void process_key(char key) {
    switch (current_state) {
        case STATE_ENTER_ID:
            if (input_index < ID_LENGTH) {
                input_id[input_index++] = key;
                printf("%c", key);
                if (input_index == ID_LENGTH) {
                    input_id[ID_LENGTH] = '\0';
                    current_user = find_user(input_id);
                    if (current_user == NULL || current_user->is_blocked) {
                        if (current_user && current_user->is_blocked) {
                            printf("\n¡Usuario bloqueado! Contacte al administrador.\n");
                            led_on_gpio11_2_seconds();                                           //----
                        } else {
                            printf("\nID de usuario no existe.\n");
                            led_on_gpio11_2_seconds();                                           //-----
                        }
                        reset_state();
                    } else {
                        printf("\nIngrese contraseña de 4 dígitos:\n");
                        start_blink();                                                        // titilea led amarillo
                        current_state = STATE_ENTER_PASSWORD;
                        input_index = 0;
                    }
                }
            }
            break;

        case STATE_ENTER_PASSWORD:
            if (input_index < PASSWORD_LENGTH) {
                input_password[input_index++] = key;
                printf("*");
                if (input_index == PASSWORD_LENGTH) {
                    input_password[PASSWORD_LENGTH] = '\0';
                    if (strcmp(current_user->password, input_password) == 0) {
                        printf("\n\n¡Bienvenido, %s!\n", current_user->name);
                        stop_blink();                                            // apaga titileo led amarillo
                        led_on_gpio10_5_seconds();                               //----
                        current_user->failed_attempts = 0;
                        current_state = STATE_LOGGED_IN;
                        show_menu();
                    } else {
                        current_user->failed_attempts++;
                        if (current_user->failed_attempts >= MAX_FAILED_ATTEMPTS) {
                            current_user->is_blocked = true;
                            printf("\n\n¡Usuario bloqueado! Demasiados intentos fallidos.\n");
                            led_on_gpio11_2_seconds();                                               //-----
                        } else {
                            printf("\n\nContraseña incorrecta. Intentos restantes: %d\n", 
                                   MAX_FAILED_ATTEMPTS - current_user->failed_attempts);
                            stop_blink();                                                          // apaga titileo        
                            led_on_gpio11_2_seconds();                                             //----
                        }
                        reset_state();
                    }
                }
            }
            break;

        case STATE_LOGGED_IN:
            process_logged_in_state(key);
            break;
        
        case STATE_CHECK_BALANCE:  // Maneja la consulta de saldo
            if (key == '#') { // Confirma para salir del estado
                printf("\nVolviendo al menú principal...\n");
                current_state = STATE_LOGGED_IN;
                show_menu();
            } else {
                check_balance();
            }
            break;

        case STATE_WITHDRAW_MONEY:  // Maneja el retiro de dinero
            amount_selection(key);
            break;

        case STATE_CHANGE_PASSWORD:
            if (input_index < PASSWORD_LENGTH) {
                new_password[input_index++] = key;
                printf("*");
                if (input_index == PASSWORD_LENGTH) {
                    printf("\nConfirme la nueva contraseña:\n");
                    current_state = STATE_CONFIRM_PASSWORD;
                    input_index = 0;
                    memset(input_password, 0, sizeof(input_password));
                }
            }
            break;

        case STATE_CONFIRM_PASSWORD:
            if (input_index < PASSWORD_LENGTH) {
                input_password[input_index++] = key;
                printf("*");
                if (input_index == PASSWORD_LENGTH) {
                    if (strcmp(new_password, input_password) == 0) {
                        strcpy(current_user->password, new_password);
                        printf("\n¡Contraseña cambiada exitosamente!\n");
                    } else {
                        printf("\nLas contraseñas no coinciden. Intente de nuevo.\n");
                    }
                    current_state = STATE_LOGGED_IN;
                    show_menu();
                }
            }
            break;

    }
}
