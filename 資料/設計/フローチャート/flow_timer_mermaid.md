# timer 状態遷移

``` mermaid

stateDiagram-v2

risechatta : 立ち上がり時チャタチェック
risechatta : entry / チェックカウントをクリア
risechatta : entry / SWのIOC割込みを禁止
risechatta : do / SWをリードする
risechatta : do / チェックカウントを1増加
risechatta : do / SWが3回ONかチェックする

ongoing : 継続状態
ongoing : entry / SW1の状態をON
ongoing : do / SWをリードする
ongoing : do / SWカウントを1増加

fallchatta : 立ち下がり時チャタチェック
fallchatta : entry / チェックカウントをクリア
fallchatta : do / SWをリードする
fallchatta : do / SWカウントを1増加
fallchatta : do / チェックカウントを1増加
fallchatta : do / SWが3回OFFかチェックする

fallstate : 立ち下がり状態
fallstate : entry / SWカウントをクリア
fallstate : entry / SWの状態をOFF
fallstate : entry / SWのチャタフラグをクリア
fallstate : entry / SWのIOC割込みの許可



[*] --> risechatta : SW1のチャタフラグが立った時
risechatta --> ongoing : SW1の値が3回<br>ONの時
risechatta --> fallstate : SW1の値が3回<br>ONではない時

ongoing --> fallchatta : SW1の値が<br>OFFの時
fallchatta --> ongoing : SW1の値が<br>3回OFFではない時
fallchatta --> fallstate : SW1の値が<br>3回OFFの時

fallstate --> risechatta : 立ち下がり処理が終わったら

```
