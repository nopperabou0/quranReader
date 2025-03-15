const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.resolveTargetQuery(.{ .os_tag = .linux, .cpu_arch = .x86_64, .abi = .gnu });
    const targetWindows = b.resolveTargetQuery(.{ .os_tag = .windows, .cpu_arch = .x86_64 });
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

    exe.addCSourceFiles(.{ .files = &.{"./src/main.c"}, .flags = &.{ "-Wall", "-Wextra", "-std=c99", "-pedantic", "-g3"} });

    exeWindows.addCSourceFiles(.{ .files = &.{"./src/main.c"}, .flags = &.{ "-Wall", "-Wextra", "-std=c99", "-pedantic" } });

    exe.addIncludePath(b.path("."));
    exeWindows.addIncludePath(b.path("."));

    exe.addObjectFile(b.path("lib/linux/libraylib.a"));

    exeWindows.addObjectFile(b.path("lib/windows/libraylib.a"));

    exeWindows.linkSystemLibrary("gdi32");
    exeWindows.linkSystemLibrary("winmm");
    exeWindows.linkSystemLibrary("opengl32");

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
