# Copyright 2010 NVIDIA Foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Developed by Mustard Seed Software, LLC 
# http://mseedsoft.com
#
# @author <a href="mailto:ben.hocking@mseedsoft.com">Ashlie B. Hocking</a>
# Makefile
ifndef OSTYPE
   OSTYPE = $(shell uname)
endif


# Get the number of processors
ifeq ("$(OSTYPE)","Darwin")
   CPU_COUNT=2
else
   CPU_COUNT := $(shell cat /proc/cpuinfo | grep -P "processor\t" | wc -l)
   ifeq ($(CPU_COUNT),0)
      CPU_COUNT=1
   endif
endif

# Set the number of build threads to the number of processors
ARGS=-j ${CPU_COUNT}

ifeq ($(TERM),msys)
	CMAKE_FLAGS = -G "MSYS Makefiles"
endif
BLD=build

.PHONY: default test xcode vs clean

default:
	(mkdir -p $(BLD); cd $(BLD); cmake $(CMAKE_FLAGS) ..; make ${ARGS})

test: $(BLD)
	(cd $(BLD); make test)

XBLD=build.xc
xcode:
	(mkdir -p ${XBLD}; cd ${XBLD}; cmake .. -DTESTING:BOOL=OFF -G"Xcode"; open VisualizePhysics-Wave.xcodeproj)

XBLDT=build.xc-test
xcode-test:
	(mkdir -p ${XBLDT}; cd ${XBLDT}; cmake .. -G"Xcode"; open VisualizePhysics-Wave.xcodeproj)
WBLD=build-win32
win32pkg:
	(mkdir -p ${WBLD}; cd ${WBLD}; cmake .. -DWIN32=1;  make ${ARGS} package)
win32cross:
	(mkdir -p ${WBLD}; cd ${WBLD}; cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake .. -DWIN32=1;  make ${ARGS} install)

VSBLD=build-vs
vs:
	make.bat vs

clean:
	rm -rf $(BLD) $(XBLD) $(VSBLD) $(XBLDT)

install:
	(mkdir -p $(BLD); cd $(BLD); cmake $(CMAKE_FLAGS) ..;  make ${ARGS} install)

package:
	(mkdir -p $(BLD); cd $(BLD); cmake $(CMAKE_FLAGS) ..;  make ${ARGS} package)
