// Stubs for host-only asset-tooling symbols that are excluded from the
// emscripten (web) build. The wasm runtime only *reads* pre-generated o2r
// archives; it never exports assets, so these are safe no-ops.

// Defined in OTRExporter (host-only, not built for wasm). Referenced by the
// ZAPD library's startup path.
void ImportExporters() {}
