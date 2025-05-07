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

- 回転方向の定義（A相電圧が ```HIGH``` から ```LOW``` に変化したときにB相が ```HIGH```となる回転方向）は「 ```CW``` 」
- スイッチの接続モードは「 ```INPUT_PULLUP``` 」

であるものと設定されます。

これら「回転方向の定義」と「接続モード」はインスタンス生成時にオプション引数として与えることができます。

```C++
RotEnc myRE(2, 3, CW);

RotEnc myRE(2, 3, CW, INPUT_PULLUP);
```

第3引数は「回転方向の定義」で、```CCW```　か ```CW``` を与えます（省略した場合のデフォルト値は ```CW```）。

第4引数はスイッチの「接続モード」で、```INPUT``` か ```INPUT_PULLUP``` のどちらかを与えます（省略した場合のデフォルト値は ```INPUT_PULLUP```）。

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
である関数のポインタとなっています。引数「 ```dir``` 」には回転方向（ ```CCW``` か ```CW``` ）が入っています。

各関数がコールバックされるタイミングは、それぞれの動作が確定したと判定された ```poll()``` 内からとなっています。

## 時定数設定関数
### setTimeParalyze(uint32_t)
スイッチのチャタリング除去（デバウンシング）のための「麻痺」時間（ T<sub>Paralyze</sub> ）をミリ秒単位で設定します（デフォルト値は「2」ミリ秒）。

## 外部割り込み割当て関数
### ```void attachExtInterrupt(void(* func)(void))```
### ```void detachExtInterrupt(void)```

```poll()``` を定期的に呼び出す ```loop()``` 内での処理が重い場合などには、ロータリーエンコーダの回転を取りこぼすことが起こります。そのような場合にはロータリーエンコーダのA相信号の変化を外部割り込みとすることが有効です。

ロータリーエンコーダのA相が外部割り込み可能なピン（**Arduino UNO**なら ピン [```2```, ```3```]、**Arduino Leonardo**なら ピン [```0```, ```1```, ```2```, ```3```, ```7```] など）に接続されている場合、```setup()``` 内で

```C++

myRE.attachExtInterrupt([]{myRE.detect();});

```

のように記述することで、A相信号の変化を外部割り込みとして回転を検出することができるようになります。これにより回転の取りこぼしが改善します。A相が外部割り込み可能なピンに接続されていない場合には、このメソッドは無視されます。

```detachExtInterrupt()``` を呼ぶと割当てが解除されます。
