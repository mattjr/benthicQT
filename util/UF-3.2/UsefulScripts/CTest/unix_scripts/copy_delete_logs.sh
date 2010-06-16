#!/bin/sh
fp='/home/amaclean/Work/Testing/UFTest/logs'
if [ -e $fp/continuous.log ]
then
  mv -u $fp/continuous.log $fp/continuous_last_week.log
fi

if [ -e $fp/nightly.log ]
then
  mv -u $fp/nightly.log $fp/nightly_last_week.log
fi
