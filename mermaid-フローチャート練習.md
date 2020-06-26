# mermaid 練習

``` mermaid

graph TD

A["四角"]
B("角丸")
C(("丸"))
D>"非対称"]
E{"ひし形"}

A-->B
C--"テスト"-->D
B---E
D--"テスト"---B

E-.->A
C-."テストC".->A

B==>F

F=="テストF"==>E

```