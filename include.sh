#!/usr/bin/env bash
MOD_LEVELREWARD_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source $MOD_LEVELREWARD_ROOT"/conf/conf.sh.dist"

if [ -f $MOD_LEVELREWARD_ROOT"/conf/conf.sh" ]; then
    source $MOD_LEVELREWARD_ROOT"/conf/conf.sh"
fi
