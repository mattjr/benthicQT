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

ifeq ($(TERM),msys)
	CMAKE_FLAGS = -G "MSYS Makefiles"
endif
BLD=build

.PHONY: default test xcode vs clean

default:
	(mkdir -p $(BLD); cd $(BLD); cmake $(CMAKE_FLAGS) ..; make)

test: $(BLD)
	(cd $(BLD); make test)

XBLD=build.xc
xcode:
	(mkdir -p ${XBLD}; cd ${XBLD}; cmake .. -DTESTING:BOOL=OFF -G"Xcode"; open VisualizePhysics-Wave.xcodeproj)

XBLDT=build.xc-test
xcode-test:
	(mkdir -p ${XBLDT}; cd ${XBLDT}; cmake .. -G"Xcode"; open VisualizePhysics-Wave.xcodeproj)

VSBLD=build-vs
vs:
	make.bat vs

clean:
	rm -rf $(BLD) $(XBLD) $(VSBLD) $(XBLDT)

install:
	(mkdir -p $(BLD); cd $(BLD); cmake $(CMAKE_FLAGS) ..;  make install)

package:
	(mkdir -p $(BLD); cd $(BLD); cmake $(CMAKE_FLAGS) ..;  make package)
