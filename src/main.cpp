#include <Arduino.h>
volatile int interruptCounter;
int totalInterruptCounter;

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
#define BUFFER_SIZE 250
int16_t ping[BUFFER_SIZE];
int16_t pong[BUFFER_SIZE];

bool is_pong = false;
bool is_readable = false;
int16_t idx = 0;
int16_t idx_cnt = 0;

void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&timerMux);

  int16_t *pbuff = ping;
  if (is_pong){
    pbuff = pong;
  }

  pbuff[idx] = analogRead(A0); //local_adc1_read(ADC1_CHANNEL_0);
  
  idx++;
  if (idx >= BUFFER_SIZE)
  {
    idx = 0;

    //is_pong = (is_pong==false); // change the flag
    is_pong = !is_pong;
    
    //if(is_pong) is_pong
    is_readable = true;
    // Notify adcTask that the buffer is full.
    // static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // vTaskNotifyGiveFromISR(&adcTaskHandle, &xHigherPriorityTaskWoken);
    // if (xHigherPriorityTaskWoken)
    // {
    //   portYIELD_FROM_ISR();
    // }
  }

  portEXIT_CRITICAL_ISR(&timerMux);
}


void print_to_serial(void){
 int16_t *pbuff = ping;
  if(is_pong){
    pbuff = ping;
    //Serial.print("ping buff: ");
  }else{
    pbuff = pong;
    //Serial.print("pong buff: ");
  }

  for(int ik=0;ik<BUFFER_SIZE;ik++){
    //Serial.print("\t");
    Serial.println(pbuff[ik],DEC);
    //Serial.print(" , ");
  }
  Serial.println();

}

void setup()
{

  Serial.begin(115200);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 4000, true);
  timerAlarmEnable(timer);
}

void loop()
{

  if(is_readable){
    is_readable=false;
    print_to_serial();
  }
  // portENTER_CRITICAL(&timerMux);

  // portEXIT_CRITICAL(&timerMux);
}

// #include <Arduino.h>
// #include <driver/adc.h>
// #include <soc/sens_reg.h>
// #include <soc/sens_struct.h>

// void print_to_serial(void);
// void adcTask(void *param);

// int IRAM_ATTR local_adc1_read(int channel);

// portMUX_TYPE DRAM_ATTR timerMux = portMUX_INITIALIZER_UNLOCKED;
// static TaskHandle_t adcTaskHandle = NULL;

// hw_timer_t * adcTimer = NULL; // our timer

// // the setup function runs once when you press reset or power the board
// #define BUFFER_SIZE 5
// int16_t ping[BUFFER_SIZE];
// int16_t pong[BUFFER_SIZE];

// bool is_pong=false;
// bool is_readable = false;
// int16_t idx = 0;

// void IRAM_ATTR onTimer() {
//   portENTER_CRITICAL_ISR(&timerMux);
//   int16_t *pbuff = ping;
//   if(is_pong){
//     pbuff = pong;
//   }else{
//     pbuff = ping;
//   }

//   pbuff[idx] =idx;//local_adc1_read(ADC1_CHANNEL_0);
//   idx++;
//   if(idx >=BUFFER_SIZE){
//       idx=0;
//       is_pong = ~is_pong; // change the flash
//       is_readable=true;
//           // Notify adcTask that the buffer is full.
//       static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//       vTaskNotifyGiveFromISR(&adcTaskHandle, &xHigherPriorityTaskWoken);
//       if (xHigherPriorityTaskWoken) {
//           portYIELD_FROM_ISR();
//       }
//   }

//   portEXIT_CRITICAL_ISR(&timerMux);
// }

// int IRAM_ATTR local_adc1_read(int channel) {
//     uint16_t adc_value;
//     SENS.sar_meas_start1.sar1_en_pad = (1 << channel); // only one channel is selected
//     while (SENS.sar_slave_addr1.meas_status != 0);
//     SENS.sar_meas_start1.meas1_start_sar = 0;
//     SENS.sar_meas_start1.meas1_start_sar = 1;
//     while (SENS.sar_meas_start1.meas1_done_sar == 0);
//     adc_value = SENS.sar_meas_start1.meas1_data_sar;
//     return adc_value;
// }

// void adcTask(void *param) {
//   while (true) {
//     // Sleep until the ISR gives us something to do, or for 1 second
//     ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000));
//      if (is_readable) {
//           is_readable = false;
//        // Do something complex and CPU-intensive
//         // print_to_serial();
//      }
//   }
// }

// void print_to_serial(void){
//  int16_t *pbuff = ping;
//   if(is_pong){
//     pbuff = ping;
//     Serial.print("ping buff: ");
//   }else{
//     pbuff = pong;
//     Serial.print("pong buff: ");
//   }

//   for(int ik=0;ik<BUFFER_SIZE;ik++){
//     Serial.print(pbuff[ik],DEC);
//     Serial.print(" , ");
//   }
//   Serial.println();

// }

// void setup() {
//   // initialize digital pin LED_BUILTIN as an output.
//   pinMode(LED_BUILTIN, OUTPUT);
//   Serial.begin(115200);
//   Serial.print("setup() running on core ");
//   Serial.println(xPortGetCoreID());

//   xTaskCreate(adcTask, "ADC Handler Task", 4096, NULL, 1, &adcTaskHandle);
//   adcTimer = timerBegin(3, 80, true); // 80 MHz / 80 = 1 MHz hardware clock for easy figuring
//   timerAttachInterrupt(adcTimer, &onTimer, true); // Attaches the handler function to the timer
//   timerAlarmWrite(adcTimer, 500000, true); // Interrupts when counter == 500000
//   timerAlarmEnable(adcTimer);
// }

// // the loop function runs over and over again forever
// uint32_t i=0;
// void loop() {
//   // Serial.print("setup() running on core ");
//   // Serial.println(xPortGetCoreID());

//   // buffer->write(i++);
//   // if(buffer->get_ready()==READY){
//   //     uint32_t *rbuff = buffer->get_read_buff();
//   //     for(int ik=0;ik<BUFFER_SIZE;ik++){
//   //       Serial.print(rbuff[ik],DEC);
//   //       Serial.print(" , ");
//   //     }
//   //     buffer->set_ready(NOT_READY);
//   // }
//   // Serial.println();
//   // Serial.println(i,DEC);
//   // Serial.println(analogRead(A0));

//   // delay(1000);                       // wait for a second

// }