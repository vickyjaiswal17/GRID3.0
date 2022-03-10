#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>
//#include <TimerOne.h>  //used in AVR's microcontroller like Arduino
// Create a new servo object:
Servo myservo;


////////////////////////////////MOTOR CONTROL////////////////////////////////////

const byte MOTOR_A = 12;  // Motor 2 Interrupt Pin - INT 1 - Right Motor
const byte MOTOR_B = 13;  // Motor 1 Interrupt Pin - INT 0 - Left Motor
 
// Constant for steps in disk
const float stepcount = 45.00;  // 20 Slots in disk, change if different

//pwm speed for motors
int Equal_speed =150;
int mspeed1 = Equal_speed; //R
int mspeed2 = Equal_speed+12; //L

// Constant for wheel diameter
const float wheeldiameter = 66.10; // Wheel diameter in millimeters, change if different
 
// Integers for pulse counters
volatile int counter_A = 0;
volatile int counter_B = 0;


// Motor A
 
#define in1 D0
#define in2 D1
#define enA D2
 
// Motor B
 
#define in3 D4
#define in4 D3
#define enB D5

//servo pin
#define servoPin D8

// Interrupt Service Routines
 
// Motor A pulse count ISR
ICACHE_RAM_ATTR void ISR_countA()  
{
  counter_A++;  // increment Motor A counter value
} 
 
// Motor B pulse count ISR
ICACHE_RAM_ATTR void ISR_countB()  
{
  counter_B++;  // increment Motor B counter value
}

// TimerOne ISR
/*void ISR_timerone()
{
  Timer1.detachInterrupt();  // Stop the timer
  float RPM1 = (counter1 / diskslots) * 60.00;  // calculate RPM for Motor 1
  counter1 = 0;  //  reset counter to zero
  float RPM2 = (counter2 / diskslots) * 60.00;  // calculate RPM for Motor 2
  counter2 = 0;  //  reset counter to zero
  Timer1.attachInterrupt( ISR_timerone );  // Enable the timer

  if(RPM1==RPM2)
  {
    mspeed1 = Equal_speed;
    mspeed2 = Equal_speed;
  }
  else if(RPM1>RPM2)
  {
    mspeed1 = Equal_speed;
    mspeed2 = Equal_speed+50;
  }
  else
  {
    mspeed1 = Equal_speed;
    mspeed2 = Equal_speed-50;
  }
}
*/

// Function to convert from centimeters to steps
int CMtoSteps(float cm) {
 
  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  
  float f_result = cm / cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)
  
  return result;  // End and return result
 
}

// Function to Move Forward
void MoveForward(int steps) 
{
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A forward
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
 
   // Set Motor B forward
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   
   // Go forward until step value is reached
   while (steps > counter_A && steps > counter_B) {
   
    if (steps > counter_A) {
    analogWrite(enA, mspeed1); //255 ->Arduino// 1023->esp
    } else {
    analogWrite(enA, 0);
    }
    if (steps > counter_B) {
    analogWrite(enB, mspeed2);
    } else {
    analogWrite(enB, 0);
    }
   }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 
 
}
 
// Function to Move in Reverse
void MoveBackward(int steps) 
{
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
 
  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
   
   // Go in reverse until step value is reached
   while (steps > counter_A && steps > counter_B) {
   
    if (steps > counter_A) {
    analogWrite(enA, mspeed1);
    } else {
    analogWrite(enA, 0);
    }
    if (steps > counter_B) {
    analogWrite(enB, mspeed2);
    } else {
    analogWrite(enB, 0);
    }
    }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 
 
}
 
// Function to Spin Right
void TurnRight(int steps) 
{
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
 
  // Set Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
   
   // Go until step value is reached
   while (steps > counter_A && steps > counter_B) {
   
    if (steps > counter_A) {
    analogWrite(enA, mspeed1);
    } else {
    analogWrite(enA, 0);
    }
    if (steps > counter_B) {
    analogWrite(enB, mspeed2);
    } else {
    analogWrite(enB, 0);
    }
   }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 
 
}
 
// Function to Spin Left
void TurnLeft(int steps) 
{
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
 
  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
   
   // Go until step value is reached
   while (steps > counter_A && steps > counter_B) {
   
    if (steps > counter_A) {
    analogWrite(enA, mspeed1);
    } else {
    analogWrite(enA, 0);
    }
    if (steps > counter_B) {
    analogWrite(enB, mspeed2);
    } else {
    analogWrite(enB, 0);
    }
  }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 
 
}


////////////////////////////////QUEUE CLASSS/////////////////////////////////////
template <typename T>

