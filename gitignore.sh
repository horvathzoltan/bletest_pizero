#!/bin/bash
FILES=`git ls-files -i -c --exclude-from=./.gitignore`
if [ ! -z "$FILES" ]; then
	git rm --cached $FILES
	echo "commit and puch changes"  
  	git commit -am "actualized .gitignore"
	git push
else
	echo "nothing changed"  
fi
