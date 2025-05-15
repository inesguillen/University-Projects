#!/bin/bash

cutfreq_1() {
    local INPUT_FILE="$2/../images/input/lake-large.ppm"
    local OUTPUT_FILE="$2/../images/aos-output/cutfreq-lake-large-100k.ppm"
    local OPERATION="cutfreq"
    local FREQ="100000"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $FREQ"

    local EXPECTED_FILE="$2/../images/expected/cutfreq/lake-large-100K.ppm"

    test_file "$1" "$COMMAND" "$EXPECTED_FILE" "$OUTPUT_FILE"
}

cutfreq_2() {
    local INPUT_FILE="$2/../images/input/lake-large.ppm"
    local OUTPUT_FILE="$2/../images/aos-output/cutfreq-lake-large-162k.ppm"
    local OPERATION="cutfreq"
    local FREQ="162000"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $FREQ"

    local EXPECTED_FILE="$2/../images/expected/cutfreq/lake-large-162K.ppm"

    test_file "$1" "$COMMAND" "$EXPECTED_FILE" "$OUTPUT_FILE"
}

cutfreq_zero() {
    local INPUT_FILE="$2/../images/input/lake-large.ppm"
    local OUTPUT_FILE="$2/../images/aos-output/image-cutfreq-0.ppm"
    local OPERATION="cutfreq"
    local FREQ="0"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $FREQ"

    local EXPECTED_ERROR="Error: Invalid cutfreq: 0"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

cutfreq_invalid_negative() {
    local INPUT_FILE="$2/../images/input/lake-large.ppm"
    local OUTPUT_FILE="$2/../images/aos-output/image-cutfreq-invalid.ppm"
    local OPERATION="cutfreq"
    local FREQ="-1"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $FREQ"

    local EXPECTED_ERROR="Error: Invalid cutfreq: -1"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

cutfreq_colores() {
    local INPUT_FILE="$2/../images/input/lake-large.ppm"
    local OUTPUT_FILE="$2/../images/aos-output/image-cutfreq-too-high.ppm"
    local OPERATION="cutfreq"
    local FREQ="100000000" # Número superior a los colores únicos esperados
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $FREQ"

    local EXPECTED_ERROR="Invalid cutfreq"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}

cutfreq_string() {
    local INPUT_FILE="$2/../images/input/lake-large.ppm"
    local OUTPUT_FILE="$2/../images/aos-output/image-cutfreq-invalid-string.ppm"
    local OPERATION="cutfreq"
    local FREQ="abc"
    local COMMAND="../imtool-aos/imtool-aos $INPUT_FILE $OUTPUT_FILE $OPERATION $FREQ"

    local EXPECTED_ERROR="Error: Invalid cutfreq: abc"

    test_error "$1" "$COMMAND" "$EXPECTED_ERROR"
}