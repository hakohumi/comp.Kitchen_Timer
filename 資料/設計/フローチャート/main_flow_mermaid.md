# main flowchart

``` mermaid

graph TB

start(["start"])
input[["入力"]]
processing[["処理"]]
output[["出力"]]
endgraph(["end"])
if{"終了するまで"}



start --> if
if --> input
input --> processing
processing --> output
output --> endgraph


```
