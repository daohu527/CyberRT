#! /usr/bin/env bash
TOP_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd -P)"
source ${TOP_DIR}/scripts/apollo.bashrc

export CYBER_PATH="${APOLLO_ROOT_DIR}/cyber"

# FIXME(all): preconfigured in /etc/profile.d/apollo.sh
export QT5_PATH="/usr/local/qt5"
export QT_QPA_PLATFORM_PLUGIN_PATH=${QT5_PATH}/plugins
add_to_path "${QT5_PATH}/bin"

bazel_bin_path="${APOLLO_ROOT_DIR}/bazel-bin"
visualizer_path="${bazel_bin_path}/modules/tools/visualizer"

cyber_bin_path="${bazel_bin_path}/cyber"
cyber_tool_path="${bazel_bin_path}/cyber/tools"
recorder_path="${cyber_tool_path}/cyber_recorder"
monitor_path="${cyber_tool_path}/cyber_monitor"

launch_path="${CYBER_PATH}/tools/cyber_launch"
channel_path="${CYBER_PATH}/tools/cyber_channel"
node_path="${CYBER_PATH}/tools/cyber_node"
service_path="${CYBER_PATH}/tools/cyber_service"
rosbag_to_record_path="${bazel_bin_path}/modules/data/tools/rosbag_to_record"

# TODO(all): place all these in one place and add_to_path
for entry in "${cyber_bin_path}" "${recorder_path}" "${monitor_path}" "${launch_path}" \
    "${channel_path}" "${node_path}" "${service_path}" \
    "${visualizer_path}" \
    "${rosbag_to_record_path}" ; do
    add_to_path "${entry}"
done

export PYTHONPATH=${bazel_bin_path}/cyber/python/internal:${CYBER_PATH}/python:$PYTHONPATH

export CYBER_DOMAIN_ID=80
export CYBER_IP=127.0.0.1

export GLOG_log_dir="${APOLLO_ROOT_DIR}/data/log"
export GLOG_alsologtostderr=0
export GLOG_colorlogtostderr=1
export GLOG_minloglevel=0

export sysmo_start=0

# for DEBUG log
#export GLOG_minloglevel=-1
#export GLOG_v=4

source ${CYBER_PATH}/tools/cyber_tools_auto_complete.bash
