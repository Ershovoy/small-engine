#include <stdio.h>
#include <windows.h>

#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define CYAN    "\x1B[36m"
#define RESET   "\x1B[0m"

void enableAnsi(void);
void printRegisterMap(void);
void printErrorLog(void);

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    enableAnsi();

    printf("\n");
    printf(CYAN "================================================================\n");
    printf("  Universal Configurable Controller PLC <-> RS-485\n");
    printf("  STM32F103C8T6 (Blue Pill)  |  Modbus RTU Slave\n");
    printf("================================================================" RESET "\n");
    printf("\n");

    printf("[ModbusAdapter] Initializing RS485 on USART1...\n");
    printf("[ModbusAdapter]   TX = PA9, RX = PA10, DE/RE = PA1\n");
    printf("[ModbusAdapter]   Baud rate: 9600\n");
    printf("[ModbusAdapter]   Slave ID:  1\n");
    printf("[ModbusAdapter] RS485 ready.\n");
    printf("\n");

    printf("[DeviceManager] Registering devices:\n");
    printf("[DeviceManager]   [+] DigitalOutput   id=0   PB0,PB1,PB2,PB3          " GREEN "OK" RESET "\n");
    printf("[DeviceManager]   [+] Tc1508a         id=1   PA0,PA1,PA8,PA9    (PWM) " GREEN "OK" RESET "\n");
    printf("[DeviceManager]   [+] StepperMotor    id=2   PB13,PB14,PB15,PB16      " GREEN "OK" RESET "\n");
    printf("[DeviceManager]   [+] Pca9685         id=3   PB6,PB7            (I2C) " GREEN "OK" RESET "\n");
    printf("[DeviceManager]   [+] As5600          id=4   PA6,PA7            (I2C) " GREEN "OK" RESET "\n");
    printf("[DeviceManager]   [+] AnalogInput     id=5   PA2,PA3            (ADC) " GREEN "OK" RESET "\n");

    /* ---- Failure: Ds18b20 requests PB13 already claimed by StepperMotor id=2 ---- */
    printf(RED    "[DeviceManager]   [!] Ds18b20         id=6   PB13            (1-Wire) FAILED" RESET "\n");
    printf(RED    "[PinRegistry]      ERROR: pin PB13 already claimed by 'StepperMotor' (id=2)" RESET "\n");
    printf(RED    "[ErrorLog]         + record added: device='Ds18b20', pin=PB13, code=PIN_CONFLICT" RESET "\n");
    printf(YELLOW "[DeviceManager]     Device 'Ds18b20' marked INVALID. Registers will NOT be allocated." RESET "\n");
    printf(YELLOW "[DeviceManager]     Device will be skipped in updateDevices() cycle." RESET "\n");

    printf("[DeviceManager]   [+] DigitalInput    id=7   PB10,PB11                " GREEN "OK" RESET "\n");
    printf("[DeviceManager] Total registered: " GREEN "7 active" RESET ", " RED "1 failed" RESET ".\n");
    printf("\n");

    printRegisterMap();
    printf("\n");
    printErrorLog();

    printf("\n");
    printf(YELLOW "[DeviceManager] Initialization completed with errors. See log above." RESET "\n");
    printf("[Main] Modbus polling loop started.\n");
    printf("\n");

    return 0;
}

