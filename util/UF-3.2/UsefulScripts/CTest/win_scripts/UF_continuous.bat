@ECHO OFF

REM Set The Environment Variables (if you are using nmake)
REM =============================
REM CALL "C:\Program Files\Microsoft Visual Studio 8\Common7\Tools\vsvars32.bat"


REM Run the CTest to do continuous updates
REM ===================================
SET FP=C:\development\Testing\UFTest
"C:\Program Files\CMake24\bin\ctest" "-S" "%FP%\scripts\UF_continuous.script" "-VV" >>"%FP%\logs\continuous.log"

