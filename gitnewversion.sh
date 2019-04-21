#!/bin/sh
command -v git >/dev/null 2>&1 || { echo "gitnewversion requires git but it is not installed.  Aborting." >&2; exit 1; }


filecheck="gitpull.sh"

if [ -f "$filecheck" ]; then
 if [ $# -eq 1 ]; then
  if git show-ref --tags $1 ; then
    echo Version $1 already committed
    exit 
  fi
  if git ls-remote --exit-code --tags origin $1 ; then
    echo Version $1 already committed on origin
    exit 
  fi
  make cleanall
  echo ' '
  echo 'Calling git status to check what changes will be pushed..'
  echo ' '
  git status
  echo ' '
  echo -n "Add these changes in new version $1 on github? (y/N)"
  read answer

  if [ "$answer" != "${answer#[Yy]}" ] ;then
    echo Yes
  else
    echo No
    exit 1
  fi


  echo 'setting new version in src/version.hh'
  echo '#define VERSION "'"$1"'"' > src/version.hh
  echo 'Committing and pushing files to github..'
  git add -A .
  git commit -a -m "$1" 
  git tag -d $1 > /dev/null 2>&1
  git tag -a $1 -m "$1"
  git push --tags origin master



 else
  echo "gitnewversion.sh needs one argument: the new version string"
 fi
else
 echo "gitnewversion.sh must be run from the NODPLOT main directory."
fi


