
compress_1() {
    local INPUT_FILE="$2/../deer-small.ppm"
    local OUTPUT_FILE="$2/../compress-deer-small.cppm"
    local OPERATION="compress"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION"
    local EXPECTED_FILE="$2/../deer-small.cppm"
    test_file "$1" "$COMMAND" "$EXPECTED_FILE" "$OUTPUT_FILE"
}

compress_2() {
    local INPUT_FILE="$2/../lake-small.ppm"
    local OUTPUT_FILE="$2/../compress-lake-small.cppm"
    local OPERATION="compress"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION"
    local EXPECTED_FILE="$2/../lake-small.cppm"
    test_file "$1" "$COMMAND" "$EXPECTED_FILE" "$OUTPUT_FILE"
}