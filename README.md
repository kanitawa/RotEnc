# RotEnc
This is a library for Arduino to easily handle a 2-phase (A/B) rotary encoder.

This library can detect the encoder rotation and its direction (clockwise “CW”, counterclockwise “CCW”) after removing debouncing of the switch contacts.

These events can be detected by getter functions such as " ```isRotated()``` ”. The detection like "Event-Driven" style is also possible by assigning a callback function to each action.

## How to use
Needs to create an instance of ```RotEnc``` for each rotary encoder. The required arguments are  "pin numbers" for A and B phase. All digital input pins can be used.

```C++
RotEnc myRE(2, 3); // A-phase is pin 2 and B-phase is pin 3.
```

If only the “pin numbers” are specified, the rotary encoder is set in

- "definition of direction (in which phase B is ```HIGH``` when phase A voltage changes from ```HIGH``` to ```LOW``` )" is " ```CW``` "
- “connection mode” of switches is ```INPUT_PULLUP```

These "definition of direction" and “connection mode” can be given as optional arguments of constructor.

```C++
RotEnc myRE(2, 3, CW);

RotEnc myRE(2, 3, CW, INPUT_PULLUP);
```

The "definition of direction" is the 3rd argument, and one of ```CCW``` or ```CW``` is given (default value is ```CW``` ).

The “connection mode” is the 4th argument, and either ```INPUT``` or ```INPUT_PULLUP``` is given (default value is ```INPUT_PULLUP``` ).

For all instances, ```poll()``` is called periodically to check and update the rotary encoder state. This will be usually done within ```loop()```. 

After calling ```poll()``` to check and update the rotary encoder state, they can be retrieved with the getter function.

```C++
void loop() {
    myRE.poll();

    if (myRE.isRotated()) {

        if (myRE.getRotatedDirection() == CCW) {

            Serial.println("CCW");

        } else {

            Serial.println("CW");

        }

    }
}
```

Also, assigning a callback function to a operation in ```setup()``` , the callback function will be called automatically in ```poll()``` when those operations are performed.

```C++
void setup() {

    myRE.attachCallback_RotatedInCCW(on_re_rotated_ccw);
    
    myRE.attachCallback_RotatedInCW(on_re_rotated_cw);

}

void loop() {

    myRE.poll();

}

void on_re_rotated_ccw() {

    Serial.println("CCW");

}

void on_re_rotated_cw() {

    Serial.println("Long-CW");

}
```

## Getter functions
### ```boolean isRotated(void)```
Returns ```true``` if the rotary encoder was rotated.

### ```boolean isRotatedInCCW(void)```
Returns ```true``` if the rotary encoder was rotated in ```CCW``` direction.

### ```boolean isRotatedInCW(void)```
Returns ```true``` if the rotary encoder was rotated in ```CW``` direction.

### ```uint8_t getRotatedDirection(void)```
Return following value :

- " ```CCW (0x00)``` " if the rotary encoder was rotated in ```CCW``` direction.
- " ```CW (0x01)``` " if the rotary encoder was rotated in ```CW``` direction.
- " ```PAUSE (0xff)``` " if the rotary encoder was not rotated.

## Attach callback functions
### ```void attachCallback_RotatedInCCW( void(*)(void) )```
### ```void attachCallback_RotatedInCW( void(*)(void) )```
Attach a callback function when the rotary encoder rotated in the CCW or CW direction.

 The type of the argument is ```void(*)(void)```  pointer of function, like a following:
```C++
void func() {
    ...
}
```

### ```void attachCallback_Rotated( void(*)(uint8_t) )```
Attach a callback function when the rotary encoder rotated.

 The type of the argument is ```void(*)(uint8_t)```  pointer of function, like a following:
```C++
void func(uint8_t dir) {
    ...
}
```
```dir``` is ```CCW (0x00)``` of ```CW (0x01)```.

Each function is callbacked from within ```poll()``` when the switch action confirmed.

## Setter functions for time constatnt
### setTimeParalyze(uint32_t)
Sets the time constant for debouncing, called as "time of paralyze ( T<sub>Paralyze</sub> )", in msec. The default value is "2".

## Attach external interrupt functions
### ```void attachExtInterrupt(void(* func)(void))```
### ```void detachExiInterrupt(void)```

When ``loop()`` that periodically calls ``poll()`` is heavy, some rotation events may be missed. In such cases, it is effective to apply an **external interrupt** at the ```FALLING``` of the A-phase signal of the rotary encoder.

If A-phase signal is connected to an **external interruptable pin**, such a pin [```2```, ```3```] on **Arduino UNO** and pin [```0```, ```1```, ```2```, ```3```, ```7```] on **Arduino Leonardo**, 

```C++

myRE.attachExtInterrupt([]{myRE.detect();});

```

call ```attachExtInterrupt``` in ```setup()``` as described above, the rotation events are detected using external interrupt and it improves the capture of ratation events.

Calling this function is ignored when pin A is not connected to the **external interruptable pin**.

Calling ```detachExiInterrupt()```, this attachment will be cancelled.
