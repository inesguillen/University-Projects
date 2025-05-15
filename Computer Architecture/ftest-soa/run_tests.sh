
dir=$(dirname $0)
mkdir -p "$dir/../images/soa-output"

source $dir/args_test.sh
source $dir/info_test.sh
source $dir/maxlevel_test.sh
source $dir/resize_test.sh
source $dir/compress_test.sh
source $dir/cutfreq_test.sh

failed_tests=()


# poner tests


rm -rf "$dir/../imagenes/soa-output"

summary