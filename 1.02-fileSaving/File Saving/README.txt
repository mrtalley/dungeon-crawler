README (1.02)
Maxwell Talley

This weeks project gave me some trouble. I got stuck on reading from the file.
I was able to read the first 12 bytes from the file but since the version is always
0, I was trying to fseek over the 32 bit version number. That didn't work because
as soon as I properly scanned in the version number, it worked just fine and I was
able to finish the entire project in a few hours.

The function readFile scans the binary save file, either created by the writeFile
function or copied in from somewhere. It first scans in 'RLG327-S2017', then version
num, then size, then hardness & room locations. It properly sets variables in the
program as well.

The function writeFile does exactly that, writes the file to disk. It scans in the same
order as it reads. It reads the size in and then calls htobe32 to convert it to the
correct format.
