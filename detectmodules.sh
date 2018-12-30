#!/bin/bash

if [ ! -d ./plugins ]; then
        mkdir ./plugins
        mkdir ./plugins/cpu
        mkdir ./plugins/audio
        mkdir ./plugins/video
        mkdir ./plugins/input

        FCOUNT=1

        for here in $(ls) ; do

        if [ -d $here ]; then

        if [ -d "$here/.libs/" ]; then

        for now in $(ls $here/.libs/) ; do

                islib=$(echo $now | sed 's/\.so$/LOOKIMALIBRARYHERE/' | grep LOOKIMALIBRARYHERE)
                if [ -n "$islib" ]; then

                        echo -n "$here/.libs/$now: "
nm "$here/.libs/$now" | grep "T main_cpu_loop" >/dev/null && ln -s "../../$here/.libs/$now" ./plugins/cpu/"lib${FCOUNT}.so" && echo -n "is cpu "
nm "$here/.libs/$now" | grep "T ai_init" >/dev/null && ln -s "../../$here/.libs/$now" ./plugins/audio/"lib${FCOUNT}.so" && echo -n "is audio "
nm "$here/.libs/$now" | grep "T vi_init" >/dev/null && ln -s "../../$here/.libs/$now" ./plugins/video/"lib${FCOUNT}.so" && echo -n "is video "
nm "$here/.libs/$now" | grep "T init_pifram" >/dev/null && ln -s "../../$here/.libs/$now" ./plugins/input/"lib${FCOUNT}.so" && echo -n "is input "
			echo
                        FCOUNT=$( expr $FCOUNT + 1 )

                fi

        done

        fi

        fi

        done
fi

