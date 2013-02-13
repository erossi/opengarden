# Copyright (C) 2013 Enrico Rossi
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

GIT_TAG = "$(shell git describe --tags)"
REMOVE = rm -r

# Remote www install
MYWEBSERVER = www.enricorossi.org:/home/doxygen/opengarden/
RSYNC = rsync -crlvz --delete --whole-file doc/html/ $(MYWEBSERVER)

# Drawtiming parameters.
DT_PRG = drawtiming
DT_OPT = --cell-width 48 --cell-height 64 --pagesize 1024x200 --font-size 15
DT_FILEIN = bistable_idle.timing
DT_FILEOUT = $(DT_FILEIN).png

# Image Magick parameters.
IM_PRG = convert
IM_OPT = -gravity south -extent x300 -fill black -gravity north -pointsize 50

obj_timing = bistable_idle.timing.png

.PHONY: clean doxygen timing
.SILENT: help

# Define a pattern rule
%.timing.png: %.timing
	$(DT_PRG) $(DT_OPT) --output $@ $<
	$(IM_PRG) $@ $(IM_OPT) -draw "text 0,0 '$< $(GIT_TAG)'" $@

all:
	# I'm a submake!

clean:
	$(REMOVE) $(obj_timing)
	$(REMOVE) html/*

version:
	# Last Git tag: $(GIT_TAG)

timing: $(obj_timing)
	# Done!

doc: timing
	cd ../ && ( cat doc/Doxyfile ; echo "PROJECT_NUMBER=$(GIT_TAG)" ) | doxygen -

install: doc
	cd ../ && ( $(RSYNC) )

