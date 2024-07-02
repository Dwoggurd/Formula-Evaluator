@echo off
set cmd="out\build\x64-debug\FormulaEvaluator\FormulaEvaluator.exe" --formulas=Config/formulas.example.txt --data=Config/data.example.txt --log-level=5
echo -------------------------------------------------------------
echo No config file
echo Starting %cmd% %*
echo -------------------------------------------------------------
%cmd% %*
