SHELL=/bin/bash
source ~/.bashrc
fp='/home/amaclean/Work/Testing/UFTest'
/usr/local/bin/ctest -S $fp/scripts/nightly.cfg -V >> $fp/logs/nightly.log 2>&1
