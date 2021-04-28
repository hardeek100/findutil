make clean
make all
echo "*******************************************"
echo "Welcome to find utility test cases"
echo "Starting from 1 with find testdir\n\n"
./find testdir
echo "=================================================="
echo 2.1 find testdir -n file1
./find testdir -n file1
echo "*******************************************"
echo Lets create a new file foo inside testdir touch testdir/foo
touch testdir/foo
echo 2.2 Check if the foo is modified within 10 minutes find testdir -m -10 
./find testdir -m -10
echo "*******************************************"
echo lets see files that have been modified after 10 minutes
./find testdir -m +10
echo "******************************************"
echo 2.3 Look for find with inode 60162272
./find testdir -i 60162272
echo "===================================================="
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
echo "========================================================"
echo for 4
echo cat by name
./find testdir -n file5 -e cat
echo "*****************************************"
echo cat by min
echo lets create a new file called yoyo and put some string in it.
echo Hey I am from yoyo > yoyo
./find testdir -m -10 -e cat
echo "******************************************"
echo rm by name
touch testdir/dir3/foo
./find testdir -n foo -e rm
echo "*****************************************"

echo rm by minutes
touch testdir/dir1/uuuu
./find testidr -m -3 -e rm
echo "****************************************"
echo for move first we create  a new file called oldfile
touch oldfile
./find . -n oldfile
echo "****************************************"
echo now execute move with new file
./find . -n oldfile -e mv newfile 
./find . -n newfile
echo "*****************************************"


