#!/bin/bash

MAX=0

while read X; do
	if [[ $X == '' ]]; then
		ELVE_CALORIES=0
	else
		((ELVE_CALORIES += X))
		if ((ELVE_CALORIES > MAX)); then
			MAX=$ELVE_CALORIES
		fi
		echo ELVE: $ELVE_CALORIES, MAX: $MAX
	fi
done

echo $MAX
