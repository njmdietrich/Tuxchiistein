# Upstream with shader recipes

Branch: `test/upstream-shader-recipes`.
Base: `zortos293/Lichii:nx2` at `907d54757b838fb4275b8d466ff76de79738ed21`.
Recipe support is ported from `njmdietrich/Tuxchiistein:nx2-lichiistein-linux-pgo` at
`a955e7cf73be580ab12139e68f3c07f7f9e117ca`.

The runtime additions are recipe import from the game list, extraction of programs from the
owner's game, pipeline preparation at launch with progress and Skip, retention of shader infos
and SPIR-V, validation before reusing those results, and fallback to normal translation when
they cannot be reused. Installing an identical recipe preserves its preparation timestamp.
Saved shader infos include the current upstream sky, barrier and pointer-classification flags;
older saved-info formats are prepared again.

Upstream's ordinary pipeline keys, pipeline eviction, image allocation and collection,
buffer uploads, draw shortcuts, synchronization, timing, shader translation and FSR4 behavior
are retained. The other fork workarounds, image recycler, canonical pipeline keys, crash-build
journal and profiling extensions are not ported.

The Linux x86-64 PGO workflow uses the full integration's clang 23, PGO profile, Qt version and
build flags. It packages upstream's Linux FSR4 runtime and disables the updater for these test
artifacts. It uploads an AppImage and tar.gz, without publishing a release.

## Trying the build

1. Download the artifact from this branch's `linux-x86_64-pgo-recipes` workflow.
2. In the game list, select **Prepare Precompiled Shaders from Recipe...** and choose the same `.nx2recipe`
   used with the full integration. It must match the game and shader-cache version.
3. Launch the game and let preparation finish. The new executable prepares the recipe again;
   the full integration's prepared shader infos are not reused as-is.
4. Compare the same scene with the same settings, driver and recipe. Repeat the route and test
   later launches as well as the first run. The log reports recipe reuse and rejection reasons.

For independent cache histories, use separate copies of the emulator's user directory and
launch each build with its own portable `user` directory. Keep settings equal. Switching both
builds through one cache changes their used-pipeline history, and the recipe preparation marker
includes the executable's revision, so switching revisions triggers preparation again.
