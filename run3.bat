@echo off
set cmd="out\build\x64-debug\Formula Evaluator\FormulaEvaluator.exe" --cfg=Config/cfg.multi-files.txt
echo -------------------------------------------------------------
echo Multi-files config
echo Starting %cmd% %*
echo -------------------------------------------------------------
%cmd% %*