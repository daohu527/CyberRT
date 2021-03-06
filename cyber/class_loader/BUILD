load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools/bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "shared_library",
    srcs = [
        "utility/shared_library.cc",
    ],
    hdrs = [
        "utility/shared_library.h",
        "utility/exception.h",
    ],
    linkopts = ['-ldl'],
)

cc_library(
    name = "class_loader",
    srcs = [
        "class_loader.cc",
        "utility/class_factory.cc",
        "utility/class_loader_utility.cc",
    ],
    hdrs = [
        "class_loader.h",
        "class_loader_register_macro.h",
        "utility/class_factory.h",
        "utility/class_loader_utility.h",
    ],
    deps = [
        "//cyber:init",
        "//cyber/common:log",
        "//cyber/class_loader:shared_library",
    ],
)

cc_library(
    name = "class_loader_manager",
    srcs = ["class_loader_manager.cc"],
    hdrs = ["class_loader_manager.h"],
    deps = [
        ":class_loader",
    ],
)

cc_test(
    name = "class_loader_test",
    size = "small",
    srcs = ["class_loader_test.cc"],
    data = [
        "//cyber/class_loader/test:libplugin1.so",
        "//cyber/class_loader/test:libplugin2.so",
    ],
    deps = [
        "//cyber",
        "//cyber/class_loader/test:base",
        "//cyber/proto:unit_test_cc_proto",
        "@com_google_googletest//:gtest",
    ],
)

cpplint()
