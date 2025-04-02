const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.resolveTargetQuery(.{ .os_tag = .linux, .cpu_arch = .x86_64, .abi = .gnu });
    const targetWindows = b.resolveTargetQuery(.{ .os_tag = .windows, .cpu_arch = .x86_64, .abi = .gnu });
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "quranReader-linux",
        .target = target,
        .optimize = optimize,
    });

    const exeWindows = b.addExecutable(.{
        .name = "quranReader-windows",
        .target = targetWindows,
        .optimize = optimize,
    });

    exe.addCSourceFiles(.{ .files = &.{ "./src/main.c", "./include/cJSON/cJSON.c" }, .flags = &.{ "-Wall", "-Wextra", "-std=c99", "-pedantic", "-g3" } });

    exeWindows.addCSourceFiles(.{ .files = &.{ "./src/main.c", "./include/cJSON/cJSON.c", "./src/windowsonly.c" }, .flags = &.{ "-Wall", "-Wextra", "-std=c99", "-pedantic" } });

    exe.addIncludePath(b.path("."));
    exeWindows.addIncludePath(b.path("."));

    exe.addObjectFile(b.path("lib/linux/libraylib.a"));
    //curl stuffs
    exe.addObjectFile(b.path("lib/linux/libcurl.a"));
    exe.addObjectFile(b.path("lib/linux/libssl.a"));
    exe.addObjectFile(b.path("lib/linux/libcrypto.a"));
    exe.addObjectFile(b.path("lib/linux/libz.a"));
    exe.addObjectFile(b.path("lib/linux/libzstd.a"));
    exe.addObjectFile(b.path("lib/linux/libidn2.a"));
    exe.addObjectFile(b.path("lib/linux/libnghttp3.a"));
    exe.addObjectFile(b.path("lib/linux/libnghttp2.a"));
    exe.addObjectFile(b.path("lib/linux/libssh2.a"));
    exe.addObjectFile(b.path("lib/linux/libbrotlidec.a"));
    exe.addObjectFile(b.path("lib/linux/libbrotlienc.a"));
    exe.addObjectFile(b.path("lib/linux/libbrotlicommon.a"));
    exe.addObjectFile(b.path("lib/linux/libpsl.a"));
    exe.addObjectFile(b.path("lib/linux/libunistring.a"));
    exe.addObjectFile(b.path("lib/linux/libcares.a"));

    exeWindows.addObjectFile(b.path("lib/windows/libraylib.a"));
    // exeWindows.addObjectFile(b.path("lib/windows/libcurl.a"));

    exeWindows.linkSystemLibrary("gdi32");
    exeWindows.linkSystemLibrary("winmm");
    exeWindows.linkSystemLibrary("opengl32");
    exeWindows.linkSystemLibrary("bcrypt");

    exe.linkLibC();
    exeWindows.linkLibC();

    b.installArtifact(exe);
    b.installArtifact(exeWindows);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
