{
  description = "Pong game in SDL2/C++";

  outputs = {nixpkgs, ...}: let
    supportedSystems = ["x86_64-linux" "x86_64-darwin" "aarch64-darwin"];

    genSystems = nixpkgs.lib.genAttrs supportedSystems;

    pkgs = nixpkgs.legacyPackages;
  in {
    devShells = genSystems (system: let
      p = pkgs.${system};
      llvm = p.llvmPackages_latest;
    in {
      default = p.mkShell.override {stdenv = p.clangStdenv;} {
        packages = with p; [
          gnumake
          cmake
          llvm.lldb
          llvm.libstdcxxClang
          llvm.libcxx
          cppcheck

          SDL2
          glm
        ];
        name = "pong";
      };
    });
  };

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };
}