void enableAnsi(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void printRegisterMap(void)
{
    printf("[RegisterTable] Auto-generated Modbus register map:\n");
    printf("\n");
    printf("+--------+----------------+----+--------+----------------------------------+--------------+\n");
    printf("| Addr   | Device         | ID | Pin    | Description                      | Range        |\n");
    printf("+--------+----------------+----+--------+----------------------------------+--------------+\n");
    printf("                                          [ Coils ]                                        \n");
    printf("| 00001  | DigitalOutput  |  0 | PB0    | Светодиод 1                      | 0..1         |\n");
    printf("| 00002  | DigitalOutput  |  0 | PB1    | Светодиод 2                      | 0..1         |\n");
    printf("| 00003  | DigitalOutput  |  0 | PB2    | Светодиод 3                      | 0..1         |\n");
    printf("| 00004  | DigitalOutput  |  0 | PB3    | Светодиод 4                      | 0..1         |\n");
    printf("+--------+----------------+----+--------+----------------------------------+--------------+\n");
    printf("                                      [ Discrete Inputs ]                                  \n");
    printf("| 10001  | DigitalInput   |  7 | PB10   | Состояние кнопки 1               | 0..1         |\n");
    printf("| 10002  | DigitalInput   |  7 | PB11   | Состояние кнопки 2               | 0..1         |\n");
    printf("+--------+----------------+----+--------+----------------------------------+--------------+\n");
    printf("                                      [ Input Registers ]                                  \n");
    printf("| 30001  | As5600         |  4 | PA6    | Угол энкодера (I2C)              | 0..4095      |\n");
    printf("| 30002  | As5600         |  4 | PA7    | Статус магнита                   | 0..1         |\n");
    printf("| 30003  | AnalogInput    |  5 | PA2    | АЦП канал 1                      | 0..4095      |\n");
    printf("| 30004  | AnalogInput    |  5 | PA3    | АЦП канал 2                      | 0..4095      |\n");
    printf("+--------+----------------+----+--------+----------------------------------+--------------+\n");
    printf("                                      [ Holding Registers ]                                \n");
    printf("| 40001  | Tc1508a        |  1 | PA0    | Угол сервопривода 1 (ШИМ)        | 0..180       |\n");
    printf("| 40002  | Tc1508a        |  1 | PA1    | Угол сервопривода 2 (ШИМ)        | 0..180       |\n");
    printf("| 40003  | Tc1508a        |  1 | PA8    | Угол сервопривода 3 (ШИМ)        | 0..180       |\n");
    printf("| 40004  | Tc1508a        |  1 | PA9    | Угол сервопривода 4 (ШИМ)        | 0..180       |\n");
    printf("| 40005  | StepperMotor   |  2 | PB13   | Целевая позиция (шаги)           | 0..65535     |\n");
    printf("| 40006  | StepperMotor   |  2 | PB14   | Скорость (шагов/с)               | 0..5000      |\n");
    printf("| 40007  | StepperMotor   |  2 | PB15   | Команда (стоп/вперёд/назад)      | 0..2         |\n");
    printf("| 40008  | StepperMotor   |  2 | PB16   | Текущая позиция                  | 0..65535     |\n");
    printf("| 40009  | Pca9685        |  3 | PB6    | ШИМ канал 0 (I2C, 12 бит)        | 0..4096      |\n");
    printf("| 40010  | Pca9685        |  3 | PB7    | ШИМ канал 1 (I2C, 12 бит)        | 0..4096      |\n");
    printf("+--------+----------------+----+--------+----------------------------------+--------------+\n");
    printf("\n");
    printf("[RegisterTable] Total: 20 registers  (Coils=4, Discrete=2, Input=4, Holding=10)\n");
    printf(YELLOW "[RegisterTable] Skipped due to errors: 1 device (Ds18b20)" RESET "\n");
}

void printErrorLog(void)
{
    printf(RED "================================================================\n");
    printf("                ERROR LOG   -   1 record\n");
    printf("================================================================" RESET "\n");
    printf(RED " [E001]" RESET "  PIN_CONFLICT\n");
    printf("         Time      : 00:00:00.142\n");
    printf("         Phase     : initialization\n");
    printf("         Device    : Ds18b20  (id=6)\n");
    printf("         Pin       : PB13\n");
    printf("         Owner     : StepperMotor  (id=2)\n");
    printf("         Action    : device marked INVALID, registers not allocated\n");
    printf(RED "================================================================" RESET "\n");
}
