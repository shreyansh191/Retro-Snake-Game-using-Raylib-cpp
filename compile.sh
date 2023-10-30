filename="retrosnake.cpp"
compile="g++-13"

base_name="retrosnake"

eval $compile $filename -o $base_name -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -L./lib -lraylib -Wl,-ld_classic




#You can also use a function like this(paste it into ~/.zshrc or ~/.bashrc or etc)->
#ry() {
#    if [ -z "$1" ]; then
#        echo "Please provide the script filename as the first argument."
#    else
#        if [ -f "$1" ]; then
#
#	    filename="$1"
#	    extension="${filename##*.}"
#	    base_name="${filename%.*}"
#	    compile="gcc-13"
#	    if [ "$extension" = "cpp" ]; then
#		compile="g++-13"
#
#	   eval $compile $filename -o $base_name -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -L/Users/shreyanshkedawat/RawUocode/ransan5_raylib/src -lraylib -Wl,-ld_classic         
#        else
#            echo "The file $1 does not exist."
#      fi
#    fi
#}
## Here, -L/Users/shreyanshkedawat/RawUocode/ransan5_raylib/src  is nothing but -L/path/to/libraylib.a
#ry retrosnake.cpp

