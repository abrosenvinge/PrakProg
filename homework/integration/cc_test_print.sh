echo "Comparison of methods for: int $1 from $2 to $3"
echo "	Method		Result			n_evals		Estimated Error"
printf "	Analytical	%s\n" $4
printf "	%s	%s	%s		%s\n	%s		%s	%s		%s\n" $(./main --test $1 $2 $3)
printf "	%s		%s	%s		%s\n" $(python cc_test.py $1 $2 $3)
