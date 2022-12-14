#include "unity.h"
#include "clock.h"

#define TICKS_PER_SECOND 5


/*


3) Configurar la biblioteca, ajustar la hora (con valores incorrectos) tiene devolver un error y al consultar la hora tiene que ser invalida.


8) Configurar la hora de la alarma (con valores incorrectos) y revisar si la rechaza.


11) Si la alarma esta desactivada y la activo, queda activa, pero no cambia la hora.

14) Si la se activo, y las pospongo n minutos, vuelve a sonar n minutos
15) Si la se activo, y la cancelo, al día siguiente, vuelve a sonar
16) Si alarma se activa y pasan 23:59:59 no debe volver a activarse, pero si pasa un segundo mas entonces si se debe volver a activar


AGREGAR: *) cuando aranca el reloj la alarma esta en false! -> La agregue en la primera prueba
*/

clock_t clock;
bool alarm_state;

void alarmEventHandler(clock_t clock, bool state){ // funcion de callback para disparar alarma
    alarm_state = state;
}

void setUp(void){  // iniciamos un solo reloj para todas las pruebas. Nombre reservado para ceedling. Se ejecuta antes de cada prueba
    static const uint8_t INITIAL[] = {1,2,3,4};
    clock = createClock(TICKS_PER_SECOND, alarmEventHandler);
    configClock(clock, INITIAL, sizeof(INITIAL));
    alarm_state = false; // antes de cada prueba la alarma no esta disparada
}

void simulateSeconds(int seconds){
    for(int index=0; index < seconds * TICKS_PER_SECOND; index++){
        tickClock(clock);
    }
}



// 1) Configurar la biblioteca, consultar la hora y tiene que ser invalida.

void test_initial_hour_invalid(void){
    static const uint8_t EXPECTED[] = {0,0,0,0,0,0}; 
    uint8_t hour[6];
    uint8_t alarm[4];
    clock = createClock(TICKS_PER_SECOND, alarmEventHandler); // necesitamos que el reloj NO este en hora!
    TEST_ASSERT_FALSE(getTimeClock(clock, hour, sizeof(hour)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED,hour, sizeof(EXPECTED));
    TEST_ASSERT_FALSE(clockGetAlarm(clock, alarm, sizeof(alarm))); // me aseguro que la alarma esta desactivada cuando arranca
}

// 2) Configurar la biblioteca, ajustar la hora (con valores correctos), consultar la hora y tiene que ser valida.

void test_set_up_current_time(void){
    static const uint8_t INITIAL[] = {1,2,3,4};
    static const uint8_t EXPECTED[] = {1,2,3,4,0,0};
    uint8_t hour[6];
    TEST_ASSERT_TRUE(getTimeClock(clock,hour,sizeof(hour)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, hour, sizeof(EXPECTED));

}

// 4) Simular el paso de n ciclos de reloj, consultar la hora y verificar que avanzo un segundo

void test_one_second_elapsed(void){
    static const uint8_t EXPECTED[] = {1,2,3,4,0,1};
    uint8_t hour[6];
    simulateSeconds(1);
    getTimeClock(clock,hour,sizeof(hour));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, hour, sizeof(EXPECTED));
}


// 5) Simular el paso de 10*n ciclos de reloj, consultar la hora y verificar que avanzo diez segundos

void test_ten_second_elapsed(void){
    static const uint8_t EXPECTED[] = {1,2,3,4,1,0};
    uint8_t hour[6];
    simulateSeconds(10);
    getTimeClock(clock,hour,sizeof(hour));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, hour, sizeof(EXPECTED));
}

// 6) Simular el paso de un minuto, diez minutos, una hora, diez horas y un día completo.

void test_one_minute_elapsed(void){
    static const uint8_t EXPECTED[] = {1,2,3,5,0,0};
    uint8_t hour[6];
    simulateSeconds(60);
    getTimeClock(clock,hour,sizeof(hour));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, hour, sizeof(EXPECTED));
}

void test_ten_minute_elapsed(void){
    static const uint8_t EXPECTED[] = {1,2,4,4,0,0};
    uint8_t hour[6];
    simulateSeconds(600);
    getTimeClock(clock,hour,sizeof(hour));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, hour, sizeof(EXPECTED));
}

void test_one_hour_elapsed(void){
    static const uint8_t EXPECTED[] = {1,3,3,4,0,0};
    uint8_t hour[6];
    simulateSeconds(3600);
    getTimeClock(clock,hour,sizeof(hour));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, hour, sizeof(EXPECTED));
}

void test_ten_hour_elapsed(void){
    static const uint8_t EXPECTED[] = {2,2,3,4,0,0};
    uint8_t hour[6];
    simulateSeconds(36000);
    getTimeClock(clock,hour,sizeof(hour));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, hour, sizeof(EXPECTED));
}

// 7) Configurar la hora de la alarma (con valores correctos) y revisar si la guarda.
// 9) Configurar la hora de la alarma (con valores correctos) y revisar si la queda activada.

void test_setup_and_get_alarm(void){
    static const uint8_t ALARM[] = {1,2,3,5};
    uint8_t hour[4];
    clockSetUpAlarm(clock, ALARM, sizeof(ALARM));
    TEST_ASSERT_TRUE(clockGetAlarm(clock, hour, sizeof(hour)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARM, hour, sizeof(ALARM));

}

// 10) Si la alarma esta activa y la desactivo, queda desactivada, pero no cambia la hora.

void test_setup_and_disable_alarm(void){
    static const uint8_t ALARM[] = {1,2,3,5};
    uint8_t hour[4];
    clockSetUpAlarm(clock, ALARM, sizeof(ALARM));
    TEST_ASSERT_FALSE(clockToggleAlarm(clock));
    TEST_ASSERT_FALSE(clockGetAlarm(clock, hour, sizeof(hour))); // ahora debe devolver falso por estar desactivada
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARM, hour, sizeof(ALARM));

}

// 12) Si la alarma esta activa, y la hora del reloj coincide con la hora de la alarma, entonces suena.

void test_setup_and_fire_alarm(void){
    static const uint8_t ALARM[] = {1,2,3,5};
    
    clockSetUpAlarm(clock, ALARM, sizeof(ALARM));
    simulateSeconds(60); // tiempo que falta para que suene la alarma
    TEST_ASSERT_TRUE(alarm_state);
    alarm_state = false;
    simulateSeconds(10); // si pasan mas de 1 segundo despues de la hora exacta de la alarma, no deberia estar disparada
    // TEST_ASSERT_FALSE(alarm_state);
    // simulateSeconds(86329); // si pasan 24hr menos 1 segundo no deberia estar disparada
    // TEST_ASSERT_FALSE(alarm_state);
    // simulateSeconds(1); // si pasan 24hrs deberia dispararse
    // TEST_ASSERT_TRUE(alarm_state);
}

// 17) Si la alarma esta inactiva no debe sonar cuando la hora del reloj coincida con la hora de la alarma

void test_disable_alarm(void){
    static const uint8_t ALARM[] = {1,2,3,5};
    
    clockSetUpAlarm(clock, ALARM, sizeof(ALARM));
    clockToggleAlarm(clock);
    simulateSeconds(60); // tiempo que falta para que suene la alarma
    TEST_ASSERT_FALSE(alarm_state);
}