# RotEnc
2相（A相 / B相）型インクリメンタル方式のロータリーエンコーダをArduinoで容易に扱うためのライブラリです。

このライブラリではスイッチ接点のチャタリング除去（デバウンシング）をおこなった上で、エンコーダの回転とその方向（時計回り"CW"、反時計周り"CCW"）を検出することができます。

これらの検出は、「 ```isRotated()``` 」のような状態取得関数が返す真偽値による検出のほか、回転動作にコールバック関数を割り当てることによってイベント-ドリブン的な検出をすることもできるようになっています。

## 使い方
各ロータリエンコーダについて ```RotEnc``` のインスタンスを生成します。

引数はA相とB相のピン番号が必須で、すべてのデジル入力ピンを使用することができます。

```C++
RotEnc myRE(2, 3); // A-phase is pin 2 and B-phase is pin 3.
```

ピン番号のみを与えた場合、そのスイッチは
- スイッチの接続モードは「 ```INPUT_PULLUP``` 」
- スイッチ接点が閉じた場合のピン電圧は「 ```LOW```」
- 回転方向の定義（A相電圧が ```HIGH``` から ```LOW``` に変化したときにB相が ```HIGH```となる回転方向）は「 ```CW``` 」

であるものと設定されます。

これら「接続モード」と「閉じた時のピン電圧」「回転方向の定義」はインスタンス生成時にオプション引数として与えることができます。

```C++
RotEnc myRE(2, 3, CW);

RotEnc myRE(2, 3, CW, INPUT_PULLUP);

RotEnc myRE(2, 3, CW, INPUT_PULLUP, LOW);
```

第2引数は「回転方向の定義」で、```CCW```　か ```CW``` を与えます（省略した場合のデフォルト値は ```CW```）。

第3引数はスイッチの「接続モード」で、```INPUT``` か ```INPUT_PULLUP``` のどちらかを与えます（省略した場合のデフォルト値は ```INPUT_PULLUP```）。

第4引数はスイッチが閉じた場合のピン電圧で、 ```LOW```・```HIGH```・```AUTO``` のいずれかを与えます（省略した場合のデフォルト値は ```AUTO```）。

第4引数が ```AUTO``` の場合、
- 第3引数が ```INPUT``` なら ```HIGH```
- 第3引数が ```INPUT_PULLUP``` なら ```LOW```

と見做されます。

さらに、第5引数としてスイッチのチャタリング防止に要する時定数を与えることができます（単位はミリ秒、省略した場合のデフォルト値は ```2```）。

```C++
RotEnc myRE(2, 3, CW, INPUT_PULLUP, LOW, 2);
```

すべてのインスタンスについて、エンコーダ状態の確認と更新のために定期的に ```poll()``` を呼び出します。通常の場合、 ```loop()``` 内で呼び出すことになります。

```poll()``` を呼び出してエンコーダの状態を確認・更新した後、それらを状態取得関数で取得することができます。

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

また、 ```setup()``` 内で回転動作にコールバック関数を割り当てておくと、```loop()``` 内で ```poll()```を呼んだ際に、それらの動作がおこなわれていればコールバック関数が自動的に呼び出されます。

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

## 状態取得関数
### ```boolean isRotated(void)```
ロータリーエンコーダが CCW / CW いずれかに回転していれば "true" を返します。

### ```boolean isRotatedInCCW(void)```
ロータリーエンコーダが CCW 方向に回転していれば "true" を返します。

### ```boolean isRotatedInCW(void)```
ロータリーエンコーダが CW 方向に回転していれば "true" を返します。

### ```uint8_t getRotatedDirection(void)```
ロータリーエンコーダが

- CCW 方向に回転していれば "```CCW (0x00)```" 
- CW 方向に回転していれば "```CW (0x01)```" 
- 回転していなければ "```PAUSE (0xff)```" 

を返します。

## コールバック割当て関数
### ```void attachCallback_RotatedInCCW( void(*)(void) )```
### ```void attachCallback_RotatedInCW( void(*)(void) )```

ロータリーエンコーダが CCW もしくは CW 方向に回転した場合にコールバックされる関数を割り当てます。
割り当てる関数は
```C++
void func() {
    ...
}
```
である関数のポインタです。

### ```void attachCallback_Rotated( void(*)(uint8_t) )```
ロータリーエンコーダが CCW / CW のいずれかに回転した場合にコールバックされる関数を割り当てます。
割り当てる関数は
```C++
void func(uint8_t dir) {
    ...
}
```
である関数のポインタとなっています。

各関数がコールバックされるタイミングは、それぞれの動作が確定したと判定された ```poll()``` 内からとなっています。

## 時定数設定関数
### setTimeParalyze(uint32_t)
スイッチのチャタリング除去（デバウンシング）のための「麻痺」時間（ T<sub>Paralyze</sub> ）をミリ秒単位で設定します（デフォルト値は「2」ミリ秒）。
