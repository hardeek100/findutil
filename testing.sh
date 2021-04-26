make clean
make all
echo "*******************************************"
echo Starting from 1 with find testdir
./find testdir
echo "*******************************************"
echo 2.1 find testdir -n file1
./find testdir -n file1
echo "*******************************************"
echo Lets create a new file foo inside testdir
touch testdir/foo
echo 2.2 find testdir -m -10 
./find testdir -m -10
echo "*******************************************"
echo lets see for files modified 10 mins ago
./find testdir -m +10
echo "******************************************"
echo 2.3 find by inode 60162272
./find testdir -i 60162272
echo "******************************************"
echo 3 find by criteria and delete
echo by name,
./find testdir -n foo -d
echo "*******************************************"
echo by min but before that lets create a new file called gooo
touch testdir/dir3/gooo
echo before deleting
./find testdir
echo "*****************************************"
echo after deleting,
./find testdir -m -2 -d
./find testdir
echo "*******************************************"
echo with inode 60162273 find testdir -i 60162273  -d
./find testdir -i 60162273 -d
echo "******************************************"
echo for 4
echo cat by name
./find testdir -n fil1 -e cat
echo "*****************************************"
echo cat by min
./find testdir -m -10 -e cat
echo "******************************************"
echo cat by inode
./find testdir -i 60162273 -e cat
echo "******************************************"
echo rm by name
touch testdir/dir3/foo
./find testdir -n foo -e rm
echo "*****************************************"
echo rm by inode 60162263
./find testdir -i 60162263 -e rm
echo "*****************************************"
echo rm by minutes
touch testdir/dir1/uuuu
./find testidr -m -3 -e rm
echo "****************************************"

