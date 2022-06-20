question url: https://www.dcard.tw/f/softwareengineer/p/239212468

question author: 元智大學 電機工程學系

question title: #請益 有人會寫嗎

date: 2022/6/18 22:11

我問了好多人都不會，自己也寫了三天還是寫不出來。眼看期限快到了，就在明天晚上12.前...
各路大神救救我😟

附上題目

題目:
(100%) 在邏輯電路設計中，給定一個 function G(X, Y, Z) 如下:

	G(X, Y, Z)=Σm(5, 6)+Σd(1, 2, 4)

請指出下列小題目 a~e 是否為 G 的一組解。

a. XYZ' + XY'Z

b. Z' + XY'Z

C. X(Y' + Z')

d. Y'Z + XZ' + X'Z

e. XZ' + X'Z

解題規定:
1. 以 C++ 語法實現。
2. 使用愈多 C++的特殊功能，則分數愈高。例如 class, template, inheritance, etc.
3. 只使用基本 C++語法，無 C++進階功能，則分數上限 60%。
4. 以動態輸入方式提供 function 的題目，如僅能解上述題目，則以零分計算。
5. 請務必做好格式化輸入及輸出，如無則扣分 20%。

輸入範例
輸入:
>> 請輸入變數個數: 3

>> 請輸入 minterms: 5, 6

>> 請輸入 don't care terms: 1, 2, 4

>> 請輸入小題確認是否為一組解: xyz'+xy'z


reply:
1. To solve this question, you should know Digital Logic Design. Especially Sum-Of-Product (SOP), minterm and don't care terms.
2. To write the program, you might need to know one of the algorithms that to evaluate a math expression. i.e. Get Reverse Polish notation (RPN) of a expression and evaluate it (["x=1", "y=1", "x & y"]->["1 & 1"]->["1 1 &"]->["true"]). 
3. Or some advanced skills to parse a boolean expression into minters. i.e. ["X and Y and not Z"]->["minterm 6"].
4. Please read the code in logic.cpp. It tries to solve this question with enumerating all possible values of variables and evaluating the RPN. This method might be the easiest way to solve this question.
