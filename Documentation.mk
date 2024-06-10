## doc:   build documentation

# Documentation by 'doxygen'
# The sed command displays inserts a project name in the otherwise
# generic configuration file Doxyfile.
#Doxyfile:
#	doxygen -g $@ 
#	sed -i 's/PROJECT_NAME[ ]*=.*/PROJECT_NAME=CloudTracker/' $@

doc: cloudtracker.pdf

cloudtracker.pdf: Doxyfile 
	doxygen
	cd latex && pdflatex refman.tex && pdflatex refman.tex
	mv latex/refman.pdf ./docs/cloudtracker.pdf

html/index.html: Doxyfile 
	doxygen

clean:
	rm -rf html latex ./docs/cloudtracker.pdf
.PHONY: doc
