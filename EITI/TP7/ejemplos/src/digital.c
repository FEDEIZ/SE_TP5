/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Digital inputs/outputs definitions
 **
 ** \addtogroup hal HAL
 ** \brief Hardware abstraction layer
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "digital.h"
#include "chip.h"

/* === Macros definitions ====================================================================== */

#ifndef INPUT_INSTANCES
    #define INPUT_INSTANCES        4
#endif


#ifndef OUTPUT_INSTANCES
    #define OUTPUT_INSTANCES       4
#endif

/* === Private data type declarations ========================================================== */

//! Estructura para almacenar el descriptor de una entrada digital
struct digital_input_s {
    uint8_t port;           //!< Puerto GPIO de la entrada digital
    uint8_t pin;            //!< Terminal del puerto GPIO de la entrada digital
    bool inverted;          //!< La entrada opera con lógica invertida
    bool last_state;        //!< Bandera con el ultimo estado reportado de la entrada
    bool allocated;         //!< Bandera para indicar que el descriptor esta en uso
};

//! Estructura para almacenar el descriptor de una salida digital
struct digital_output_s {
    uint8_t port;           //!< Puerto GPIO de la salida digital
    uint8_t pin;            //!< Terminal del puerto GPIO de la salida digital
    bool allocated;         //!< Bandera para indicar que el descriptor esta en uso
};

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

// Function para asignar un descriptor para crear una nueva entrada digital
digital_input_t DigitalInputAllocate(void);

// Function para asignar un descriptor para crear una nueva salida digital
digital_output_t DigitalOutputAllocate(void);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

digital_input_t DigitalInputAllocate(void) {
    digital_input_t input = NULL;

    static struct digital_input_s instances[INPUT_INSTANCES] =  {0};

    for(int index = 0; index < INPUT_INSTANCES; index++) {
        if (!instances[index].allocated) {
            instances[index].allocated = true;
            input = &instances[index];
            break;
        }
    }
    return input;
}

digital_output_t DigitalOutputAllocate(void) {
    digital_output_t output = NULL;

    static struct digital_output_s instances[OUTPUT_INSTANCES] =  {0};

    for(int index = 0; index < OUTPUT_INSTANCES; index++) {
        if (!instances[index].allocated) {
            instances[index].allocated = true;
            output = &instances[index];
            break;
        }
    }
    return output;
}

/* === Public function implementation ========================================================= */

digital_input_t DigitalInputCreate(uint8_t port, uint8_t pin, bool inverted) {
    digital_input_t input = DigitalInputAllocate();

    if (input) {
        input->port = port;
        input->pin = pin;
        input->inverted = inverted;
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, input->port, input->pin, false);
    }

    return input;
}

bool DigitalInputGetState(digital_input_t input) {
    return input->inverted ^ Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, input->port, input->pin);
}

bool DigitalInputHasChanged(digital_input_t input) {
    bool state = DigitalInputGetState(input);
    bool result = state != input->last_state;
    input->last_state = state;
    return result;
}

bool DigitalInputHasActivated(digital_input_t input) {
    bool state = DigitalInputGetState(input);
    bool result = state && !input->last_state;
    input->last_state = state;
    return result;
}

bool DigitalInputHasDeactivated(digital_input_t input) {
    bool state = DigitalInputGetState(input);
    bool result = !state && !input->last_state;
    input->last_state = state;
    return result;
}

digital_output_t DigitalOutputCreate(uint8_t port, uint8_t pin) {
    digital_output_t output = DigitalOutputAllocate();

    if (output) {
        output->port = port;
        output->pin = pin;
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, false);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, output->port, output->pin, true);
    }

    return output;
}

void DigitalOutputActivate(digital_output_t output) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, true);
}

void DigitalOutputDeactivate(digital_output_t output) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, false);
}

void DigitalOutputToggle(digital_output_t output) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, output->port, output->pin);
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
