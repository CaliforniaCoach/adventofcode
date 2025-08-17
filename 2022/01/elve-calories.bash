#!/bin/bash

MAX=0

TOP1=0
TOP2=0
TOP3=0

ELVE_CALORIES=0

while read X; do
	if [[ $X == '' ]]; then
		if ((ELVE_CALORIES > TOP1)); then
			TOP3=$TOP2
			TOP2=$TOP1
			TOP1=$ELVE_CALORIES
		elif ((ELVE_CALORIES > TOP2)); then
			TOP3=$TOP2
			TOP2=$ELVE_CALORIES
		elif ((ELVE_CALORIES > TOP3)); then
			TOP3=$ELVE_CALORIES
		fi
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

echo "Top1: $TOP1, Top2: $TOP2, Top3: $TOP3 --> sum: $((TOP1 + TOP2 + TOP3))"
