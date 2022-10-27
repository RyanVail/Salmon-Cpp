# This file is just a quick way for me to test if things are working
cd ../build/ || mkdir ../build/ && cd ../build/
rm Salmon || echo "Didn't remove salmon executable."
cmake .. && make && ./Salmon ../tests/test.sal

# This code under here just gets the lines of code of files ending in ".cpp"
#find . -name '*.cpp' | xargs wc -l