class QueueUsingArray
{
    T *data;
    int nextIndex;
    int firstIndex;
    int size;
    int capacity;

public:
    QueueUsingArray(int s)
    {
        data = new T[s];
        nextIndex = 0;
        firstIndex = -1;
        size = 0;
        capacity = s;
    }
    int getSize()
    {
        return size;
    }
    bool isEmpty()
    {
        return size == 0;
    }
    //insert elements
    void enqueue(T element)
    {
        if (size == capacity)
        {
            Serial.print("Queue Full !");
            return;
        }

        data[nextIndex] = element;
        nextIndex = (nextIndex + 1) % capacity;
        if (firstIndex == -1)
        {
            firstIndex = 0;
        }
        size++;
    }
    //to know the first element
    T front()
    {
        if (isEmpty())
        {
            //Serial.print("Queue is Empty !");
            return "0";
        }
        return data[firstIndex];
    }

    //removal function
    T dequeue()
    {
        if (isEmpty())
        {
            //Serial.print("Queue is Empty !");
            return "0";
        }
        T ans = data[firstIndex];
        firstIndex = (firstIndex + 1) % capacity;
        size--;
        if (size == 0)
        {
            firstIndex = -1;
            nextIndex = 0;
        }
        return ans;
    }
};

//////////////////////////////////COMMUNICATION SETUP////////////////////////////////

// Structure example to receive data
typedef struct RecievedCommand {
    String dir;
    String ang;
    String servo;
} RecievedCommand;

// Create a struct_message called myData
RecievedCommand command;


QueueUsingArray<String>dirQueue(25);
QueueUsingArray<String>angQueue(25);

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&command, incomingData, sizeof(command));
  Serial.println("Bytes received!!");
  dirQueue.enqueue(command.dir);
  angQueue.enqueue(command.ang);
}

 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Define motor pinsMode
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enB,OUTPUT);
  pinMode(MOTOR_A, INPUT_PULLUP);
  pinMode(MOTOR_B, INPUT_PULLUP);
  pinMode(servoPin,OUTPUT);

  // Attach the Interrupts to their ISR's
  attachInterrupt(digitalPinToInterrupt (MOTOR_A), ISR_countA, RISING);  // Increase counter A when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR_B), ISR_countB, RISING);  // Increase counter B when speed sensor pin goes High
  //Timer1.initialize(500000); // set timer for 1sec
  //Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
  
  //servo attach and set to 10 degree
  myservo.attach(15, 500, 2500); 
  myservo.write(10);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  millis();
}

void loop() {
  
  if(dirQueue.front()=="F")
  {
    MoveForward(45); 
    yield();
    dirQueue.dequeue();
    angQueue.dequeue();
    
  }
  else if(dirQueue.front()=="B")
  {
    MoveBackward(45);
    yield();
    dirQueue.dequeue();
    angQueue.dequeue();
  }
  else if(dirQueue.front()=="R")
  {
     if(angQueue.front()=="2")
     {

      dirQueue.dequeue();
      angQueue.dequeue();
     }
     else if(angQueue.front()=="5")
     {

      dirQueue.dequeue();
      angQueue.dequeue();
     }
      else if(angQueue.front()=="45")
     {
      
      dirQueue.dequeue();
      angQueue.dequeue();
     }
      else if(angQueue.front()=="90")
     {
      TurnRight(36);
      dirQueue.dequeue();
      angQueue.dequeue();
     }
      else if(angQueue.front()=="180")
     {
      TurnRight(72);
      dirQueue.dequeue();
      angQueue.dequeue();
     }
  }
  else if(dirQueue.front()=="L")
  {
    if(angQueue.front()=="2")
     {
      
      dirQueue.dequeue();
      angQueue.dequeue();
     }
     else if(angQueue.front()=="5")
     {
      
      dirQueue.dequeue();
      angQueue.dequeue();
     }
      else if(angQueue.front()=="45")
     {
      
      dirQueue.dequeue();
      angQueue.dequeue();
     }
      else if(angQueue.front()=="90")
     {
      TurnLeft(36);
      dirQueue.dequeue();
      angQueue.dequeue();
     }
      else if(angQueue.front()=="180")
     {
      TurnLeft(72);
      dirQueue.dequeue();
      angQueue.dequeue();
     } 
  }
  else if(dirQueue.front()=="D")
  {
    Serial.println("Drop");
    for(int i=10;i<=90;i++)
    {
      myservo.write(i);
      delay(2);
    }
    yield();
    delay(500);
    for(int i=90;i>=10;i--)
    {
      myservo.write(i);
      delay(2);
    }
    yield();
    dirQueue.dequeue();
    angQueue.dequeue();
    Serial.println ("Bot is droped");
  }
  else
  {
    digitalWrite(enA,LOW);
    digitalWrite(enB,LOW);
  }

}
