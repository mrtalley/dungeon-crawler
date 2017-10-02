DIRECTORY=talley_maxwell.assignment-$1

cp -r $2 $DIRECTORY

# ./createChangelog

cp CHANGELOG $DIRECTORY

echo $DIRECTORY

tar cvfz \
  $DIRECTORY.tar.gz $DIRECTORY

rm -rf $DIRECTORY
