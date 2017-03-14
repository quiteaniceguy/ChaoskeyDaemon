#!/bin/bash

file="/dev/chaoskey"

for i in {0..100}
do
	if [ -c $file$i ]; then
		if ./TestSuiteTest --raw --nbytes 125000 /dev/chaoskey$i; then
			echo "test success"
		else
			echo "failed test"
		fi
	else
		echo "$i doesn't exist"
		break
	fi
done

exit 1
