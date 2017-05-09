#!/bin/sh
PGM="`basename $0`"
CHAOSKEY_PREFIX="/dev/chaoskey"

for f in ${CHAOSKEY_PREFIX}[0-9]*
do
        if [ -c "$f" ]; then
                if ./TestSuite --raw --chaoskeybits --nbytes 125000 "$f"; then
                       echo "$PGM: $f: NIST tests succeeded" >&2
                else
                        echo "$PGM: $f: NIST tests failed, exiting" >&2
                        exit 1
                fi
        else
                echo "$PGM: $f disappeared" >&2
        fi
done
exit 0
