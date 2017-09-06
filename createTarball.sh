DIRECTORY=talley_maxwell.assignment-$1
cp -r $2 $DIRECTORY
echo $DIRECTORY
tar cvfz \
  $DIRECTORY.tar.gz $DIRECTORY

rm -rf $DIRECTORY
