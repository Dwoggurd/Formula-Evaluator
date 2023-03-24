@echo off
set cmd="out\build\x64-debug\Formula Evaluator\FormulaEvaluator.exe" --cfg=Config/cfg.txt
echo -------------------------------------------------------------
echo Single config file
echo Starting %cmd% %*
echo -------------------------------------------------------------
%cmd% %*