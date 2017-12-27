#!/usr/bin/env bash
set -e

if [ $1 = "jenkins" ]; then
    test_root="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    for filename in "${test_root}"/test_*; do
        test_name=$(basename "${filename}")
        "${filename}" --output_format=XML --log_sink="results/${test_name}_log.xml" --log_level=all --report_sink="results/${test_name}_report.xml" --report_level=detailed --result_code=no
    done
else
    for file in **/test/test_*
        do ./$file
    done
fi