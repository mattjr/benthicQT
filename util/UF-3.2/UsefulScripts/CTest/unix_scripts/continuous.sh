SHELL=/bin/bash
source ~/.bashrc
fp='/home/amaclean/Work/Testing/UFTest'
/usr/local/bin/ctest -S $fp/scripts/continuous.cfg -V >> $fp/logs/continuous.log 2>&1
