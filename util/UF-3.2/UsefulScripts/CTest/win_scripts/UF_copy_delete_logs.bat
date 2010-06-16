@ECHO OFF

SET FP=C:\development\Testing\UFTest\logs
SET SFN=continuous.log
SET DFN=continuous_last_week.log
SET SF=%FP%\%SFN%
SET DF=%FP%\%DFN%
IF EXIST "%SF%" (
  MOVE /Y "%SF%"  "%DF%"
)

SET SFN=nightly.log
SET DFN=nightly_last_week.log
SET SF=%FP%\%SFN%
SET DF=%FP%\%DFN%
IF EXIST "%SF%" (
  MOVE /Y "%SF%"  "%DF%"
)

