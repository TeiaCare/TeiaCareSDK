mkdir -p callgrind

valgrind --tool=callgrind \
    --callgrind-out-file=callgrind/callgrind.out \
    --verbose \
    --log-file=callgrind/callgrind.log \
    --error-exitcode=1 \
    ./$1
    
    # --exit-on-first-error=yes \
    
# callgrind_annotate --auto=yes callgrind.out

ret=$?
echo ""
echo "callgrind exit code: " $ret
exit $ret